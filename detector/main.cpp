#include <iostream>


#include <opencv_detector.h>

int main()
{
	OpenCVDetector opencv_detector("detector_config.json");

	while (opencv_detector.process_frame() == 0)
	{
 
	}
	std::cout << "program done\n";

	return 0;
}
