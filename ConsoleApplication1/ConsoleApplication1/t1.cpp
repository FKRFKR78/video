#include<iostream>
#include<math.h>
#include<opencv.hpp>
#include<set>
#include "readstlfile.h"
using namespace std;
using namespace cv;
Mat src(600, 600, CV_8UC3, Scalar(0, 0, 0));
const char *name = "src";
vector<Point>mpoint_data;
vector<Point>mpoint_data2;
map<int, set<int>>map_data;
map<int, Mat>m3d_show_xy;
map<int, Mat>m3d_show_xz;
map<int, int>msrc_data_toal;
vector<SPoint3f>sp3f_src;
vector<SPoint3f>sp3f_data;
double mk;
map<int, map<int, set<int>>>mpoint_toal;//Z,Y,X
map<int, map<int, set<int>>>mpoint_toal_ZX;//Y,Z,X
double mgetdistance(SPoint3f p1, SPoint3f p2)
{
	double s1=abs(p1.x - p2.x)*abs(p1.x - p2.x) + abs(p1.y - p2.y)*abs(p1.y - p2.y) +abs(p1.z - p2.z)*abs(p1.z - p2.z);
	return sqrt(s1);
}
void insert_line(SPoint3f p1, SPoint3f p2)
{
	double l2 = mgetdistance(p1, p2);
	if (l2 <=0.1)
		return;
	int mnumber =l2*10;
	double mx = (p2.x - p1.x) / mnumber;
	double my = (p2.y - p1.y) / mnumber;
	double mz = (p2.z - p1.z) / mnumber;
	for (int i = 1; i <mnumber; i++)
	{
		double sx, sy, sz;
		sx = p1.x + i*mx;
		sy = p1.y + i*my;
		sz = p1.z + i*mz;
		sp3f_data.push_back(SPoint3f(sx, sy, sz));
	}
}
void make(SPoint3f p1, SPoint3f p2, SPoint3f p3)     
{
	SPoint3f mkey((p1.x + p2.x + p3.x) / 3.0, (p1.y + p2.y + p3.y) / 3.0, (p1.z + p2.z + p3.z) / 3.0);
	double ps1 = mgetdistance(p1, p2);
	double ps2 = mgetdistance(p1, p3);
	double ps3 = mgetdistance(p2, p3);
	double ps4 = mgetdistance(mkey, p3);
	double ps5 = mgetdistance(p2, mkey);
	double ps6 = mgetdistance(p1, mkey);
	if (ps1 < 0.01 || ps2 < 0.01 || ps3 < 0.01 || ps4 < 0.01 || ps5 < 0.01 || ps6 < 0.01)
		return;
	double l1 = mgetdistance(p1, p2);
	double l2 = mgetdistance(p1, p3);
	double l3 = mgetdistance(p3, p2);
	double max_l = max(max(l1, l2), l3);
	double p = (l1 + l2 + l3) / 2;
	double s = sqrt(p*(p - l1)*(p - l2)*(p - l3));
	if (s<=0.05*max_l)
		return;
	//sp3f_data.push_back(mkey);
	mpoint_toal[mkey.z][mkey.y].insert(mkey.x);
	mpoint_toal_ZX[mkey.y][mkey.z].insert(mkey.x);

	make(p1, p2, mkey);
	make(p3, p2, mkey);
	make(p1, p3, mkey);
}
void insert_line(Point A, Point B)
{
	int number = max(abs(A.x - B.x), abs(A.y - B.y));
	int mnumber = sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y))*100;
	if (number <= 2)
		return;
	double mkx = (double)(A.x - B.x) / (double)number;
	double mky = (double)(A.y - B.y) / (double)number;
	double smkx = (double)(A.x - B.x) / (double)mnumber;
	double smky = (double)(A.y - B.y) / (double)mnumber;
	for (int i = 1; i < number; i++)
	{
		Point C(B.x + mkx*i, B.y + mky*i);
		mpoint_data2.push_back(C);
	}
	for (int i = 1; i < mnumber; i++)
	{
		int sx = B.x + smkx*i;
		int sy = B.y + smky*i;
		map_data[sx].insert(sy);
	}

}
void make(Point p1, Point p2, Point p3)
{
	Point key;
	key.x = (p1.x + p2.x + p3.x) / 3;
	key.y = (p1.y + p2.y + p3.y) / 3;
	if (key == p1 || key == p2 || key == p3 || p1 == p2 || p2 == p3 || p3 == p1)
		return;
	double l1 = sqrt((p1.x - p2.x) *(p1.x - p2.x)+ (p1.y - p2.y)*((p1.y - p2.y)));
	double l2 = sqrt((p1.x - p3.x)*(p1.x - p3.x) + (p1.y - p3.y)*(p1.y - p3.y));
	double l3 = sqrt((p3.x - p2.x)*(p3.x - p2.x) + (p3.y - p2.y)*(p3.y - p2.y));
	double p = (l1 + l2 + l3) / 2;
	double s = sqrt(p*(p - l1)*(p - l2)*(p - l3));
	double maxlegth = max(max(l1, l2), l3);
	if (s <= 0.01*maxlegth)
		return;
	mpoint_data.push_back(key);
	//line(src, p1, key, Scalar(0, 255, 0));
	//line(src, p2, key, Scalar(0, 255, 0));
	//line(src, p3, key, Scalar(0, 255, 0));
	make(p1, p2, key);
	make(p3, p2, key);
	make(p1, p3, key);
}
//最小凸多边形
//polylines绘制多边形
void main()
{
	/*
	Mat mshow = src.clone();
	Point SP1(10, 10);
	Point SP2(520, 100);
	Point SP3(200, 410);
	mpoint_data.push_back(SP1);
	mpoint_data.push_back(SP2);
	mpoint_data.push_back(SP3);
	line(src, SP1, SP2, Scalar(0,255, 0));
	line(src, SP3, SP2, Scalar(0, 255, 0));
	line(src, SP3, SP1, Scalar(0, 255, 0));
	make(SP1, SP2, SP3);
	for (auto i = mpoint_data.begin(); i != mpoint_data.end(); i++)
		src.at<Vec3b>(*i) = Vec3b(255, 0, 0);
	imshow("triangle", src);
	vector<Point>hull;
	convexHull(Mat(mpoint_data), hull, false);
	polylines(mshow, hull, true, Scalar(0, 0, 255));
	imshow("mshow", mshow);
	*/
	
	const char *name = "MSP1.STL";
	ReadSTLFile mdata;
	Mat formwork(600, 600, CV_8UC3, Scalar(0, 0, 0));
	Mat srcsrc = formwork.clone();
	cout << "read:" << mdata.ReadFile(name)<<endl;
	mdata.mchange();
	mdata.mshow();
	mk = mdata.k_xy;
	sp3f_src = mdata.pointList;
	for (int i = 0; i < sp3f_src.size(); i = i + 3)
	{
		//sp3f_data.push_back(sp3f_src[i]);
		//sp3f_data.push_back(sp3f_src[i+1]);
		//sp3f_data.push_back(sp3f_src[i+2]);
		mpoint_toal[sp3f_src[i].z][sp3f_src[i].y].insert(sp3f_src[i].x);
		mpoint_toal[sp3f_src[i+1].z][sp3f_src[i+1].y].insert(sp3f_src[i+1].x);
		mpoint_toal[sp3f_src[i+2].z][sp3f_src[i+2].y].insert(sp3f_src[i+2].x);

		mpoint_toal_ZX[sp3f_src[i].y][sp3f_src[i].z].insert(sp3f_src[i].x);
		mpoint_toal_ZX[sp3f_src[i + 1].y][sp3f_src[i + 1].z].insert(sp3f_src[i + 1].x);
		mpoint_toal_ZX[sp3f_src[i + 2].y][sp3f_src[i + 2].z].insert(sp3f_src[i + 2].x);
		//insert_line(sp3f_src[i], sp3f_src[i + 1]);
		//insert_line(sp3f_src[i], sp3f_src[i + 2]);
		//insert_line(sp3f_src[i+1], sp3f_src[i + 2]);
		make(sp3f_src[i], sp3f_src[i + 1], sp3f_src[i + 2]);
	}

	for (auto i = mpoint_toal.begin(); i != mpoint_toal.end(); i++)
	{
		m3d_show_xy[i->first] = formwork.clone();
		waitKey(100);
	}
	for (auto i = mpoint_toal.begin(); i != mpoint_toal.end(); i++)
	{
		for (auto j = i->second.begin(); j != i->second.end();j++)
			for (auto w = j->second.begin(); w != j->second.end(); w++)
		{
			m3d_show_xy[i->first].at<Vec3b>(Point(*w, j->first)) = Vec3b(255, 0, 0);
		}
	}

	for (auto i = mpoint_toal_ZX.begin(); i != mpoint_toal_ZX.end(); i++)
	{
		m3d_show_xz[i->first] = formwork.clone();
		waitKey(100);
	}
	for (auto i = mpoint_toal_ZX.begin(); i != mpoint_toal_ZX.end(); i++)
	{
		for (auto j = i->second.begin(); j != i->second.end(); j++)
		for (auto w = j->second.begin(); w != j->second.end(); w++)
		{
			m3d_show_xz[i->first].at<Vec3b>(Point(*w, j->first)) = Vec3b(255, 0, 0);
		}
	}
	/*
	for (int i = 0; i < sp3f_data.size(); i=i+2)
	{
		int mz = sp3f_data[i].z;
		int my = sp3f_data[i].y ;
		int mx = sp3f_data[i].x ;
		line(srcsrc, Point(sp3f_data[i].x, sp3f_data[i].z), Point(sp3f_data[i + 1].x, sp3f_data[i + 1].z), Scalar(255, 0, 0));
		srcsrc.at<Vec3b>(Point(my, mz)) = Vec3b(255, 0, 0);
		
		if (m3d_show_xy.find(mz) == m3d_show_xy.end())
		{
			m3d_show_xy[mz] = formwork.clone();
			waitKey(100);
		}
		if (m3d_show_xy[mz].at<Vec3b>(Point(mx, my)) != Vec3b(255, 255, 255))
			msrc_data_toal[mz]++;
			m3d_show_xy[mz].at<Vec3b>(Point(mx, my)) = Vec3b(255, 255, 255);		
	}
	*/

	VideoWriter output_src("demo_src4.avi", CV_FOURCC('X', 'V', 'I', 'D'), 10, formwork.size());
	
	cout << m3d_show_xy.size() << "," << m3d_show_xz.size() << endl;
	int number = 0;
	for (int i = 0; i<mpoint_toal_ZX.size(); i++)
	{
		output_src << m3d_show_xz[i];
	//	string sname = to_string(number++);
	//	imshow(sname.data(), m3d_show_xy[i]);
	//	waitKey(100);
	}
	//imshow("srcsrc", srcsrc);
	
	waitKey(0);
//	convexHull(Mat(mpoint_data), hull, false);
//	polylines(mshow, hull, true, Scalar(0, 255, 0));
}