#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;

int main()
{

    Ptr<BackgroundSubtractor> pBackSub;

    pBackSub = createBackgroundSubtractorMOG2();

    VideoCapture capture(0);

    Mat frame, fgMask;
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        //update the background model
        pBackSub->apply(frame, fgMask);

        //get the frame number and write it on the current frame
        rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
                  cv::Scalar(255,255,255), -1);
        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));

        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);

        //get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
    return 0;
}
