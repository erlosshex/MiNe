#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

string imagePath = "E:\\File\\image\\test\\image02.bmp";

void splitContoursAndHoles(vector<vector<Point> > &contours, vector<Vec4i> &hierarchy, vector<vector<Point> > &cContours, vector<vector<Point> > &hContours) {
	//  findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	//  contours and hierarchy are obtained by findContours with RETR_CCOMP parameter.

	cContours.clear();
	hContours.clear();

	for (int i = 0;i < contours.size();++i) {
		if (hierarchy[i][3] >= 0) {
			hContours.push_back(contours[i]);
		}
		else {
			cContours.push_back(contours[i]);
		}
	}

}

int main(int argc, char **argv)
{
	Mat img = imread(imagePath);

	cvtColor(img, img, CV_BGR2GRAY);

	imshow("gray image", img);

	threshold(img, img, 128, 255, ThresholdTypes::THRESH_BINARY_INV);
	imshow("image", img);
	//
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	cout << "contours size : " << contours.size() << endl;

	for (int i = 0;i < hierarchy.size();++i) {
		cout << "hierarchy " << i << " : " << hierarchy[i][0] << ", " << hierarchy[i][1] << ", " << hierarchy[i][2] << ", " << hierarchy[i][2] << endl;
	}


	Mat drawImage(img.size(),CV_8UC1,Scalar(0));
	for (int i = 0;i < contours.size();++i) {
		drawContours(drawImage, contours, i, Scalar(255), 1);
	}
	imshow("draw image", drawImage);


	vector<vector<Point> > cContours, hContours;
	/*for (int i = 0;i < contours.size();++i) {
		if (hierarchy[i][3] >= 0) {
			cContours.push_back(contours[i]);
		}
		else {
			hContours.push_back(contours[i]);
		}
	}*/

	splitContoursAndHoles(contours, hierarchy, cContours, hContours);

	Mat drawImageC(img.size(), CV_8UC1, Scalar(0));
	Mat drawImageH(img.size(), CV_8UC1, Scalar(0));

	for (int i = 0;i < cContours.size();++i) {
		drawContours(drawImageC, cContours, i, Scalar(255));
	}

	imshow("contours draw image", drawImageC);

	for (int i = 0;i < hContours.size();++i) {
		drawContours(drawImageH, hContours, i, Scalar(255));
	}

	imshow("holes draw image", drawImageH);
	//


	
	waitKey();

	destroyAllWindows();

	return 0;
}
