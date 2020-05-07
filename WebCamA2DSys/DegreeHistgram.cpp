#include "DegreeHistgram.h"

DegreeHistgram::DegreeHistgram()
{
}

DegreeHistgram::~DegreeHistgram()
{
}
//��_�Ԃ̋�������
float DegreeHistgram::getDistance(float x, float y, float x2, float y2) {
	float distance = sqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
	return distance;
}

//�p�x�q�X�g�O�����ɂ��ڐ���擾�Ǝn�I�_�T��
int DegreeHistgram::histgramMethodUzGray(Mat lbl, Point c, Mat draw, Point& s, Point& e, Point& l, RotatedRect& el, Mat& res, Mat& dline) {
	int cols = lbl.cols;
	int rows = lbl.rows;
	//�~���o�̔��a�𐧌�
	if (c.x >= cols) {
		cout << "�������~�����o�ł��Ă��Ȃ�(�傫���j" << endl;
		return 0;
	}
	if (c.y >= rows) {
		cout << "�������~�����o�ł��Ă��Ȃ�(�傫��)" << endl;
		return 0;
	}

	dlbl = lbl.clone();
	//y�ő�l�ɂ��n�I
	//�g���~���O
	Mat Llabel(lbl, Rect(0, c.y, c.x, lbl.rows - c.y));
	Mat Rlabel(lbl, Rect(c.x, c.y, lbl.cols - c.x, lbl.rows - c.y));
	//�`��Debug
	//imshow("R", Rlabel);
	//imshow("L", Llabel);
	//Rlabel = ~Rlabel;
	//Llabel = ~Llabel;
	//imwrite("R.jpg", Rlabel);
	//imwrite("L.jpg", Llabel);

	//�I�_y�ő�l
	for (int row = 0; row < Rlabel.rows; row++) {
		for (int col = 0; col < Rlabel.cols; col++) {
			if (Rlabel.at<unsigned char>(row, col) == 255) {
				if(row > Rmax){
					Rmax = row;
					Rpt.x = col;
					Rpt.y = row;
				}
			}
		}
	}
	//�g���~���O�̕␳
	Rpt.x += c.x;
	Rpt.y += c.y;
	//cout << "rmin" << Rpt << endl;
	//�n�_y�ő�l
	for (int row = 0; row < Llabel.rows; row++) {
		for (int col = 0; col < Llabel.cols; col++) {
			if (Llabel.at<unsigned char>(row, col) == 255) {
				if (row > Lmax) {
					Lmax = row;
					Lpt.x = col;
					Lpt.y = row;
				}
			}
		}
	}
	//�g���~���O�̕␳
	Lpt.y += c.y;

	//�q�X�g�O�����ɂ��c�����W�Z�o
	Mat calc_hist = Mat::zeros(360, 360, lbl.type());
	Mat draw_hist = Mat::zeros(360, 360, lbl.type());
	//�p�x�i�[�p�z��
	int buf[360] = {0};
	//�x�N�g�����Œ�
	lv.x = 0 - c.x;
	lv.y = 0 - c.y;
	//�p�x�v�Z�p�ϐ�
	float sita;
	float radian;
	float inner;
	float cross;
	int q;

	//�x�N�g���̊p�x�Z�o
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (lbl.at<unsigned char>(row, col) == 255) {
				tv.x = col - c.x;
				tv.y = row - c.y;
				//�O�ϓ���
				inner = lv.x * tv.x + lv.y * tv.y;
				cross = lv.x * tv.y - lv.y * tv.x;
				sita = atan2f(cross, inner);
				//360�x�ɕ␳
				if (sita < 0) {
					sita = sita + (2 * CV_PI);
					radian = sita * 180.0 / (CV_PI);
				}
				else {
					radian = sita * 180.0 / (CV_PI);
				}
				//�p�x���Z�l
				buf[(int)radian] += 3;
			}
		}
	}

	//�v�Z�p�q�X�g�O�����ɑ��
	int maxrad = 0;
	int tmpbuf = 0;
	for (int i = 0; i < 360; i++) {
		if (buf[i] >= calc_hist.rows) {
			buf[i] = calc_hist.rows -1;
		}
		if (buf[i] > tmpbuf) {
			tmpbuf = buf[i];
			maxrad = i;
		}
		calc_hist.at<unsigned char>(buf[i], i) = 255;
		//�`��p�q�X�g�O�����ɕ`��
		if (buf[i] > 0) {
			cv::line(draw_hist, Point(i, buf[i]), Point(i, 0), Scalar(255, 255, 255), 1, 4);
		}
	}

	//�Z�o�����p�x�ƍł��߂��p�x�̉�f��T��
	int maxp = 0;
	int sminrad = 1000;
	int eminrad = 1000;
	int maxminrad = 1000;
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (lbl.at<unsigned char>(row, col) == 255) {
				tv.x = col - c.x;
				tv.y = row - c.y;
				float inner = lv.x * tv.x + lv.y * tv.y;
				float cross = lv.x * tv.y - lv.y * tv.x;
				sita = atan2f(cross, inner);
				if (sita < 0) {
					sita = sita + (2 * CV_PI);
					radian = sita * 180.0 / (CV_PI);
				}
				else {
					radian = sita * 180.0 / (CV_PI);
				}
				if (abs(radian - maxrad) < maxminrad) {
					maxminrad = abs(radian - maxrad);
					maxValueP.x = col;
					maxValueP.y = row;
				}
			}
		}
	}
	//�ŉ��[�̎n�I�_���W���i�[
	if (Rpt.x < Lpt.x) {
		Rpt.x += 1;
		Lpt.x -= 1;
		s = Rpt;
		e = Lpt;
	}
	else {
		Rpt.x += 1;
		Lpt.x -= 1;
		e = Rpt;
		s = Lpt;
	}

	//�ԋp�l�̑��
	dline = dlbl;
	l = maxValueP;
	res = draw_hist;

	cout << "e" << endl;
	cout << e << endl;

	cout << "s" << endl;
	cout <<  s << endl;

	return 0;
}