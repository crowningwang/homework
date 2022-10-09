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
1. 设置一个全局变量记录下每个点的中心坐标all_center
在一帧里面只有一个center中的情况
第一个是id=0，然后x值如果一直在减小，那么给这个center标记的还是原来的id，如果出现x值变大，i加一
如果一帧里面有两个center（center.size()==2）
那么小的那个是原的i，但是大的是i加一，然后用大的坐标进行判断
后面写了屎山代码，为了防止连续几帧都有两个球导致id一直增加

*/
int main(int argc, char ** argv)
{
    VideoCapture video; 
    video.open("/home/wyx/homework/work1/3/IMG_8511.mp4");
    cv::Mat src;
    std::vector<cv::Point2f> all_center;   
    cv::Point2f last_center;
    int id=0;
    int index=0;
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
		    if (cv::contourArea(contours[i]) < 130) continue; 
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
                approxPolyDP(contours[i],ploys, 0, true);
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
                all_center.emplace_back(aver_x,aver_y);
            }
            else
            {
                //长条形完成
                center.emplace_back(rect.center);
                all_center.emplace_back(rect.center);
                cv::Point2f* touchVertices = new cv::Point2f[4];
                rect.points(touchVertices);
                for (int i = 0; i < 4; ++i)
                {
                    cv::line(src, touchVertices[i % 4], touchVertices[(i + 1) % 4], cv::Scalar(255), 2);
                }
                // std::cout<<"x:"<<center.x<<"y:"<<center.y<<endl;
            }
        }
        if(center.size()==0){continue;}
        if(all_center.size()>1) last_center=all_center.at(all_center.size()-2);
        else
        {
            last_center=all_center.at(all_center.size()-1);
        }
        if(center.size()==1){
            if(center[0].x>last_center.x)
            {
                id++;
            }
            cv::circle(src, center[0], 2, cv::Scalar(255, 255, 255), cv::FILLED);
            cv::putText(src, "ID: " + to_string(id), center[0],cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        }
        else{
            if(center[0].x<center[1].x)
            {
                cv::circle(src, center[0], 2, cv::Scalar(255, 255, 255), cv::FILLED);
                if(index==0){
                cv::putText(src, "ID: " + to_string(id), center[0],cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
                id++;
                }
                else 
                {cv::putText(src, "ID: " + to_string(--id), center[0],cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
                id++;}
                index ++;
                if(index==4)index=0;
                cv::circle(src, center[1], 2, cv::Scalar(255, 255, 255), cv::FILLED);
                cv::putText(src, "ID: " + to_string(id), center[0],cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
            }
            else
            {
                cv::circle(src, center[1], 2, cv::Scalar(255, 255, 255), cv::FILLED);
                if(index==0){
                cv::putText(src, "ID: " + to_string(id), center[1],cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
                id++;
                }
                else 
                {cv::putText(src, "ID: " + to_string(--id), center[1],cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
                id++;}
                index++;
                if(index==4)index=0;
                cv::circle(src, center[0], 2, cv::Scalar(255, 255, 255), cv::FILLED);
                cv::putText(src, "ID: " + to_string(id), center[0],cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);     
            }
        }
        center.clear();
        cv::imshow("src",src);
        cv::waitKey(200);
    }
    cv::destroyAllWindows();
    return 0;   
}
