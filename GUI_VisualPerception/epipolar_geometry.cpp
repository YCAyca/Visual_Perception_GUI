#include "epipolar_geometry.h"
#include "ui_epipolar_geometry.h"
#include <tuple>
#include <opencv2/calib3d.hpp>
#include <Eigen/Dense>
#include <QFileDialog>
#include <QMessageBox>


#include "Input_Output.hpp"

extern std::string Main_Folder;
extern std::string InputImage_Folder;

using namespace Eigen;

Epipolar_Geometry::Epipolar_Geometry(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Epipolar_Geometry)
{
    ui->setupUi(this);
}

Epipolar_Geometry::~Epipolar_Geometry()
{
    delete ui;
}

void Epipolar_Geometry::on_pushButton_clicked() // Demo Button
{
    std::ifstream  MyFile1(Main_Folder+"Matched_Points1.txt");
    std::ifstream  MyFile2(Main_Folder+"Matched_Points2.txt");

    std::string line;
    std::vector<cv::Point2d> point_matches1_demo;
    std::vector<cv::Point2d> point_matches2_demo;

    QMessageBox msgBox;

    auto it1 = point_matches1_demo.begin();
    auto it2 = point_matches2_demo.begin();

    int x = -1, y;

    while (getline (MyFile1, line)) {
        std::string word = "";
        for (auto letter : line)
        {
            if (letter == ' ')
            {
                if(x == -1)
                {
                    x = std::stoi(word);
                    word = "";
                }
                else
                {
                    y = std::stoi(word);

                    cv::Point2f p;
                    p.x = x;
                    p.y = y;
                    it1 = point_matches1_demo.insert(it1, p);

                    x = -1;
                    word = "";
                }

            }
            else
                word = word + letter;
        }
    }


    while (getline (MyFile2, line)) {
        std::string word = "";
        for (auto letter : line)
        {
            if (letter == ' ')
            {
                if(x == -1)
                {
                    x = std::stoi(word);
                    word = "";
                }
                else
                {
                    y = std::stoi(word);

                    cv::Point2f p;
                    p.x = x;
                    p.y = y;
                    it2 = point_matches2_demo.insert(it2, p);
                    x = -1;
                    word = "";
                }

            }
            else
                word = word + letter;
        }
    }

    cv::Mat FundamentalMatrix(3, 3, CV_32F);
    FundamentalMatrix = cv::findFundamentalMat(point_matches1_demo,point_matches2_demo, cv::FM_RANSAC,3, 0.99);

    std::ofstream MyFile3(Main_Folder+"FundamentalMatrix.txt");
    MyFile3 << FundamentalMatrix;

    cv::FileStorage file2(Main_Folder+"FundamentalMatrix.ext", cv::FileStorage::WRITE);
    file2 << "FundamentalMatrix" << FundamentalMatrix;
    file2.release();

    std::string str = "FundamentalMatrix is saved to " + Main_Folder;
    msgBox.setText(QString::fromStdString(str));
    msgBox.exec();

    std::vector<cv::Vec3d> leftepiLines, rightepiLines;

    cv::computeCorrespondEpilines(point_matches1_demo, 1, FundamentalMatrix, rightepiLines);
    cv::computeCorrespondEpilines(point_matches2_demo, 2, FundamentalMatrix, leftepiLines);


    std::ofstream MyFile4(Main_Folder+"rightepiLines.txt");

    for(unsigned long long i = 0; i < rightepiLines.size(); i++)
         MyFile4 << rightepiLines[i] << std::endl;

    std::ofstream MyFile5(Main_Folder+"leftepiLines.txt");

    for(unsigned long long i = 0; i < leftepiLines.size(); i++)
         MyFile5 << leftepiLines[i] << std::endl;

    str = "Epipolar Lines are saved to " + Main_Folder;
    msgBox.setText(QString::fromStdString(str));
    msgBox.exec();

     cv::Mat cameraMatrix;

     cv::FileStorage file(Main_Folder+"CameraMatrix.ext", cv::FileStorage::READ);
     file["CameraMatrix"] >> cameraMatrix;
     file.release();

     cv::Mat EssentialMatrix = cv::findEssentialMat(point_matches1_demo,point_matches2_demo, cameraMatrix, cv::RANSAC, 0.999, 1.0);

     std::ofstream MyFile6(Main_Folder+"EssentialMatrix.txt");
     MyFile6 << EssentialMatrix;

     cv::FileStorage file3(Main_Folder+"EssentialMatrix.ext", cv::FileStorage::WRITE);
     file3 << "EssentialMatrix" << EssentialMatrix;
     file3.release();

     str = "Essential Matrix is saved to " + Main_Folder;
     msgBox.setText(QString::fromStdString(str));
     msgBox.exec();

     cv::Mat R,T, Reconstructed_3D_Points;
     cv::recoverPose(EssentialMatrix, point_matches1_demo, point_matches2_demo, cameraMatrix,R,T,	Reconstructed_3D_Points);

     std::ofstream MyFile7(Main_Folder+"R.txt");
     MyFile7 << R;

     std::ofstream MyFile8(Main_Folder+"T.txt");
     MyFile8 << T;

     cv::FileStorage file4(Main_Folder+"R.ext", cv::FileStorage::WRITE);
     file4 << "R" << R;
     file4.release();

     cv::FileStorage file5(Main_Folder+"T.ext", cv::FileStorage::WRITE);
     file5 << "T" << T;
     file5.release();

     str = "R and T matrices are saved to " + Main_Folder;
     msgBox.setText(QString::fromStdString(str));
     msgBox.exec();


     float data[12] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
     cv::Mat ProjMatr1 = cv::Mat(3, 4, CV_32F, data);

     cv::Mat temp;

     cv::hconcat(R, T, temp);

     cv::Mat ProjMatr2 = cameraMatrix * temp;

     cv::Mat Triangulated_3D_Points;
     cv::triangulatePoints(ProjMatr1, ProjMatr2, point_matches1_demo, point_matches2_demo, Triangulated_3D_Points);

     std::ofstream MyFile9(Main_Folder+"Triangulated_3D_Points.txt");




     std::cout << Triangulated_3D_Points.size() << std::endl;

     int row_size = Triangulated_3D_Points.rows;
     int col_size = Triangulated_3D_Points.cols;

     for(int i = 0; i < col_size; i++)
     {
        for(int k=0; k < row_size; k++)
             MyFile9 << Triangulated_3D_Points.at<float>(i, k) << " ";
        MyFile9 << std::endl;
     }

     str = "Triangulated 3D Points is saved to " + Main_Folder;
     msgBox.setText(QString::fromStdString(str));
     msgBox.exec();
}

std::vector<cv::Point2d> point_matches1;
std::vector<cv::Point2d> point_matches2;
bool points_loaded = false;

void Epipolar_Geometry::on_pushButton_2_clicked() // select 2d image matches
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Text files (*.txt)"));
    QStringList fileNames;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec())
       fileNames = dialog.selectedFiles();


    QMessageBox msgBox;
    if(fileNames.size() != 2)
    {

            msgBox.setText("Please select 2 text files 1 for each matched points");
            msgBox.exec();
            return;
    }

    std::ifstream  MyFile1(fileNames.at(0).toLocal8Bit().constData());
    std::ifstream  MyFile2(fileNames.at(1).toLocal8Bit().constData());

    std::string line;


    auto it1 = point_matches1.begin();
    auto it2 = point_matches2.begin();

    int x = -1, y;

    while (getline (MyFile1, line)) {
        std::string word = "";
        for (auto letter : line)
        {
            if (letter == ' ')
            {
                if(x == -1)
                {
                    x = std::stoi(word);
                    word = "";
                }
                else
                {
                    y = std::stoi(word);

                    cv::Point2f p;
                    p.x = x;
                    p.y = y;
                    it1 = point_matches1.insert(it1, p);

                    x = -1;
                    word = "";
                }

            }
            else
                word = word + letter;
        }
    }


    while (getline (MyFile2, line)) {
        std::string word = "";
        for (auto letter : line)
        {
            if (letter == ' ')
            {
                if(x == -1)
                {
                    x = std::stoi(word);
                    word = "";
                }
                else
                {
                    y = std::stoi(word);

                    cv::Point2f p;
                    p.x = x;
                    p.y = y;
                    it2 = point_matches2.insert(it2, p);
                    x = -1;
                    word = "";
                }

            }
            else
                word = word + letter;
        }
    }

    points_loaded = true;

    msgBox.setText("Point matches are loaded succesfully!");
    msgBox.exec();

}

void Epipolar_Geometry::on_pushButton_4_clicked() // essential matrix
{
    QMessageBox msgBox;
    msgBox.setText("Before starting, the camera calibration matrix K should be chosen");
    msgBox.exec();

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Yaml files (*.ext)"));
    QStringList fileNames;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
       fileNames = dialog.selectedFiles();


    if(fileNames.size() != 1)
    {

            msgBox.setText("Camera Calibration matrix was not selected");
            msgBox.exec();
            return;
    }

    if (points_loaded == false)
    {
        msgBox.setText("For essential matrix estimation, 2D image point pairs are required. Please load the points before Essential Matrix estimation");
        msgBox.exec();
        return;
    }

    cv::Mat cameraMatrix;
    cv::FileStorage file(fileNames.at(0).toLocal8Bit().constData(), cv::FileStorage::READ);
    file["CameraMatrix"] >> cameraMatrix;
    file.release();

    cv::Mat EssentialMatrix = cv::findEssentialMat(point_matches1,point_matches2, cameraMatrix, cv::RANSAC, 0.999, 1.0);

    std::ofstream MyFile6(Main_Folder+"EssentialMatrix.txt");
    MyFile6 << EssentialMatrix;

    cv::FileStorage file3(Main_Folder+"EssentialMatrix.ext", cv::FileStorage::WRITE);
    file3 << "EssentialMatrix" << EssentialMatrix;
    file3.release();

    std::string str = "Essential Matrix is saved to " + Main_Folder + "as EssentialMatrix.txt and EssentialMatrix.ext";

    msgBox.setText(QString::fromStdString(str));
    msgBox.exec();
}

void Epipolar_Geometry::on_pushButton_5_clicked() // fundamental matrix
{
    QMessageBox msgBox;


    if (points_loaded == false)
    {
        msgBox.setText("For Fundamental matrix estimation, 2D image point pairs are required. Please load the points before Fundamental Matrix estimation");
        msgBox.exec();
        return;
    }


    cv::Mat FundamentalMatrix(3, 3, CV_32F);
    FundamentalMatrix = cv::findFundamentalMat(point_matches1,point_matches2, cv::FM_RANSAC,3, 0.99);

    std::ofstream MyFile3(Main_Folder+"FundamentalMatrix.txt");
    MyFile3 << FundamentalMatrix;

    cv::FileStorage file2(Main_Folder+"FundamentalMatrix.ext", cv::FileStorage::WRITE);
    file2 << "FundamentalMatrix" << FundamentalMatrix;
    file2.release();

    std::string str = "Fundamental Matrix is saved to " + Main_Folder + "as FundamentalMatrix.txt and FundamentalMatrix.ext";

    msgBox.setText(QString::fromStdString(str));
    msgBox.exec();

}

void Epipolar_Geometry::on_pushButton_7_clicked() // epipolar lines
{
    QMessageBox msgBox;

    msgBox.setText("Before starting, fundamental matrix F should be chosen");
    msgBox.exec();

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Yaml files (*.ext)"));
    QStringList fileNames;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
       fileNames = dialog.selectedFiles();


    if(fileNames.size() != 1)
    {

            msgBox.setText("Fundamental matrix was not selected");
            msgBox.exec();
            return;
    }


    if (points_loaded == false)
    {
        msgBox.setText("For epipolar lines estimation, 2D image point pairs are required. Please load the points");
        msgBox.exec();
        return;
    }

    cv::Mat FundamentalMatrix(cv::Size(3,3), CV_32F);
    cv::FileStorage file(fileNames.at(0).toLocal8Bit().constData(), cv::FileStorage::READ);
    file["FundamentalMatrix"] >> FundamentalMatrix;
    file.release();


    std::vector<cv::Vec3d> leftepiLines, rightepiLines;

    cv::computeCorrespondEpilines(point_matches1, 1, FundamentalMatrix, rightepiLines);
    cv::computeCorrespondEpilines(point_matches2, 2, FundamentalMatrix, leftepiLines);


    std::ofstream MyFile4(Main_Folder+"rightepiLines.txt");

    for(unsigned long long i = 0; i < rightepiLines.size(); i++)
         MyFile4 << rightepiLines[i] << std::endl;

    std::ofstream MyFile5(Main_Folder+"leftepiLines.txt");

    for(unsigned long long i = 0; i < leftepiLines.size(); i++)
         MyFile5 << leftepiLines[i] << std::endl;

    std::string str = "Epipolar lines are saved to " + Main_Folder + "as leftepiLines.txt and rightepiLines.txt";

    msgBox.setText(QString::fromStdString(str));
    msgBox.exec();

}

void Epipolar_Geometry::on_pushButton_3_clicked() // triangulation
{
    QMessageBox msgBox;

    if (points_loaded == false)
    {
        msgBox.setText("For triangulation, 2D image point pairs are required. Please load the points");
        msgBox.exec();
        return;
    }

    msgBox.setText("Before starting, essential matrix E should be chosen");
    msgBox.exec();

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Yaml files (*.ext)"));
    QStringList fileNames;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
       fileNames = dialog.selectedFiles();


    if(fileNames.size() != 1)
    {

            msgBox.setText("Essential matrix was not selected");
            msgBox.exec();
            return;
    }

    cv::Mat EssentialMatrix;
    cv::FileStorage file(fileNames.at(0).toLocal8Bit().constData(), cv::FileStorage::READ);
    file["EssentialMatrix"] >> EssentialMatrix;
    file.release();

    msgBox.setText("Essential matrix loaded succesfully");
    msgBox.exec();

    msgBox.setText("Before starting, camera matrix K should be chosen");
    msgBox.exec();

    QFileDialog dialog2(this);
    dialog2.setNameFilter(tr("Yaml files (*.ext)"));
    QStringList fileNames2;
    dialog2.setAcceptMode(QFileDialog::AcceptOpen);
    dialog2.setFileMode(QFileDialog::ExistingFile);
    if (dialog2.exec())
       fileNames2 = dialog2.selectedFiles();


    if(fileNames2.size() != 1)
    {

            msgBox.setText("Camera matrix was not selected");
            msgBox.exec();
            return;
    }

    cv::Mat cameraMatrix;
    cv::FileStorage file2(fileNames2.at(0).toLocal8Bit().constData(), cv::FileStorage::READ);
    file2["CameraMatrix"] >> cameraMatrix;
    file2.release();

    msgBox.setText("Camera matrix loaded succesfully");
    msgBox.exec();

    cv::Mat R,T, Reconstructed_3D_Points;

    std::cout << EssentialMatrix.size() << std::endl;
    std::cout << cameraMatrix.size() << std::endl;
    cv::recoverPose(EssentialMatrix, point_matches1, point_matches2, cameraMatrix,R,T,Reconstructed_3D_Points);

    std::ofstream MyFile7(Main_Folder+"R.txt");
    MyFile7 << R;
    MyFile7.close();

    std::ofstream MyFile8(Main_Folder+"T.txt");
    MyFile8 << T;
    MyFile8.close();

    cv::FileStorage file4(Main_Folder+"R.ext", cv::FileStorage::WRITE);
    file4 << "R" << R;
    file4.release();

    cv::FileStorage file5(Main_Folder+"T.ext", cv::FileStorage::WRITE);
    file5 << "T" << T;
    file5.release();

    float data[12] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
    cv::Mat ProjMatr1 = cv::Mat(3, 4, CV_32F, data);

    cv::Mat temp;

    cv::hconcat(R, T, temp);

    cv::Mat ProjMatr2 = cameraMatrix * temp;

    cv::Mat Triangulated_3D_Points;
    cv::triangulatePoints(ProjMatr1, ProjMatr2, point_matches1, point_matches2, Triangulated_3D_Points);

    std::ofstream MyFile9(Main_Folder+"Triangulated_3D_Points.txt");

    std::cout << Triangulated_3D_Points.size() << std::endl;

    int row_size = Triangulated_3D_Points.rows;
    int col_size = Triangulated_3D_Points.cols;

    for(int i = 0; i < col_size; i++)
    {
       for(int k=0; k < row_size; k++)
            MyFile9 << Triangulated_3D_Points.at<float>(i, k) << " ";
       MyFile9 << std::endl;
    }

    MyFile9.close();

    std::string str = "Triangulated_3D_Points, R and T matrices are saved to " + Main_Folder;
    msgBox.setText(QString::fromStdString(str));
    msgBox.exec();

}
