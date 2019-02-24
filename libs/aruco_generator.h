#ifndef _ARUCO_GENERATOR_H_
#define _ARUCO_GENERATOR_H_

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

class ArucoGenerator
{
    public:
        ArucoGenerator(unsigned int image_size, unsigned int code_size);
        virtual ~ArucoGenerator();

        unsigned int get_count();
        unsigned int get_image_size();
        unsigned int get_code_size();

        std::vector<float>& get(unsigned int idx);

        void save_images(std::string path);

    private:
        void clear();
        void init(unsigned int image_size, unsigned int code_size);
        std::vector<float> mat_to_image(    cv::Mat &mat,
                                            unsigned int image_size,
                                            unsigned int code_size);

    private:
        unsigned int image_size, code_size;
        std::vector<std::vector<float>> codes;
};


#endif
