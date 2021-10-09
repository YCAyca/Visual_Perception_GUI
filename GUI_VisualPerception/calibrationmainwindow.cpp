#include "calibrationmainwindow.h"
#include "ui_calibrationmainwindow.h"
#include "mainwindow.h"

#include "Calibration.hpp"
#include <QMessageBox>
#include <QFileDialog>


extern std::string CalibrationImages_Folder;
extern std::string Main_Folder;
extern int total_number_calibration_images;

uint8_t x_corners;
uint8_t y_corners;

calibrationMainWindow::calibrationMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::calibrationMainWindow)
{
    ui->setupUi(this);
}

calibrationMainWindow::~calibrationMainWindow()
{
    delete ui;
}

void calibrationMainWindow::on_pushButton_clicked() // Demo
{
    Camera_Calibration(IMAGE_FOLDER,CalibrationImages_Folder);

    QMessageBox msgBox;
    std::string str = "Calibration is done with " + std::to_string(total_number_calibration_images) + " images";
    msgBox.setInformativeText(QString::fromStdString(str));
    msgBox.exec();
    str = "The parameters are saved successfully to Calibraiton_Matrix.txt in";
    msgBox.setText(QString::fromStdString(str));
    msgBox.setInformativeText(QString::fromStdString(Main_Folder));
    msgBox.exec();
}

void calibrationMainWindow::on_pushButton_2_clicked() // Image Seq
{
    QMessageBox msgBox;
    x_corners = ui->x_im->value();
    y_corners = ui->y_im->value();

    if (!x_corners || !y_corners)
    {
        msgBox.setText("Please enter the expected corner counts. They can't be zero!");
        msgBox.setInformativeText(QString::fromStdString(Main_Folder));
        msgBox.exec();
        return;
    }

    QFileDialog dialog(this);
    QDir directory;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    if (dialog.exec())
       directory = dialog.directory();

    try {
        Camera_Calibration(IMAGE_FOLDER, directory.absolutePath().toLocal8Bit().constData(), x_corners, y_corners);
    }
    catch (cv::Exception) {
        msgBox.setText("camera calibration could not be completed. Probably there is not enough image with detected corners or you put the expected corner counts wrong" );
        msgBox.exec();
        return;
    }

    std::string str = "Calibration is done with " + std::to_string(total_number_calibration_images) + " images";
    msgBox.setInformativeText(QString::fromStdString(str));
    msgBox.exec();
    str = "The parameters are saved successfully to Calibraiton_Matrix.txt in";
    msgBox.setText(QString::fromStdString(str));
    msgBox.setInformativeText(QString::fromStdString(Main_Folder));
    msgBox.exec();

}

void calibrationMainWindow::on_x_im_valueChanged(int arg1)
{
    x_corners = arg1;
}

void calibrationMainWindow::on_y_im_valueChanged(int arg1)
{
    y_corners = arg1;
}

void calibrationMainWindow::on_x_cam_valueChanged(int arg1)
{
    x_corners = arg1;
}

void calibrationMainWindow::on_y_cam_valueChanged(int arg1)
{
    y_corners = arg1;
}

void calibrationMainWindow::on_pushButton_3_clicked() // CAM
{
    x_corners = ui->x_cam->value();
    y_corners = ui->y_cam->value();
    try {
        Camera_Calibration(CAM, Main_Folder, x_corners,y_corners);
    }
    catch (cv::Exception) {
        std::cout << "camera calibration could not be completed. Probably there is no enough image with detected corners" << std::endl;
        QMessageBox msgBox;
        msgBox.setText("camera calibration could not be completed. Probably there is not enough image with detected corners or you put the expected corner counts wrong" );
        msgBox.exec();
        return;
    }


    QMessageBox msgBox;
    std::string str = "Calibration is done with " + std::to_string(total_number_calibration_images) + " images";
    msgBox.setInformativeText(QString::fromStdString(str));
    msgBox.exec();
    str = "The parameters are saved successfully to Calibraiton_Matrix.txt in";
    msgBox.setText(QString::fromStdString(str));
    msgBox.setInformativeText(QString::fromStdString(Main_Folder));
    msgBox.exec();
}

void calibrationMainWindow::on_pushButton_4_clicked()
{
    hide();
    QWidget *parent = this->parentWidget();
    if (parent == nullptr)
        std::cout << "no parent" << std::endl;

    parent->show();
}
