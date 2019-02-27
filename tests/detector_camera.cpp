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
	unsigned int width  = padding(1920, 16);
	unsigned int height = padding(1080, 16);
	*/

	unsigned int width  = padding(640, 16);
	unsigned int height = padding(480, 16);

	cv::VideoCapture cap(0); // open the default camera
   	if(!cap.isOpened())  // check if we succeeded
	   return -1;

	cap.set(CV_CAP_PROP_FRAME_WIDTH,width);
 	cap.set(CV_CAP_PROP_FRAME_HEIGHT,height);


	unsigned int real_width = int(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	unsigned int real_height = int(cap.get(CV_CAP_PROP_FRAME_HEIGHT));


	std::cout << real_width << " " << real_height << "\n";

   cv::Mat edges;
   cv::namedWindow("camera",1);
   for(;;)
   {
	   cv::Mat frame;
	   cap >> frame;
	   cv::imshow("camera", frame);
	   if( cv::waitKey(10) == 27 )
	   	break; // stop capturing by pressing ESC
   }

	std::cout << "program done\n";

 	return 0;
}
