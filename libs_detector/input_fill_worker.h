#ifndef _INPUT_FILL_WORKER_H_
#define _INPUT_FILL_WORKER_H_

#include <worker.h>

#include <vector>
#include "opencv2/opencv.hpp"


class InputFillWorker: public Worker
{
    public:
        InputFillWorker();
        virtual ~InputFillWorker();

        void set_params(std::vector<float> *cnn_input, cv::Mat *image,
                        unsigned int min, unsigned int max,
                        unsigned int image_width, unsigned int image_height);

    protected:
        void main();

    private:
        std::vector<float> *cnn_input;
        cv::Mat *image;
        unsigned int min, max;
        unsigned int image_width, image_height;
};

#endif
