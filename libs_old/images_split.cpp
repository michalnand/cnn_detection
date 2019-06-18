#include <images_split.h>

#include <iostream>
#include <experimental/filesystem>
#include <image_load.h>
#include <image_save.h>
#include <json_config.h>

ImagesSplit::ImagesSplit(std::string config_file_name)
{
    JsonConfig json(config_file_name);

    std::string output_prefix       = json.result["output prefix"].asString();
    unsigned int output_size        = json.result["output size"].asInt();
    unsigned int output_stride      = json.result["output stride"].asInt();
    unsigned int crop               = json.result["crop"].asInt();
    unsigned int max_images_count   = json.result["max images count"].asInt();

    white_noise = json.result["white noise"].asFloat();
    luma_noise  = json.result["luma noise"].asFloat();
    angle_noise = json.result["angle noise"].asFloat();


    use_background = json.result["use background"].asFloat();
    background_threshold = 0.0;
    if (use_background)
    {
        background_threshold = json.result["background parameters"]["threshold"].asFloat();
        unsigned int images_per_dir = json.result["background parameters"]["background images per dir"].asInt();

        for (unsigned int j = 0; j < json.result["background parameters"]["background dirs"].size(); j++)
        {
            std::string path_dir = json.result["background parameters"]["background dirs"][j].asString();
            unsigned int images_count = 0;

            for (auto & p : std::experimental::filesystem::directory_iterator(path_dir))
            if (images_count < images_per_dir)
            {
                std::string image_file_name;
                image_file_name = p.path();

                std::cout << "loading background " << image_file_name  << "\n";


                if  (   (std::experimental::filesystem::path(image_file_name).extension() == ".png") ||
                        (std::experimental::filesystem::path(image_file_name).extension() == ".jpg") )
                {
                    ImageLoad image;
                    image.load(image_file_name, false, true);

                    backgrounds.push_back(image.get());
                    images_count++;
                }
            }
        }

    }




    unsigned int source_image_idx = 0;
    processed_count = 0;

    for (unsigned int j = 0; j < json.result["input dirs"].size(); j++)
    {
        std::string path_dir = json.result["input dirs"][j].asString();

        for (auto & p : std::experimental::filesystem::directory_iterator(path_dir))
        {
            std::string image_file_name;
            image_file_name = p.path();

            if  (   (std::experimental::filesystem::path(image_file_name).extension() == ".png") ||
                    (std::experimental::filesystem::path(image_file_name).extension() == ".jpg") )
            {
                std::cout << "processing " << image_file_name  << "\n";

                std::string output_file_name_prefix;
                output_file_name_prefix = output_prefix + std::to_string(source_image_idx);

                ImageLoad image;
                image.load(image_file_name, false, true);

                split_image(    output_file_name_prefix,
                                image,
                                output_size,
                                output_stride,
                                crop);

                source_image_idx++;
                std::cout << "images count " << processed_count << "\n";
                if (processed_count > max_images_count)
                    return;
            }
        }
    }
}

ImagesSplit::~ImagesSplit()
{

}


void ImagesSplit::split_image(  std::string output_file_name_prefix,
                                ImageLoad &image,
                                unsigned int size, unsigned int stride, unsigned int crop)
{
    unsigned int image_height   = image.height();
    unsigned int image_width    = image.width();

    unsigned int tmp;

    if (size > crop)
        tmp = size;
    else
        tmp = crop;

    unsigned int width  = image_width - tmp;
    unsigned int height = image_height - tmp;

    std::vector<float> input_v = image.get();
    std::vector<float> result_v(size*size*3);

    ImageSave result_image(size, size, false);

    float luma  = rndf(-luma_noise, luma_noise);

    /*
    float angle = rndf(-angle_noise, angle_noise)*3.141592654/180.0;

    float a = cos(angle);
    float b = -sin(angle);
    float c = sin(angle);
    float d = cos(angle);
    */

    for (unsigned int y = crop; y < height; y+=stride)
    for (unsigned int x = crop; x < width; x+=stride)
    {
        if (use_background)
        {
            unsigned int background_image = rand()%backgrounds.size();

            for (unsigned int i = 0; i < result_v.size(); i++)
                result_v[i] = backgrounds[background_image][i];
        }
        else
        {
            for (unsigned int i = 0; i < result_v.size(); i++)
                result_v[i] = 0.0;
        }

        for (unsigned int ch = 0; ch < 3; ch++)
        for (unsigned int ky = 0; ky < size; ky++)
        for (unsigned int kx = 0; kx < size; kx++)
        {
            unsigned int input_idx  = (ch*image_height + y + ky)*image_width + x + kx;
            unsigned int output_idx = (ch*size + ky)*size + kx;

            float pixel = input_v[input_idx];
            float noised_output = (1.0 - white_noise)*pixel + white_noise*rndf(-1.0, 1.0) + luma;

            if (use_background)
            {
                if (pixel < background_threshold)
                    result_v[output_idx] = noised_output;
            }
            else
            {
                result_v[output_idx] = noised_output;
            }
        }

        std::string file_name = output_file_name_prefix + "_" + std::to_string(y) + "_" + std::to_string(x) + ".png";
        result_image.save(file_name, result_v);

        processed_count++;
    }
}


float ImagesSplit::rndf(float min, float max)
{
    float result = (rand()%100000)/100000.0;
    result = (max - min)*result + min;
    return result;
}
