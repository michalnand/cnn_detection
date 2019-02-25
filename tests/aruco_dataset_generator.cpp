#include <aruco_generator.h>
#include <image_augmentation.h>
#include <iostream>

#include <string>
#include <image_save.h>



int main()
{
    srand(time(NULL));

    unsigned int id = 0, back_id = 0;
    unsigned int image_size = 64;

    ImageAugmentation image_augmentation("aruco_dataset_generator.json");

    ImageSave image(image_size, image_size, false);

    for (unsigned int code_size = 16; code_size <= 64; code_size+= 8)
    {
        std::cout << "generating codes with size " << code_size << "\n";
        ArucoGenerator code_generator(code_size, code_size);

        std::cout << "saving images\n";

        for (unsigned int i = 0; i < 10000; i++)
        {
            auto code_image = code_generator.get(rand()%code_generator.get_count());

            {
                auto result = image_augmentation.process(code_image, code_size, code_size, false);
                std::string file_name = "/home/michal/dataset/marker_detection/aruco_code/" + std::to_string(code_size) + "/" + std::to_string(id) + ".png";
                image.save(file_name, result);
                id++;
            }

            {
                auto result = image_augmentation.process(code_image, code_size, code_size, true);
                std::string file_name = "/home/michal/dataset/marker_detection/background/" + std::to_string(back_id%10) + "/" + std::to_string(back_id) + ".png";
                image.save(file_name, result);
                back_id++;
            }
        }
    }

    return 0;
}
