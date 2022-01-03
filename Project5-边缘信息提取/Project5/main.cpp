#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

//边缘检测
int main()
{
    Mat img = imread("result.png");
    imshow("Original", img);
    Mat DstPic, edge, grayImage;

    //创建与src同类型和同大小的矩阵
    DstPic.create(img.size(), img.type());

    //将原始图转化为灰度图
    cvtColor(img, grayImage, COLOR_BGR2GRAY);

    //先使用3*3内核来降噪
    blur(grayImage, edge, Size(3, 3));

    //运行canny算子
    Canny(edge, edge, 3, 9, 3);
    cv::imshow("edge", edge);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edge, contours, CV_RETR_EXTERNAL,//检索外部轮廓
    CV_CHAIN_APPROX_NONE);//每个轮廓的全部像素
    cv::Mat result(edge.size(), CV_8UC3, cv::Scalar(0,0,0));
    cv::drawContours(result, contours,
            -1,//画全部轮廓
            cv::Scalar(0,255,0),//用绿色画
            2);//宽度为2
    cv::imshow("contours", result);
    waitKey(0);

}
