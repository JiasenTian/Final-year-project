#include "kalma.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <cmath>
#include <vector>
#include <iostream>
using namespace std;


const int winHeight=600;
const int winWidth=800;
Point mousePosition=cvPoint(winWidth>>1,winHeight>>1);

inline Point kalma::calcPoint(Point2f center, double R, double angle){
    return center + Point2f((float)cos(angle), (float)-sin(angle))*(float)R;
}

kalma::kalma()
{

}

void mouseEvent(int event, int x, int y, int flags, void *param){
    if (event==CV_EVENT_MOUSEMOVE) {
            mousePosition=cvPoint(x,y);
        }
}

void kalma::kalman_2d_point_test(){
    //1.kalman filter setup
       const int stateNum=4;
       const int measureNum=2;
       CvKalman* kalman = cvCreateKalman( stateNum, measureNum, 0 );//state(x,y,detaX,detaY)
       CvMat* process_noise = cvCreateMat( stateNum, 1, CV_32FC1 );
       CvMat* measurement = cvCreateMat( measureNum, 1, CV_32FC1 );//measurement(x,y)
       CvRNG rng = cvRNG(-1);
       float A[stateNum][stateNum] ={//transition matrix
           1,0,1,0,
           0,1,0,1,
           0,0,1,0,
           0,0,0,1
       };

       memcpy( kalman->transition_matrix->data.fl,A,sizeof(A));
       cvSetIdentity(kalman->measurement_matrix,cvRealScalar(1) );
       cvSetIdentity(kalman->process_noise_cov,cvRealScalar(1e-5));
       cvSetIdentity(kalman->measurement_noise_cov,cvRealScalar(1e-1));
       cvSetIdentity(kalman->error_cov_post,cvRealScalar(1));
       //initialize post state of kalman filter at random
       cvRandArr(&rng,kalman->state_post,CV_RAND_UNI,cvRealScalar(0),cvRealScalar(winHeight>winWidth?winWidth:winHeight));

       CvFont font;
       cvInitFont(&font,CV_FONT_HERSHEY_SCRIPT_COMPLEX,1,1);

       cvNamedWindow("kalman");
       cvSetMouseCallback("kalman",mouseEvent);
       IplImage* img=cvCreateImage(cvSize(winWidth,winHeight),8,3);
       while (1){
           //2.kalman prediction
           const CvMat* prediction=cvKalmanPredict(kalman,0);
           CvPoint predict_pt=cvPoint((int)prediction->data.fl[0],(int)prediction->data.fl[1]);

           //3.update measurement
           measurement->data.fl[0]=(float)mousePosition.x;
           measurement->data.fl[1]=(float)mousePosition.y;

           //4.update
           cvKalmanCorrect( kalman, measurement );

           //draw
           cvSet(img,cvScalar(255,255,255,0));
           cvCircle(img,predict_pt,5,CV_RGB(0,255,0),3);//predicted point with green
           cvCircle(img,mousePosition,5,CV_RGB(255,0,0),3);//current position with red
           char buf[256];
           sprintf_s(buf,256,"predicted position:(%3d,%3d)",predict_pt.x,predict_pt.y);
           cvPutText(img,buf,cvPoint(10,30),&font,CV_RGB(0,0,0));
           sprintf_s(buf,256,"current position :(%3d,%3d)",mousePosition.x,mousePosition.y);
           cvPutText(img,buf,cvPoint(10,60),&font,CV_RGB(0,0,0));

           cvShowImage("kalman", img);
           int key=cvWaitKey(3);
           if (key==27){//esc
               break;
           }
       }

       cvReleaseImage(&img);
       cvReleaseKalman(&kalman);
}

void kalma::sample_test(){
    /*
    Description:    手势检测
                    先滤波去噪
                    -->转换到HSV空间
                    -->根据皮肤在HSV空间的分布做出阈值判断，这里用到了inRange函数，
                    然后进行一下形态学的操作，去除噪声干扰，是手的边界更加清晰平滑
                    -->得到的2值图像后用findContours找出手的轮廓，去除伪轮廓后，再用convexHull函数得到凸包络

    */
       int delay = 1;
       char c;
       int frameNum = -1;          // Frame counter
       bool lastImgHasHand=false;

       int previousX=0;
       int previousY=0;
       VideoCapture cap(0);
       //Size refS = Size( (int) captRefrnc.get(CV_CAP_PROP_FRAME_WIDTH),
       //  (int) captRefrnc.get(CV_CAP_PROP_FRAME_HEIGHT) );

       bool bHandFlag = false;
       const char* WIN_SRC = "Source";
       const char* WIN_RESULT = "Result";

       // Windows
       namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE );
       namedWindow(WIN_RESULT, CV_WINDOW_AUTOSIZE);

       Mat frame;  // 输入视频帧序列
       Mat frameHSV;   // hsv空间
       Mat mask(frame.rows, frame.cols, CV_8UC1);  // 2值掩膜
       Mat dst(frame); // 输出图像

   //  Mat frameSplit[4];

       vector< vector<Point> > contours;   // 轮廓
       vector< vector<Point> > filterContours; // 筛选后的轮廓
       vector< Vec4i > hierarchy;    // 轮廓的结构信息
       vector< Point > hull; // 凸包络的点集

       bool movement=false;
       int count=0;

       int presentX=0;
       int presentY=0;

       while(true) //Show the image captured in the window and repeat
       {
           //captRefrnc >> frame;
           int minX=320;//屏幕的一半
       int maxX=240;
       int minY=320;
       int maxY=240;

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
           cvtColor( frame, frameHSV, CV_BGR2HSV );

           Mat dstTemp1(frame.rows, frame.cols, CV_8UC1);
           Mat dstTemp2(frame.rows, frame.cols, CV_8UC1);
           // 对HSV空间进行量化，得到2值图像，亮的部分为手的形状
           inRange(frameHSV, Scalar(0,30,30), Scalar(40,170,256), dstTemp1);
           inRange(frameHSV, Scalar(156,30,30), Scalar(180,170,256), dstTemp2);
           bitwise_or(dstTemp1, dstTemp2, mask);
       //  inRange(frameHSV, Scalar(0,30,30), Scalar(180,170,256), dst);

           // 形态学操作，去除噪声，并使手的边界更加清晰
           Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
           erode(mask, mask, element);
           morphologyEx(mask, mask, MORPH_OPEN, element);
           dilate(mask, mask, element);
           morphologyEx(mask, mask, MORPH_CLOSE, element);
           frame.copyTo(dst, mask);
           contours.clear();
           hierarchy.clear();
           filterContours.clear();
           // 得到手的轮廓
           findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
           // 去除伪轮廓

           for (size_t i = 0; i < contours.size(); i++)
           {
           //  approxPolyDP(Mat(contours[i]), Mat(approxContours[i]), arcLength(Mat(contours[i]), true)*0.02, true);
               if (fabs(contourArea(Mat(contours[i]))) > 30000) //判断手进入区域的阈值
               {
                   filterContours.push_back(contours[i]);
               }
           }
           // 画轮廓
           if(filterContours.size()>0)
           {

               count++;
               lastImgHasHand=true;
               drawContours(dst, filterContours, -1, Scalar(255,0,255), 3/*, 8, hierarchy*/);

               for (size_t j=0; j<filterContours.size(); j++)
               {
                   convexHull(Mat(filterContours[j]), hull, true);
                   int hullcount = (int)hull.size();
                   for (int i=0; i<hullcount-1; i++)
                   {
                       line(dst, hull[i+1], hull[i], Scalar(255,255,255), 2, CV_AA);//白色
                       printf("num%d:x=%d\ty=%d\t\n",i,hull[i].x,hull[i].y);
                       if(hull[i].x>maxX)
                           maxX=hull[i].x;
                       if(hull[i].x<minX)
                           minX=hull[i].x;
                       if(hull[i].y>maxY)
                           maxY=hull[i].y;
                       if(hull[i].y<minY)
                           minY=hull[i].y;
                       printf("miniX=%d\tminiY=%d\tmaxX=%d\tmaxY=%d\t\n",minX,minY,maxX,maxY);

                   }

                   line(dst, hull[hullcount-1], hull[0], Scalar(0,255,0), 2, CV_AA);//绿色，最后一条

                   if(count==1)//第一个轮廓的中心位置存在全局变量中，到最后一个再跟它比。
                   {
                       previousX=(minX+maxX)/2;
                       printf("previousX=%d\n",previousX);
                       previousY=(minY+maxY)/2;
                       printf("previousY=%d\n",previousY);
                   }
                   else
                   {
                       presentX=(minX+maxY)/2;
                       presentY=(minY+maxY)/2;
                   }
               }
           }
           else
           {
               if(lastImgHasHand==true)
               {
                   if((previousX-presentX)<0)//中文的大括号和英文的大括号用肉眼看不出来，坑啊
                   {
                       printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<left\n");//镜像，没有flip过来，所以这里注意点。
                       //keybd_event(VK_LEFT,(BYTE)0, 0 ,0);
                       //keybd_event(VK_LEFT, (BYTE)0, KEYEVENTF_KEYUP,0);
                   }
                   if((previousX-presentX)>0)
                   {
                        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>right\n");
                        //keybd_event(VK_RIGHT,(BYTE)0, 0 ,0);
                        //keybd_event(VK_RIGHT, (BYTE)0, KEYEVENTF_KEYUP,0);
                   }
                   if((previousY-presentY)<0)
                   {
                       printf("downVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n");
                   }
                   if((previousY-presentY)>0)
                   {
                       printf("upAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n");
                   }

                   count=0;
                   lastImgHasHand=false;
               }
           }

           imshow(WIN_RESULT, dst);
           dst.release();


           printf("previousX=%d\tpresentX=%d\tpreviousY=%d\tpresentY=%d\n",previousX,presentX,previousY,presentY);
           printf("count=%d\n",count);
           // End
           c = cvWaitKey(1);
           if (c == 27)
               break;
       }
}

void kalma::kalman_test(){
    /*
       使用kalma步骤一
       下面语句到for前都是kalman的初始化过程，一般在使用kalman这个类时需要初始化的值有：
       转移矩阵，测量矩阵，过程噪声协方差，测量噪声协方差，后验错误协方差矩阵，
       前一状态校正后的值，当前观察值
       */

       Mat img(500, 500, CV_8UC3);
       KalmanFilter KF(2, 1, 0);
       Mat state(2, 1, CV_32F); /* (phi, delta_phi) */
       Mat processNoise(2, 1, CV_32F);
       Mat measurement = Mat::zeros(1, 1, CV_32F);
       char code = (char)-1;

       for (;;)
       {
           randn(state, Scalar::all(0), Scalar::all(0.1));//产生均值为0，标准差为0.1的二维高斯列向量
           KF.transitionMatrix = (Mat_<float>(2, 2) << 1, 1, 0, 1);//转移矩阵为[1,1;0,1]

           //函数setIdentity是给参数矩阵对角线赋相同值，默认对角线值值为1
           setIdentity(KF.measurementMatrix);
           setIdentity(KF.processNoiseCov, Scalar::all(1e-5));//系统过程噪声方差矩阵
           setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));//测量过程噪声方差矩阵
           setIdentity(KF.errorCovPost, Scalar::all(1));//后验错误估计协方差矩阵

           //statePost为校正状态，其本质就是前一时刻的状态
           randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));

           for (;;)
           {
               Point2f center(img.cols*0.5f, img.rows*0.5f);
               float R = img.cols / 3.f;
               //state中存放起始角，state为初始状态
               double stateAngle = state.at<float>(0);
               Point statePt = calcPoint(center, R, stateAngle);


               /*
               使用kalma步骤二
               调用kalman这个类的predict方法得到状态的预测值矩阵
               */


               Mat prediction = KF.predict();
               //用kalman预测的是角度
               double predictAngle = prediction.at<float>(0);
               Point predictPt = calcPoint(center, R, predictAngle);

               randn(measurement, Scalar::all(0), Scalar::all(KF.measurementNoiseCov.at<float>(0)));

               // generate measurement
               //带噪声的测量
               measurement += KF.measurementMatrix*state;

               double measAngle = measurement.at<float>(0);
               Point measPt = calcPoint(center, R, measAngle);

               // plot points
               //这个define语句是画2条线段(线长很短)，其实就是画一个“X”叉符号

   #define drawCross( center, color, d )                                 \
                   line( img, Point( center.x - d, center.y - d ),                \
                                Point( center.x + d, center.y + d ), color, 1, CV_AA, 0); \
                   line( img, Point( center.x + d, center.y - d ),                \
                                Point( center.x - d, center.y + d ), color, 1, CV_AA, 0 )

               img = Scalar::all(0);
               //状态坐标白色
               drawCross(statePt, Scalar(255, 255, 255), 3);
               //测量坐标蓝色
               drawCross(measPt, Scalar(0, 0, 255), 3);
               //预测坐标绿色
               drawCross(predictPt, Scalar(0, 255, 0), 3);
               //真实值和测量值之间用红色线连接起来
               line(img, statePt, measPt, Scalar(0, 0, 255), 3, CV_AA, 0);
               //真实值和估计值之间用黄色线连接起来
               line(img, statePt, predictPt, Scalar(0, 255, 255), 3, CV_AA, 0);


               /*
               使用kalma步骤三
               调用kalman这个类的correct方法得到加入观察值校正后的状态变量值矩阵
               */

               if (theRNG().uniform(0, 4) != 0)
                   KF.correct(measurement);

               randn(processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
               //不加噪声的话就是匀速圆周运动，加了点噪声类似匀速圆周运动，因为噪声的原因，运动方向可能会改变
               state = KF.transitionMatrix*state + processNoise;

               imshow("Kalman", img);
               code = (char)waitKey(100);

               if (code > 0)
                   break;
           }
           if (code == 27 || code == 'q' || code == 'Q')
               break;
       }
}

