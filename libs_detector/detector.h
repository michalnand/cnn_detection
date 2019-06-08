#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include <vector>
#include <string>
#include <cnn_deployment.h>

#include <json_config.h>

#include "opencv2/opencv.hpp"

typedef std::vector<std::vector<unsigned int>>           Class;


struct sDetectorResult
{
    Class class_result;

    unsigned int output_width  ;
    unsigned int output_height ;
    unsigned int classes_count ;

    float computing_time, network_computing_time;

    Json::Value json;
    std::string json_string;
};

class Detector
{
    private:
        sDetectorResult result;

        CNNDeployment *cnn;

        unsigned int image_width, image_height;
        float confidence;

        std::vector<float> cnn_input, cnn_output;
        std::vector<std::vector<std::vector<float>>> softmax_output;

        unsigned int width_ratio, height_ratio;
        unsigned int output_width, output_height, output_depth;

    private:
        unsigned int padding;
        std::vector<std::vector<float>> color_palette;

    public:
        Detector(std::string network_config_file_name, unsigned int image_width, unsigned int image_height, float = 0.9);
        virtual ~Detector();

        void process(std::vector<float> &image_v);
        void process(cv::Mat &image);

        sDetectorResult &get_result();
        void inpaint_class_result(std::vector<float> &image_v, float alpha = 0.3);
        void inpaint_class_result(cv::Mat &image, float alpha = 0.3);

        std::vector<float> get_mask();

        void set_padding(unsigned int padding);

    private:
        void result_init();

        std::vector<std::vector<float>> generate_color_palette(unsigned int count);
        std::vector<float>& get_class_color(unsigned int class_id);

        float cnn_output_get(unsigned int x, unsigned y, unsigned ch);
        void fill_softmax();
        void fill_cnn_input_thread(cv::Mat *image, unsigned int min, unsigned int max);

};


#endif
