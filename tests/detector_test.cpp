#include <iostream>

#include <detector.h>


#include <cnn_detector.h>
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
	float confidence = 0.7;
	auto image = load_image("images/warehouse_02_code_24_40.png");
	Detector detector("networks/net_1/trained/cnn_config.json", image.width, image.height, confidence);

	Timer timer;

	unsigned int loops = 10;

	timer.start();

	for (unsigned int i = 0; i < loops; i++)
		detector.process(image.data);

	timer.stop();

	detector.inpaint_class_result(image.data);

	ImageSave image_output(image.width, image.height, false);
	image_output.save("output.png", image.data);

	std::cout << "network run time " << detector.get_result().computing_time << "[ms]\n";

	std::cout << "total run time per frame " << timer.get_duration()/loops << "[ms]\n";

	/*
	{
		auto image = load_image("images/hall_0_code_16_40.png");
		CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
		detector.process("result/hall_0_code_16_40.png", image.data);
	}
	{
		auto image = load_image("images/hall_1_code_16_40.png");
		CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
		detector.process("result/hall_1_code_16_40.png", image.data);
	}
	{
		auto image = load_image("images/hall_2_code_16_40.png");
		CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
		detector.process("result/hall_2_code_16_40.png", image.data);
	}
	{
		auto image = load_image("images/hall_3_code_16_40.png");
		CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
		detector.process("result/hall_3_code_16_40.png", image.data);
	}
	{
		auto image = load_image("images/hall_4_code_16_40.png");
		CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
		detector.process("result/hall_4_code_16_40.png", image.data);
	}


		{
			auto image = load_image("images/hall_0_code_64_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/hall_0_code_64_40.png", image.data);
		}
		{
			auto image = load_image("images/hall_1_code_64_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/hall_1_code_64_40.png", image.data);
		}
		{
			auto image = load_image("images/hall_2_code_64_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/hall_2_code_64_40.png", image.data);
		}
		{
			auto image = load_image("images/hall_3_code_64_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/hall_3_code_64_40.png", image.data);
		}
		{
			auto image = load_image("images/hall_4_code_64_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/hall_4_code_64_40.png", image.data);
		}
*/

/*
		{
			auto image = load_image("images/warehouse_00_code_16_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/warehouse_00_code_16_40.png", image.data);
		}
		{
			auto image = load_image("images/warehouse_01_code_16_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/warehouse_01_code_16_40.png", image.data);
		}
		{
			auto image = load_image("images/warehouse_02_code_16_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/warehouse_02_code_16_40.png", image.data);
		}
		{
			auto image = load_image("images/warehouse_03_code_16_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/warehouse_03_code_16_40.png", image.data);
		}


		{
			auto image = load_image("images/warehouse_00_code_24_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/warehouse_00_code_24_40.png", image.data);
		}
		{
			auto image = load_image("images/warehouse_01_code_24_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/warehouse_01_code_24_40.png", image.data);
		}
		{
			auto image = load_image("images/warehouse_02_code_24_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/warehouse_02_code_24_40.png", image.data);
		}
		{
			auto image = load_image("images/warehouse_03_code_24_40.png");
			CNNDetector detector("networks/net_1/trained/cnn_config.json", image.width, image.height);
			detector.process("result/warehouse_03_code_24_40.png", image.data);
		}

*/
	std::cout << "program done\n";

 	return 0;
}
