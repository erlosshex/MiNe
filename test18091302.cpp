// read video and deal frame -- sample for OpenCV3 Computer Vision Application Programming Cookbook, Third Edition
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{
  VideoProcessor processor;
  processor.setInput("xxx.avi");
  processor.displayInput("Current Frame");
  processor.displayOuput("Output Frame");
  processor.setDelay(1000.0/processor.getFrameRate());
  processor.setFrameProcessor(canny);
	return 0;
}

class VideoProcessor {
private:
	// OpenCV视频捕获对象
	cv::VideoCapture capture;
	// 处理每一帧时都会调用的回调函数
	void(*process)(cv::Mat &, cv::Mat &);
	// 布尔类型变量，表示该回调函数是否会被调用
	bool callIt;
	// 输入窗口的显示名称
	std::string windowNameInput;
	// 输出窗口的显示名称
	std::string windowNameOutput;
	// 帧之间的延时
	int delay;
	// 已经处理的帧数
	long fnumber;
	// 达到这个帧数时结束
	long frameToStop;
	// 结束处理
	bool stop;
	//---------------------------------------------//
	// OpenCV写入视频对象
	cv::VideoWriter writer;
	// 输出文件名
	std::string outputFile;
	// 输出图像的当前序号
	int currentIndex;
	// 输出图像文件名中数字的位数
	int digits;
	// 输出图像的扩展名
	std::string extension;

public:
	// 设置针对每一帧调用的回调函数
	void setFrameProcessor(void(*frameProcessingCallback)(cv::Mat &, cv::Mat &)) {
		process = frameProcessingCallback;
	}
	// 设置视频文件的名称
	bool setInput(std::string filename) {
		fnumber = 0;
		// 防止已经有资源与VideoCapture实例关联
		capture.release();
		// 打开视频文件
		return capture.open(filename);
	}
	// 用于显示输入的帧
	void displayInput(std::string wn) {
		windowNameInput = wn;
		cv::namedWindow(windowNameInput, CV_WINDOW_NORMAL);
	}
	// 用于显示处理过的帧
	void displayOutput(std::string wn) {
		windowNameOutput = wn;
		cv::namedWindow(windowNameOutput, CV_WINDOW_NORMAL);
	}
	// 抓取（并处理）序列中的帧
	void run() {
		// 当前帧
		cv::Mat frame;
		// 输出帧
		cv::Mat output;

		// 如果没有设置捕获设备
		if (!isOpened())
			return;

		stop = false;

		while (!isStopped()) {
			// 读取下一帧（如果有）
			if (!readNextFrame(frame))
				break;
			// 显示输入的帧
			if (windowNameInput.length() != 0)
				cv::imshow(windowNameInput,frame);

			// 调用处理函数
			if (callIt) {
				// 处理帧
				if (process)
					process(frame, output);
				else if (frameProcessor)
					frameProcessor->process(frame, output);
				// 递增帧数
				fnumber++;
			}
			else
			{
				// 没有处理
				output = frame;
			}
			//----------------------------------------------------//
			if (outputFile.length() != 0)
				writeNextFrame(output);
			// 显示输出的帧
			if (windowNameOutput.length() != 0)
				cv::imshow(windowNameOutput,output);
			if (delay >= 0 && cv::waitKey(delay) >= 0)
				stopIt();
			// 检查是否需要结束
			if (frameToStop >= 0 && getFrameNumber() == frameToStop)
				stopIt();
		}
	}
	// 结束处理
	void stopIt() {
		stop = true;
	}
	// 处理过程是否已经停止
	bool isStopped() {
		return stop;
	}
	// 捕获设备是否已经打开
	bool isOpened() {
		return capture.isOpened();
	}
	// 设置帧之间的延时
	// 0表示每一帧都等待
	// 负数表示不延时
	void setDelay(int d) {
		delay = d;
	}
	// 取得下一帧
	// 可以是视频文件或者摄像机
	bool readNextFrame(cv::Mat &frame) {
		return capture.read(frame);
	}
	// 需要调用回调函数
	void callPorcess() {
		callIt = true;
	}
	// 不需要调用回调函数process
	void dontCallProcess() {
		callIt = false;
	}
	// 
	void stopAtFrameNo(long frame) {
		frameToStop = frame;
	}
	// 返回下一帧的编号
	long getFrameNumber() {
		// 从捕获设备获取信息
		long fnumber = static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
		return fnumber;
	}
	//--------------------------------------------------//
	// 设置输出视频文件
	// 默认情况下会使用与输入视频相同的参数
	bool setOutput(const std::string &filename, int codec = 0, double framerate = 0.0, bool isColor = true) {
		outputFile = filename;
		extension.clear();
		if (framerate == 0.0)
			framerate = getFrameRate();  // 与输入相同

		char c[4];
		// 使用与输入相同的编解码器
		if (codec == 0) {
			codec = getCodec(c);
		}

		// 打开输出视频
		return writer.open(outputFile,         // 文件名
			               codec,              // 所用的编解码器
						   framerate,          // 视频的帧速率
						   getFrameSize(),     // 帧的尺寸
						   isColor);           // 彩色视频？
	}

	// 写输出的帧
	// 可以是视频文件或图像组
	void writeNextFrame(cv::Mat &frame) {
		if (extension.length()) {  // 写入到图像组
			std::stringstream ss;
			// 组合成输出文件名
			ss << outputFile << std::setfill('0')
				<< std::setw(digits)
				<< currentIndex++ << extension;
			cv::imwrite(ss.str(),frame);
		}
		else 
		{
			// 写入到视频文件
			writer.write(frame);
		}
	}
	// 设置输出为一系列图像文件
	// 扩展名必须是.jpg或者.bmp
	bool setOuput(const std::string &filename,     // 前缀
		const std::string &ext,                    // 图像文件的扩展名
		int numberOfDigits = 3,                    // 数字的位数
		int startIndex = 0) {                      // 开始序号
		// 数字的位数必须是正数
		if (numberOfDigits < 0)
			return false;
		// 文件名和常用的扩展名
		outputFile = filename;
		extension = ext;

		// 文件编号方案中数字的位数
		digits = numberOfDigits;
		// 从这个序号开始编号
		currentIndex = startIndex;

		return true;
	}

};

