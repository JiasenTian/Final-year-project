#include "mainwindow.h"
#include <QApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main(){
    //read a image
    Mat image= imread("D:\\Jason.png");
    // avoid mem error
    if (image.empty()){
        namedWindow("can not find image : img.jpg");
        waitKey(5000);
        return -1;
    }
    //creat image windows named "My Image"
    namedWindow("My Image",1);
    //show the image on window
    imshow("My Image",image);
    //wait key for 5000ms
    return 0;
}
