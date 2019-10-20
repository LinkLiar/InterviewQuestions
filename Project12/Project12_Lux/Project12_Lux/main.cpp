#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#define BLUE 	0
#define GREEN	1
#define RED 	2

#ifndef SCREEN_XY
#define SCREEN_XY(x, y)  (255 - ((255 - (x))*(255 - (y)) >> 8))
#endif

int main()
{
    Mat src = imread("photo1.png", CV_LOAD_IMAGE_COLOR);
    imshow("src", src);
    int size = src.cols * src.rows;
    uchar r = 0, g = 0, b = 0, g_alpha = 0;
    uchar *pSrcData = src.data;
    for (int i = 0; i < size; i++, pSrcData += 3)
    {
        g_alpha = 255 - pSrcData[GREEN];
        b = pSrcData[BLUE]*g_alpha  >> 8;
        g = pSrcData[GREEN]*g_alpha >> 8;
        r = pSrcData[RED]*g_alpha   >> 8;
        pSrcData[BLUE]  = SCREEN_XY(pSrcData[BLUE],  b);
        pSrcData[GREEN] = SCREEN_XY(pSrcData[GREEN], g);
        pSrcData[RED]   = SCREEN_XY(pSrcData[RED],   r);
    }
    imshow("dst", src);
    waitKey();
    return 0;
}
