#ifndef FMMAINWINDOW_H
#define FMMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class FMMainWindow;
}

class FMMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FMMainWindow(QWidget *parent = nullptr);
    ~FMMainWindow();

private slots:
    void on_pushButton_clicked();

    void on_kernel_size_valueChanged(int arg1);

    void on_neighborhood_valueChanged(int arg1);

    void on_k_valueChanged(double arg1);

    void on_minrad_valueChanged(int arg1);

    void on_maxrad_valueChanged(int arg1);

    void on_thresh_valueChanged(int arg1);

    void on_minll_valueChanged(int arg1);

    void on_maxlg_valueChanged(int arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FMMainWindow *ui;
};

#endif // FMMAINWINDOW_H
