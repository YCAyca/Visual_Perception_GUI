#ifndef EPIPOLAR_GEOMETRY_H
#define EPIPOLAR_GEOMETRY_H

#include <QMainWindow>

namespace Ui {
class Epipolar_Geometry;
}

class Epipolar_Geometry : public QMainWindow
{
    Q_OBJECT

public:
    explicit Epipolar_Geometry(QWidget *parent = nullptr);
    ~Epipolar_Geometry();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Epipolar_Geometry *ui;
};

#endif // EPIPOLAR_GEOMETRY_H
