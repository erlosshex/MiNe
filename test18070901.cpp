#include <iostream>
#include <thread>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void printMat(string prompt, Mat &m, int i, int j, int c) {
	cout << prompt << (int)m.at<Vec3b>(i, j)[c] << endl;
}

void modifiedMat(Mat &m, int i, int j, int c, int num) {
	m.at<Vec3b>(i, j)[c] = num;
}

void prompt() {
	static int count = 0;
	string pr(20, '*');
	printf("%-3d - %s\n", count,pr.c_str());
	//cout << count << " - " << pr << endl;
	++count;
}

void anotherPrompt() {
	for (int i = 0; i < 10; ++i) {
		cout << i << " - ";
		prompt();
	}
		
}

int main(int argc, char **argv)
{
	//Thread
	thread pt(anotherPrompt);
	pt.join();
	//
	prompt();
	Mat img = imread("F:/DesktopStuff/6666.bmp");
	//Print information of picture
	cout << "size : " << img.size() << endl;
	cout << "type : " << img.type() << endl;
	prompt();
	//ROI : 100-150, 250-300
	printMat("img origin : ", img, 101, 251, 0);
	Mat myROI = img(Range(100, 150),Range(250,300));
	printMat("roi origin : ", myROI, 1, 1, 0);
	prompt();
	//Modified
	modifiedMat(img,101,251,0,78);
	printMat("img modified : ", img, 101, 251, 0);
	printMat("roi modified : ", myROI, 1, 1, 0);
	prompt();
	//Another modified
	modifiedMat(myROI, 1, 1, 0, 113);
	printMat("img another modified : ", img, 101, 251, 0);
	printMat("roi another modified : ", myROI, 1, 1, 0);
	prompt();
	//
	Mat t,tt;
	t.create(2,3,CV_32F);
	tt.create(Size(2, 3), CV_32F);
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 3; ++j)
			t.at<float>(i, j) = (float)(i+j);
	cout << t << endl;
	cout << t.size() << endl;
	cout << t.size().width << endl;
	cout << t.size().height << endl;
	cout << t.size().area() << endl;
	prompt();
	//
	srand(NULL);
	for (int i = 0; i < 3; ++i) 
		for (int j = 0; j < 2; ++j) {
			auto p = tt.ptr<float>(i);
			*(p + j) = (float)(rand()%10);
		}
	cout << tt << endl;
	prompt();
	//Matrix multiple
	cout << t*tt << endl;
	prompt();
	//Matrix inv
	Mat tInv((t*tt).inv());
	cout << tInv << endl;
	prompt();
	//ones zeros eye diag
	cout << " --> ones : " << endl;
	cout << Mat::ones(3, 2, CV_8UC1) << endl;
	prompt();
	cout << " --> zeros : " << endl;
	cout << Mat::zeros(2, 3, CV_8UC1) << endl;
	prompt();
	cout << " --> eye : " << endl;
	cout << Mat::eye(3, 4, CV_8UC1) << endl;
	prompt();
	cout << " --> diag : " << endl;
	cout << Mat::diag(Mat(1, 5, CV_32F, Scalar(3.2))) << endl;
	prompt();
	//
	Mat diagM = Mat::diag(Mat(5, 1, CV_32F, Scalar(3.2)));
	cout<< diagM.at<float>(0, 4) << endl;
	prompt();
	cout << "one dimension size : " << (Mat(5, 1, CV_32F, Scalar(3.2))).size() << endl;
	cout << "one dimension size : " << (Mat(1, 5, CV_32F, Scalar(3.2))).size() << endl;
	prompt();
	//Eigen
	Mat temp = (Mat_<double>(3, 3) << 1,2,3,5,7,8,6,4,9 );
	Mat eValue, eVector;
	eigen(temp,eValue,eVector);
	cout << "eValue = " << endl << eValue << endl;
	cout << "eVector = " << endl << eVector << endl;
	prompt();
	//Eigen test
	auto l = eValue.at<double>(0);
	auto v = eVector.row(0).t();
	cout << "l = " << endl << l << endl;
	cout << "v = " << endl << v << endl;
	cout << "first : " << temp*v << endl;
	cout << "second : " << l*v << endl;
	prompt();
	//Another try
	//src*eigenvectors.row(i).t() = eigenvalues.at<srcType>(i)*eigenvectors.row(i).t()
	Mat src=temp.clone();
	Mat eigenvectors, eigenvalues;
	eigen(src, eigenvalues, eigenvectors);
	cout << "src*eigenvectors.row(i).t() = " << endl << src*eigenvectors.row(0).t() << endl;
	cout << "eigenvalues.at<srcType>(i)*eigenvectors.row(i).t()" << endl << eigenvalues.at<double>(0)*eigenvectors.row(0).t() << endl;
	prompt();
	//Picture to Matrix
	Mat img013C = imread("F:/DesktopStuff/33.JPG", IMREAD_COLOR);
	cout << "img013C channels :" << img013C.channels() << endl;
	Mat img01 = imread("F:/DesktopStuff/6666.bmp", IMREAD_GRAYSCALE);
	//Mat img01;
	vector<Mat> vecMat;
	split(img013C, vecMat);
	img01 = vecMat[0];
	imshow("B", img01);
	waitKey(6000);
	Mat dst;
	merge(vecMat, dst);
	Mat img01M(img01.size(), CV_64FC1);
	for (int i = 0; i < img01.rows; ++i)
		for (int j = 0; j < img01.cols; ++j)
			img01M.at<double>(i, j) = img01.at<uchar>(i, j);
	cout << "size : " << img01.size() << endl;
	cout << "img01 type : " << img01.type() << endl;
	cout << "img01M type : " << img01M.type() << endl;
	//cout<<"img01(0,2) : "<<img01.at<>
	Mat mulM(img01.size().width, 1, CV_64FC1,Scalar(1));
	Mat mulM1(1, img01.size().width, CV_64FC1, Scalar(1));
	cout << "result : " << endl << mulM1*mulM << endl;
	cout << "img01M : " << img01M.size() << endl;
	cout << "mulM : " << mulM.size() << endl;
	//cout << "img01M x mulM" << endl << img01M*mulM << endl;
	//Mat resultM = img01*mulM;
	//cout << "resultM : " << endl << resultM << endl;
	prompt();
	//
	char c;
	cin >> c;
	return 0;
}
