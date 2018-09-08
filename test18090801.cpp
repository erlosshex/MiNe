// this program is test code for studying the watershed algoritm
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

const std::string readImageDirectory("D:\\Codes\\VS\\VC\\DoCpp\\Image\\");
const std::string readImageFile("testImage4Watershed.png");

// *randomColor
//  generate a 3-channels color corresponding to the given parameter
// *input arguments
//  -value
//   the given parameter for generating the random color
// *output type
//  -cv::Vec3b
//   the representation of the random 3-channels color
cv::Vec3b randomColor(int value);

int main(int argc, char **argv)
{
	std::string readImagePath = readImageDirectory + readImageFile;
	
	std::cout << "Image Path\t : " << readImagePath << std::endl;

	cv::namedWindow("Origin Image", CV_WINDOW_NORMAL);
	cv::namedWindow("Gray Image", CV_WINDOW_NORMAL);
	cv::namedWindow("GaussianBlur Image", CV_WINDOW_NORMAL);
	cv::namedWindow("Edge Image", CV_WINDOW_NORMAL);
	cv::namedWindow("Marks Image", CV_WINDOW_NORMAL);
	cv::namedWindow("Contours Image", CV_WINDOW_NORMAL);
	cv::namedWindow("Watershed Marks", CV_WINDOW_NORMAL);
	cv::namedWindow("Watershed Image", CV_WINDOW_NORMAL);
	cv::namedWindow("Mix Watershed Image", CV_WINDOW_NORMAL);

	cv::Mat originImage = cv::imread(readImagePath,cv::IMREAD_COLOR);
	cv::Mat grayImage;
	cv::cvtColor(originImage,grayImage,CV_BGR2GRAY);

	//first, using GaussianBlur to deal the gray image, in order to reduce noisy of the image
	cv::Mat gausBlurImage;
	cv::GaussianBlur(grayImage, gausBlurImage, cv::Size(5, 5), 2);

	//second, using Canny algorithm to find edges of the gaussian-blur-after image
	cv::Mat edgeImage;
	cv::Canny(gausBlurImage,edgeImage,80,150);

	//third, using findContours function to find contours of the edge image and constructing the mark matrix
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(edgeImage,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,cv::Point());
	cv::Mat contImage = cv::Mat::zeros(edgeImage.size(), CV_8UC1);
	cv::Mat marks(edgeImage.size(),CV_32S,cv::Scalar::all(0));
	int indexCont = 0;
	int compCount = 0;
	std::cout << "contours size\t : " << contours.size() << std::endl;
	for (; indexCont >= 0; indexCont = hierarchy[indexCont][0], compCount++) {
		drawContours(marks,contours,indexCont,cv::Scalar::all(compCount+1),1,8,hierarchy);
		drawContours(contImage, contours, indexCont, cv::Scalar(255), 1, 8, hierarchy);
	}
	cv::Mat marksImage;
	cv::convertScaleAbs(marks, marksImage);

	//fouth, using watershed function to segment the image
	cv::watershed(originImage, marks);
	cv::Mat watershedMarks;
	cv::convertScaleAbs(marks,watershedMarks);

	//fifth, filling each region with different color
	cv::Mat watershedImage = cv::Mat::zeros(originImage.size(),CV_8UC3);
	for(int i=0; i<marks.rows; ++i)
		for (int j = 0; j < marks.cols; ++j) {
			int index = marks.at<int>(i, j);
			if (marks.at<int>(i, j) == -1)
				watershedImage.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
			else
				watershedImage.at<cv::Vec3b>(i, j) = randomColor(index);
		}
	
	//sixth, mixing the watershedImage and originImage
	cv::Mat mixWshedImage;
	cv::addWeighted(originImage,0.4,watershedImage,0.6,0.0,mixWshedImage);

	cv::imshow("Origin Image", originImage);
	cv::imshow("Gray Image", grayImage);
	cv::imshow("GaussianBlur Image", gausBlurImage);
	cv::imshow("Edge Image", edgeImage);
	cv::imshow("Marks Image", marksImage);
	cv::imshow("Contours Image", contImage);
	cv::imshow("Watershed Marks", watershedMarks);
	cv::imshow("Watershed Image", watershedImage);
	cv::imshow("Mix Watershed Image", mixWshedImage);

	//
	cv::RNG rng;
	std::cout << "test rng : " << rng.uniform(0, 213) << std::endl;
	std::cout << "test rng : " << rng.uniform(0, 213) << std::endl;
	std::cout << "test rng : " << rng.uniform(0, 213) << std::endl;
	cv::RNG rng1;
	std::cout << "test rng1 : " << rng1.uniform(0, 213) << std::endl;
	std::cout << "test rng1 : " << rng1.uniform(0, 213) << std::endl;
	std::cout << "test rng1 : " << rng1.uniform(0, 213) << std::endl;
	//

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}

// *randomColor
//  generate a 3-channels color corresponding to the given parameter
// *input arguments
//  -value
//   the given parameter for generating the random color
// *output type
//  -cv::Vec3b
//   the representation of the random 3-channels color
cv::Vec3b randomColor(int value) {
	value = value % 256;
	cv::RNG randomGenerator;
	uchar randomValueBlue = randomGenerator.uniform(0, value);
	uchar randomValueGreen = randomGenerator.uniform(0, value);
	uchar randomValueRed = randomGenerator.uniform(0, value);
	return cv::Vec3b(randomValueBlue, randomValueGreen, randomValueRed);
}
