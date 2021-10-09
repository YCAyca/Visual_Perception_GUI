#ifndef FEATURES_MATCHING_H
#define FEATURES_MATCHING_H

#include "Input_Output.hpp"
#include "opencv2/features2d.hpp"


typedef enum
{
    HARRIS,
    MORPHOLOGIC,
}CORNER_DETECTION_TYPE;

cv::Mat Corner_Detection(cv::Mat, CORNER_DETECTION_TYPE, uint8_t neighborhood=2, uint8_t kernel_size=3, float harris_corner_k=0.04);
cv::Mat Feature_Matching(cv::Mat, cv::Mat);
cv::Mat Extract_Lines(cv::Mat img,int,double,double);
cv::Mat Extract_Circles(cv::Mat img, int,int);
#endif // FEATURES_MATCHING_H
