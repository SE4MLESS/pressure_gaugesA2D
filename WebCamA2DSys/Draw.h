#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>

using namespace cv;
using namespace std;
//���[�^�̖ڐ���̑������`
#define MEMORI_NUM 50
//���[�^�̖ڐ���̍ő�l���`
#define MTR_MAX_VALUE 1.0
//0����0.02�̃������̊Ԋu��␳//�ʏ�̖ڐ���Ɣ�r�����ۂ̔{��
#define STABLE_RATE 0.3

class CDraw
{
public:
	CDraw();
	~CDraw();
	float PlotData(Mat lab, Point c, Point s, Point e, uint rad, Point lp, Mat& dst, RotatedRect box, float rate);
	float CalcAnalogValue(Point c, Point s, Point e, Point li, float rate);
};

