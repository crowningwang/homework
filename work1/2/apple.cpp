#include "stdio.h"
#include<iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

/*
刚开始想直接用hsv提取求并集，但是发现效果也就一般，所以还是直接采用通道相减;
问题是后面黄色的叶子和苹果底部的黄色区分问题，一直没办法很好的区分

主要思路就是，通道相减，中值滤波，腐蚀膨胀，提取轮廓画矩形
*/
int main(int argc, char ** argv)
{
    cv::Mat src = cv::imread("/home/wyx/homework/work1/2/apple.png");
    assert(src.channels() == 3);

    //预处理
    cv::Mat channels[3];
    cv::split(src, channels);
    cv::Mat red_sub_green = channels[2] - channels[1];
    cv::Mat normal_mat;
    cv::normalize(red_sub_green, normal_mat, 0., 255., cv::NORM_MINMAX);
    cv::threshold(normal_mat, normal_mat, 30, 255, cv::THRESH_BINARY);
    cv::medianBlur(normal_mat,normal_mat,11);

    cv::imshow("src", src);
    cv::imshow("norm", normal_mat);
    cv::Mat result;
    //开运算
    cv::Mat kernel_gradient = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 21));
    cv::morphologyEx(normal_mat,result,MORPH_OPEN,kernel_gradient);
    cv::imshow("result", result);

    //轮廓
    vector< vector<Point> > contours;
    vector<Vec4i> hierarcy;
    findContours(result, contours, hierarcy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	for (int n = 0; n < contours.size(); n++) {
		Rect rect = boundingRect(contours[n]);
		rectangle(src, rect, Scalar(0, 0, 255), 4);
	}

    imshow("final", src);
    imwrite("./appel_detect.png",src);
    cv::waitKey(0);
    return 0;
}
