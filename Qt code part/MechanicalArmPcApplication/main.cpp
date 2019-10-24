#include "mainwindow.h"
#include <QApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "testapplication_methods.h"
#include "kalma.h"

using namespace cv;

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);


    namedWindow("My Image",1);

    MainWindow w;
    w.show();

    return a.exec();
   // test_1_create_mat();
   //kalma w;
    //w.kalman_2d_point_test();
    //w.kalman_test();
   // w.sample_test();
    //test_method1();
    /*test_method3();
    return 0;*/
}

