#include <image_augmentation.h>

#include <iostream>
#include <experimental/filesystem>
#include <image_load.h>
#include <image_save.h>
#include <json_config.h>


ImageAugmentation::ImageAugmentation(std::string json_config_file_name)
{
    json_config.load(json_config_file_name);

    width = json_config.result["width"].asInt();
    height = json_config.result["height"].asInt();

    color_noise = json_config.result["color noise"].asFloat();
    white_noise = json_config.result["white noise"].asFloat();
    offset_noise = json_config.result["offset noise"].asFloat();
    angle_noise = json_config.result["angle noise"].asFloat();

    max_files_per_folder = json_config.result["max files per folder"].asInt();

    for (unsigned int j = 0; j < json_config.result["background images"].size(); j++)
    {
        auto path_dir = json_config.result["background images"][j].asString();
        std::cout << "loading backgrounds from " << path_dir  << "\n";

        load_images(backgrounds, path_dir, width, height);
    }
}

ImageAugmentation::~ImageAugmentation()
{

}


std::vector<float> ImageAugmentation::process( std::vector<float> &input,
                                                unsigned int input_width,
                                                unsigned int input_height,
                                                bool is_background)
{
    std::vector<float> result;

    result = backgrounds[rand()%backgrounds.size()];

    unsigned int layer_size = width*height;
    unsigned int input_layer_size = input_width*input_height;

    if (is_background == false)
    {
        float x_offset = rnd(-offset_noise, offset_noise);
        float y_offset = rnd(-offset_noise, offset_noise);

        float angle = 2.0*3.141592654*rnd(-angle_noise, angle_noise)/360.0;

        for (unsigned int j = 0; j < input_height; j++)
            for (unsigned int i = 0; i < input_width; i++)
            {
                float x_ = i - input_width/2.0;
                float y_ = j - input_height/2.0;

                float x_transformed = (height)/2.0 + y_offset + x_*cos(angle) - y_*sin(angle);
                float y_transformed = (width)/2.0 + x_offset + x_*sin(angle) + y_*cos(angle);


                if ((x_transformed > 0.0)&&(y_transformed > 0.0)&&(x_transformed < width)&&(y_transformed < height))
                {
                    unsigned int input_idx  = j*input_width + i;

                    float r = input[input_idx + 0*input_layer_size];
                    float g = input[input_idx + 1*input_layer_size];
                    float b = input[input_idx + 2*input_layer_size];

                    unsigned int x = x_transformed;
                    unsigned int y = y_transformed;

                    result[y*width + x + layer_size*0] = r;
                    result[y*width + x + layer_size*1] = g;
                    result[y*width + x + layer_size*2] = b;

                    unsigned int x1 = x_transformed + 1;
                    unsigned int y1 = y_transformed + 0;

                    if ((x1 < width)&&(y1 < height))
                    {
                        result[y1*width + x1 + layer_size*0] = r;
                        result[y1*width + x1 + layer_size*1] = g;
                        result[y1*width + x1 + layer_size*2] = b;
                    }

                    unsigned int x2 = x_transformed + 0;
                    unsigned int y2 = y_transformed + 1;

                    if ((x2 < width)&&(y2 < height))
                    {
                        result[y2*width + x2 + layer_size*0] = r;
                        result[y2*width + x2 + layer_size*1] = g;
                        result[y2*width + x2 + layer_size*2] = b;
                    }
                }
            }
    }


    float r_noise = rnd(-color_noise, color_noise);
    float g_noise = rnd(-color_noise, color_noise);
    float b_noise = rnd(-color_noise, color_noise);

    for (unsigned int j = 0; j < height; j++)
        for (unsigned int i = 0; i < width; i++)
        {
            float r = result[(0*height + j)*width + i];
            float g = result[(1*height + j)*width + i];
            float b = result[(2*height + j)*width + i];

            r = r + r_noise + rnd(-1, 1)*white_noise;
            g = g + g_noise + rnd(-1, 1)*white_noise;
            b = b + b_noise + rnd(-1, 1)*white_noise;

            result[(0*height + j)*width + i] = r;
            result[(1*height + j)*width + i] = g;
            result[(2*height + j)*width + i] = b;
        }


    unsigned int filter_size = 1;
    switch (rand()%3)
    {
        case 0: filter_size = 1; break;
        case 1: filter_size = 3; break;
        case 2: filter_size = 5; break;
    }

    std::vector<float> filtered_result;
    if (filter_size != 1)
        filtered_result = filter(result, width, height, filter_size);
    else
        filtered_result = result;

    return filtered_result;
}

void ImageAugmentation::load_images(    std::vector<std::vector<float>> &result,
                                        std::string path_dir,
                                        unsigned int width,
                                        unsigned height)
{
    std::vector<float> mat;
    unsigned int channels = 3;
    mat.resize(height*width*channels);

    for (unsigned int i = 0; i < mat.size(); i++)
        mat[i] = 0.0;

    unsigned int count = 0;
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

            if ( (width != input_width) ||
                 (height != input_height) )
                 continue;

            for (unsigned int ch = 0; ch < channels; ch++)
            for (unsigned int y = 0; y < height; y++)
            for (unsigned int x = 0; x < width; x++)
            {
                unsigned int input_idx  = (ch*input_height + y)*input_width + x;
                unsigned int output_idx = (ch*height + y)*width + x;

                mat[output_idx] = image.get()[input_idx];
            }


            result.push_back(mat);
            count++;
            if (count > max_files_per_folder)
                return;
        }
    }
}


std::vector<float> ImageAugmentation::filter(std::vector<float> &input, unsigned int width, unsigned int height, unsigned int size)
{
    std::vector<float> result = input;

    float w = 0.5/size;

    unsigned int size_half = size/2;
    for (unsigned int ch = 0; ch < 3; ch++)
        for (unsigned int y = 0; y < height - size_half; y++)
            for (unsigned int x = 0; x < width - size_half; x++)
            {
                float sum = 0.0;
                for (unsigned int ky = 0; ky < size; ky++)
                    for (unsigned int kx = 0; kx < size; kx++)
                    {
                        unsigned int input_idx;
                        input_idx = (ch*height + (y + ky))*width + (x + kx);
                        sum+= input[input_idx]*w;
                    }

                unsigned int output_idx =  (ch*height + y + size_half)*width + x + size_half;

                result[output_idx] = sum;
            }


    return result;
}


float ImageAugmentation::rnd(float min, float max)
{
    float rf = (rand()%1000000)/1000000.0;

    return rf*(max - min) + min;
}
