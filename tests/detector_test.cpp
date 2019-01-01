#include <iostream>

#include <cnn_detector.h>
#include <image_load.h>

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

	return result;
}

int main()
{
	auto image = load_image("images/id_03.jpg");

	CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);


	detector.process(image.data);


	std::cout << "program done\n";

 	return 0;
}
