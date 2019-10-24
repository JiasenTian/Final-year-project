#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <stdlib.h>
#include <iostream>
using namespace cv;
using namespace std;

int c;
bool Lstop;
int hand_x = 300, hand_y = 300, hand_z= 300;
int no_of_fingers_under=0;
int temp_joint_1, temp_joint_2, temp_joint_3, temp_joint_4, temp_joint_5, temp_joint_6;
int oldAngle[18] = {0, 9, 0, 0, 9, 0, 0, 9, 0, 0, 9, 0, 0, 9, 0, 0, 9, 0};
int newAngle[18] = {0, 9, 0, 0, 9, 0, 0, 9, 0, 0, 9, 0, 0, 9, 0, 0, 9, 0};
char newAngleChar[18] = {'0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0'};
char oldAngleChar[18] = {'0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0'};
char finalOrder[36] = {'0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0', '0', '9', '0'};
CSerialPort mySerialPort;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Lstop = false;
    mySerialPort.InitPort(5);
    mySerialPort.OpenListenThread();
    int delay = 1;
    char c;
    int frameNum = -1;          // Frame counter
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Start_clicked()
{
    VideoCapture cap_under(0);
    VideoCapture cap_side(2);
    int Frame1_x = (int) cap_under.get(CV_CAP_PROP_FRAME_WIDTH);
    int Frame1_y = (int) cap_under.get(CV_CAP_PROP_FRAME_HEIGHT);

    int Frame2_x = (int) cap_under.get(CV_CAP_PROP_FRAME_WIDTH);
    int Frame2_y = (int) cap_under.get(CV_CAP_PROP_FRAME_HEIGHT);

    bool bHandFlag = false;
    cout << Frame1_x;
    cout << "   ";
    cout << Frame1_y;

    Mat frame_under, frame_side;  // 输入视频帧序列
    Mat temp_under, temp_side;
    vector<Mat> channels_under, channels_side;
    Mat dst_under(frame_under); // 输出图像
    Mat dst_side(frame_side);
    Mat Y_under, Cr_under, Cb_under, Y_side, Cr_side, Cb_side;
//  Mat frameSplit[4];
    vector<pair<Point, double> > palm_centers_1, palm_centers_2;
    vector< vector<Point> > contours_1, contours_2;   // 轮廓
    vector< vector<Point> > filterContours_1, filterContours_2; // 筛选后的轮廓
    vector< Vec4i > hierarchy_1, hierarchy_2;    // 轮廓的结构信息
    vector< Point > hull_1, hull_2; // 凸包络的点集

    while(!Lstop) //Show the image captured in the window and repeat
    {
        cap_under >> frame_under;
        cap_side >> frame_side;

        if( frame_under.empty())
        {
            cout << " < < <  First camera failed to catch!  > > > ";
            break;
        }
        if( frame_side.empty())
        {
            cout << " < < <  Second camera failed to catch!  > > > ";
            break;
        }
        QImage view_under = Mat2QImage(frame_under);
        ui->Camera1->setPixmap(QPixmap::fromImage(view_under));

        QImage view_side = Mat2QImage(frame_side);
        ui->Camera2_2->setPixmap(QPixmap::fromImage(view_side));
        // Begin

        // 中值滤波，去除椒盐噪声
        medianBlur(frame_under, frame_under, 5);
        medianBlur(frame_side, frame_side, 5);
        // 转换到HSV颜色空间，更容易处理
//      cvtColor( frame, frameHSV, CV_BGR2HSV );
        frame_under.copyTo(temp_under);
        frame_side.copyTo(temp_side);

        cvtColor(temp_under, temp_under, CV_BGR2YCrCb);
        cvtColor(temp_side, temp_side, CV_BGR2YCrCb);

        split(temp_under, channels_under);
        split(temp_side, channels_side);

        Y_under = channels_under.at(0);
        Cr_under = channels_under.at(1);
        Cb_under = channels_under.at(2);

        Y_side = channels_side.at(0);
        Cr_side = channels_side.at(1);
        Cb_side = channels_side.at(2);


        Mat dstTemp_under(frame_under.rows, frame_under.cols, CV_8UC1);
        Mat dstTemp_side(frame_side.rows, frame_side.cols, CV_8UC1);

        for (int j = 1; j < Y_under.rows - 1; j++)  {
            uchar* currentCr = Cr_under.ptr< uchar>(j);
            uchar* currentCb = Cb_under.ptr< uchar>(j);
            uchar* current = dstTemp_under.ptr< uchar>(j);
            for (int i = 1; i < Y_under.cols - 1; i++)  {
               if ((currentCr[i] > 137) && (currentCr[i] < 175) && (currentCb[i] > 100) && (currentCb[i] < 118))
                  current[i] = 255;
               else
                  current[i] = 0;
               }
        }

        for (int j = 1; j < Y_side.rows - 1; j++)  {
            uchar* currentCr = Cr_side.ptr< uchar>(j);
            uchar* currentCb = Cb_side.ptr< uchar>(j);
            uchar* current = dstTemp_side.ptr< uchar>(j);
            for (int i = 1; i < Y_side.cols - 1; i++)  {
               if ((currentCr[i] > 137) && (currentCr[i] < 175) && (currentCb[i] > 100) && (currentCb[i] < 118))
                  current[i] = 255;
               else
                  current[i] = 0;
               }
        }


        // 形态学操作，去除噪声，并使手的边界更加清晰
        Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
        erode(dstTemp_under, dstTemp_under, element);
        erode(dstTemp_side, dstTemp_side, element);
        morphologyEx(dstTemp_under, dstTemp_under, MORPH_OPEN, element);
        morphologyEx(dstTemp_side, dstTemp_side, MORPH_OPEN, element);
        dilate(dstTemp_under, dstTemp_under, element);
        dilate(dstTemp_side, dstTemp_side, element);
        morphologyEx(dstTemp_under, dstTemp_under, MORPH_CLOSE, element);
        morphologyEx(dstTemp_side, dstTemp_side, MORPH_CLOSE, element);

        frame_under.copyTo(dst_under, dstTemp_under);
        frame_side.copyTo(dst_side, dstTemp_side);

        contours_1.clear();
        contours_2.clear();
        hierarchy_1.clear();
        hierarchy_2.clear();
        filterContours_1.clear();
        filterContours_2.clear();
        // 得到手的轮廓
        findContours(dstTemp_under, contours_1, hierarchy_1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        findContours(dstTemp_side, contours_2, hierarchy_2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        // 去除伪轮廓
        for (size_t i = 0; i < contours_1.size(); i++)
        {
            if (fabs(contourArea(Mat(contours_1[i]))) > 6000) //判断手进入区域的阈值
            {
                vector<vector<Point> > tcontours;
                tcontours.push_back(contours_1[i]);
                filterContours_1.push_back(contours_1[i]);
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
                     line(dst_under, rect_points[j], rect_points[(j + 1)%4],Scalar(0,155,155), 1, 8);
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
                      palm_centers_1.push_back(soln_circle);
                      if(palm_centers_1.size() > 10){
                         palm_centers_1.erase(palm_centers_1.begin());
                      }
                      Point palm_center;
                      double radius=0;
                      for(int i = 0; i < palm_centers_1.size(); i++){
                          palm_center += palm_centers_1[i].first;
                          radius += palm_centers_1[i].second;
                      }
                      palm_center.x/=palm_centers_1.size();
                      palm_center.y/=palm_centers_1.size();
                      radius/=palm_centers_1.size();

                      circle(dst_under,palm_center,5,Scalar(144,144,255),3);
                      circle(dst_under,palm_center,radius,Scalar(144,144,255),2);
                      if((int) palm_center.x > 0 && (int) palm_center.x <= 600){
                         hand_x = (int) palm_center.x;
                      }
                      else{
                         hand_x = 320;
                      }
                      if((int) palm_center.y > 0 && (int) palm_center.y <= 600){
                         hand_y = (int) palm_center.y;
                      }
                      else{
                          hand_y = 240;
                      }


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
                                  if((Xdist>=0.1*radius&&Xdist<=1.3*radius&&Xdist<Ydist)||(Ydist>=0.1*radius&&Ydist<=1.3*radius&&Xdist>Ydist)){
                                      line(frame_under, ptEnd, ptFar, Scalar(0,0,0), 1 );
                                      no_of_fingers_under++;
                                  }
                              }
                      }
                      no_of_fingers_under = min(5,no_of_fingers_under);
                      cout<<"NO OF FINGERS IN UNDER CAMERA: "<<no_of_fingers_under<<endl;
                   }

                 }

             }
        }



        for (size_t i = 0; i < contours_2.size(); i++)
        {
            if (fabs(contourArea(Mat(contours_2[i]))) > 5000) //判断手进入区域的阈值
            {   vector<vector<Point> > tcontours;
                tcontours.push_back(contours_2[i]);
                filterContours_2.push_back(contours_2[i]);
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
                     line(dst_side, rect_points[j], rect_points[(j + 1)%4],Scalar(0,155,155), 1, 8);
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
                    palm_centers_2.push_back(soln_circle);
                    if(palm_centers_2.size() > 10){
                       palm_centers_2.erase(palm_centers_2.begin());
                    }
                    Point palm_center;
                    double radius=0;
                    for(int i = 0; i < palm_centers_2.size(); i++){
                        palm_center += palm_centers_2[i].first;
                        radius += palm_centers_2[i].second;
                    }
                    palm_center.x/=palm_centers_2.size();
                    palm_center.y/=palm_centers_2.size();
                    circle(dst_side,palm_center,5,Scalar(144,144,255),3);
                    circle(dst_side,palm_center,10,Scalar(144,144,255),2);

                    if((int) palm_center.y > 0 && (int) palm_center.y <= 600){
                       hand_z = (int) palm_center.y;
                    }
                    else
                       hand_z = 240;

                    }
                }
            }
        }
        // 画轮廓
        drawContours(dst_under, filterContours_1, -1, Scalar(0,0,255), 3/*, 8, hierarchy*/);
        drawContours(dst_side, filterContours_2, -1, Scalar(0,0,255), 3/*, 8, hierarchy*/);
        cout << " < < <  X2=====";
        cout << hand_x;
        cout << " > > > ";
        cout << " < < <  ";
        cout << hand_y;
        cout << " > > > ";
        cout << hand_z;
        cout << "\n";
        ui->x->display(hand_x);
        ui->y->display(hand_y);
        ui->z->display(hand_z);
        transformVideoData(hand_x, hand_y, hand_z, no_of_fingers_under);

        // 得到轮廓的凸包络


        QImage under = Mat2QImage(dst_under);
        ui->Foreground1->setPixmap(QPixmap::fromImage(under));

        QImage side = Mat2QImage(dst_side);
        ui->Foreground2->setPixmap(QPixmap::fromImage(side));
        dst_under.release();
        dst_side.release();
        // End

        c = cvWaitKey(1);
        if (c == 27) break;
    }
}

int MainWindow::tan_angle(int x, int y){
    int X = x - 320;
    if(X == 0){
        return 90;
    }
    else if(X < 0){
        X = -X;
        double y_scale = y/480;
        double x_scale = X/640;
      //  double temp = atan(y_scale,x_scale);
       // int angle = (temp * 180)/3.1415;
        return 45;
    }
    else{
        double y_scale = y/480;
        double x_scale = X/640;
        double temp = atan(y_scale/x_scale);
        int angle = (temp * 180)/3.1415;
        return 135;
    }
}

/*int MainWindow::sin(int x, int y){
return 0;
}

int MainWindow::cos(int x, int y){
return 0;
}
*/

int MainWindow::getAngleOfJoint6(int x, int y){
    cout<<"~~~~~~~~~~~~~~~~~~~~~~";
    cout<<tan_angle(x, y);
    cout<<"~~~~~~~~~~~~~~~~~~~~~~\n";
    return tan_angle(x, y);
//640   480
}

int MainWindow::getAngleOfJoint5(int x, int y, int z){
    if(y >= 320)
        return 135;
    else if( y < 320 && y >= 160)
        return 90;
    else if(y>=0 && y < 160)
        return 45;
}

int MainWindow::getAngleOfJoint4(int x, int y, int z){
    if(y >= 320){
        if(z >=0 && z <= 240){
            return 135;
        }
        else
            return 180;
    }
    else if(y < 320 && y >= 160){
        if(z >=0 && z <= 160){
            return 90;
        }
        else if(z>160 && z<= 320)
            return 135;
        else
            return 180;
    }
    else if(y>=0 && y < 160){
        if(z >=0 && z < 160)
            return 45;
        else if(z >= 160 && z < 320)
            return 90;
        else
            return 135;

    }
}

int MainWindow::getAngleOfJoint3(int x, int y, int z){
    if(y >= 320){
        if(z >=0 && z <= 240){
            return 90;
        }
        else
            return 45;
    }
    else if(y < 320 && y >= 160){
        if(z >=0 && z <= 160){
            return 90;
        }
        else if(z>160 && z<= 320)
            return 45;
        else
            return 1;
    }
    else if(y>=0 && y < 160){
        if(z >=0 && z < 160)
            return 90;
        else if(z >= 160 && z < 320)
            return 45;
        else
            return 1;

    }
}

int MainWindow::getAngleOfJoint2(int x, int y, int z){

}

int MainWindow::getAngleOfJoint1(int numberOfFinger){
    if(numberOfFinger != 0){
        return 135;
    }
    else{
        return 90;
    }
}

void MainWindow::changeFormate(int angle, int posi){
    int temp = (posi-1)*3;
    char temp1 = NumberTransfor(angle/100);
    char temp2 = NumberTransfor((angle%100)/10);
    char temp3 = NumberTransfor(angle%10);

    oldAngleChar[temp] = newAngleChar[temp];
    oldAngleChar[temp+1] = newAngleChar[temp+1];
    oldAngleChar[temp+2] = newAngleChar[temp+2];
    newAngleChar[temp] = temp1;
    newAngleChar[temp+1] = temp2;
    newAngleChar[temp+2] = temp3;
    switch(posi){
    case 1: ui->joint1->display(angle); break;
    case 2: ui->joint2->display(angle); break;
    case 3: ui->joint3->display(angle); break;
    case 4: ui->joint4->display(angle); break;
    case 5: ui->joint5->display(angle); break;
    case 6: ui->joint6->display(angle); break;
    }
}

char MainWindow::NumberTransfor(int n){
    n+=48;
    return n;
}

void MainWindow::combinNewAndOldOrder(){
    for(int i = 0; i < 18; i=i+3){
        finalOrder[i * 2] = oldAngleChar[i];
        finalOrder[i * 2 + 1] = oldAngleChar[i + 1];
        finalOrder[i * 2 + 2] = oldAngleChar[i + 2];
        finalOrder[i * 2 + 3] = newAngleChar[i];
        finalOrder[i * 2 + 4] = newAngleChar[i + 1];
        finalOrder[i * 2 + 5] = newAngleChar[i + 2];
    }
}

void MainWindow::sendData(char* temp, CSerialPort mySerialPort){
    mySerialPort.WriteData(temp, 36);
}

void MainWindow::transformVideoData(int x, int y, int z, int numOfFinger){
    changeFormate(getAngleOfJoint6(x, y), 6);
    changeFormate(getAngleOfJoint5(x, y, z), 5);
    changeFormate(getAngleOfJoint4(x, y, z), 4);
    changeFormate(getAngleOfJoint3(x, y, z), 3);
    //changeFormate(getAngleOfJoint5(x, y, z), 2);
    changeFormate(getAngleOfJoint1(numOfFinger), 1);
    combinNewAndOldOrder();

    for(int i=0; i<36; ++i ){
        cout<< finalOrder[i];
    }
    cout<<"\n";
    sendData(finalOrder, mySerialPort);
}


double MainWindow::dist(Point x,Point y)
{
    return (x.x-y.x)*(x.x-y.x)+(x.y-y.y)*(x.y-y.y);
}

pair<Point,double> MainWindow::circleFromPoints(Point p1, Point p2, Point p3)
{
    double offset = pow(p2.x,2) +pow(p2.y,2);
    double bc =   ( pow(p1.x,2) + pow(p1.y,2) - offset )/2.0;
    double cd =   (offset - pow(p3.x, 2) - pow(p3.y, 2))/2.0;
    double det =  (p1.x - p2.x) * (p2.y - p3.y) - (p2.x - p3.x)* (p1.y - p2.y);
    double TOL = 0.0000001;
    if (abs(det) < TOL) { cout<<"POINTS TOO CLOSE"<<endl;return make_pair(Point(0,0),0); }

    double idet = 1/det;
    double centerx =  (bc * (p2.y - p3.y) - cd * (p1.y - p2.y)) * idet;
    double centery =  (cd * (p1.x - p2.x) - bc * (p2.x - p3.x)) * idet;
    double radius = sqrt( pow(p2.x - centerx,2) + pow(p2.y-centery,2));

    return make_pair(Point(centerx,centery),radius);
}

QImage MainWindow::Mat2QImage(const cv::Mat &mat)
{
    if(mat.type()==CV_8UC1)
    {
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));

        const uchar *qImageBuffer = (const uchar*)mat.data;
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }

    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        return QImage();
    }
}

void MainWindow::on_Pause_clicked()
{
    Lstop = true;
}

void Mog(){

}
