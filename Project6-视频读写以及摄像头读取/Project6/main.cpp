#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    // Create a VideoCapture object and use camera to capture the video
    VideoCapture cap(0);

    // Check if camera opened successfully
    if (!cap.isOpened())
    {
        cout << "Error opening video stream" << endl;
        return -1;
    }

    // Default resolution of the frame is obtained.The default resolution is system dependent.
    int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    int frame_fps = cap.get(CV_CAP_PROP_FPS);

    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
    VideoWriter video("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), frame_fps, Size(frame_width, frame_height));

    //time start
     double start = static_cast<double>(getTickCount());


    while (1)
    {
        Mat frame;

        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Write the frame into the file 'outcpp.avi'
        video.write(frame);

        // Display the resulting frame
        imshow("Frame", frame);

        //time report
        double time = ((double)getTickCount() - start) / getTickFrequency();
        cout << "所用时间为：" << time << "秒" << endl;
        if (time > 30.0 )
            break;

        // Press  ESC on keyboard to  exit
        char c = (char)waitKey(1);
        if (c == 27 )
            break;
    }

    // When everything done, release the video capture and write object
    cap.release();
    video.release();

    // Closes all the windows
    destroyAllWindows();

    //read video
    cap.open("outcpp.avi");
    Mat frame;
        while (1)
        {
            cap >> frame;
            if (frame.empty()) break;
            imshow("outcpp.avi",frame);
            if (waitKey(33) >= 0) break;
        }

    return 0;
}
