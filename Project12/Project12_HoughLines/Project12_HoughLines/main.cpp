#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
Mat src1,gray_src,src2,dst;
int main()
{
    src1 = imread("result.png");
    if (src1.empty())
    {
        cout << "could not find src1" << endl;
        return -1;
    }
    namedWindow("input", CV_WINDOW_AUTOSIZE);
    imshow("input", src1);
    //霍夫直线   canny边缘提取 --> cvtcolor转灰度 --> HoughLinesP霍夫边缘检测
    cvtColor(src1,src1,CV_BGR2GRAY);
    Canny(src1,gray_src,100,200,3,false);
    cvtColor(gray_src,dst,CV_GRAY2BGR);
    vector<Vec4f> lines;

    HoughLinesP(gray_src,lines,1,CV_PI/180,1,0,0);

    for (size_t i = 0; i < lines.size(); i++)
    {
        Vec4f plines=lines[i];  //一个plines里边是四个点一条直线
        line(dst,Point(plines[0],plines[1]),Point(plines[2],plines[3]),Scalar(0,0,255),3,LINE_AA);
    }
    imshow("output", dst);
    waitKey(0);
    return 0;
}
