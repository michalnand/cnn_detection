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

	return result;
}

int main()
{
	float confidence = 0.9;

	auto image = load_image("mer_input_03.jpg");
	Detector detector("networks/mars_net_0/trained/cnn_config.json", image.width, image.height, confidence);

	Timer timer;

	unsigned int loops = 10;

	timer.start();

	for (unsigned int i = 0; i < loops; i++)
		detector.process(image.data);

	timer.stop();

	detector.inpaint_class_result(image.data);

	ImageSave image_output(image.width, image.height, false);
	image_output.save("mer_output_03.jpg", image.data);

	std::cout << "network run time " << detector.get_result().computing_time << "[ms]\n";

	std::cout << "total run time per frame " << timer.get_duration()/loops << "[ms]\n";

	std::cout << "program done\n";

 	return 0;
}
