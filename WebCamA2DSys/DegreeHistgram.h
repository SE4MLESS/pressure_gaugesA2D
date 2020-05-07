#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>

using namespace cv;
using namespace std;

class DegreeHistgram
{

private:
	Mat dlbl;
	Point Rpt;
	Point Lpt;
	//radian�v�Z�p
	Point lv, tv;
	//�Z�o�����p�x�ƍł��߂��p�x�̉�f��T��
	Point sMinRadP;
	Point eMinRadP;
	Point maxValueP;
	int dtmp = 0;
	int mind = 10000;
	Point minDp = 0;
	//�n�I�_�i�[�p
	int Rmax = 0;
	int Lmax = 0;

public:
	DegreeHistgram();
	~DegreeHistgram();
	float getDistance(float x, float y, float x2, float y2);
	int histgramMethodUzGray(Mat lbl, Point c, Mat draw, Point&,Point&, Point&,RotatedRect& ,Mat&,Mat&);
};

