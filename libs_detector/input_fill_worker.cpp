#include <input_fill_worker.h>

InputFillWorker::InputFillWorker()
                :Worker()
{
    this->cnn_input = nullptr;
    this->image = nullptr;
    this->min = 0;
    this->max = 0;
    this->image_width = 0;
    this->image_height = 0;
}

InputFillWorker::~InputFillWorker()
{

}

void InputFillWorker::set_params(std::vector<float> *cnn_input, cv::Mat *image,
                                    unsigned int min, unsigned int max,
                                    unsigned int image_width, unsigned int image_height)
{
    this->cnn_input = cnn_input;
    this->image = image;
    this->min = min;
    this->max = max;
    this->image_width = image_width;
    this->image_height = image_height;
}


void InputFillWorker::main()
{
    unsigned int layer_size = image_width*image_height;
    unsigned int input_idx  = min*image_width;

    for (unsigned int y = min; y < max; y++)
    {
        cv::Vec3b* row = image->ptr<cv::Vec3b>(y);

        for (unsigned int x = 0; x < image_width; x++)
        {
            float r = row[x][2]/256.0;
            float g = row[x][1]/256.0;
            float b = row[x][0]/256.0;

            (*cnn_input)[input_idx + 0*layer_size] = r;
            (*cnn_input)[input_idx + 1*layer_size] = g;
            (*cnn_input)[input_idx + 2*layer_size] = b;

            input_idx++;
        }
    }
}
