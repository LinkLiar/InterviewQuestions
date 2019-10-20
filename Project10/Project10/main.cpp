#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    while(1)
{   namedWindow("Progress");
    char a = (char)waitKey(0);
    switch (a)
    {
        case 49:{
                 namedWindow("Progress 1");
                 cout<<"进程1"<<endl;
                 while(1)
                     {
                       char c = (char)waitKey(1);
                       if (c == 48 )
                       {
                          destroyWindow("Progress 1");
                         break;
                       }
                     }break;
                 }
        case 50:{\
                 namedWindow("Progress 2");
                 VideoCapture cap(0);
                  if (!cap.isOpened())
                  {
                     cout << "Error opening video stream" << endl;
                   return -1;
                  }
                 while (1)
                  {
                        Mat frame;
                        cap >> frame;
                        if (frame.empty())
                        break;
                        imshow("Progress 2", frame);
                        char c = (char)waitKey(1);
                        if (c == 48 )
                        {
                         destroyWindow("Progress 2");
                         break;
                        }
                  }break;
                }
         case 51:{
                        namedWindow("Progress 3");
                        Mat Img = imread("photo1.png");
                        imshow("Progress 3",Img);
                        while(1)
                       {
                        char c = (char)waitKey(1);
                          if (c == 48 )
                           {
                             destroyWindow("Progress 3");
                             break;
                           }
                       }break;
                 }
         default:break;
     }
    if (a == 27 )
     {
       break;
       destroyAllWindows();
     }

}
    return 0;
}
