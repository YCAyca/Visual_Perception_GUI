#ifndef CALIBRATIONMAINWINDOW_H
#define CALIBRATIONMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class calibrationMainWindow;
}

class calibrationMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit calibrationMainWindow(QWidget *parent = nullptr);
    ~calibrationMainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_x_im_valueChanged(int arg1);

    void on_y_im_valueChanged(int arg1);

    void on_x_cam_valueChanged(int arg1);

    void on_y_cam_valueChanged(int arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::calibrationMainWindow *ui;
};

#endif // CALIBRATIONMAINWINDOW_H
