#include "stdio.h"
#include<iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <sstream>
#include <fstream>
using namespace std;
using namespace cv;
using namespace Eigen;

void  txt_to_vectordouble(vector<vector<double>>& res, string pathname)
{
	ifstream infile;
	infile.open(pathname.data());   //将文件流对象与文件连接起来 
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	vector<double> suanz;
	string s;
	while (getline(infile, s)) {
		istringstream is(s); //将读出的一行转成数据流进行操作
		double d;
		while (!is.eof()) {
			is >> d;
			suanz.push_back(d);
		}
		res.push_back(suanz);
		suanz.clear();
		s.clear();
	}
	infile.close();             //关闭文件输入流 
}
/*

*/
int main()
{
	Eigen::Matrix<double, 3, 4> cam;
    cam<< 400., 0., 190., 0.,
             0., 400., 160., 0.,
             0., 0., 1., 0.;
	vector<vector<double>> data;
	txt_to_vectordouble(data, "/home/wyx/homework/work2/3/points.txt");
	double num=data[0][0];
	cv::Mat image(1080,1980,CV_8UC3);
	//下面就是世界坐标系下的各个点

	for(int i=1;i<data.size()-1;i++)
	{
		Eigen::Vector3d point_w(data[i][0],data[i][1],data[i][2]);
		Eigen::Quaterniond q={-0.5,0.5,0.5,-0.5};
		Isometry3d T = Isometry3d::Identity();
		T.rotate(q.inverse()); 
		T.pretranslate(q.inverse()*Vector3d(-2, -2, -2));  
		Eigen::Vector3d point_c = cam*T*point_w;
		cv::Point2d point_img(point_c(0,0)/point_c(2.0),point_c(1,0)/point_c(2,0));
		cv::circle(image,point_img,0.1,cv::Scalar(255,255,255));
	}
	cv::imshow("final",image);
	cv::waitKey(50); 
    cv::waitKey(0);
}