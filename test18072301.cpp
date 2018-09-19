#include <iostream>
#include <string>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char **argv)
{
	int flag = 0;
	if (argc == 2) {
		flag = 1;
		//std::cout << "name : " << argv[1] << std::endl;
		std::string imread_path("F:\\Codes\\Mine\\cpp\\OpenCV\\Image\\");

		imread_path += "MyTest.jpg";

		cv::Mat img = cv::imread(imread_path, -1);
		if (img.empty())  return -1;
		cv::namedWindow("Image", cv::WINDOW_NORMAL);
		cv::imshow("Image", img);
		cv::waitKey(0);
		cv::destroyWindow("Image");
	}
	else {
		;
	}
	std::cout << (flag ? "True" : "False") << std::endl;
	for (int i = 0; i < argc; ++i)
		std::cout << "argv[" << i << "] : " << argv[i] << std::endl;
	
	char c;
	std::cin >> c;

	return 0;
}
