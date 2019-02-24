#ifndef ImageAugmentation_H_
#define ImageAugmentation_H_

#include <string>
#include <vector>
#include <json_config.h>


class ImageAugmentation
{
    public:
        ImageAugmentation(std::string json_config_file_name);
        virtual ~ImageAugmentation();

        std::vector<float> process( std::vector<float> &input,
                                    unsigned int input_width,
                                    unsigned int input_height,
                                    bool is_background = false);

    private:
        void load_images(   std::vector<std::vector<float>> &result,
                            std::string path_dir,
                            unsigned int width,
                            unsigned height);

        std::vector<float> filter(std::vector<float> &input, unsigned int width, unsigned int height, unsigned int size);
        float rnd(float min, float max);


    private:
        JsonConfig json_config;
        std::vector<std::vector<float>> backgrounds;
        std::vector<std::vector<float>> foreground;

    private:
        unsigned int width, height;
        float color_noise, white_noise, offset_noise, angle_noise;
        unsigned int max_files_per_folder;

        std::string output_folder;


};


#endif
