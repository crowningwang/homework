#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Eigen/Dense"
#include "opencv2/core/eigen.hpp"

using namespace cv;
using namespace Eigen;
inline const std::vector<cv::Point3d> PW_BIG{// 灯条坐标，单位：m
                                             {-0.115, 0.0265, 0.},
                                             {-0.115, -0.0265, 0.},
                                             {0.115, -0.0265, 0.},
                                             {0.115, 0.0265, 0.}};

int main()
{
    cv::Mat src;
    cv::Mat camera_matrix;
    cv::Mat distort_matrix;
    cv::FileStorage reader("/home/wyx/homework/work3/pnp/hw3_t1/hw3_t1/f_mat_and_c_mat.yml", cv::FileStorage::READ);
    reader["F"] >> camera_matrix;
    reader["C"] >> distort_matrix;


    src = imread("/home/wyx/homework/work3/pnp/hw3_t1/hw3_t1/hero.jpg");
    std::vector<cv::Point2f> corners;
    corners.push_back(cv::Point2f(575.508,282.175));
    corners.push_back(cv::Point2f(573.93 ,331.819));
    corners.push_back(cv::Point2f(764.518 ,337.652));
    corners.push_back(cv::Point2f(765.729,286.741));

    imshow("Opencv Demo", src);
    cv::waitKey(100);

    cv::Mat rvec, tvec;
    cv::solvePnP(PW_BIG, corners, camera_matrix, distort_matrix, rvec, tvec);
    // 在相机坐标系下
    // rvec - 输出的旋转向量。使坐标点从世界坐标系旋转到相机坐标系
    // tvec - 输出的平移向量。使坐标点从世界坐标系平移到相机坐标系
    Eigen::Vector3d t_cam;
    Eigen::Quaterniond q={ -0.0816168,0.994363,-0.0676645,-0.00122528};
    Eigen::Vector3d t_1;
    Eigen::Matrix3d r_1;
    cv::Mat rotM;
    Rodrigues(rvec, rotM); 
    Isometry3d T = Isometry3d::Identity();
    cv2eigen(tvec,t_1);
    cv2eigen(rotM,r_1);
    T.rotate(r_1);
    T.translate(t_1);
    //P_imu=q_c2i*P_cam
    Vector3d t_imu = q*t_1;
    for(int i=0;i<3;i++)
    {
        std::cout << "t_imu:" << std::endl << t_imu(i) << std::endl << std::endl;
    }
    cv::waitKey(0);
    return 0;
}