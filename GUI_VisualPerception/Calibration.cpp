#include "Input_Output.hpp"
#include "Calibration.hpp"

extern std::string Main_Folder;

int total_number_calibration_images;


bool Camera_Calibration(IO_TYPE IO_type, std::string checkerboard_imgs_path, uint8_t corner_counts_x, uint8_t corner_counts_y) // x in "height" way, y in "width" way
{
    int checkerboard[2] = {corner_counts_x,corner_counts_y};

    // Creating vector to store vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f> > objpoints;

    // Creating vector to store vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f> > imgpoints;

    // Defining the world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for(int i = 0; i < checkerboard[1]; i++)
    {
        for(int j = 0; j<  checkerboard[0]; j++)
            objp.push_back(cv::Point3f(j,i,0));
    }

    // vector to store the pixel coordinates of detected checker board corners
    std::vector<cv::Point2f> corner_pts;
    bool success;

    cv::Mat img, gray;

    switch(IO_type)
    {
        case CAM:
        {
            cv::VideoCapture capture;
            capture.open(0);
            if (!capture.isOpened()){
                std::cerr << "Unable to open cam!" << std::endl;
                return false;
            }
            while(true)
            {

                capture >> img;
                if (img.empty())
                    break;
                cvtColor(img, gray, cv::COLOR_BGR2GRAY);

                // Finding checker board corners
                // If desired number of corners are found in the image then success = true
                success = cv::findChessboardCorners(gray, cv::Size(checkerboard[0], checkerboard[1]), corner_pts, CV_CALIB_CB_ADAPTIVE_THRESH);

                // Displaying the detected corner points on the checker board
                cv::drawChessboardCorners(img, cv::Size(checkerboard[0], checkerboard[1]), corner_pts, success);

                    if(success)
                    {
                        cv::TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);

                        // refining pixel coordinates for given 2d points.
                        cv::cornerSubPix(gray,corner_pts,cv::Size(corner_counts_x,corner_counts_y), cv::Size(-1,-1),criteria);

                        // Displaying the detected corner points on the checker board
                        cv::drawChessboardCorners(img, cv::Size(checkerboard[0], checkerboard[1]), corner_pts, success);

                        objpoints.push_back(objp);
                        imgpoints.push_back(corner_pts);

                        cv::imshow("Successfull corner detection, this image will be used in calibration",img);
                    }
                    else
                    {
                        cv::imshow("Corner detection < desired number of corners",img);
                    }

                    int keyboard = cv::waitKey(WAITKEY);
                    if (keyboard == 'q')
                        break;

            }

            capture.release();
            cv::destroyAllWindows();
            break;
        }
        case IMAGE_FOLDER:
        {
            // Extracting path of individual image stored in a given directory
            std::vector<cv::String> images;

            cv::glob(checkerboard_imgs_path, images);

            // Looping over all the images in the directory
            for(uint i = 0; i < images.size(); i++)
            {
                img = cv::imread(images[i]);
                if (img.empty())
                {
                    std::cout << "Image could not read";
                }
                cv::cvtColor(img,gray,cv::COLOR_BGR2GRAY);

                // Finding checker board corners
                // If desired number of corners are found in the image then success = true
                success = cv::findChessboardCorners(gray, cv::Size(checkerboard[0], checkerboard[1]), corner_pts, CV_CALIB_CB_ADAPTIVE_THRESH);

                // Displaying the detected corner points on the checker board
                cv::drawChessboardCorners(img, cv::Size(checkerboard[0], checkerboard[1]), corner_pts, success);

                /*
             * If desired number of corner are detected,
             * we refine the pixel coordinates and display
             * them on the images of checker board
            */
                if(success)
                {
                    cv::TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);

                    // refining pixel coordinates for given 2d points.
                    cv::cornerSubPix(gray,corner_pts,cv::Size(corner_counts_x,corner_counts_y), cv::Size(-1,-1),criteria);

                    // Displaying the detected corner points on the checker board
                    cv::drawChessboardCorners(img, cv::Size(checkerboard[0], checkerboard[1]), corner_pts, success);

                    objpoints.push_back(objp);
                    imgpoints.push_back(corner_pts);

                    cv::imshow("Successfull corner detection, this image will be used in calibration",img);
                }
                else
                {
                    cv::imshow("Corner detection < desired number of corners",img);

                }
                int keyboard = cv::waitKey(WAITKEY);
                if (keyboard == 'q')
                    break;
            }
            cv::destroyAllWindows();
            break;
        }
        default:
        {
            std::cout << "Undefined IO type!" << std::endl;
            return false;
        }

    }


    cv::Mat cameraMatrix,distCoeffs,R,T;

    /*
     * Performing camera calibration by
     * passing the value of known 3D points (objpoints)
     * and corresponding pixel coordinates of the
     * detected corners (imgpoints)
    */
    double error = cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows,gray.cols), cameraMatrix, distCoeffs, R, T);

    std::cout << "cameraMatrix : " << cameraMatrix << std::endl;

    cv::FileStorage file(Main_Folder+"CameraMatrix.ext", cv::FileStorage::WRITE);
    file << "CameraMatrix" << cameraMatrix;

    total_number_calibration_images = objpoints.size();
    std::cout << "Calibration is done with : " << total_number_calibration_images << "images and with RMS error : "<< error << std::endl;

    std::ofstream MyFile(Main_Folder+"Camera_Matrix.txt");
    MyFile << cameraMatrix;

    return true;
}


