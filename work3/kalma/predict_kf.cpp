#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <ctime>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
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


int main()
{
    srand((unsigned int) time(NULL));

    vector<vector<double>> data;
    txt_to_vectordouble(data,"/home/wyx/homework/work3/kalma/hw3_t2/hw3_t2/dollar.txt");

    // calculate speed
    /*
    $x$：$k-1$时刻的滤波值，也就是$k-1$时刻的状态。
    $x’ $： $x$的先验估计
    $A$：状态转移方程，即$x_{k-1}$与$x_k$之间的转移关系
    $K$：卡尔曼增益
    */
    const int Z_N = 1, X_N = 2;
    Matrix<double, X_N, 1> X;
    Matrix<double, X_N, X_N> A;
    Matrix<double, X_N, X_N> P;
    Matrix<double, X_N, X_N> R;
    Matrix<double, X_N, Z_N> K;
    Matrix<double, Z_N, X_N> C;
    Matrix<double, Z_N, Z_N> Q;

    X << data[0][0], 0;
    A << 1, 1, 0, 1;
    C << 1, 0;
    R << 2, 0, 0, 2;
    Q << 10;

    for (int i = 1; i < data.size(); i++)
    {
        // 更新预测
        Matrix<double, X_N, 1> X_k = A * X;
        P = A * P * A.transpose() + R;
        // $P$：预测误差协方差
        // $R$：预测过程噪声偏差的方差

        // 更新观测
        // $z$：$k$时刻的观测数据
        // $C$：观测矩阵，观测量与预测量之间的关系
        // $Q$：测量过程噪声偏差的方差
        K = P * C.transpose() * (C * P * C.transpose() + Q).inverse();
        Matrix<double, Z_N, 1> Z{data[i][0]};
        X = X_k + K * (Z - C * X_k);
        P = (Matrix<double, X_N, X_N>::Identity() - K * C) * P;

        std::cout << "step " << i << ": " << X[1] << std::endl;
    }

    std:cout << "final speed: " << X[1] << std::endl;
    
    return 0;
}
