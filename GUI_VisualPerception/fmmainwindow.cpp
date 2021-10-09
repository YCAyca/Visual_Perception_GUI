#include "fmmainwindow.h"
#include "ui_fmmainwindow.h"
#include "Features_Matching.hpp"
#include "mainwindow.h"
#include <iostream>
#include <opencv2/opencv.hpp>

#include <QFileDialog>
#include <QSlider>
#include <QButtonGroup>
#include <QImage>
#include <QMessageBox>
#include <QString>

extern std::string Main_Folder;
extern std::string InputImage_Folder;

QString DCM_DEFAULT_IMAGE =  "C:\\Users\\aktas\\GUI_VisualPerception\\Input_Images\\cameraman.png";
QString DCH_DEFAULT_IMAGE = "C:\\Users\\aktas\\GUI_VisualPerception\\Input_Images\\cube1.png";
QString EL_DEFAULT_IMAGE =  "C:\\Users\\aktas\\GUI_VisualPerception\\Input_Images\\cube1.png";
QString EC_DEFAULT_IMAGE =  "C:\\Users\\aktas\\GUI_VisualPerception\\Input_Images\\circless.jpg";
QString FM1_DEFAULT_IMAGE = "C:\\Users\\aktas\\GUI_VisualPerception\\Input_Images\\cube1.png";
QString FM2_DEFAULT_IMAGE = "C:\\Users\\aktas\\GUI_VisualPerception\\Input_Images\\cube2.png";



int corner_detect_kernel = 3, harris_neighborhood = 3;
double harris_k = 0.04;

int minrad = 30, maxrad = 50;
int thresh = 50, minll = 50, maxlg = 10;

FMMainWindow::FMMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FMMainWindow)
{
    ui->setupUi(this);
}

FMMainWindow::~FMMainWindow()
{
    delete ui;
}

void FMMainWindow::on_pushButton_clicked() // detect  corners
{
    QImage Qimg;
    cv::Mat img;
    cv::Mat out;

    ui->match->clear();

    if(ui->demo->isChecked())
    {
        if (ui->morpho->isChecked())
        {
            img = cv::imread(DCM_DEFAULT_IMAGE.toLocal8Bit().constData());

            Qimg = QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);

            ui->rawIm->setPixmap(QPixmap::fromImage(Qimg));
            //resize the label to fit the image
            ui->rawIm->resize(ui->rawIm->pixmap()->size());

            out = Corner_Detection(img,MORPHOLOGIC);
        }
        else // harris corner detection
        {
            img = cv::imread(DCH_DEFAULT_IMAGE.toLocal8Bit().constData());

            Qimg = QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);

            ui->rawIm->setPixmap(QPixmap::fromImage(Qimg));
            //resize the label to fit the image
            ui->rawIm->resize(ui->rawIm->pixmap()->size());


            out = Corner_Detection(img, HARRIS, harris_neighborhood,corner_detect_kernel,harris_k);

        }
        Qimg = QImage((const unsigned char*)(out.data),out.cols,out.rows,QImage::Format_RGB888);

        ui->processedIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->processedIm->resize(ui->rawIm->pixmap()->size());

    }
    else
    {
        QFileDialog dialog(this);
        dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg *.tiff)"));
        QStringList fileNames;
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        if (dialog.exec())
            fileNames = dialog.selectedFiles();

        if(!fileNames.size())
        {
            QMessageBox msgBox;
            msgBox.setText("Please select an image");
            msgBox.exec();
            return;
        }

        std::cout << fileNames.at(0).toLocal8Bit().constData() << std::endl;
        img = cv::imread(fileNames.at(0).toLocal8Bit().constData());
        if (img.empty())
        {
            std::cout << "Image could not read";
            return;
        }
        Qimg = QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);
        if (img.cols > 896 || img.rows > 896) // if image is too big, either we cant see the whole image in label, or the label gets too big causing a bad view
        {
            Qimg = Qimg.scaled(896,896, Qt::KeepAspectRatio);
        }
        ui->rawIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->rawIm->resize(ui->rawIm->pixmap()->size());

        if (ui->morpho->isChecked())
            out = Corner_Detection(img,MORPHOLOGIC);
        else
             out = Corner_Detection(img, HARRIS, harris_neighborhood,corner_detect_kernel,harris_k);

        Qimg = QImage((const unsigned char*)(out.data),out.cols,out.rows,QImage::Format_RGB888);

        ui->processedIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->processedIm->resize(ui->rawIm->pixmap()->size());
    }
}

void FMMainWindow::on_kernel_size_valueChanged(int arg1) //harris - morpho kernel size
{
    corner_detect_kernel = arg1;
}

void FMMainWindow::on_neighborhood_valueChanged(int arg1)
{
    harris_neighborhood = arg1;
}

void FMMainWindow::on_k_valueChanged(double arg1)
{
    harris_k = arg1;
}

void FMMainWindow::on_minrad_valueChanged(int arg1)
{
    minrad = arg1;
}

void FMMainWindow::on_maxrad_valueChanged(int arg1)
{
    maxrad = arg1;
}

void FMMainWindow::on_thresh_valueChanged(int arg1)
{
    thresh = arg1;
}

void FMMainWindow::on_minll_valueChanged(int arg1)
{
    minll = arg1;
}

void FMMainWindow::on_maxlg_valueChanged(int arg1)
{
    maxlg = arg1;
}

void FMMainWindow::on_pushButton_4_clicked() // extract circles
{
    QImage Qimg;
    cv::Mat img;
    cv::Mat out;

    ui->match->clear();

    if(ui->demo->isChecked())
    {
        img = cv::imread(EC_DEFAULT_IMAGE.toLocal8Bit().constData());

        Qimg = QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);

        ui->rawIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->rawIm->resize(ui->rawIm->pixmap()->size());

        out = Extract_Circles(img, 30, 100);

        Qimg = QImage((const unsigned char*)(out.data),out.cols,out.rows,QImage::Format_RGB888);

        ui->processedIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->processedIm->resize(ui->rawIm->pixmap()->size());

    }
    else
    {
        QFileDialog dialog(this);
        dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg *.tiff)"));
        QStringList fileNames;
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        if (dialog.exec())
            fileNames = dialog.selectedFiles();

        if(!fileNames.size())
        {
            QMessageBox msgBox;
            msgBox.setText("Please select an image");
            msgBox.exec();
            return;
        }

        std::cout << fileNames.at(0).toLocal8Bit().constData() << std::endl;
        img = cv::imread(fileNames.at(0).toLocal8Bit().constData());
        if (img.empty())
        {
            std::cout << "Image could not read";
            return;
        }
        Qimg = QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);
        if (img.cols > 896 || img.rows > 896) // if image is too big, either we cant see the whole image in label, or the label gets too big causing a bad view
        {
            Qimg = Qimg.scaled(896,896, Qt::KeepAspectRatio);
        }
        ui->rawIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->rawIm->resize(ui->rawIm->pixmap()->size());

        out = Extract_Circles(img, minrad, maxrad);

        Qimg = QImage((const unsigned char*)(out.data),out.cols,out.rows,QImage::Format_RGB888);

        ui->processedIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->processedIm->resize(ui->rawIm->pixmap()->size());
    }
}

void FMMainWindow::on_pushButton_3_clicked() // extract lines
{
    QImage Qimg;
    cv::Mat img;
    cv::Mat out;

    ui->match->clear();

    if(ui->demo->isChecked())
    {
        img = cv::imread(EL_DEFAULT_IMAGE.toLocal8Bit().constData());

        Qimg = QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);

        ui->rawIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->rawIm->resize(ui->rawIm->pixmap()->size());

        out = Extract_Lines(img, thresh,minll,maxlg);

        Qimg = QImage((const unsigned char*)(out.data),out.cols,out.rows,QImage::Format_RGB888);

        ui->processedIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->processedIm->resize(ui->rawIm->pixmap()->size());

    }
    else
    {
        QFileDialog dialog(this);
        dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg *.tiff)"));
        QStringList fileNames;
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        if (dialog.exec())
            fileNames = dialog.selectedFiles();

        if(!fileNames.size())
        {
            QMessageBox msgBox;
            msgBox.setText("Please select an image");
            msgBox.exec();
            return;
        }

        std::cout << fileNames.at(0).toLocal8Bit().constData() << std::endl;
        img = cv::imread(fileNames.at(0).toLocal8Bit().constData());
        if (img.empty())
        {
            std::cout << "Image could not read";
            return;
        }
        Qimg = QImage((const unsigned char*)(img.data),img.cols,img.rows,QImage::Format_RGB888);
        if (img.cols > 896 || img.rows > 896) // if image is too big, either we cant see the whole image in label, or the label gets too big causing a bad view
        {
            Qimg = Qimg.scaled(896,896,Qt::KeepAspectRatio);
        }
        ui->rawIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->rawIm->resize(ui->rawIm->pixmap()->size());

        out = Extract_Lines(img, thresh,minll,maxlg);

        Qimg = QImage((const unsigned char*)(out.data),out.cols,out.rows,QImage::Format_RGB888);

        ui->processedIm->setPixmap(QPixmap::fromImage(Qimg));
        //resize the label to fit the image
        ui->processedIm->resize(ui->processedIm->pixmap()->size());
    }
}

void FMMainWindow::on_pushButton_5_clicked() // back button
{
    hide();
    QWidget *parent = this->parentWidget();
    if (parent == nullptr)
        std::cout << "no parent" << std::endl;
    else
        parent->show();
}

void FMMainWindow::on_pushButton_2_clicked() // match
{
    cv::Mat image1, image2, matched;
    QImage Qimg1, Qimg2, Qimg3;

    ui->match->clear();

    if(ui->demo->isChecked())
    {
        image1 = cv::imread(FM1_DEFAULT_IMAGE.toLocal8Bit().constData());
        image2 = cv::imread(FM2_DEFAULT_IMAGE.toLocal8Bit().constData());
    }
    else
    {
        QFileDialog dialog(this);
        dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg *.tiff)"));
        QStringList fileNames;
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFiles);
        if (dialog.exec())
            fileNames = dialog.selectedFiles();

        if(!fileNames.size())
        {
            QMessageBox msgBox;
            msgBox.setText("Please select an image");
            msgBox.exec();
            return;
        }

        if (fileNames.size() == 1)
        {
            image1 = cv::imread(fileNames.at(0).toLocal8Bit().constData());
            image2 = image1;
        }
        else
        {
            for (int i = 0; i < 2; ++i)
            {
                 image1 = cv::imread(fileNames.at(i).toLocal8Bit().constData());
                 image2 = cv::imread(fileNames.at(i).toLocal8Bit().constData());
            }
        }

    }


    Qimg1 = QImage((const unsigned char*)(image1.data),image1.cols,image1.rows,QImage::Format_RGB888);

    ui->rawIm->setPixmap(QPixmap::fromImage(Qimg1));
    //resize the label to fit the image
    ui->rawIm->resize(ui->rawIm->pixmap()->size());

    Qimg2 = QImage((const unsigned char*)(image2.data),image2.cols,image2.rows,QImage::Format_RGB888);

    ui->processedIm->setPixmap(QPixmap::fromImage(Qimg2));
    //resize the label to fit the image
    ui->processedIm->resize(ui->processedIm->pixmap()->size());

    matched = Feature_Matching(image1,image2);

    std::cout << matched.size() << std::endl;

    Qimg3 = QImage((const unsigned char*)(matched.data),matched.cols,matched.rows,QImage::Format_RGB888);

    if (matched.cols > 896 || matched.rows > 896) // if image is too big, either we cant see the whole image in label, or the label gets too big causing a bad view
    {
        Qimg3 = Qimg3.scaled(896,896, Qt::KeepAspectRatio);
    }


    ui->match->setPixmap(QPixmap::fromImage(Qimg3));
    //resize the label to fit the image
    ui->match->resize(ui->match->pixmap()->size());


}
