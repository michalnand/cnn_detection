#include <aruco_generator.h>

#include <opencv2/aruco.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <image_save.h>

#include <iostream>

ArucoGenerator::ArucoGenerator(unsigned int image_size, unsigned int code_size)
{
    init(image_size, code_size);
}

ArucoGenerator::~ArucoGenerator()
{

}

unsigned int ArucoGenerator::get_count()
{
    return codes.size();
}

unsigned int ArucoGenerator::get_image_size()
{
    return image_size;
}

unsigned int ArucoGenerator::get_code_size()
{
    return code_size;
}

std::vector<float>& ArucoGenerator::get(unsigned int idx)
{
    return codes[idx];
}

void ArucoGenerator::save_images(std::string path)
{
    ImageSave image(get_image_size(), get_image_size(), false);

    for (unsigned int j = 0; j < get_count(); j++)
    {
        std::string file_name = path + std::to_string(j) + ".png";
        auto v = get(j);
        image.save(file_name, v);
    }
}

void ArucoGenerator::clear()
{
    for (unsigned int j = 0; j < codes.size(); j++)
    {
        codes[j].clear();
    }

    codes.clear();
}


void ArucoGenerator::init(unsigned int image_size, unsigned int code_size)
{
    clear();

    this->image_size = image_size;
    this->code_size = code_size;

    unsigned int count = 1000;

    cv::Mat marker_mat;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_1000);

    codes.resize(count);

    for (unsigned int id = 0; id < count; id++)
    {
        cv::aruco::drawMarker(dictionary, id, code_size, marker_mat, 1);

        codes[id] = mat_to_image(marker_mat, image_size, code_size);
    }
}


std::vector<float> ArucoGenerator::mat_to_image(    cv::Mat &mat,
                                                    unsigned int image_size,
                                                    unsigned int code_size)
{
    unsigned int size = 3*image_size*image_size;
    unsigned int layer_size = image_size*image_size;

    std::vector<float> result(size);

    for (unsigned int j = 0; j < result.size(); j++)
        result[j] = 0.0;

    for (unsigned int j = 0; j < code_size; j++)
    {
        for (unsigned int i = 0; i < code_size; i++)
        {
            float value = mat.at<unsigned char>(i, j)/255.0;


            unsigned int x = (image_size - code_size)/2 + i;
            unsigned int y = (image_size - code_size)/2 + j;

            unsigned int idx = y*image_size + x;

            result[idx + layer_size*0] = value;
            result[idx + layer_size*1] = value;
            result[idx + layer_size*2] = value;
        }
    }

    return result;
}
