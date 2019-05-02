#include <iostream>

#include <detector.h>

#include <image_load.h>
#include <image_save.h>


#include <timer.h>



struct sImagePadded
{
	unsigned int width;
	unsigned int height;
	std::vector<float> data;
};

sImagePadded load_image(std::string file_name)
{
	unsigned int padding = 16;
	ImageLoad image(file_name, false, true);

	unsigned width 	= ((image.width()+padding)/padding)*padding;
	unsigned height = ((image.height()+padding)/padding)*padding;

	sImagePadded result;
	result.width = width;
	result.height = height;
	result.data.resize(width*height*3);

	std::cout << width << "\n";
	std::cout << height << "\n";

	for (unsigned int k = 0; k < 3; k++)
	for (unsigned int j = 0; j < image.height(); j++)
	for (unsigned int i = 0; i < image.width(); i++)
	{
		unsigned int input_idx = (k*image.height() + j)*image.width() + i;
		unsigned int output_idx = (k*height + j)*width + i;
		result.data[output_idx] = image.get()[input_idx];
	}

	float max = result.data[0];
	float min = result.data[0];

	for (unsigned int i = 0; i < result.data.size(); i++)
	{
		if (result.data[i] > max)
			max = result.data[i];

		if (result.data[i] < min)
			min = result.data[i];
	}

	float k = 0.0;
	float q = 0.0;

	if (max > min)
	{
		k = 1.0/(max - min);
		q = 1.0 - k*max;
	}

	std::cout << "MIN = " << min << "\n";
	std::cout << "MAX = " << max << "\n";

	for (unsigned int i = 0; i < result.data.size(); i++)
		result.data[i] = result.data[i]*k + q;


	return result;
}


void process_image(std::string output_file_name, std::string input_file_name, std::string network_file_name)
{
	float confidence = 0.0;

	auto image = load_image(input_file_name);
	Detector detector(network_file_name, image.width, image.height, confidence);

	Timer timer;

	timer.start();

	detector.process(image.data);

	timer.stop();

	detector.inpaint_class_result(image.data);

	ImageSave image_output(image.width, image.height, false);
	image_output.save(output_file_name, image.data);

	std::cout << "network run time " << detector.get_result().computing_time << "[ms]\n";
}

int main()
{
	/*
	std::string file_name_prefix =
	float confidence = 0.99;

	auto image = load_image("/home/michal/cells/source/3.png");
	Detector detector("networks/cells_net_0/trained/cnn_config.json", image.width, image.height, confidence);

	Timer timer;

	unsigned int loops = 10;

	timer.start();

	for (unsigned int i = 0; i < loops; i++)
		detector.process(image.data);

	timer.stop();

	detector.inpaint_class_result(image.data);

	ImageSave image_output(image.width, image.height, false);
	image_output.save("/home/michal/cells/source/3_result.png", image.data);

	std::cout << "network run time " << detector.get_result().computing_time << "[ms]\n";

	std::cout << "total run time per frame " << timer.get_duration()/loops << "[ms]\n";
	*/

	process_image("/home/michal/cells/source/0_result.png", "/home/michal/cells/source/0.png", "networks/cells_net_0/trained/cnn_config.json");
	process_image("/home/michal/cells/source/1_result.png", "/home/michal/cells/source/1.png", "networks/cells_net_0/trained/cnn_config.json");
	process_image("/home/michal/cells/source/2_result.png", "/home/michal/cells/source/2.png", "networks/cells_net_0/trained/cnn_config.json");
	process_image("/home/michal/cells/source/3_result.png", "/home/michal/cells/source/3.png", "networks/cells_net_0/trained/cnn_config.json");
	process_image("/home/michal/cells/source/4_result.png", "/home/michal/cells/source/4.png", "networks/cells_net_0/trained/cnn_config.json");
	process_image("/home/michal/cells/source/5_result.png", "/home/michal/cells/source/5.png", "networks/cells_net_0/trained/cnn_config.json");
	process_image("/home/michal/cells/source/6_result.png", "/home/michal/cells/source/6.png", "networks/cells_net_0/trained/cnn_config.json");
	process_image("/home/michal/cells/source/7_result.png", "/home/michal/cells/source/7.png", "networks/cells_net_0/trained/cnn_config.json");
	process_image("/home/michal/cells/source/8_result.png", "/home/michal/cells/source/8.png", "networks/cells_net_0/trained/cnn_config.json");

	std::cout << "program done\n";

 	return 0;
}
