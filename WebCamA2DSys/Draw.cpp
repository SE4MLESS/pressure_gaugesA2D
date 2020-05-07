#include "Draw.h"

CDraw::CDraw()
{
}

CDraw::~CDraw()
{
}

//�o�͌��ʂ̕`��p
float CDraw::PlotData(Mat lab, Point c, Point s, Point e, uint rad, Point lp, Mat& dst, RotatedRect box, float rate) {
	dst = lab.clone();
	//�p�x�v�Z�p�֐�
	//�A�i���O�l�o��
	float analog_value = CalcAnalogValue(c, s, e, lp, rate);
	//�A���S���Y�����ʂ̕`��
	cv::ellipse(dst, box, Scalar(237, 85, 27), 2, CV_AA);
	cv::circle(dst, box.center, 6, Scalar(237, 85, 27), -1, 8, 0);
	cv::line(dst, Point(e.x, e.y), Point(c.x, c.y), Scalar(0, 255, 0), 2, CV_AA);
	cv::line(dst, Point(s.x, s.y), Point(c.x, c.y), Scalar(0, 255, 0), 2, CV_AA);
	cv::line(dst, Point(lp),Point(c), Scalar(255, 0, 255), 3, CV_AA);
	cv::circle(dst, c, 1, Scalar(255, 0, 255), -1, 8, 0);
	const int face[] = { cv::FONT_HERSHEY_SIMPLEX, cv::FONT_HERSHEY_PLAIN, cv::FONT_HERSHEY_DUPLEX, cv::FONT_HERSHEY_COMPLEX,
		cv::FONT_HERSHEY_TRIPLEX, cv::FONT_HERSHEY_COMPLEX_SMALL, cv::FONT_HERSHEY_SCRIPT_SIMPLEX,
		cv::FONT_HERSHEY_SCRIPT_COMPLEX, cv::FONT_ITALIC };
	string str = to_string(analog_value);
	putText(dst, "Analog:" + str, cv::Point(40, 150), face[1], 2.5, cv::Scalar(255, 0, 255), 3, CV_AA);
	return analog_value;
}

//�n�_�Ɛc���̊p�x�Z�o�p
float CDraw::CalcAnalogValue(Point c, Point s, Point e, Point li, float rate) {
	//���ϊO�ς̌v�Z�p�ϐ�
	float inner;
	float cross;
	float atan_rad;
	float atan_degree;
	//�ڐ���Ɩڐ���̊Ԋu�ɑ΂���l
	float m2mValue = 1 * MTR_MAX_VALUE / MEMORI_NUM;

	Point v1, v2, v3;
	//�x�N�g�� e s line
	v1.x = e.x - c.x;	v1.y = e.y - c.y;
	v2.x = s.x - c.x;	v2.y = s.y - c.y;
	v3.x = li.x - c.x;	v3.y = li.y - c.y;
	//start��end�̊p�x����m2m�Z�o
	inner = v1.x * v2.x + v1.y * v2.y;
	cross = v1.x * v2.y - v1.y * v2.x;
	atan_rad = atan2f(cross, inner);
	//360�x�ɕ␳
	if (atan_rad < 0) {
		atan_rad = atan_rad + (2 * CV_PI);
		atan_degree = atan_rad * 180.0 / (CV_PI);
	}
	else {
		atan_degree = atan_rad * 180.0 / (CV_PI);
	}
	float m2mRad = (360 - atan_degree) / MEMORI_NUM;
	inner = v2.x * v3.x + v2.y * v3.y;
	cross = v2.x * v3.y - v2.y * v3.x;
	atan_rad = atan2f(cross, inner);
	//cout << "���ϊO�σ��W�A��:" << atan_rad << endl;
	if (atan_rad < 0) {
		atan_rad = atan_rad + (2 * CV_PI);
		atan_degree = atan_rad * 180.0 / (CV_PI);//0-360
	}
	else {
		atan_degree = atan_rad * 180.0 / (CV_PI);
	}

	//0����0.02�ڐ���̕␳
	atan_degree += m2mRad * STABLE_RATE;
	//cout << "m2mrad" << m2mRad << endl;
	//cout << "���ϊO�ϊp�x:" << atan_degree << endl;

	//�A�i���O�l�Z�o����
	float ResultValue = (atan_degree) * (m2mValue / m2mRad);
	//�x�N�g���̋t�����␳(0�̏ꍇ)
	if(ResultValue >= MTR_MAX_VALUE){
		ResultValue = 0.0;
	}

	return ResultValue;
}
