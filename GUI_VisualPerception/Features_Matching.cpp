#include "Input_Output.hpp"
#include "Features_Matching.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
// #include <opencv2/xfeatures2d.hpp>

#include <QMessageBox>
#include <QFileDialog>
#include <QString>

extern std::string Main_Folder;

cv::Mat Corner_Detection(cv::Mat image, CORNER_DETECTION_TYPE type, uint8_t neighborhood, uint8_t kernel_size, float harris_corner_k)
{
    cv::Mat image_gray, image_corner_detected = image.clone();
    cv::cvtColor(image, image_gray, CV_BGR2GRAY); //

    switch(type)
    {
        case HARRIS:
        {
          // Detecting corners
          cv::Mat output = cv::Mat::zeros(image.size(), CV_8UC1);
          cornerHarris(image_gray, output, neighborhood, kernel_size, harris_corner_k);

          // Normalizing
          normalize(output, output, 0, 255, cv::NORM_MINMAX, CV_8UC1, cv::Mat());
          convertScaleAbs(output, output);

          // Drawing a circle around corners
          for(int j = 0; j < output.rows ; j++){
              for(int i = 0; i < output.cols; i++){
                  if((int) output.at<uint8_t>(j,i) > 100){
                     circle(image_corner_detected, cv::Point(i,j), 2,  cv::Scalar(0,0,255), 2, 8, 0 );
                  }
              }
          }
          break;
        }
        case MORPHOLOGIC:
        {
            // Creating morphological structure elements that we need for corner detection
            cv::Mat cross(5,5,CV_8UC1,cv::Scalar(0));
            cv::Mat diamond(5,5,CV_8UC1,cv::Scalar(1));
            cv::Mat square(5,5,CV_8UC1,cv::Scalar(1));
            cv::Mat x(5,5,CV_8UC1,cv::Scalar(0));

            // Creating the cross-shaped structuring element
            for (int i=0; i<5; i++)
            {
                cross.at<uint8_t>(2,i)= 1;
                cross.at<uint8_t>(i,2)= 1;
            }
            // Creating the diamond-shaped structuring element
            diamond.at<uint8_t>(0,0)= 0;
            diamond.at<uint8_t>(0,1)= 0;
            diamond.at<uint8_t>(1,0)= 0;
            diamond.at<uint8_t>(4,4)= 0;
            diamond.at<uint8_t>(3,4)= 0;
            diamond.at<uint8_t>(4,3)= 0;
            diamond.at<uint8_t>(4,0)= 0;
            diamond.at<uint8_t>(4,1)= 0;
            diamond.at<uint8_t>(3,0)= 0;
            diamond.at<uint8_t>(0,4)= 0;
            diamond.at<uint8_t>(0,3)= 0;
            diamond.at<uint8_t>(1,4)= 0;
            // Creating the x-shaped structuring element
            for (int i=0; i<5; i++)
            {
                x.at<uint8_t>(i,i)= 1;
                x.at<uint8_t>(4-i,i)= 1;
            }


            cv::Mat tmp;
            // Dilate with a cross
            cv::dilate(image,image_corner_detected,cross);
            // Erode with a diamond
            cv::erode(image_corner_detected,image_corner_detected,diamond);
            // Dilate with a X
            cv::dilate(image,tmp,x);
            // Erode with a square
            cv::erode(tmp,tmp,square);
            // Corners are obtained by differencing
            // the two closed images
            cv::absdiff(tmp,image_corner_detected,image_corner_detected);
            // Apply threshold to obtain a binary image
            cv::threshold(image_corner_detected, image_corner_detected, 30, 255, cv::THRESH_BINARY);
            break;
        }
        default:
        {
           std::cout << "Undefined corner operation!" << std::endl;
           return image_gray;
        }
    }
    return image_corner_detected;
}

cv::Mat Feature_Matching(cv::Mat image1, cv::Mat image2)
{
    cv::Mat image_gray1, image_gray2;
    cv::Mat img_match;
    cv::cvtColor(image1, image_gray1, CV_BGR2GRAY);
    cv::cvtColor(image2, image_gray2, CV_BGR2GRAY);
    cv::Mat descriptor_1, descriptor_2;
    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;


    cv::Ptr<cv::ORB> detector = cv::ORB::create();
    std::vector<cv::DMatch> matches;
    cv::FlannBasedMatcher matcher;
    cv::Ptr<cv::DescriptorExtractor> extractor;


    detector->detect(image_gray1, keypoints_1);
    detector->detect(image_gray2, keypoints_2);

    if(descriptor_1.type()!=CV_32F) {
        descriptor_1.convertTo(descriptor_1, CV_32F);
    }

    if(descriptor_2.type()!=CV_32F) {
        descriptor_2.convertTo(descriptor_2, CV_32F);
    }

    matcher.match(descriptor_1, descriptor_2, matches);
    drawMatches(image_gray1, keypoints_1, image_gray2, keypoints_2, matches, img_match);

    // Extraction of the orb descriptors

    detector->compute(image_gray1, keypoints_1, descriptor_1);
    detector->compute(image_gray2, keypoints_2, descriptor_2);

    if(descriptor_1.type()!=CV_32F) {
        descriptor_1.convertTo(descriptor_1, CV_32F);
    }

    if(descriptor_2.type()!=CV_32F) {
        descriptor_2.convertTo(descriptor_2, CV_32F);
    }

    std::ofstream MyFile1(Main_Folder+"Matched_Points1.txt");
    std::ofstream MyFile2(Main_Folder+"Matched_Points2.txt");

    for(auto it = keypoints_1.begin(); it < keypoints_1.end(); it++)
    {
        MyFile1 << int(it->pt.x) << " " << int(it->pt.y) << std::endl;
    }

    for(auto it2 = keypoints_2.begin(); it2 < keypoints_2.end(); it2++)
    {
        MyFile2 << int(it2->pt.x) << " " << int(it2->pt.y) << std::endl;
    }

//    std::cout << "first point matches" << std::endl;
//    for(auto it = keypoints_1.begin(); it < keypoints_1.end(); it++)
//    {
//        std::cout << int(it->pt.x) << " " << int(it->pt.y) << std::endl;
//    }

//    std::cout << "first point matches" << std::endl;

//    for(auto it2 = keypoints_2.begin(); it2 < keypoints_2.end(); it2++)
//    {
//        std::cout << int(it2->pt.x) << " " << int(it2->pt.y) << std::endl;
//    }


    matcher.match(descriptor_1, descriptor_2, matches);

    drawMatches(image_gray1, keypoints_1, image_gray2, keypoints_2, matches, img_match);

    QMessageBox msgBox;
    std::string str = "2D point matches are saved to " + Main_Folder;
    msgBox.setText(QString::fromStdString(str));
    msgBox.exec();

    return img_match;
}


cv::Mat Extract_Lines(cv::Mat img, int thresh, double min, double max)
{
    cv::Mat output_im, output_im_rgb;

    // Edge detection
    Canny(img, output_im, 50, 200, 3);

    cvtColor(output_im, output_im_rgb, cv::COLOR_GRAY2BGR);

    // Probabilistic Line Transform

    std::vector<cv::Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(output_im, linesP, 1, CV_PI/180, thresh, min, max); // runs the actual detection
    // Draw the lines
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        cv::Vec4i l = linesP[i];
        line(output_im_rgb, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 1, cv::LINE_AA);
    }

    return output_im_rgb;
}



cv::Mat Extract_Circles(cv::Mat img, int minrad, int maxrad)
{
    std::vector<cv::Vec3f> circles;
    cv::Mat im_gray;
    cv::cvtColor(img, im_gray, cv::COLOR_BGR2GRAY);
    HoughCircles(im_gray, circles, cv::HOUGH_GRADIENT, 1, im_gray.rows/4, 100, 30, minrad, maxrad);
    for(size_t i = 0; i < circles.size(); i++ )
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        circle(img, center, 1, cv::Scalar(0,100,100), 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        circle(img, center, radius, cv::Scalar(255,0,255), 3, cv::LINE_AA);
    }
    return img;
}

