#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

 using namespace std;
 using namespace cv;

 int main()
 {
     Mat img;
     img = imread("photo1.png");
     imshow("Original",img);
     Mat imgHSV;

    //saving the Red HSV
     int iLowH = 0;
     int iHighH = 50;
     int iLowS = 43;
     int iHighS = 255;
     int iLowV = 46;
     int iHighV = 255;

     //2HSV
     cvtColor(img, imgHSV, COLOR_BGR2HSV);

     //Smooth
     Mat imgThresholded;
     vector<Mat> hsvSplit;
     split(imgHSV, hsvSplit);
     equalizeHist(hsvSplit[2],hsvSplit[2]);
     merge(hsvSplit,imgHSV);

     //Threshold the image
     inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

     //open (remove noise)
     Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
     morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);

     //close (connect connected-component)
     morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

     imshow("Result",imgThresholded);

     vector<int> compression_params;
     compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
     compression_params.push_back(0);
     imwrite("result.png",imgThresholded,compression_params);    //设置压缩等级为0
     waitKey(0);
     return 0;
 }
