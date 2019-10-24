#include "testapplication_methods.h"
#include "cv.h"
#include "iostream"
#include "SerialPort.h"
#include <iostream>   // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O

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
using namespace std;
using namespace cv;

void transformVideoData(int x, int y, int z){
    //operation


    //writeData();
}


char transformFromIntToChar(int temp){
    if(temp == 1)
        return '1';
    else if(temp == 2)
        return '2';
    else if(temp == 3)
        return '3';
    else if(temp == 4)
        return '4';
    else if(temp == 5)
        return '5';
    else if(temp == 6)
        return '6';
    else if(temp == 7)
        return '7';
    else if(temp == 8)
        return '8';
    else if(temp == 9)
        return '9';
    else if(temp == 0)
        return '0';
}

void sendData(){
   // mySerialPort.WriteData(tempSendData, 36);
}

double dist(Point x,Point y)
{
    return (x.x-y.x)*(x.x-y.x)+(x.y-y.y)*(x.y-y.y);
}

pair<Point,double> circleFromPoints(Point p1, Point p2, Point p3)
{
    double offset = pow(p2.x,2) +pow(p2.y,2);
    double bc =   ( pow(p1.x,2) + pow(p1.y,2) - offset )/2.0;
    double cd =   (offset - pow(p3.x, 2) - pow(p3.y, 2))/2.0;
    double det =  (p1.x - p2.x) * (p2.y - p3.y) - (p2.x - p3.x)* (p1.y - p2.y);
    double TOL = 0.0000001;
    if (abs(det) < TOL) { cout<<"POINTS TOO CLOSE"<<endl; return make_pair(Point(0,0),0); }

    double idet = 1/det;
    double centerx =  (bc * (p2.y - p3.y) - cd * (p1.y - p2.y)) * idet;
    double centery =  (cd * (p1.x - p2.x) - bc * (p2.x - p3.x)) * idet;
    double radius = sqrt( pow(p2.x - centerx,2) + pow(p2.y-centery,2));

    return make_pair(Point(centerx,centery),radius);
}

void test_method3(){

    char temp[36] = {'0','9','0','0','9','5','0','9','0','0','9','5','0','9','0','0','9','5','0','9','0','0','9','5','0','9','0','0','9','5','0','9','0','0','9','5'};//"090095090095090095090095090095090095";
    CSerialPort mySerialPort;

       if (!mySerialPort.InitPort(5))
       {
           std::cout << "initPort fail !" << std::endl;
       }
       else
       {
           std::cout << "initPort success !" << std::endl;
       }

       if (!mySerialPort.OpenListenThread())
       {
           std::cout << "OpenListenThread fail !" << std::endl;
       }
       else
       {
           std::cout << "OpenListenThread success !" << std::endl;
       }

       if(mySerialPort.WriteData(temp, 36)){
           std::cout << "Send success !" << std::endl;
       }
       else
       {
           std::cout << "Send fail !" << std::endl;
       }
}


void test_method2(){
      //const std::string sourceReference = "test3.avi";
       int delay = 1;

       char c;
       int frameNum = -1;          // Frame counter

       VideoCapture cap(1);

       if ( !cap.isOpened())
       {
                // cout  << "Could not open reference " << sourceReference << endl;
           //return -1;
       }

       Size refS = Size( (int) cap.get(CV_CAP_PROP_FRAME_WIDTH),
           (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT) );

       int Frame_x = (int) cap.get(CV_CAP_PROP_FRAME_WIDTH);
       int Frame_y = (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT);

       bool bHandFlag = false;

       const char* WIN_SRC = "Source";
       const char* WIN_RESULT = "Result";
       const char* WIN_TEMP = "Temp";

       // Windows
       namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE );
       namedWindow(WIN_RESULT, CV_WINDOW_AUTOSIZE);
       namedWindow(WIN_TEMP, CV_WINDOW_AUTOSIZE);

       Mat frame;  // 输入视频帧序列
       Mat temp;
       vector<Mat> channels;
       Mat dst(frame); // 输出图像
       Mat Y, Cr, Cb;
   //  Mat frameSplit[4];
       vector<pair<Point, double> > palm_centers;
       vector< vector<Point> > contours;   // 轮廓
       vector< vector<Point> > filterContours; // 筛选后的轮廓
       vector< Vec4i > hierarchy;    // 轮廓的结构信息
       vector< Point > hull; // 凸包络的点集

       while(true) //Show the image captured in the window and repeat
       {
           cap >> frame;

           if( frame.empty() )
           {
               cout << " < < <  Game over!  > > > ";
               break;
           }
           imshow( WIN_SRC, frame);

           // Begin

           // 中值滤波，去除椒盐噪声
           medianBlur(frame, frame, 5);
           // 转换到HSV颜色空间，更容易处理
   //      cvtColor( frame, frameHSV, CV_BGR2HSV );
           frame.copyTo(temp);
           cvtColor(temp, temp, CV_BGR2YCrCb);
           split(temp, channels);
           Y = channels.at(0);
           Cr = channels.at(1);
           Cb = channels.at(2);
   //      split(frameHSV, frameSplit);

   //      imshow(WIN_H, frameSplit[0]);
   //      imshow(WIN_S, frameSplit[1]);
   //      imshow(WIN_V, frameSplit[2]);

           Mat dstTemp(frame.rows, frame.cols, CV_8UC1);
           for (int j = 1; j < Y.rows - 1; j++)  {
               uchar* currentCr = Cr.ptr< uchar>(j);
               uchar* currentCb = Cb.ptr< uchar>(j);
               uchar* current = dstTemp.ptr< uchar>(j);
               for (int i = 1; i < Y.cols - 1; i++)  {
                  if ((currentCr[i] > 137) && (currentCr[i] < 175) && (currentCb[i] > 100) && (currentCb[i] < 118))
                     current[i] = 255;
                  else
                     current[i] = 0;
                  }
          }
           imshow(WIN_TEMP, dstTemp);
           //对HSV空间进行量化，得到2值图像，亮的部分为手的形状
           //inRange(temp, Scalar(0,30,30), Scalar(40,170,256), dstTemp1);
           //inRange(temp, Scalar(156,30,30), Scalar(180,170,256), dstTemp2);
           //bitwise_or(dstTemp1, dstTemp2, mask);
           //inRange(frameHSV, Scalar(0,30,30), Scalar(180,170,256), dst);

           // 形态学操作，去除噪声，并使手的边界更加清晰
           Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
           erode(dstTemp, dstTemp, element);
           morphologyEx(dstTemp, dstTemp, MORPH_OPEN, element);
           dilate(dstTemp, dstTemp, element);
           morphologyEx(dstTemp, dstTemp, MORPH_CLOSE, element);

           frame.copyTo(dst, dstTemp);

           contours.clear();
           hierarchy.clear();
           filterContours.clear();
           // 得到手的轮廓
           findContours(dstTemp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
           // 去除伪轮廓
           for (size_t i = 0; i < contours.size(); i++)
           {
   //          approxPolyDP(Mat(contours[i]), Mat(approxContours[i]), arcLength(Mat(contours[i]), true)*0.02, true);
               if (fabs(contourArea(Mat(contours[i]))) > 6000) //判断手进入区域的阈值
               {   vector<vector<Point> > tcontours;
                   tcontours.push_back(contours[i]);
                   filterContours.push_back(contours[i]);
                   vector<vector<Point> > hulls(1);
                   vector<vector<int> > hullsI(1);
                   convexHull(Mat(tcontours[0]),hulls[0],false);
                   convexHull(Mat(tcontours[0]),hullsI[0],false);
                   //drawContours(foreground1,hulls,-1,cv::Scalar(155,255,155),2, 8);
                   //Find minimum area rectangle to enclose hand
                                   RotatedRect rect=minAreaRect(Mat(tcontours[0]));

                                   vector<Vec4i> defects;
                                   if(hullsI[0].size() > 0){
                                       Point2f rect_points[4];
                                       rect.points(rect_points);
                                       for(int j = 0; j < 4; j++){
                                           line(dst, rect_points[j], rect_points[(j + 1)%4],Scalar(0,155,155), 1, 8);
                                       }
                                       Point rough_palm_center;
                                       convexityDefects(tcontours[0], hullsI[0], defects);
                                       if(defects.size() >= 3){
                                           vector<Point> palm_points;
                                           for(int j = 0; j < defects.size(); j++){
                                               int startidx = defects[j][0];
                                               Point ptStart(tcontours[0][startidx]);
                                               int endidx = defects[j][1];
                                               Point ptEnd(tcontours[0][endidx]);
                                               int faridx = defects[j][2];
                                               Point ptFar(tcontours[0][faridx]);
                                               rough_palm_center += ptStart + ptEnd + ptFar;
                                               palm_points.push_back(ptFar);
                                               palm_points.push_back(ptStart);
                                               palm_points.push_back(ptEnd);
                                           }
                                           rough_palm_center.x/=defects.size()*3;
                                           rough_palm_center.y/=defects.size()*3;
                                           Point closest_pt=palm_points[0];
                                           vector<pair<double, int> > distvec;
                                           for(int i = 0; i < palm_points.size(); i++){
                                               distvec.push_back(make_pair(dist(rough_palm_center, palm_points[i]), i));
                                           }
                                           sort(distvec.begin(), distvec.end());
                                           pair<Point, double> soln_circle;
                                           for(int i = 0; i + 2 < distvec.size(); i++){
                                               Point p1 = palm_points[distvec[i + 0].second];
                                               Point p2 = palm_points[distvec[i + 1].second];
                                               Point p3 = palm_points[distvec[i + 2].second];
                                               soln_circle = circleFromPoints(p1, p2, p3);
                                               if(soln_circle.second!=0)
                                                  break;
                                           }
                                           palm_centers.push_back(soln_circle);
                                           if(palm_centers.size() > 10){
                                               palm_centers.erase(palm_centers.begin());
                                           }
                                           Point palm_center;
                                           double radius=0;
                                           for(int i = 0; i < palm_centers.size(); i++){
                                               palm_center += palm_centers[i].first;
                                               radius += palm_centers[i].second;
                                           }
                                           palm_center.x/=palm_centers.size();
                                           palm_center.y/=palm_centers.size();
                                           radius/=palm_centers.size();
                                           int center_x = (int) palm_center.x;
                                           int center_y = (int)  palm_center.y;
                                           circle(dst,palm_center,5,Scalar(144,144,255),3);
                                           circle(dst,palm_center,radius,Scalar(144,144,255),2);
                                           cout << " < < <  X=====";
                                           cout << center_x;
                                           cout << " > > > ";
                                           cout << " < < <  Y=====";
                                           cout << center_y;
                                           cout << " > > > ";
                                           cout << "\n";
                                       }
                                   }
               }

           }
           // 画轮廓
           drawContours(dst, filterContours, -1, Scalar(0,0,255), 3/*, 8, hierarchy*/);
           // 得到轮廓的凸包络
           /*for (size_t j=0; j<filterContours.size(); j++)
           {
               convexHull(Mat(filterContours[j]), hull, true);
               int hullcount = (int)hull.size();

               for (int i=0; i<hullcount-1; i++)
               {
                   line(dst, hull[i+1], hull[i], Scalar(255,0,0), 2, CV_AA);
               }
               line(dst, hull[hullcount-1], hull[0], Scalar(255,0,0), 2, CV_AA);
           }*/

           imshow(WIN_RESULT, dst);
           dst.release();
           // End

           c = cvWaitKey(delay);
           if (c == 27) break;
       }
}

void test_method1(){
        VideoCapture cap(0);;
        //capture.open("E:/image/01.avi");

        Mat frame;
        namedWindow("input", CV_WINDOW_AUTOSIZE);
        namedWindow("MOG2", CV_WINDOW_AUTOSIZE);
        namedWindow("KNN", CV_WINDOW_AUTOSIZE);
        Mat maskMOG2, maskKNN;
        Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2(500,25,true);
        Ptr<BackgroundSubtractor> pKNN = createBackgroundSubtractorKNN();

        Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
        while (cap.read(frame))
        {
            imshow("input", frame);
            pMOG2->apply(frame, maskMOG2);
            pKNN->apply(frame, maskKNN);
            //对处理后的帧进行开操作，减少视频中较小的波动造成的影响
            morphologyEx(maskMOG2,maskMOG2, MORPH_OPEN, kernel, Point(-1,-1));
            morphologyEx(maskKNN,maskKNN, MORPH_OPEN, kernel, Point(-1,-1));
            
            imshow("MOG2", maskMOG2);
            imshow("KNN", maskKNN);
            waitKey(3);
        }

        cap.release();
}

void test_method(){
     Mat frame;
     Mat back;
     Mat fore;
     QImage img;
     QImage fore1;
     vector<pair<Point,double> > palm_centers;
     VideoCapture cap(0);
     Ptr<BackgroundSubtractorMOG2> bg = createBackgroundSubtractorMOG2(100, 25, false);
     bg->setNMixtures(3);
     bg->setDetectShadows(false);

     namedWindow("Frame");
     namedWindow("Background");
     int backgroundFrame=500;
     for(;;)
         {
             vector<vector<Point> > contours;
             //Get the frame
             cap >> frame;
             //img = Mat2QImage(frame);
             //ui->Camera1->setPixmap(QPixmap::fromImage(img));
             //Update the current background model and get the foreground
             if(backgroundFrame>0)
             {bg->apply(frame,fore);backgroundFrame--;}
             else
             {bg->apply(frame,fore,0);}

             //Get background image to display it
             //bg.getBackgroundImage(back);
             bg->getBackgroundImage(back);

             //Enhance强调 edges in the foreground by applying erosion腐蚀 and dilation膨胀
             //cv::threshold(fore,fore,128, 255, cv::THRESH_BINARY_INV);
             erode(fore,fore,Mat());
             dilate(fore,fore,Mat());
             //cv::normalize(fore, fore, 0, 1., cv::NORM_MINMAX);
             //cv::threshold(fore, fore, .5, 1., CV_THRESH_BINARY);

             //Find the contours in the foreground
             findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
             for(int i=0;i<contours.size();i++)
                 //Ignore all small insignificant areas
                 if(contourArea(contours[i])>=5000)
                 {
                     //Draw contour
                     vector<vector<Point> > tcontours;
                     tcontours.push_back(contours[i]);
                     drawContours(frame,tcontours,-1,cv::Scalar(0,0,255),2);

                     //Detect Hull in current contour
                     vector<vector<Point> > hulls(1);
                     vector<vector<int> > hullsI(1);
                     convexHull(Mat(tcontours[0]),hulls[0],false);
                     convexHull(Mat(tcontours[0]),hullsI[0],false);
                     drawContours(frame,hulls,-1,cv::Scalar(0,255,0),2);

                     //Find minimum area rectangle to enclose hand
                     RotatedRect rect=minAreaRect(Mat(tcontours[0]));

                     //Find Convex Defects
                     vector<Vec4i> defects;
                     if(hullsI[0].size()>0)
                     {
                         Point2f rect_points[4]; rect.points( rect_points );
                         for( int j = 0; j < 4; j++ )
                             line( frame, rect_points[j], rect_points[(j+1)%4], Scalar(255,0,0), 1, 8 );
                         Point rough_palm_center;
                         convexityDefects(tcontours[0], hullsI[0], defects);
                         if(defects.size()>=3)
                         {
                             vector<Point> palm_points;
                             for(int j=0;j<defects.size();j++)
                             {
                                 int startidx=defects[j][0]; Point ptStart( tcontours[0][startidx] );
                                 int endidx=defects[j][1]; Point ptEnd( tcontours[0][endidx] );
                                 int faridx=defects[j][2]; Point ptFar( tcontours[0][faridx] );
                                 //Sum up all the hull and defect points to compute average
                                 rough_palm_center+=ptFar+ptStart+ptEnd;
                                 palm_points.push_back(ptFar);
                                 palm_points.push_back(ptStart);
                                 palm_points.push_back(ptEnd);
                             }

                             //Get palm center by 1st getting the average of all defect points, this is the rough palm center,
                             //Then U chose the closest 3 points ang get the circle radius and center formed from them which is the palm center.
                             rough_palm_center.x/=defects.size()*3;
                             rough_palm_center.y/=defects.size()*3;
                             Point closest_pt=palm_points[0];
                             vector<pair<double,int> > distvec;
                             for(int i=0;i<palm_points.size();i++)
                                 distvec.push_back(make_pair(dist(rough_palm_center,palm_points[i]),i));
                             sort(distvec.begin(),distvec.end());

                             //Keep choosing 3 points till you find a circle with a valid radius
                             //As there is a high chance that the closes points might be in a linear line or too close that it forms a very large circle
                             pair<Point,double> soln_circle;
                             for(int i=0;i+2<distvec.size();i++)
                             {
                                 Point p1=palm_points[distvec[i+0].second];
                                 Point p2=palm_points[distvec[i+1].second];
                                 Point p3=palm_points[distvec[i+2].second];
                                 soln_circle=circleFromPoints(p1,p2,p3);//Final palm center,radius
                                 if(soln_circle.second!=0)
                                     break;
                             }

                             //Find avg palm centers for the last few frames to stabilize its centers, also find the avg radius
                             palm_centers.push_back(soln_circle);
                             if(palm_centers.size()>10)
                                 palm_centers.erase(palm_centers.begin());

                             Point palm_center;
                             double radius=0;
                             for(int i=0;i<palm_centers.size();i++)
                             {
                                 palm_center+=palm_centers[i].first;
                                 radius+=palm_centers[i].second;
                             }
                             palm_center.x/=palm_centers.size();
                             palm_center.y/=palm_centers.size();
                             radius/=palm_centers.size();

                             //Draw the palm center and the palm circle
                             //The size of the palm gives the depth of the hand
                             circle(frame,palm_center,5,Scalar(144,144,255),3);
                             circle(frame,palm_center,radius,Scalar(144,144,255),2);

                             //Detect fingers by finding points that form an almost isosceles triangle with certain thesholds
                             int no_of_fingers=0;
                             for(int j=0;j<defects.size();j++)
                             {
                                 int startidx=defects[j][0]; Point ptStart( tcontours[0][startidx] );
                                 int endidx=defects[j][1]; Point ptEnd( tcontours[0][endidx] );
                                 int faridx=defects[j][2]; Point ptFar( tcontours[0][faridx] );
                                 //X o--------------------------o Y
                                 double Xdist=sqrt(dist(palm_center,ptFar));
                                 double Ydist=sqrt(dist(palm_center,ptStart));
                                 double length=sqrt(dist(ptFar,ptStart));

                                 double retLength=sqrt(dist(ptEnd,ptFar));
                                 //Play with these thresholds to improve performance
                                 if(length<=3*radius&&Ydist>=0.4*radius&&length>=10&&retLength>=10&&max(length,retLength)/min(length,retLength)>=0.8)
                                     if(min(Xdist,Ydist)/max(Xdist,Ydist)<=0.8)
                                     {
                                         if((Xdist>=0.1*radius&&Xdist<=1.3*radius&&Xdist<Ydist)||(Ydist>=0.1*radius&&Ydist<=1.3*radius&&Xdist>Ydist))
                                             line(frame, ptEnd, ptFar, Scalar(0,255,0), 1 );
                                             no_of_fingers++;
                                     }


									 
                             }
                             no_of_fingers=min(5,no_of_fingers);
                             cout<<"NO OF FINGERS: "<<no_of_fingers<<endl;
                             //mouseTo(palm_center.x,palm_center.y);//Move the cursor corresponding to the palm
                             // if(no_of_fingers<4)//If no of fingers is <4 , click , else release
                             //mouseClick();
                             // else
                             //mouseRelease();
                         }

                     }

                 }
               //if(backgroundFrame>0)
               //  putText(frame, "Recording Background", cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
             imshow("Frame",frame);
             imshow("Background",back);
             if(waitKey(10) >= 0) break;
         }
}

void test_1_create_mat(){
    float vals[] = {1,2,3,4};

    CvMat rotmate;

    cvInitMatHeader(&rotmate, 2, 2, CV_32FC1, vals);

    cout << rotmate.cols << endl;
    cout << rotmate.step << endl;
    cout << rotmate.rows << endl;
    cout << rotmate.type << endl;
}

int test_2_cv_mat_elem(){     //矩阵数据的存取
    float vals[] = {1,2,3,4};

    CvMat* rotmat = cvCreateMat(2, 2, CV_32FC1);

    //cvInitImageHeader(rotmat, 2, 2, CV_32FC1, vals);

    float ele = CV_MAT_ELEM(*rotmat, float, 1, 1);
    cout << ele << endl;
    getchar();
    return 0;
}

/*int c;
VideoCapture cap(0);
VideoCapture cap1(1);
VideoCapture cap2(2);
if(!cap.isOpened()){
    cout<<"no cap from #0"<<endl;
}
if(!cap1.isOpened()){
    cout<<"no cap from #1"<<endl;
}
if(!cap2.isOpened()){
    cout<<"no cap from #2"<<endl;
}
Mat frame, frame1, frame2, foreground1, foreground2;
Ptr<BackgroundSubtractorMOG2> mog = createBackgroundSubtractorMOG2(100, 25, false);
float learningRate = 0.01;
Lstop = false;

while(!Lstop){
    cap>>frame;
    cap1>>frame1;
    cap2>>frame2;
    QImage img = Mat2QImage(frame);
   // QImage img1 = Mat2QImage(frame1);
   //QImage img2 = Mat2QImage(frame2);
    ui->Camera1->setPixmap(QPixmap::fromImage(img));
    //mog->apply(frame1, foreground1, 0.01);
    //ui->Camera2->setPixmap(QPixmap::fromImage(img2));
    //ui->Camera3->setPixmap(QPixmap::fromImage(img2));
    mog->apply(frame, foreground2, 0.01);
    cv::threshold(foreground1, foreground1, 128, 255, cv::THRESH_BINARY_INV);
    cv::threshold(foreground2, foreground2, 128, 255, cv::THRESH_BINARY_INV);
    //ui->Camera1->setScaledContents(true);
    mog->apply(frame, foreground1, learningRate);
   // mog->apply(frame2, foreground2, learningRate);
    QImage fore1 = Mat2QImage(foreground1);
    //QImage fore2 = Mat2QImage(foreground2);
    ui->Foreground1->setPixmap(QPixmap::fromImage(fore1));
   // ui->Foreground2->setPixmap(QPixmap::fromImage(fore2));
    c = waitKey(30);
    if(c >= 0){
        waitKey(0);
    }
}
cvDestroyWindow("camera");*/
