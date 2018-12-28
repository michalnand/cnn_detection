#ifndef _CNN_DETECTOR_H_
#define _CNN_DETECTOR_H_

#include <vector>
#include <cnn.h>

typedef std::vector<std::vector<unsigned int>>           ConfMat;
typedef std::vector<std::vector<std::vector<float>>>     ConfTensor;

struct sCNNDetectorResult
{
    unsigned int output_width, output_height, output_depth;

    ConfMat class_result;
    ConfTensor confidence_result;
};

class CNNDetector
{
    private:
        CNN *nn;

        std::vector<float> nn_output;
        unsigned int image_width, image_height;
        unsigned int output_width, output_height, output_depth;
        unsigned int width_ratio, height_ratio;
    private:

        sCNNDetectorResult result;

    public:
        CNNDetector(std::string network_config_file_name, unsigned int image_width, unsigned int image_height);
        virtual ~CNNDetector();

        void process(std::vector<float> &image_v);


    private:
        void result_init();
        void filter(std::vector<std::vector<unsigned int>> &class_result);
        std::vector<float> class_color(unsigned int id, unsigned int max_count);

};

#endif
