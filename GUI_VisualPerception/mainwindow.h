#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "calibrationmainwindow.h"
#include "fmmainwindow.h"
#include "epipolar_geometry.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked(); // calibration

    void on_pushButton_4_clicked(); // feature matching

    void on_pushButton_5_clicked(); // epipolar geometry

private:
    Ui::MainWindow *ui;
    calibrationMainWindow * calibwindow;
    FMMainWindow * fmwindow;
    Epipolar_Geometry * epwindow;
};
#endif // MAINWINDOW_H
