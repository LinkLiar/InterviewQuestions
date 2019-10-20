#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/** @function main */
 int main()
 {
   Point2f srcTri[3];
   Point2f dstTri[3];

   Mat warp_mat( 2, 3, CV_32FC1 );
   Mat src, warp_dst;

   /// 加载源图像
   src = imread("photo1.png", 1 );

   /// 设置目标图像的大小和类型与源图像一致
   warp_dst = Mat::zeros( src.rows, src.cols, src.type() );

   /// 设置源图像和目标图像上的三组点以计算仿射变换
   srcTri[0] = Point2f( 0,0 );
   srcTri[1] = Point2f( src.cols - 1, 0 );
   srcTri[2] = Point2f( 0, src.rows - 1 );

   dstTri[0] = Point2f( src.cols*0.0, src.rows*0 );
   dstTri[1] = Point2f( src.cols*0.5, src.rows*0 );
   dstTri[2] = Point2f( src.cols*0.5, src.cols*0.5 );

   /// 求得仿射变换
   warp_mat = getAffineTransform( srcTri, dstTri );

   /// 对源图像应用上面求得的仿射变换
   warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

   /// 显示结果
   namedWindow( "Source image", CV_WINDOW_AUTOSIZE );
   imshow( "Source image", src );

   namedWindow( "Warp", CV_WINDOW_AUTOSIZE );
   imshow( "Warp", warp_dst );

   /// 等待用户按任意按键退出程序
   waitKey(0);

   return 0;
  }
