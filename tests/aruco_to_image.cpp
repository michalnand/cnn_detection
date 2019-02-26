#include <aruco_generator.h>
#include <image_augmentation.h>
#include <iostream>

#include <string>
#include <image_load.h>
#include <image_save.h>

std::vector<float> filter(std::vector<float> &input, unsigned int width, unsigned int height, unsigned int size)
{
    std::vector<float> result = input;

    float w = 1.0/size;

    for (unsigned int ch = 0; ch < 3; ch++)
        for (unsigned int y = 0; y < height - size; y++)
            for (unsigned int x = 0; x < width - size; x++)
            {
                float sum = 0.0;
                for (unsigned int ky = 0; ky < size; ky++)
                    for (unsigned int kx = 0; kx < size; kx++)
                    {
                        unsigned int input_idx;
                        input_idx = (ch*height + (y + ky))*width + (x + kx);
                        sum+= input[input_idx]*w;
                    }

                unsigned int output_idx =  (ch*height + y)*width + x;

                result[output_idx] = sum;
            }


    return result;
}

void generate_code_image(std::string input_file_name, unsigned int code_size, unsigned int codes_count)
{
    ImageLoad input_image(input_file_name + ".png", false, true);
    auto v = input_image.get();

    unsigned int output_layer_size = input_image.width()*input_image.height();
    unsigned int input_layer_size = code_size*code_size;

    ArucoGenerator code_generator(code_size, code_size);

    for (unsigned int code = 0; code < codes_count; code++)
    {
        auto code_image = code_generator.get(rand()%code_generator.get_count());
        //std::vector<float> code_image(code_size*code_size*3);

        unsigned int x = code_size + rand()%(input_image.width() - 2*code_size);
        unsigned int y = code_size + rand()%(input_image.height() - 2*code_size);

        for (unsigned j = 0; j < code_size; j++)
            for (unsigned i = 0; i < code_size; i++)
            {
                unsigned int output_idx = (j + y)*input_image.width() + i + x;
                unsigned int input_idx  = j*code_size + i;


                v[output_idx + output_layer_size*0] = code_image[input_idx + input_layer_size*0];
                v[output_idx + output_layer_size*1] = code_image[input_idx + input_layer_size*1];
                v[output_idx + output_layer_size*2] = code_image[input_idx + input_layer_size*2];
            }
    }

    auto filtered = filter(v, input_image.width(), input_image.height(), 3);

    std::string output_file_name = input_file_name + "_code_" + std::to_string(code_size) + "_" + std::to_string(codes_count) + ".png";
    ImageSave image(input_image.width(), input_image.height(), false);
    image.save(output_file_name, filtered);

}

int main()
{
    srand(time(NULL));

    /*
    for (unsigned int code_size = 16; code_size <= 64; code_size+= 8)
    {
        generate_code_image("images/hall_0", code_size, 40);
        generate_code_image("images/hall_1", code_size, 40);
        generate_code_image("images/hall_2", code_size, 40);
        generate_code_image("images/hall_3", code_size, 40);
        generate_code_image("images/hall_4", code_size, 40);
    }
    */

    for (unsigned int code_size = 16; code_size <= 64; code_size+= 8)
    {
        generate_code_image("images/warehouse_00", code_size, 40);
        generate_code_image("images/warehouse_01", code_size, 40);
        generate_code_image("images/warehouse_02", code_size, 40);
        generate_code_image("images/warehouse_03", code_size, 40);
    }

    /*
    unsigned int id = 0;
    unsigned int image_size = 64;

    ImageAugmentation image_augmentation("marker_config.json");

    ImageSave image(image_size, image_size, false);

    for (unsigned int code_size = 16; code_size <= 64; code_size+= 8)
    {
        std::cout << "generating codes with size " << code_size << "\n";
        ArucoGenerator code_generator(code_size, code_size);

        std::cout << "saving images\n";

        for (unsigned int i = 0; i < 10000; i++)
        {
            auto background = code_generator.get(rand()%code_generator.get_count());
            auto result = image_augmentation.process(background, code_size, code_size);

            std::string file_name = "/home/michal/dataset/images_dataset/aruco_code/" + std::to_string(code_size) + "/" + std::to_string(id) + ".png";
            image.save(file_name, result);
            id++;
        }
    }
    */
    return 0;
}
