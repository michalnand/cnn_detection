#ifndef _IMAGES_SPLIT_H_
#define _IMAGES_SPLIT_H_

#include <string>
#include <image_load.h>

class ImagesSplit
{
    private:
        unsigned int processed_count;
        float white_noise, luma_noise, angle_noise;
    public:
        ImagesSplit(std::string config_file_name);
        virtual ~ImagesSplit();

        void split_image(   std::string output_file_name_prefix,
                            ImageLoad &image,
                            unsigned int size, unsigned int stride, unsigned int crop);

    private:

        float rndf(float min = 0.0, float max = 1.0);

    private:
        bool use_background;
        float background_threshold;
        std::vector<std::vector<float>> backgrounds;
};


#endif
