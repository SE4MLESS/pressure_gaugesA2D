#include "Ellipse.h"

CEllipse::CEllipse()
{
	GetCircle = false;
}

CEllipse::~CEllipse()
{
}

//�n�t�ϊ��ɂ��~���o/�~�����o���ꂽ���ǂ�����ԋp����
bool CEllipse::CircleDetection(Mat src, Mat& torus_mask, Point& c, uint& r, Mat& bk_mask, Mat& drawCir) {

	//�}�X�N�摜�����p
	torus_mask = Mat::zeros(src.size(), CV_8UC1);
	bk_mask = Mat::zeros(src.size(), CV_8UC1);
	draw = src.clone();
	tmp_img = src.clone();
	cvtColor(tmp_img, gray, CV_BGR2GRAY);
	//�n�t�ϊ�
	vector<Vec3f> circles;
	HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
		2, 200, 100, 150, 150, 180);
	//�p�����[�^�̎d�l
	//dp = 1,mindist = 200,param(cany),param2 = �~�̒��S��臒l, �ŏ�����, �ő勗��

	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		tmp_rad = cvRound(circles[i][2]);
		//�}�X�N����
		circle(torus_mask, center, tmp_rad + TH1, Scalar(255, 255, 255),
			((tmp_rad / 2) + TH2), 4);

		circle(bk_mask, center, tmp_rad + BK_TH1, Scalar(255, 255, 255), -1, CV_AA);
		//�`��p
		circle(draw, center, tmp_rad, Scalar(255, 0, 255), 1);
		circle(draw, center, 3, Scalar(255, 0, 255), -1, 8, 0);
		x = center.x;
		y = center.y;
		radius = cvRound(circles[i][2]);
		c = center;
		r = radius;
		cnt++;
		//cout << "radius:" << radius << endl;
		break;
	}

	//�~�����o����Ȃ��ꍇ
	if (cnt == 0) {
		cout << "�~�����o����܂���\n���͌v�ƃJ�����ʒu�𒲐����Ă�������\n" << endl;
	}
	else{
		drawCir = draw.clone();
		GetCircle = true;
	}

	return GetCircle;
}

//�ڐ���̈�̑ȉ~�ߎ�
void CEllipse::FittingEllipse(Mat bi, RotatedRect& el, float &r, Mat &draw_el,Mat& elMask,Point c) {
	vector<vector<Point> > contours;
	Mat bimage;
	bimage = bi.clone();
	//�N���[�W���O����
	int dilate_size = 2;
	Mat element = getStructuringElement(cv::MORPH_ELLIPSE,
		Size(2 * dilate_size + 1, 2 * dilate_size + 1),
		Point(dilate_size, dilate_size));
	dilate(bimage, bimage, element);
	erode(bimage, bimage, element);
	Mat cimage = Mat::zeros(bimage.size(), CV_8UC3);

	//�ڐ���␳
	float bmax = 0;
	float bmin = 10000;
	float sum = 0;
	int cnt = 0;
	float dmin = 10000;
	//�����l�̌v�Z�ɗp����z��
	int buf[100] = { -1 };

	//�֊s���o
	findContours(bimage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	for (size_t i = 0; i < contours.size(); i++) {
		size_t count = contours[i].size();
		if (count < 6)
			continue;
		Mat pointsf;
		Mat(contours[i]).convertTo(pointsf, CV_32F);
		RotatedRect box = fitEllipse(pointsf);

		//�擾����֊s�̑傫����臒l
		//if (count < 100 || count > 1000) continue;
		if (count < 30 || count > 2000) continue;
		//�֊s���o
		drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);
		//�ȉ~�Z�o
		ellipse(cimage, box, Scalar(0, 0, 255), 1, CV_AA);
		ellipse(cimage, box.center, box.size*0.5f, box.angle, 0, 360, Scalar(0, 255, 0), 1, CV_AA);
		circle(cimage, box.center, 5, Scalar(0, 255, 0), -1, 8, 0);
		//cout << box.center << endl;
		
		//�~�̒��S�Ƒȉ~�̒��S�̋���
		float distance = sqrt((box.center.x - c.x) * (box.center.x - c.x) + (box.center.y -c. y) * (box.center.y - c.y));
		buf[cnt] = box.size.width;
		//�␳�Ώۂ̑ȉ~����
		if (dmin > distance) {
			bmax = box.size.width;
			dmin = distance;
			el = box;
		}
		//�ȉ~�̍ŏ��̕��擾
		else if (box.size.width < bmin) {
			bmin = box.size.width;
		}
		sum += box.size.width;
		cnt++;
		/*BB�`��p
		Point2f vtx[4];
		box.points(vtx);
		for (int j = 0; j < 4; j++) {
			line(cimage, vtx[j], vtx[(j + 1) % 4], Scalar(0, 255, 0), 1, CV_AA);
		}*/
	}
	//�ȉ~�̕��̒����l�v�Z�̂��߃\�[�g
	float tmp, median = 0;
	for (int i = 0; i < 60 - 1; i++) {
		for (int j = i + 1; j < 60; j++) {
			if (buf[i] > 0) {
				if (buf[i] > buf[j]) {
					tmp = buf[i];
					buf[i] = buf[j];
					buf[j] = tmp;
				}
			}
		}
	}
	//�����l�Z�o
	median = (buf[29] + buf[30]) / 2;
	//�ڐ���␳�̎����v�Z
	float rate = bmin / median;
	//�ԋp�l
	r = rate;
	elMask = Mat::zeros(bi.size(), CV_8UC1);
	circle(elMask, el.center, el.size.width + 10, Scalar(255, 255, 255), -1, CV_AA);
	draw_el = cimage;

}
