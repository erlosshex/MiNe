//简单的图像拼接代码
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	string path1("E:\\Project\\C085\\Codes\\02Code\\3DLMI\\C085LMI\\Image\\A1.png");
	string path2("E:\\Project\\C085\\Codes\\02Code\\3DLMI\\C085LMI\\Image\\A2.png");
	Mat img1 = imread(path1,0);
	Mat img2 = imread(path2, 0);
	SimpleBlobDetector::Params params;
	params.minThreshold = 50;
	params.maxThreshold = 220; 
	params.filterByArea = true;
	params.minArea = 10;
	params.maxArea = 10000;
	params.blobColor = 255;
	SimpleBlobDetector detector(params);
	//Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	vector<KeyPoint> keypoints1;
	vector<KeyPoint> keypoints2;
	detector.detect(img1,keypoints1);
	detector.detect(img2,keypoints2);
	if (keypoints1.size() == 0) {
		cout << "keypoints1 no element" << endl;
	}
	else{
		int i = 0;
		for (i = 0; i < keypoints1.size(); ++i) {
			cout << "keypoints1 num " << i << " : " << keypoints1[i].pt << endl;
		}
	}
	if (keypoints2.size() == 0) {
		cout << "keypoints2 no element" << endl;
	}
	else {
		int i = 0;
		for (i = 0; i < keypoints2.size(); ++i) {
			cout << "keypoints2 num " << i << " : " << keypoints2[i].pt << endl;
		}
	}
	Mat img_with_keypoints1;
	Mat img_with_keypoints2;
	drawKeypoints(img1,keypoints1,img_with_keypoints1,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img2,keypoints2,img_with_keypoints2,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	namedWindow("keypoints1",CV_WINDOW_NORMAL);
	imshow("keypoints1",img_with_keypoints1);
	namedWindow("keypoints2",CV_WINDOW_NORMAL);
	imshow("keypoints2",img_with_keypoints2);
	//
	Mat cutImg1;
	Mat cutImg2;
	cutImg1 = img1(Rect(0, 0, keypoints1[0].pt.x, keypoints1[0].pt.y));
	cutImg2 = img2(Rect(keypoints2[0].pt.x, 0, img2.cols - keypoints2[0].pt.x, keypoints2[0].pt.y));
	int minRow = cutImg1.rows < cutImg2.rows ? cutImg1.rows : cutImg2.rows;
	int minCol = cutImg1.cols < cutImg2.cols ? cutImg1.cols : cutImg2.cols;
	cutImg1 = cutImg1(Rect(cutImg1.cols - minCol, cutImg1.rows - minRow, minCol, minRow));
	cutImg2 = cutImg2(Rect(cutImg2.cols - minCol, cutImg2.rows - minRow, minCol, minRow));
	cout << "cut image 1 : size : " << cutImg1.size() << endl;
	cout << "cut image 2 : size : " << cutImg2.size() << endl;
	namedWindow("Cut Image 1", CV_WINDOW_NORMAL);
	imshow("Cut Image 1", cutImg1);
	namedWindow("Cut Image 2",CV_WINDOW_NORMAL);
	imshow("Cut Image 2", cutImg2);
	//
	Mat combineMat;
	hconcat(cutImg1, cutImg2, combineMat);
	cout << "combine image : size : " << combineMat.size() << endl;
	namedWindow("combine image",CV_WINDOW_NORMAL);
	imshow("combine image",combineMat);
	waitKey(0);
	imwrite("E:\\Project\\C085\\Codes\\02Code\\3DLMI\\C085LMI\\Image\\C1.png", combineMat);
	destroyAllWindows();
	return 0;
}
