#ifndef KALMA_H
#define KALMA_H
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
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>

using namespace cv;

class kalma
{
public:
    kalma();
    void kalman_test();
    void kalman_2d_point_test();
    void sample_test();
  //  void mouseEvent(int event, int x, int y, int flags, void *param);

static inline Point calcPoint(Point2f ceneter, double R, double angle);

};

#endif // KALMA_H
