#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/cvdef.h>
#include "opencv2/opencv.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video.hpp"
#include "iostream"
#include "vector"
#include "algorithm"
#include "SerialPort.h"

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage Mat2QImage(const cv::Mat &mat);
    double dist(Point x, Point y);
    pair<Point,double> circleFromPoints(Point p1, Point p2, Point p3);
    void sendData(char* temp, CSerialPort mySerialPort);
    void transformVideoData(int x, int y, int z, int numOfFinger);
    char* readData();
    char transformFromIntToChar(int);
    int getAngleOfJoint6(int x, int y);
    int getAngleOfJoint5(int, int, int);
    int getAngleOfJoint4(int, int, int);
    int getAngleOfJoint3(int, int, int);
    int getAngleOfJoint2(int, int, int);
    int getAngleOfJoint1(int);
    int tan_angle(int x, int y);
    int cos_angle(int x, int y);
    int sin_angle(int x, int y);
    void changeFormate(int angle, int posi);
    char NumberTransfor(int n);
    void combinNewAndOldOrder();

private slots:
    void on_Start_clicked();

    void on_Pause_clicked();

    void on_Exit_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
