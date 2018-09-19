#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<string> fileName;
vector<string> trainFileName;
vector<string> testFileName;

void pushFileName() {
	fileName.push_back("F:\\Img\\image\\11.bmp");
	fileName.push_back("F:\\Img\\image\\12.bmp");
	fileName.push_back("F:\\Img\\image\\13.bmp");
	fileName.push_back("F:\\Img\\image\\14.bmp");
	fileName.push_back("F:\\Img\\image\\24.bmp");
	fileName.push_back("F:\\Img\\image\\31.bmp");
	fileName.push_back("F:\\Img\\image\\32.bmp");
	fileName.push_back("F:\\Img\\image\\51.bmp");
}

bool dealFlag = false;
int myCount = 0;
int regionFlag = 0;

Mat imageOK;
Mat imageNG;
Mat testImage;
Mat OKRoi;
Mat NGRoi;
Mat testRoi;
Rect rectRoi;

void createTrain() {
	trainFileName.push_back("F:\\Img\\train\\1.bmp");
	trainFileName.push_back("F:\\Img\\train\\2.bmp");
}

void createTest(const string &path) {
	if (!testFileName.empty()) {
		testFileName.pop_back();
		testFileName.pop_back();
	}
	
	testFileName.push_back(string("F:\\Img\\test\\") + path + "\\" + path + "0.bmp");
	testFileName.push_back(string("F:\\Img\\test\\") + path + "\\" + path + "1.bmp");
}

void dealCreateRoi() {
	imageOK = imread(trainFileName[1]);
	imageNG = imread(trainFileName[0]);
	OKRoi = imageOK(rectRoi);
	NGRoi = imageNG(rectRoi);
}

void dealTestRoi(const string &path) {
	testImage = imread(path);
	if (!testImage.data) {
		cout << "failure" << endl; 
		return;
	}
	
	testRoi = testImage(rectRoi);
	cout << string(20, '*') << endl;
	cout << "image name : " << path << endl;
	if (PSNR(testRoi, OKRoi) > PSNR(testRoi, NGRoi)) {
		cout << "result : OK -- "<< PSNR(testRoi, OKRoi)<<" | "<< PSNR(testRoi, NGRoi) << endl;
	}
	else {
		cout << "result : NG -- " << PSNR(testRoi, OKRoi) << " | " << PSNR(testRoi, NGRoi) << endl;
	}
}
	


void onMouseHandle(int event, int x, int y, int flags, void *param) {
	Mat& image = *(Mat*)param;
	int centerX = image.cols / 2;
	int centerY = image.rows / 2;
	switch (event) {
	case EVENT_LBUTTONDOWN:
	{
		cout << "Position | x : " << x << ", y : " << y << endl;
		if (dealFlag == false) {
			myCount++;
			if (myCount % 2 == 1) {
				rectRoi.x = x;
				rectRoi.y = y;
			}
			if (myCount % 2 == 0) {
				rectRoi.width = x - rectRoi.x;
				rectRoi.height = y - rectRoi.y;
				dealFlag = true;
				int roiCenterX = rectRoi.x+(rectRoi.width/2) ;
				int roiCenterY = rectRoi.y+(rectRoi.height/2) ;
				
				if ((roiCenterX < centerX) && (roiCenterY < centerY))  regionFlag = 1;
				if ((roiCenterX > centerX) && (roiCenterY < centerY))  regionFlag = 2;
				if ((roiCenterX > centerX) && (roiCenterY > centerY))  regionFlag = 3;
				if ((roiCenterX < centerX) && (roiCenterY > centerY))  regionFlag = 4;
				createTest(to_string(regionFlag));
			}
		}
	}
	}
}

int main(int argc, char **argv)
{
	pushFileName();
	createTrain();
	
	Mat tempImage = imread(trainFileName[1]);
	namedWindow("rect image", WINDOW_NORMAL);
	setMouseCallback("rect image",onMouseHandle,(void*)&tempImage);
	imshow("rect image", tempImage);

	while (true) {
		if (dealFlag == true) {
			dealCreateRoi();
			for (auto& pathName : testFileName) {
				dealTestRoi(pathName);
			}
				
			dealFlag = false;
			
		}
		int key = waitKey(31);
		if (key == 27)  break;
		if (key == 'r')  dealFlag = false;
	}
	waitKey(0);
	destroyAllWindows();
	return 0;
}
