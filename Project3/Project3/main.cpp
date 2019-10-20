#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
int main()
{
    Mat srcImage,dstImage;
    srcImage = imread("photo1.png");
    imshow("原图", srcImage);

    resize(srcImage,dstImage,Size(1280,720),0,0,INTER_LINEAR);
    imshow("原图大小转换为1280*720", dstImage);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);
    imwrite("result.png",dstImage,compression_params);    //设置压缩等级为0

    waitKey(0);
    return 0;
}
