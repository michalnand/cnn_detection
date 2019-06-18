#ifndef _DATASET_DETECTION_H_
#define _DATASET_DETECTION_H_

#include <string>
#include <dataset_interface.h>

class DatasetDetection: public DatasetInterface
{
    private:
        std::vector<std::vector<float>>              background;
        std::vector<std::vector<std::vector<float>>> foreground;

    private:
        float white_noise_level, luma_noise_level, rotation_noise_level;
        int shift_noise_level;
        float mix_threshold;

    public:
        DatasetDetection(std::string json_file_name);
        virtual ~DatasetDetection();

    private:
        void load_images(std::vector<std::vector<float>> &result, std::string path_dir, unsigned int downscale = 1);
        std::vector<float> process_image( std::vector<float> &background,
                                          std::vector<float> &foreground,
                                          bool mix_enabled);

    private:

        std::vector<float> add_noise(std::vector<float> &input, float white_noise, float luma_noise);
        std::vector<float> shift_noise(std::vector<float> &input, int shift_noise);
        std::vector<float> rotation_noise(std::vector<float> &input, float angle_deg);
        std::vector<float> mix_min(std::vector<float> &background, std::vector<float> &foreground, float threshold);
        std::vector<float> normalise(std::vector<float> &input);

        unsigned int to_idx(int x, int y, int ch);
        float rndf(float min = 0.0, float max = 1.0);

};


#endif
