#include <iostream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

namespace ImageUtility {
    // rotate mat --- clockwise 90
    cv::Mat matRotateClockWise90(cv::Mat _src);
    // rotate mat --- clockwise 180
    cv::Mat matRotateClockWise180(cv::Mat _src);
    // rotate mat --- clockwise 270
    cv::Mat matRotateClockWise270(cv::Mat _src);
}

//**************************************************************//

cv::Mat ImageUtility::matRotateClockWise90(cv::Mat _src){
    cv::Mat src_copy = _src.clone();
    if (src_copy.empty()){
        std::cout << "RotateMat src is empty!" << std::endl;
    }
    transpose(src_copy, src_copy);
    flip(src_copy, src_copy, 1);
    return src_copy;
}

cv::Mat ImageUtility::matRotateClockWise180(cv::Mat _src){
    cv::Mat src_copy = _src.clone();
    if (src_copy.empty()){
        std::cout << "RotateMat src is empty!" << std::endl;
    }
    flip(src_copy, src_copy, 0);
    flip(src_copy, src_copy, 0);
    return src_copy;
}

cv::Mat ImageUtility::matRotateClockWise270(cv::Mat _src){
    cv::Mat src_copy = _src.clone();
    if (src_copy.empty()){
        std::cout << "RotateMat src is empty!" << std::endl;
    }
    transpose(src_copy, src_copy);
    flip(src_copy, src_copy, 0);
    return src_copy;
}
