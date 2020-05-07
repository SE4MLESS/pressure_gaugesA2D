#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>
#include <time.h>
//����w�b�_
#include "Draw.h"
#include "Thinning.h"
#include "Labeling.h"
#include "DegreeHistgram.h"
#include "Ellipse.h"

//���J������
#define CAM_MAX_NUM 5
//�K���I��l���ɂ�����t�B���^�T�C�Y
#define ADAPT_GAUSS_FILTER 51
//�f�o�b�N�̗L���ύX
#define DEBUG_ON true
#define DEBUG_OFF false

using namespace cv;
using namespace std;

class CControl{

private:
	float analog_value;
	float crct_rate;
	bool GetCircle;
	//���N���X�Q�Ɨp
	CDraw D;
	DegreeHistgram HST;
	CThinning TH;
	CEllipse  EL;
	CLabeling LB;
	//�J�����擾�p
	VideoCapture cam;
	//�������x����p
	clock_t full_start;
	clock_t full_end;
	clock_t local_time_s;
	clock_t local_time_e;
	//�}�g���b�N�X
	Mat frame;
	Mat draw_line;
	Mat	trs_mask;
	Mat th_line;
	Mat draw_hou_c;
	Mat before_label;
	Mat al_bin;
	Mat elmask;
	Mat uz_ell_mask;
	Mat th_uz_cmask;
	Mat after_label;
	Mat l_dilate;
	Mat rot;
	Mat plot_data;
	Mat work_inv;
	Mat enhance;
	Mat fit_ell;
	Mat hist;
	Mat enh_bin;
	Mat bk_mask;
	Mat bin_uz_th;
	Mat response;
	//��f�l�̊i�[
	Point markSt;
	Point markEd;
	Point hough_ctr;
	Point elps_ctrPt;
	Point linePt;
	RotatedRect elps_box;
	//�������p
	uint radius;
	uint camNum;
	ostringstream oss;
public:
	CControl();
	~CControl();
	int Start(vector<VideoCapture>,float[CAM_MAX_NUM],bool);
	void Init(Mat, Mat&, Mat&);
	void Init();
};

