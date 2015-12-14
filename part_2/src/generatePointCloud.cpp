/*************************************************************************
    > File Name: generatePointCloud.cpp
    > Author: lengjing
    > Mail: jingleng77@sina.com 
    > Created Time: 2015年12月14日 星期一 21时57分12秒
 ************************************************************************/

#include <iostream>
#include <string>
using namespace std;


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloud;

const double camera_factor = 1000;
const double camera_cx = 325.5;
const double camera_cy = 253.5;
const double camera_fx = 518.0;
const double camera_fy = 519.0;

int main(int argc, char **argv)
{
	cv::Mat rgb, depth;
	rgb = cv::imread("../data/rgb.png");
	depth = cv::imread("../data/depth.png", -1);
	
	PointCloud::Ptr pCloud(new PointCloud);
	for(int m = 0; m < depth.rows; m++)
	{
		for(int n = 0; n < depth.cols; n++)
		{
			ushort d = depth.ptr<ushort>(m)[n];
			if(d == 0)
				continue;
			PointT p; 

			p.b = rgb.ptr<uchar>(m)[n * 3];
			p.g = rgb.ptr<uchar>(m)[n * 3 +1];
			p.r = rgb.ptr<uchar>(m)[n * 3 +2];

			p.z = double(d)/ camera_factor;
			p.x = (n - camera_cx) * p.z / camera_fx;
			p.y = (m - camera_cy) * p.z / camera_fy;

			pCloud->points.push_back(p);
		}
	}
	pCloud->height = 1;
	pCloud->width = pCloud->points.size();
	cout << "point cloud size = " << pCloud->points.size() << endl;
	pCloud->is_dense = false;
	pcl::io::savePCDFile("../data/pointcloud.pcd", *pCloud);
	cout << "point cloud saved !" << endl;
	return 0;




}

