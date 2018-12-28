#ifndef _DATASET_DETECTION_H_
#define _DATASET_DETECTION_H_

#include <string>
#include <dataset_interface.h>

class DatasetDetection: public DatasetInterface
{
    private:


        std::vector<std::vector<float>>              background;
        std::vector<std::vector<std::vector<float>>> foreground;

    public:
        DatasetDetection(std::string json_file_name);
        virtual ~DatasetDetection();

    private:
        void load_images(std::vector<std::vector<float>> &result, std::string path_dir, unsigned int downscale = 1);
        std::vector<float> mix_min( std::vector<float> &background,
                                    std::vector<float> &foreground,
                                    float threshold,
                                    float alpha,
                                    bool background_inversion,
                                    bool foreground_inversion,
                                    float noise,
                                    bool mix_enabled);

};


#endif
