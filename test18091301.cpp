// this program will open a video file
// opencv cannot easily read a MP4 file
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "unistd.h"

const std::string filePath{"D:\\Video\\testAVI.avi"};

int main(int argc, char **argv)
{
	cv::VideoCapture capture(filePath);

	if (!capture.isOpened())
		return 1;

	double rate = capture.get(CV_CAP_PROP_FPS);

	bool stop{ false };
	cv::Mat frame;
	cv::namedWindow("Extracted Frame",CV_WINDOW_NORMAL);

	int delay = 1000.0 / rate;

	int stopNum = 0;

	int fnumber = capture.get(CV_CAP_PROP_FRAME_COUNT);

	int starNum = 100;
	int curNum;

	while (true) {
		if (!capture.read(frame))
			break;
		cv::imshow("Extracted Frame",frame);
		stopNum++;

		if (stopNum % 50 == 0) {
			printf("FRAME (%d) : (%d)\n", fnumber, stopNum);
			
			curNum = static_cast<int>((double)stopNum / 10000*starNum);
			if (curNum == 0)
			{
				std::string dealStr("");
				std::string noDealStr(starNum - curNum, '*');
				std::cout << dealStr << noDealStr << std::endl;
			}
			else if (curNum == 1)
			{
				std::string dealStr(">");
				std::string noDealStr(starNum - curNum, '*');
				std::cout << dealStr << noDealStr << std::endl;
			}
			else
			{
				std::string dealStr(curNum - 1, '=');
				dealStr += ">";
				std::string noDealStr(starNum - curNum, '*');
				std::cout << dealStr << noDealStr << std::endl;
			}
		}
		
		cv::waitKey(30);

		//if (cv::waitKey(delay) > 0)
		//	stop = true;
	}

	capture.release();

	return 0;
}
