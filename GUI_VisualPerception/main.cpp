#include "Features_Matching.hpp"
#include "Calibration.hpp"


extern std::string InputImage_Folder;
extern std::string OutputImage_Folder;
extern std::string CalibrationImages_Folder;
extern std::string Video_Folder;

#include "mainwindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    w.show();
    return a.exec();

}
