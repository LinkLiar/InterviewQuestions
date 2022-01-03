#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
using namespace cv::ml;

Mat srcImg,tmpImg, MidImg, dstImage;
Ptr<SVM> SVM_params;
void on_mouse(int EVENT, int x, int y, int flags, void* ustc)
{
    static Point Pre_pt(-1, -1);
    static Point Cur_pt(-1, -1);
    char Txt_Point[50] = { 0 };
    Mat dst;
    CvScalar color;
    if (ustc == NULL)
    {
        return;
    }

    if (EVENT == CV_EVENT_LBUTTONDOWN)                                      //左键按下，读取初始坐标
    {
        Pre_pt = Point(x, y);
        color = tmpImg.at<Vec3b>(Cur_pt.x,Cur_pt.y);
        sprintf(Txt_Point, "(%d,%d) (%d %d %d)", x, y,int(color.val[2]),int(color.val[1]),int(color.val[0]));
        putText(tmpImg, Txt_Point, Pre_pt,CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
        imshow("DealImg", tmpImg);
    }

    else if ((EVENT == CV_EVENT_MOUSEMOVE) && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
    {
        if (MidImg.data == NULL)
        {
            srcImg.copyTo(tmpImg);                                            //将src复制到临时图像tmp上，用于显示实时坐标
        }
        else
        {
            MidImg.copyTo(tmpImg);                                             //当已划定矩形的情况时，在图像上显示前面所画图像
        }
        Cur_pt = Point(x, y);
        color = tmpImg.at<Vec3b>(Cur_pt.x,Cur_pt.y);
        sprintf(Txt_Point, "(%d,%d) (%d %d %d)", x, y,int(color.val[2]),int(color.val[1]),int(color.val[0]));
        putText(tmpImg, Txt_Point, Cur_pt,CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
        imshow("DealImg", tmpImg);
    }

    else if (EVENT == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形
    {
        srcImg.copyTo(MidImg);
        Cur_pt = Point(x, y);
        color = tmpImg.at<Vec3b>(Cur_pt.x,Cur_pt.y);
        sprintf(Txt_Point, "(%d,%d) (%d %d %d)", x, y,int(color.val[2]),int(color.val[1]),int(color.val[0]));
        putText(MidImg, Txt_Point, Cur_pt,CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
        rectangle(MidImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
        imshow("DealImg", MidImg);
    }
    else if (EVENT == CV_EVENT_LBUTTONUP)                                          //左键松开，将在图像上划矩形
    {
        srcImg.copyTo(MidImg);
        color = tmpImg.at<Vec3b>(Cur_pt.x,Cur_pt.y);
        sprintf(Txt_Point, "(%d,%d) (%d %d %d)", x, y,int(color.val[2]),int(color.val[1]),int(color.val[0]));
        Cur_pt = Point(x, y);
        putText(MidImg, Txt_Point, Cur_pt,CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0, 255), 1, 8);
        rectangle(MidImg, Cur_pt, Pre_pt, Scalar(0, 0, 255), 1, 8, 0);
        imshow("DealImg", MidImg);

        int width = abs(Pre_pt.x - Cur_pt.x);
        int height = abs(Pre_pt.y - Cur_pt.y);
        if (width == 0 || height == 0)
        {
            printf("width == 0 || height == 0 \n");
            return;
        }
        dst = srcImg(Rect(min(Cur_pt.x,Pre_pt.x),min(Cur_pt.y,Pre_pt.y),width,height));
        namedWindow("dst");
        imshow("dst",dst);
        Mat dstImage,dstgray,dstdata;

        resize(dst,dstImage,Size(20,20),0,0,INTER_LINEAR);
        imshow("dstImage",dstImage);
        cvtColor(dstImage, dstgray, CV_BGR2GRAY);
        threshold(dstgray,dstgray, 0, 255, THRESH_OTSU);//二值化

        imshow("dstgray",dstgray);
        dstdata.push_back(dstgray.reshape(0,1));
        dstdata.convertTo(dstdata, CV_32F);
        float res = SVM_params->predict(dstdata);
        cout << "预测结果是："<< res << endl;
    }
}

int main(void)
{
    //核心思路：//获取一张图片后会将图片特征写入到容器中，
                //紧接着会将标签写入另一个容器中，这样就保证了特征
                //和标签是一一对应的关系。
    ////===============================读取训练数据===============================////
    //训了样本图片与测试图片的尺寸应该一样
    //训练数据，每一行一个训练图片
    Mat trainingData;
    //训练样本标签
    Mat labels;
    //最终的训练样本标签
    Mat clas;
    //最终的训练数据
    Mat traindata;
    Mat img = imread("digits.png");
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    int b = 20;
    int m = gray.rows / b;   //原图为1000*2000
    int n = gray.cols / b;   //裁剪为5000个20*20的小图块

    for (int i = 0; i < n; i++)
    {
        int offsetCol = i*b; //列上的偏移量
        for (int j = 0; j < m; j++)
        {
            int offsetRow = j*b;  //行上的偏移量
            //截取20*20的小块
            Mat tmp;
            gray(Range(offsetRow, offsetRow + b), Range(offsetCol, offsetCol + b)).copyTo(tmp);
            trainingData.push_back(tmp.reshape(0,1));  //序列化后放入特征矩阵
            labels.push_back((int)j / 5);  //对应的标注
        }
    }
    //图片数据和标签转变下
    Mat(trainingData).copyTo(traindata);//复制
    traindata.convertTo(traindata, CV_32FC1);//更改图片数据的类型，必要，不然会出错
    Mat(labels).copyTo(clas);//复制

    ////===============================创建SVM模型===============================////
    // 创建分类器并设置参数
    SVM_params = SVM::create();
    SVM_params->setType(SVM::C_SVC);//C_SVC用于分类，C_SVR用于回归
    SVM_params->setKernel(SVM::LINEAR);  //LINEAR线性核函数。SIGMOID为高斯核函数
    SVM_params->setDegree(0);//核函数中的参数degree,针对多项式核函数;
    SVM_params->setGamma(1);//核函数中的参数gamma,针对多项式/RBF/SIGMOID核函数;
    SVM_params->setCoef0(0);//核函数中的参数,针对多项式/SIGMOID核函数；
    SVM_params->setC(1);//SVM最优问题参数，设置C-SVC，EPS_SVR和NU_SVR的参数；
    SVM_params->setNu(0);//SVM最优问题参数，设置NU_SVC， ONE_CLASS 和NU_SVR的参数；
    SVM_params->setP(0);//SVM最优问题参数，设置EPS_SVR 中损失函数p的值.
    //结束条件，即训练1000次或者误差小于0.01结束
    SVM_params->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.01));

    //训练数据和标签的结合
    Ptr<TrainData> tData = TrainData::create(traindata, ROW_SAMPLE, clas);

    // 训练分类器
    SVM_params->train(tData);//训练

    //保存模型
    //SVM_params->save("xxxxxxxxx.xml");

    srcImg = imread("digits.png");
    resize(srcImg,srcImg,Size(1200,800),0,0,INTER_LINEAR);
    namedWindow("DealImg", WINDOW_AUTOSIZE);
    srcImg.copyTo(tmpImg);
    imshow("DealImg", tmpImg);
    setMouseCallback("DealImg", on_mouse, (void*)(&tmpImg)); //调用回调函数
    waitKey(0);
    destroyWindow("DealImg");
}
