#include <iostream>

#include <detector.h>
#include "opencv2/opencv.hpp"

#include <timer.h>

unsigned int padding(unsigned int value, unsigned int padding)
{
	unsigned int result;
	result = (value/padding)*padding;

	std::cout << result << "\n";
	return result;
}

float round_to_two(float var)
{
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}

int main()
{
	/*
	unsigned int width  = padding(640, 16);
	unsigned int height = padding(480, 16);
	*/

	unsigned int width  = padding(1024, 16);
	unsigned int height = padding(768, 16);

	cv::VideoCapture cap(0); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;

	cap.set(CV_CAP_PROP_FRAME_WIDTH,width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,height);

	unsigned int real_width = int(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	unsigned int real_height = int(cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	std::cout << real_width << " " << real_height << "\n";


	float confidence = 0.7;
	Detector detector("networks/net_1/trained/cnn_config.json", real_width, real_height, confidence);

	float fps_filtered = 0.0;

	cv::namedWindow("camera",1);

	cv::VideoWriter video_writer("/home/michal/Videos/cnn_aruco_test.avi",CV_FOURCC('M','J','P','G'),10, cv::Size(real_width,real_height));

	while (1)
	{
		cv::Mat frame;
		cap >> frame;

		timer.start();
		detector.process(frame);
		timer.stop();

		float fps = 1.0/(0.001*timer.get_duration() + 0.000000001);

		fps_filtered = 0.95*fps_filtered + 0.05*fps;

		std::cout << "FPS = " << fps_filtered << "\n";

		detector.inpaint_class_result(frame, 0.5);

		std::string str_fps_a = "resolution = [" + std::to_string(real_width) + " " + std::to_string(real_height) + "]";
		std::string str_fps_b = "fps = " + std::to_string((int)fps_filtered);
		cv::putText(frame, str_fps_a, cv::Point(30, 30), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str_fps_b, cv::Point(30, 60), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2);


		video_writer.write(frame);

		cv::imshow("camera", frame);
		if( cv::waitKey(10) == 27 )
			break; // stop capturing by pressing ESC
	}

	video_writer.release();

	std::cout << "program done\n";
 	return 0;
}
