#include <cnn_detector.h>
#include <json_config.h>
#include <iostream>
#include <image_save.h>

CNNDetector::CNNDetector(std::string network_config_file_name, unsigned int image_width, unsigned int image_height)
{
    JsonConfig json(network_config_file_name);

    unsigned int last_layer = json.result["layers"].size() - 1;

    this->image_width = image_width;
    this->image_height = image_height;

    sGeometry input_geometry;
    input_geometry.w = image_width;
    input_geometry.h = image_height;
    input_geometry.d = 3;

    sGeometry output_geometry;

    unsigned int output_kernel_width    = json.result["layers"][last_layer]["input_geometry"][0].asInt();
    unsigned int output_kernel_height   = json.result["layers"][last_layer]["input_geometry"][1].asInt();
    unsigned int output_kernel_depth    = json.result["layers"][last_layer]["output_geometry"][2].asInt();

    width_ratio    = json.result["input_geometry"][0].asInt()/output_kernel_width;
    height_ratio   = json.result["input_geometry"][1].asInt()/output_kernel_height;

    output_geometry.w  = output_kernel_width;
    output_geometry.h  = output_kernel_height;
    output_geometry.d  = output_kernel_depth;

    /*
    std::cout << "last layer id "  << last_layer << "\n";
    std::cout << "last layer name " << json.result["layers"][last_layer]["type"].asString() << "\n";
    std::cout << width_ratio << " " << height_ratio << "\n";
    */

    output_width    = input_geometry.w/width_ratio;
    output_height   = input_geometry.h/height_ratio;
    output_depth    = output_kernel_depth;



    std::cout << output_width << "\n";
    std::cout << output_height << "\n";
    std::cout << output_depth << "\n";



    nn = new CNN(json.result, input_geometry, output_geometry, true);


    result_init();

    unsigned int size = output_width*output_height*output_depth;
    nn_output.resize(size);
    for (unsigned int j = 0; j < nn_output.size(); j++)
        nn_output[j] = 0.0;
}


CNNDetector::~CNNDetector()
{
    if (nn != nullptr)
        delete nn;
}


void CNNDetector::process(std::vector<float> &image_v)
{
    nn->forward(nn_output, image_v);

    unsigned int ptr;

    ptr = 0;
    for (unsigned int k = 0; k < output_depth; k++)
    for (unsigned int j = 0; j < output_height; j++)
    for (unsigned int i = 0; i < output_width; i++)
    {
        result.confidence_result[k][j][i] = nn_output[ptr];
        ptr++;
    }


    for (unsigned int j = 0; j < output_height; j++)
    for (unsigned int i = 0; i < output_width; i++)
    {
        unsigned int max_k = 0;
        for (unsigned int k = 0; k < output_depth; k++)
        {
            float conf_best = result.confidence_result[max_k][j][i];
            float conf = result.confidence_result[k][j][i];
            if (conf > 0.998)
            if (conf > conf_best)
                max_k = k;
        }

        result.class_result[j][i] = max_k;
    }

    // filter(result.class_result);


    for (unsigned int k = 0; k < output_depth; k++)
    {
        std::vector<float> img_data(output_width*output_height);
        ptr = 0;
        for (unsigned int j = 0; j < output_height; j++)
        {
            for (unsigned int i = 0; i < output_width; i++)
            {
                if (result.class_result[j][i] == k)
                    img_data[ptr] = 1.0;
                else
                    img_data[ptr] = 0.0;
                ptr++;
            }
        }


        std::string file_name = "result/" + std::to_string(k) + ".png";
        ImageSave image(output_width, output_height, true);
        image.save(file_name, img_data);
    }


    std::vector<float> img_data(image_width*image_height*3);
    for (unsigned int j = 0; j < img_data.size(); j++)
        img_data[j] = 0.0;

    unsigned int padding = 16;

    float alpha = 0.3;
    for (unsigned int k = 0; k < 3; k++)
        for (unsigned int j = padding; j < image_height - padding; j++)
            for (unsigned int i = padding; i < image_width - padding; i++)
            {
                unsigned int idx = (k*image_height + j)*image_width + i;
                unsigned int res_j = (j)/height_ratio;
                unsigned int res_i = (i - padding)/width_ratio;


                //img_data[idx] = alpha*image_v[idx] + (1.0 - alpha)*result.confidence_result[k][res_j][res_i];

                unsigned int class_id = result.class_result[res_j][res_i];
                if (class_id != 0)
                {
                    float v = alpha*image_v[idx] + (1.0 - alpha)*class_color(class_id, output_depth-1)[k];
                    img_data[idx] = v;
                }
                else
                {
                    img_data[idx] = image_v[idx];
                }

            }

    ImageSave image(image_width, image_height, false);
    image.save("result/result.png", img_data);
}

void CNNDetector::result_init()
{
    result.output_width     = output_width;
    result.output_height    = output_height;
    result.output_depth     = output_depth;

    result.class_result.resize(output_height);
    for (unsigned int j = 0; j < output_height; j++)
    {
        result.class_result[j].resize(output_width);
        for (unsigned int i = 0; i < output_width; i++)
            result.class_result[j][i] = 0;
    }

    result.confidence_result.resize(output_depth);
    for (unsigned int k = 0; k < output_depth; k++)
    {
        result.confidence_result[k].resize(output_height);
        for (unsigned int j = 0; j < output_height; j++)
        {
            result.confidence_result[k][j].resize(output_width);
            for (unsigned int i = 0; i < output_width; i++)
                result.confidence_result[k][j][i] = 0.0;
        }
    }
}




std::vector<float> CNNDetector::class_color(unsigned int id, unsigned int max_count)
{
    std::vector<float> result(3);
    float c = 2.0*3.141592654;
    float phase = id*c/max_count;
    result[0] = (sin(phase + c*0.0*1.0/3.0) + 1.0)/2.0;
    result[1] = (sin(phase + c*1.0*1.0/3.0) + 1.0)/2.0;
    result[2] = (sin(phase + c*2.0*1.0/3.0) + 1.0)/2.0;

    return result;
}

void CNNDetector::filter(std::vector<std::vector<unsigned int>> &class_result)
{

    std::vector<std::vector<unsigned int>> class_result_new;

    class_result_new.resize(output_height);
    for (unsigned int j = 0; j < output_height; j++)
    {
        class_result_new[j].resize(output_width);
        for (unsigned int i = 0; i < output_width; i++)
            class_result_new[j][i] = class_result[j][i];
    }

    unsigned int radius = 5;
    for (unsigned int r = 0; r < radius; r++)
    {
    for (unsigned int j = 1; j < output_height-1; j++)
        for (unsigned int i = 1; i < output_width-1; i++)
            if (class_result[j][i] == 0)
            {
                unsigned int sum = 0;

                sum+= class_result[j][i+1];
                sum+= class_result[j][i-1];
                sum+= class_result[j+1][i];
                sum+= class_result[j-1][i];
                sum+= class_result[j+1][i+1];
                sum+= class_result[j+1][i-1];
                sum+= class_result[j-1][i+1];
                sum+= class_result[j-1][i-1];

                    sum = sum/8;

                    class_result_new[j][i] = sum;
            }

    class_result = class_result_new;
    }
}
