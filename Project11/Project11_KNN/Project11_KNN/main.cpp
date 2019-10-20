#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
using namespace cv::ml;

Mat srcImg,tmpImg, MidImg, dstImage;
Ptr<KNearest> model;
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
        float res = model->predict(dstdata);
        cout << "预测结果是："<< res << endl;
    }
}

int main(void)
{
    Mat img = imread("digits.png");
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    int b = 20;
    int m = gray.rows / b;   //原图为1000*2000
    int n = gray.cols / b;   //裁剪为5000个20*20的小图块
    Mat data,labels;   //特征矩阵
    for (int i = 0; i < n; i++)
    {
        int offsetCol = i*b; //列上的偏移量
        for (int j = 0; j < m; j++)
        {
            int offsetRow = j*b;  //行上的偏移量
            //截取20*20的小块
            Mat tmp;
            gray(Range(offsetRow, offsetRow + b), Range(offsetCol, offsetCol + b)).copyTo(tmp);
            data.push_back(tmp.reshape(0,1));  //序列化后放入特征矩阵
            labels.push_back((int)j / 5);  //对应的标注
        }

    }
    data.convertTo(data, CV_32F); //uchar型转换为cv_32f
    int samplesNum = data.rows;
    int trainNum = 3000;
    Mat trainData, trainLabels;
    trainData = data(Range(0, trainNum), Range::all());   //前3000个样本为训练数据
    trainLabels = labels(Range(0, trainNum), Range::all());

    //使用KNN算法
    int K = 5;
    Ptr<TrainData> tData = TrainData::create(trainData, ROW_SAMPLE, trainLabels);
    model = KNearest::create();
    model->setDefaultK(K);
    model->setIsClassifier(true);
    model->train(tData);

    //预测分类
    double train_hr = 0, test_hr = 0;
    Mat response;
    // compute prediction error on train and test data
    for (int i = 0; i < samplesNum; i++)
    {
        Mat sample = data.row(i);
        float r = model->predict(sample);   //对所有行进行预测
        //预测结果与原结果相比，相等为1，不等为0
        r = std::abs(r - labels.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

        if (i < trainNum)
            train_hr += r;  //累积正确数
        else
            test_hr += r;
    }

    test_hr /= samplesNum - trainNum;
    train_hr = trainNum > 0 ? train_hr / trainNum : 1.;

    printf("accuracy: train = %.1f%%, test = %.1f%%\n",
        train_hr*100., test_hr*100.);

    srcImg = imread("digits.png");
    resize(srcImg,srcImg,Size(1200,800),0,0,INTER_LINEAR);
    namedWindow("DealImg", WINDOW_AUTOSIZE);
    srcImg.copyTo(tmpImg);
    imshow("DealImg", tmpImg);
    setMouseCallback("DealImg", on_mouse, (void*)(&tmpImg)); //调用回调函数
    waitKey(0);
    destroyWindow("DealImg");
}
