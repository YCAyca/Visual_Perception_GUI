#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    hide();
    calibwindow = new calibrationMainWindow(this);
    calibwindow->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    hide();
    fmwindow = new FMMainWindow(this);
    fmwindow->show();
}

void MainWindow::on_pushButton_5_clicked()
{
    hide();
    epwindow = new Epipolar_Geometry(this);
    epwindow->show();
}
