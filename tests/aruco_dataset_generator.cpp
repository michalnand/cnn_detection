#include <aruco_generator.h>
#include <image_augmentation.h>
#include <iostream>

#include <string>
#include <image_save.h>



int main()
{
    srand(time(NULL));

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

    return 0;
}
