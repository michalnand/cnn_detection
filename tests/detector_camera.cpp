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

int main()
{
	/*
	unsigned int width  = padding(640, 16);
	unsigned int height = padding(480, 16);
	*/

	unsigned int width  = padding(1024, 16);
	unsigned int height = padding(768, 16);

	cv::VideoCapture cap(1); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;

	cap.set(CV_CAP_PROP_FRAME_WIDTH,width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,height);

	unsigned int real_width = int(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	unsigned int real_height = int(cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	std::cout << real_width << " " << real_height << "\n";


	float confidence = 0.7;
	Detector detector("networks/net_7/trained/cnn_config.json", real_width, real_height, confidence);

	float fps_filtered = 0.0;

	cv::namedWindow("camera",1);

	//cv::VideoWriter video_writer("output.avi",CV_FOURCC('M','J','P','G'),10, cv::Size(real_width,real_height));
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

		//video_writer.write(frame);
		cv::imshow("camera", frame);
		if( cv::waitKey(10) == 27 )
			break; // stop capturing by pressing ESC
	}

	//video_writer.release();

	std::cout << "program done\n";
 	return 0;
}
