#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;

int main()
 {
    const char* picName = "photo1.png";
        //采用IplImage *img = cvLoadImage(picName)默认值是CV_LOAD_IMAGE_COLOR  读取无论原始图像的通道数是多少，都将被转换为3个通道读入。
        //IplImage *img = cvLoadImage(picName,CV_LOAD_IMAGE_GRAYSCALE);
        IplImage *img = cvLoadImage(picName);
        if (img == NULL){
            cout << "Load File Failed." << endl;
        }
        IplImage* imgChannel[4] = { NULL, NULL, NULL, NULL };
        IplImage* imgDst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);

        //创建单通道图像
        for (int i = 0; i < img->nChannels; i++){
            imgChannel[i] = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
        }
        //图像三通道分离
        cvSplit(img, imgChannel[0], imgChannel[1], imgChannel[2], imgChannel[3]);//BGRA

        for (int i = 0; i < img->nChannels; i++){
            //直方图均衡化中原始图像和目标图像都必须是单通道
            cvEqualizeHist(imgChannel[i], imgChannel[i]);
        }
        //通道组合
        cvMerge(imgChannel[0], imgChannel[1], imgChannel[2], NULL, imgDst);

        cvNamedWindow("Origin", CV_WINDOW_AUTOSIZE);
        cvShowImage("Origin",img);

        //直方图均衡化
        cvNamedWindow("Hist", CV_WINDOW_AUTOSIZE);
        cvShowImage("Hist", imgDst);

        cvWaitKey(0);


 }
