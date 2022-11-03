#include "stdio.h"
#include<iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
 
Mat Sharpen(Mat input, int percent)
{
	Mat result;
	Mat s = input.clone();
	Mat kernel;
		kernel = (Mat_<int>(3, 3) <<
			0, -1, 0,
			-1, 4, -1,
			0, -1, 0
			);
	filter2D(s, s, s.depth(), kernel);
	result = input+s * 0.01 * percent;
	return result;
}


int main()
{
	//载入原图像
	cv::Mat src = cv::imread("/home/wyx/homework/work2/1/1634287874477.png", 1);
	//取车牌四边形四个顶点
	cv::Point2f srcPts[4];
	srcPts[0] = cv::Point2f(743, 410);	//左上
	srcPts[1] = cv::Point2f(747,488);	//左下
	srcPts[2] = cv::Point2f(953,413);	//右上
	srcPts[3] = cv::Point2f(967, 491);	//右下
 
	int MinX = std::min(srcPts[0].x, srcPts[1].x);	
	int MaxX = std::max(srcPts[2].x, srcPts[3].x);
	int MinY = std::min(srcPts[0].y, srcPts[2].y);
	int MaxY = std::max(srcPts[1].y, srcPts[3].y);
 
	cv::Point2f dstPts[4];
	dstPts[0] = cv::Point2f(0, 0);
	dstPts[1] = cv::Point2f(0, 140);
	dstPts[2] = cv::Point2f(440, 0);
	dstPts[3] = cv::Point2f(440, 140);
 
	//计算透视变换矩阵
	cv::Mat perspectiveMat = getPerspectiveTransform(srcPts, dstPts);
 
	//对原图进行透视变换，完成车牌校正
	cv::Mat dst;
    cv::Size siz(440,140);
	cv::warpPerspective(src, dst, perspectiveMat, siz);
    Mat dst1 = Sharpen(dst, 50);
    cv::imwrite("./dst1.png",dst1);
    cv::waitKey(0);
	
	return 0;
}