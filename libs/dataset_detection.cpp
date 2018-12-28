#include <iostream>
#include <dataset_detection.h>
#include <experimental/filesystem>
#include <image_load.h>
#include <image_save.h>
#include <json_config.h>

DatasetDetection::DatasetDetection(std::string json_file_name)
                 :DatasetInterface()
{
    JsonConfig json(json_file_name);
    width   = json.result["width"].asInt();
    height  = json.result["height"].asInt();
    channels= 3;

    float training_testing_ratio = json.result["training testing ratio"].asFloat();
    unsigned int background_downscale = json.result["background downscale"].asInt();
    unsigned int foreground_downscale = json.result["foreground downscale"].asInt();

    float threshold   = json.result["threshold"].asFloat();
    float alpha       = json.result["alpha"].asFloat();
    float background_inversion = json.result["background inversion"].asBool();
    float foreground_inversion = json.result["foreground inversion"].asBool();
    float noise = json.result["noise"].asFloat();



    alpha = json.result["alpha"].asFloat();
    for (unsigned int i = 0; i < json.result["background"].size(); i++)
    {
        std::string dir = json.result["background"][i].asString();
        std::cout << "loading background from " << dir << "\n";
        load_images(background, dir, background_downscale);
    }

    unsigned int forreground_classes_count = json.result["foreground"].size();
    foreground.resize(forreground_classes_count);

    for (unsigned int j = 0; j < forreground_classes_count; j++)
    {
        for (unsigned int i = 0; i < json.result["foreground"][j].size(); i++)
        {
            std::string dir = json.result["foreground"][j][i].asString();
            std::cout << "class " << j << " loading foreground from " << dir << "\n";
            load_images(foreground[j], dir, foreground_downscale);
        }
    }

    unsigned int classes_count = forreground_classes_count + 1;
    training.resize(classes_count);

    for (unsigned int class_id = 0; class_id < foreground.size(); class_id++)
    for (unsigned int item_id = 0; item_id < foreground[class_id].size(); item_id++)
    {
        unsigned int background_idx = rand()%background.size();

        auto result_true = mix_min( background[background_idx],
                                    foreground[class_id][item_id],
                                    threshold,
                                    alpha,
                                    background_inversion,
                                    foreground_inversion,
                                    noise,
                                    true
                                    );

        auto result_false = mix_min(    background[background_idx],
                                        foreground[class_id][item_id],
                                        threshold,
                                        alpha,
                                        background_inversion,
                                        foreground_inversion,
                                        noise,
                                        false
                                    );

        sDatasetItem item_true;

        item_true.input = result_true;
        item_true.output.resize(classes_count);
        for (unsigned int i = 0; i < classes_count; i++)
            item_true.output[i] = 0.0;
        item_true.output[class_id + 1] = 1.0;

        sDatasetItem item_false;

        item_false.input = result_false;
        item_false.output.resize(classes_count);
        for (unsigned int i = 0; i < classes_count; i++)
            item_false.output[i] = 0.0;
        item_false.output[0] = 1.0;



        float rndf = (rand()%100000)/100000.0;

        if (rndf < training_testing_ratio)
        {
            add_testing(item_true);
            add_testing(item_false);
        }
        else
        {
            add_training(item_true);
            add_training(item_false);
        }
    }

}

DatasetDetection::~DatasetDetection()
{

}


void DatasetDetection::load_images(std::vector<std::vector<float>> &result, std::string path_dir, unsigned int downscale)
{

    std::vector<float> mat;
    mat.resize(height*width*3);

    for (unsigned int i = 0; i < mat.size(); i++)
        mat[i] = 0.0;

    for (auto & p : std::experimental::filesystem::directory_iterator(path_dir))
    {
        std::string image_file_name;
        image_file_name = p.path();

        if (std::experimental::filesystem::path(image_file_name).extension() == ".png")
        {
            //std::cout << "loading " << image_file_name  << "\n";

            ImageLoad image;
            image.load(image_file_name, false, true);


            unsigned int input_height   = image.height();
            unsigned int input_width    = image.width();
            for (unsigned int ch = 0; ch < channels; ch++)
            for (unsigned int y = 0; y < height; y++)
            for (unsigned int x = 0; x < width; x++)
            {
                unsigned int input_idx  = (ch*input_height + y*downscale)*input_width + x*downscale;
                unsigned int output_idx = (ch*height + y)*width + x;

                mat[output_idx] = image.get()[input_idx];
            }


            result.push_back(mat);
        }
    }
}

std::vector<float> DatasetDetection::mix_min(   std::vector<float> &background,
                                                std::vector<float> &foreground,
                                                float threshold,
                                                float alpha,
                                                bool background_inversion,
                                                bool foreground_inversion,
                                                float noise,
                                                bool mix_enabled
                                            )
{
    unsigned int size = width*height*channels;
    std::vector<float> result(size);

    bool background_invert = false;
    if (rand()%2)
        background_invert = true;

    bool foreground_invert = false;
    if (rand()%2)
        foreground_invert = true;

    for (unsigned int i = 0; i < size; i++)
    {
        result[i] = background[i];
        if (background_inversion)
        if (background_invert)
            result[i] = 1.0 - result[i];
    }

    if (mix_enabled)
    {
        float a = alpha*(rand()%100000)/100000.0;

        for (unsigned int i = 0; i < size; i++)
        {
            float raw = foreground[i];
            if (raw < threshold)
            {
                if (foreground_inversion)
                if (foreground_invert)
                    raw = 1.0 - raw;

                result[i] = (1.0 - a)*raw + a*background[i];
            }
        }
    }

    for (unsigned int i = 0; i < size; i++)
    {
        float rndf = (rand()%100000)/100000.0;
        result[i] = (1.0 - noise)*result[i] + rndf*noise;
    }


    return result;
}
