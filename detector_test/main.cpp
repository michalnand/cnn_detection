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
	unsigned int padding = 32;
	ImageLoad image(file_name, false, true);

	unsigned width 	= ((image.width() + padding)/padding)*padding;
	unsigned height = ((image.height() + padding)/padding)*padding;

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
		unsigned int output_idx = (k*height + j + padding/2)*width + i + padding/2;
		result.data[output_idx] = image.get()[input_idx];
	}

/*
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
*/

	return result;
}


void process_image(std::string output_file_name_prefix, std::string input_file_name, std::string network_file_name)
{
	float confidence = 0.75;

	auto image = load_image(input_file_name);
	Detector detector(network_file_name, image.width, image.height, confidence);

	Timer timer;

	timer.start();

	detector.set_padding(32);
	detector.process(image.data);

	timer.stop();

	detector.inpaint_class_result(image.data);
	ImageSave image_output(image.width, image.height, false);
	image_output.save(output_file_name_prefix + "_result.jpg", image.data);

	auto mask = detector.get_mask();
	ImageSave image_mask_output(image.width, image.height, false);
	image_mask_output.save(output_file_name_prefix + "_mask.jpg", mask);

	std::cout << "network run time " << detector.get_result().computing_time << "[ms]\n";
}

int main()
{
	std::string result_path = "/home/michal/programming/cnn_detection/detector_test/bin/neurons_result/";

	std::string source_path = "/home/michal/programming/cnn_detection/detector_test/bin/neurons_source/";

	process_image(result_path + "image0057", source_path + "image0057.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0058", source_path + "image0058.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0059", source_path + "image0059.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0060", source_path + "image0060.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0061", source_path + "image0061.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0062", source_path + "image0062.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0063", source_path + "image0063.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0064", source_path + "image0064.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0065", source_path + "image0065.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0066", source_path + "image0066.tif", "networks/cells_net_0/trained/cnn_config.json");
	process_image(result_path + "image0067", source_path + "image0067.tif", "networks/cells_net_0/trained/cnn_config.json");

	std::cout << "program done\n";

 	return 0;
}
