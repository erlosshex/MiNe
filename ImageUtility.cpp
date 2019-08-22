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
    // move rectangle
    cv::Rect moveRect(const cv::Rect &rect, int x, int y);
    cv::Rect moveRect(const cv::Rect &rect, const Point &offset);
    cv::Rect moveRect(cv::Rect *rect_p, int x, int y);
    cv::Rect moveRect(cv::Rect *rect_p, const Point &offset);
    bool moveRectATO(cv::Rect &rect, int x, int y);
    bool moveRectATO(cv::Rect &rect, const Point &offset);
    bool moveRectATO(cv::Rect *rect_p, int x, int y);
    bool moveRectATO(cv::Rect *rect_p, const Point &offset);
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

cv::Rect ImageUtility::moveRect(const cv::Rect &rect, int x, int y){
    cv::Rect res = rect;
    
    try{
        res.x += x;
        res.y += y;
    }
    catch(std::exception &e){
        res = rect;
    }
    
    return res;
}

cv::Rect moveRect(cv::Rect *rect_p, int x, int y);
cv::Rect moveRect(cv::Rect *rect_p, const Point &offset);
bool moveRectATO(cv::Rect &rect, int x, int y);
bool moveRectATO(cv::Rect &rect, const Point &offset);
bool moveRectATO(cv::Rect *rect_p, int x, int y);
bool moveRectATO(cv::Rect *rect_p, const Point &offset);
