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


    white_noise_level     =   json.result["white noise"].asFloat();
    luma_noise_level      =   json.result["luma noise"].asFloat();
    rotation_noise_level  =   json.result["rotation noise"].asFloat();
    shift_noise_level     =   json.result["shift noise"].asInt();
    mix_threshold         =   json.result["mix threshold"].asFloat();



    unsigned int forreground_classes_count = json.result["foreground"].size();
    foreground.resize(forreground_classes_count);

    for (unsigned int class_id = 0; class_id < forreground_classes_count; class_id++)
    {
        unsigned int downscale = json.result["foreground"][class_id]["downscale"].asInt();
        for (unsigned int i = 0; i < json.result["foreground"][class_id]["dirs"].size(); i++)
        {
            std::string dir = json.result["foreground"][class_id]["dirs"][i].asString();
            std::cout << "class " << class_id << " loading foreground from " << dir << "\n";
            load_images(foreground[class_id], dir, downscale);
        }
    }

    unsigned int background_downscale = json.result["background downscale"].asInt();
    for (unsigned int i = 0; i < json.result["background"].size(); i++)
    {
        std::string dir = json.result["background"][i].asString();
        std::cout << "loading background from " << dir << "\n";
        load_images(background, dir, background_downscale);
    }





    unsigned int classes_count = forreground_classes_count + 1;
    training.resize(classes_count);

    for (unsigned int class_id = 0; class_id < foreground.size(); class_id++)
    for (unsigned int item_id = 0; item_id < foreground[class_id].size(); item_id++)
    {
        unsigned int background_idx = rand()%background.size();

        auto result_true = process_image(   background[background_idx],
                                            foreground[class_id][item_id],
                                            true );

        auto result_false = process_image(  background[background_idx],
                                            foreground[class_id][item_id],
                                            false);

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



        if (rndf(0.0, 1.0) < training_testing_ratio)
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

    print();

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

            if ( (width*downscale != input_width) ||
                 (height*downscale != input_height) )
                 continue;

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


std::vector<float> DatasetDetection::process_image( std::vector<float> &background,
                                                    std::vector<float> &foreground,
                                                    bool mix_enabled
                                                 )
{

    auto background_noised = add_noise(background, white_noise_level, luma_noise_level);
    auto foreground_noised = add_noise(foreground, white_noise_level, luma_noise_level);

    auto foreground_rotated = rotation_noise(foreground_noised, rotation_noise_level);
    auto foreground_shifted = shift_noise(foreground_rotated, shift_noise_level);


    std::vector<float> mixed;

    if (mix_enabled)
    {
        mixed = mix_min(background_noised, foreground_shifted, mix_threshold);
    }
    else
    {
        mixed = background_noised;
    }



    auto result = normalise(mixed);

    std::cout << "\n\ninfo\n";
    std::cout << width << " " << height << "\n";

    std::cout << "background size" << background.size() << "\n";
    std::cout << "background noised size" << background_noised.size() << "\n";

    std::cout << "foreground size" << foreground.size() << "\n";
    std::cout << "foreground noised size" << foreground_noised.size() << "\n";
    std::cout << "foreground rotated size" << foreground_rotated.size() << "\n";
    std::cout << "foreground shifted size" << foreground_shifted.size() << "\n";

    std::cout << "mixed size" << mixed.size() << "\n";
    std::cout << "result size" << result.size() << "\n";

    return result;
    /*
    unsigned int size = width*height*channels;
    std::vector<float> result(size);

    bool background_invert = false;
    if (rand()%2)
        background_invert = true;

    bool foreground_invert = false;
    if (rand()%2)
        foreground_invert = true;

    float background_luma = rndf(-luma_noise, luma_noise);
    float foreground_luma = rndf(-luma_noise, luma_noise);

    for (unsigned int i = 0; i < size; i++)
    {
        float raw =  background[i];
        if (background_inversion)
        if (background_invert)
            raw = 1.0 - raw;

        result[i] = raw + background_luma;
    }

    if (mix_enabled)
    {
        float a = rndf(0.0, alpha);

        for (unsigned int i = 0; i < size; i++)
        {
            float raw = foreground[i];
            if (raw < threshold)
            {
                if (foreground_inversion)
                if (foreground_invert)
                    raw = 1.0 - raw;

                raw+= foreground_luma;
                result[i] = (1.0 - a)*raw + a*background[i];
            }
        }
    }


    for (unsigned int i = 0; i < size; i++)
    {
        float v = rndf(-1.0, 1.0);
        result[i] = (1.0 - noise)*result[i] + noise*v;
    }

    float max = result[0];
    float min = max;

    for (unsigned int i = 0; i < size; i++)
    {
        if (result[i] > max)
            max = result[i];
        if (result[i] < min)
            min = result[i];
    }

    float k = 0.0;
    float q = 0.0;

    if (max > min)
    {
        k = (1.0 - 0.0)/(max - min);
        q = 1.0 - k*max;
    }

    for (unsigned int i = 0; i < size; i++)
    {
        result[i] = k*result[i] + q;
    }



    return result;
    */
}



std::vector<float> DatasetDetection::add_noise(std::vector<float> &input, float white_noise, float luma_noise)
{
    std::vector<float> result(input.size());

    float luma_noise_ = luma_noise*rndf(-1.0, 1.0);

    for (unsigned int i  = 0; i < input.size(); i++)
    {
        result[i] = luma_noise_ + (1.0 - white_noise)*input[i] + white_noise*rndf(-1.0, 1.0);
    }

    return result;
}

std::vector<float> DatasetDetection::shift_noise(std::vector<float> &input, int shift_noise)
{
    std::vector<float> result(input.size());
    for (unsigned int i = 0; i < result.size(); i++)
        result[i] = 0.0;

    int shift_x = rndf(-shift_noise, shift_noise);
    int shift_y = rndf(-shift_noise, shift_noise);

    for (unsigned int k = 0; k < 3; k++)
    for (unsigned int y = 0; y < height; y++)
    for (unsigned int x = 0; x < width; x++)
    {
        int y_s = (int)y + shift_y;
        int x_s = (int)x + shift_x;

        result[to_idx(x, y, k)] = input[to_idx(x_s, y_s, k)];
    }

    return result;
}


std::vector<float> DatasetDetection::rotation_noise(std::vector<float> &input, float angle_deg)
{
    std::vector<float> result(input.size());
    for (unsigned int i = 0; i < result.size(); i++)
        result[i] = 0.0;

    float angle_range = angle_deg*3.141592654/180.0;

    float angle = rndf(-angle_range, angle_range);

    float a = cos(angle);
    float b = -sin(angle);
    float c = sin(angle);
    float d = cos(angle);

    for (unsigned int k = 0; k < 3; k++)
    for (unsigned int y = 0; y < height; y++)
    for (unsigned int x = 0; x < width; x++)
    {
        int x_rot = (int)x*a + (int)y*b;
        int y_rot = (int)x*c + (int)y*d;

        result[to_idx(x, y, k)] = input[to_idx(x_rot, y_rot, k)];
    }

    return result;
}

std::vector<float> DatasetDetection::mix_min(std::vector<float> &background, std::vector<float> &foreground, float threshold)
{
    std::vector<float> result(background.size());

    for (unsigned int i  = 0; i < background.size(); i++)
    {
        if (foreground[i] < threshold)
            result[i] = foreground[i];
        else
            result[i] = background[i];
    }

    return result;
}


std::vector<float> DatasetDetection::normalise(std::vector<float> &input)
{
    std::vector<float> result(input.size());

    float max = input[0];
    float min = max;

    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (input[i] > max)
            max = input[i];
        if (input[i] < min)
            min = input[i];
    }

    float k = 0.0;
    float q = 0.0;

    if (max > min)
    {
        k = (1.0 - 0.0)/(max - min);
        q = 1.0 - k*max;
    }

    for (unsigned int i = 0; i < input.size(); i++)
    {
        result[i] = k*input[i] + q;
    }

    return result;
}


unsigned int DatasetDetection::to_idx(int x, int y, int ch)
{
    if (x < 0)
        x = 0;
    if (x > ((int)width-1))
        x = ((int)width-1);

    if (y < 0)
        y = 0;
    if (y > ((int)height-1))
        y = ((int)height-1);

    return (ch*height + y)*width + x;
}


float DatasetDetection::rndf(float min, float max)
{
    float v = (rand()%100000)/100000.0;
    v = v*(max - min) + min;

    return v;
}
