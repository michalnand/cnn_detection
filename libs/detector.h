#ifndef _DETECTOR_H_
#define _DETECTOR_H_


#include <vector>
#include <string>
#include <cnn.h>

#include "opencv2/opencv.hpp"

typedef std::vector<std::vector<unsigned int>>           Class;
typedef std::vector<std::vector<std::vector<float>>>     Confidence;


struct sDetectorResult
{
    Class class_result;
    Confidence  confidence_result;

    unsigned int output_width  ;
    unsigned int output_height ;
    unsigned int classes_count ;

    float computing_time;
};

class Detector
{
    private:
        sDetectorResult result;

        CNN *cnn;
        unsigned int image_width, image_height;
        float confidence;

        std::vector<float> cnn_output;

        unsigned int width_ratio, height_ratio;
        unsigned int output_width, output_height, output_depth;

    public:
        Detector(std::string network_config_file_name, unsigned int image_width, unsigned int image_height, float = 0.9);
        virtual ~Detector();

        void process(std::vector<float> &image_v);
        void process(cv::Mat &frame);

        sDetectorResult &get_result();
        void inpaint_class_result(std::vector<float> &image_v, float alpha = 0.3);

    private:
        void result_init();
};


#endif
