#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

Mat srcImg,tmpImg, MidImg;
void on_mouse(int EVENT, int x, int y, int flags, void* ustc);

int main(void)
{
    srcImg = imread("photo1.png");
    namedWindow("DealImg", WINDOW_NORMAL);
    srcImg.copyTo(tmpImg);
    imshow("DealImg", tmpImg);
    setMouseCallback("DealImg", on_mouse, (void*)(&tmpImg)); //调用回调函数
    waitKey(0);
    destroyWindow("DealImg");
}

void on_mouse(int EVENT, int x, int y, int flags, void* ustc)
{
    static Point Pre_pt(-1, -1);
    static Point Cur_pt(-1, -1);
    char Txt_Point[50] = { 0 };
    Mat dst;
    CvScalar color;
    if (ustc == NULL)
    {
        return;
    }

    if (EVENT == CV_EVENT_LBUTTONDOWN)                                      //左键按下，读取初始坐标
    {
        Pre_pt = Point(x, y);
        color = tmpImg.at<Vec3b>(Cur_pt.x,Cur_pt.y);
        sprintf(Txt_Point, "(%d,%d) (%d %d %d)", x, y,int(color.val[2]),int(color.val[1]),int(color.val[0]));
        putText(tmpImg, Txt_Point, Pre_pt,CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
        imshow("DealImg", tmpImg);
    }

    else if ((EVENT == CV_EVENT_MOUSEMOVE) && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
    {
        if (MidImg.data == NULL)
        {
            srcImg.copyTo(tmpImg);                                            //将src复制到临时图像tmp上，用于显示实时坐标
        }
        else
        {
            MidImg.copyTo(tmpImg);                                             //当已划定矩形的情况时，在图像上显示前面所画图像
        }
        Cur_pt = Point(x, y);
        color = tmpImg.at<Vec3b>(Cur_pt.x,Cur_pt.y);
        sprintf(Txt_Point, "(%d,%d) (%d %d %d)", x, y,int(color.val[2]),int(color.val[1]),int(color.val[0]));
        putText(tmpImg, Txt_Point, Cur_pt,CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
        imshow("DealImg", tmpImg);
    }

    else if (EVENT == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形
    {
        srcImg.copyTo(MidImg);
        Cur_pt = Point(x, y);
        color = tmpImg.at<Vec3b>(Cur_pt.x,Cur_pt.y);
        sprintf(Txt_Point, "(%d,%d) (%d %d %d)", x, y,int(color.val[2]),int(color.val[1]),int(color.val[0]));
        putText(MidImg, Txt_Point, Cur_pt,CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
        rectangle(MidImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
        imshow("DealImg", MidImg);
    }
    else if (EVENT == CV_EVENT_LBUTTONUP)                                          //左键松开，将在图像上划矩形
    {
        srcImg.copyTo(MidImg);
        color = tmpImg.at<Vec3b>(Cur_pt.x,Cur_pt.y);
        sprintf(Txt_Point, "(%d,%d) (%d %d %d)", x, y,int(color.val[2]),int(color.val[1]),int(color.val[0]));
        Cur_pt = Point(x, y);
        putText(MidImg, Txt_Point, Cur_pt,CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
        rectangle(MidImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
        imshow("DealImg", MidImg);

        int width = abs(Pre_pt.x - Cur_pt.x);
        int height = abs(Pre_pt.y - Cur_pt.y);
        if (width == 0 || height == 0)
        {
            printf("width == 0 || height == 0 \n");
            return;
        }
        dst = srcImg(Rect(min(Cur_pt.x,Pre_pt.x),min(Cur_pt.y,Pre_pt.y),width,height));
        namedWindow("dst");
        imshow("dst",dst);
        cout<<"Center Point (x,y) = "<<(Pre_pt.x+Cur_pt.x)/2<<","<<(Pre_pt.y+Cur_pt.y)/2<<endl;
        waitKey(0);

    }
}
