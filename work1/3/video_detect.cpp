#include "stdio.h"
#include<iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
/*
二值化之后，应该基本可以过滤出弹丸的颜色
检测发射弹丸的限制条件：
首先去除面积小的部分，然后根据轮廓面积和外接矩形的面积进行判断：
1. 长条状，外轮廓的矩形长宽比很大，然后取矩形的中心即可
2. 碰撞时，会有一段折线：
    i. 采用凹六边形进行外轮廓的拟合，然后选取轮廓的中心位置作为弹丸的位置

进行序号标记的方法：
1. 设置一个全局变量记录
判断：

*/
int main(int argc, char ** argv)
{
    VideoCapture video; 
    video.open("/home/wyx/homework/work1/3/IMG_8511.mp4");
    cv::Mat src;
    int number=0;
    while (true)
    {
        video>>src;
        if (src.empty()) {
            break;
        }
        cv::Mat binary = src.clone();
        cv::Mat filer_1;
        //筛选弹丸
        cv::inRange(src, cv::Scalar(0, 100, 0), cv::Scalar(200, 255, 120), filer_1);
        //过滤小的部分
        std::vector<std::vector<cv::Point>> contours;
	    std::vector<cv::Vec4i> hierarchy;
	    cv::findContours(filer_1, contours, hierarchy,cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        cv::Mat drawer_counter = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
        std::vector<cv::Point2f> center;            //因为同一帧中可能会同时具备两个弹丸，因此center是一个vector
        for (int i = 0; i < contours.size(); ++i)
        {
		    if (cv::contourArea(contours[i]) < 110) continue; 
            static cv::Scalar COLOR_LIST = {220, 20, 20};
            drawContours(drawer_counter,contours,i,COLOR_LIST);
            
            //开始判断，根据轮廓面积和外接矩形的面积进行判断：
            cv::RotatedRect rect = cv::minAreaRect(contours[i]);
            double rect_area, contour_area;
            contour_area=cv::contourArea(contours[i]);
            rect_area=rect.size.height*rect.size.width;
            if(contour_area*3<rect_area)
            {
                Mat ploys;
                approxPolyDP(contours[i],ploys, 6, true);
                // draw ploy
                Vec2i pt1, pt2;
                double  aver_x = 0;
                double  aver_y = 0;
                for (int i = 0; i < ploys.rows; ++i) {
                    aver_x=aver_x+ ploys.at<Vec2i>(i)[0];
                    if (i == ploys.rows - 1) {
                        pt1 = ploys.at<Vec2i>(i);
                        pt2 = ploys.at<Vec2i>(0);
        
                    } else {
                        pt1 = ploys.at<Vec2i>(i);
                        pt2 = ploys.at<Vec2i>(i+1);
                    }
                    line(src, pt1, pt2,  cv::Scalar(255), 2);
                }
                aver_x=aver_x/ploys.rows;
                for (int i = 0; i < ploys.rows; ++i)
                {
                    if(ploys.at<Vec2i>(i)[0]<aver_x and ploys.at<Vec2i>(i+1)[0]>aver_x)
                    {
                        aver_y=(ploys.at<Vec2i>(i)[1]+ploys.at<Vec2i>(i+1)[1])/2;
                    }
                }
                center.emplace_back(aver_x,aver_y);
            }
            else
            {
                //长条形完成
                center.emplace_back(rect.center);
                cv::Point2f* touchVertices = new cv::Point2f[4];
                rect.points(touchVertices);
                for (int i = 0; i < 4; ++i)
                {
                    cv::line(src, touchVertices[i % 4], touchVertices[(i + 1) % 4], cv::Scalar(255), 2);
                }
                // std::cout<<"x:"<<center.x<<"y:"<<center.y<<endl;
            }
        }
        for(int i=0;i<center.size();i++)
        {
            // std::cout<<"x:"<<center[i].x<<"y:"<<center[i].y<<endl;
            cv::circle(src, center[i], 2, cv::Scalar(255, 255, 255), cv::FILLED);
        }
        imshow("src",src);
        waitKey(1000);
    }
    cv::destroyAllWindows();
    return 0;   

}
