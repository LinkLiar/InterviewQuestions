#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    // Create a VideoCapture object and use camera to capture the video
    VideoCapture cap10(0);
    //VideoCapture cap40(1);
    //VideoCapture cap80(2);

    //Deny auto exposere
    cap10.set(CAP_PROP_AUTO_EXPOSURE,0.25);
    //cap40.set(CAP_PROP_AUTO_EXPOSURE,0.25);
    //cap80.set(CAP_PROP_AUTO_EXPOSURE,0.25);

    //exposure time set
    cap10.set(CV_CAP_PROP_EXPOSURE, 0.1);
    //cap40.set(CV_CAP_PROP_EXPOSURE, 0.4);
    //cap80.set(CV_CAP_PROP_EXPOSURE, 0.8);

    // Check if camera opened successfully
    if (!cap10.isOpened())
    {
        cout << "Error opening video stream" << endl;
        return -1;
    }

    // Default resolution of the frame is obtained.The default resolution is system dependent.
    int frame_width = cap10.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height = cap10.get(CV_CAP_PROP_FRAME_HEIGHT);
    int frame_fps = cap10.get(CV_CAP_PROP_FPS);

    while (1)
    {
        Mat frame,frame1,frame2;
        // Capture frame-by-frame
        cap10 >> frame;
        //cap40 >> frame1;
        //cap80 >> frame2;
        // If the frame is empty, break immediately
        if (frame.empty())
            break;
        // Display the resulting frame
        imshow("10", frame);
        //imshow("40", frame);
        //imshow("80", frame);

        // Press  ESC on keyboard to  exit
        char c = (char)waitKey(1);
        if (c == 27 )
            break;
        cout <<frame_fps<<"  "<<frame_width<<"*"<<frame_height<<endl;
    }

    // Closes all the windows
    destroyAllWindows();
    return 0;
}
