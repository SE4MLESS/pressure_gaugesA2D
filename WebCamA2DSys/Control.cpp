#include "Control.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

CControl::CControl()
{
	Init();
}
CControl::~CControl()
{
}

void CControl::Init(void) {
	camNum = 5;
	crct_rate = 1.0;
	analog_value = 0;
}

void CControl::Init(Mat src, Mat& enh, Mat& inv) {
	//�_�E���X�P�[��
	resize(src, src, Size(), 0.5, 0.5, INTER_LANCZOS4);
	//�ڍ׋����t�B���^
	detailEnhance(src, enh, 20, 0.15f);
	inv = enh.clone();
}

int CControl::Start(vector<VideoCapture> list, float result[CAM_MAX_NUM], bool d_mode) {
	//������
	for (int i = 0; i < CAM_MAX_NUM; i++) {
		result[i] = -1;
	}

	//�����J�n���X�g����ɂȂ�܂�
	//���X�g�ɂ�web�J������ێ�
	while (!list.empty()) {
		//cout << "listsize" << list.size() << endl;
		//���X�g�̖����擾
		cam = list.back();
		///debug
		//if (list.size() == 2) {
		//	frame = imread("./images\\input\\6 (2).jpg");
		//}else if (list.size() == 1) {
		//	frame = imread("./images\\input\\6 (3).jpg");
		//}else if (list.size() == 0) {
		//	frame = imread("./images\\input\\6 (4).jpg");
		//}
		///debug

		//list�̖����̃J��������摜�擾
		cam >> frame;
		//������
		Init(frame, enhance, work_inv);
		//�`��p
		response = frame.clone();
		resize(response, response, Size(), 0.5, 0.5, INTER_LANCZOS4);
		//�m���I�n�t�ɂ��~���o/�~���o��true
		GetCircle = EL.CircleDetection(enhance, trs_mask, hough_ctr, radius, bk_mask, draw_hou_c);
		if (GetCircle) {
			//�������x����p�ϐ�
			//local_time_s = clock();
			//�O���C�X�P�[���ϊ�
			cvtColor(work_inv, enh_bin, CV_BGR2GRAY);
			//�K���I��l��
			adaptiveThreshold(enh_bin, enh_bin, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
				THRESH_BINARY_INV, ADAPT_GAUSS_FILTER, 2);
			//�w�i�����̃}�X�N����
			bitwise_and(bk_mask, enh_bin, bin_uz_th);
			//imwrite("./images\\bin.png", bin_uz_th);
			//���x�����O�ɂ��ڐ���̈�擾
			LB.Labeling(bin_uz_th, after_label);
			//�O���C�X�P�[���Ɠ�l��
			al_bin = after_label.clone();
			cvtColor(al_bin, al_bin, CV_BGR2GRAY);
			threshold(al_bin, al_bin, 128, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
			//�א���
			TH.PlayThinning(al_bin, th_uz_cmask);
			before_label = th_uz_cmask.clone();
			//�ȉ~�ߎ�
			EL.FittingEllipse(th_uz_cmask, elps_box, crct_rate, fit_ell, elmask, hough_ctr);
			//�n�t�̒��S����ȉ~�̒��S�ɕύX
			hough_ctr.x = elps_box.center.x;
			hough_ctr.y = elps_box.center.y;
			//�}�X�N���]
			elmask = ~elmask;
			//�}�X�N����
			bitwise_and(elmask, th_uz_cmask, uz_ell_mask);
			//�q�X�g�O�����v�Z
			HST.histgramMethodUzGray(uz_ell_mask, hough_ctr, enhance, markSt, markEd, linePt, elps_box, hist, draw_line);
			//�A�i���O�l�̎Z�o����
			analog_value = D.PlotData(response, hough_ctr, markSt, markEd, radius, linePt, plot_data, elps_box, crct_rate);
			//�������x����p�ϐ�
			//full_end = clock();
			//cout << "full_time: " << (float)(full_end - full_start) / CLOCKS_PER_SEC << "sec.\n\n";

			//�f�o�b�O���[�h
			if (d_mode) {
				oss << std::setfill('0') << std::setw(1) << list.size()/*i*/;
				//�摜�\��
				imshow("hough_circle", draw_hou_c);
				imshow("before_thining", bin_uz_th);
				imshow("adapt_binary", enh_bin);
				imshow("thinning(th_uz_cmask)", th_uz_cmask);
				imshow("before_label", before_label);
				imshow("after_label", after_label);
				imshow("ellipse", fit_ell);
				imshow("histgram", hist);
				imshow("plot", plot_data);
				//�摜���]
				before_label = ~before_label;
				after_label = ~after_label;
				fit_ell = ~fit_ell;
				draw_line = ~draw_line;
				th_uz_cmask = ~th_uz_cmask;
				hist = ~hist;
				//��΃p�X�ɉ摜�ۑ�
				//�t�H���_���쐬����ƕۑ������
				imwrite("./images\\cam" + oss.str() + "\\enhance.png", enhance);
				imwrite("./images\\cam" + oss.str() + "\\binary.png", enh_bin);
				imwrite("./images\\cam" + oss.str() + "\\hough_circle.png", draw_hou_c);
				imwrite("./images\\cam" + oss.str() + "\\bkmask.png", bk_mask);
				imwrite("./images\\cam" + oss.str() + "\\cmask.png", trs_mask);
				imwrite("./images\\cam" + oss.str() + "\\thinned.png", th_uz_cmask);
				imwrite("./images\\cam" + oss.str() + "\\before_label.png", before_label);
				imwrite("./images\\cam" + oss.str() + "\\after_label.png", after_label);
				imwrite("./images\\cam" + oss.str() + "\\Ellipse.png", fit_ell);
				imwrite("./images\\cam" + oss.str() + "\\histgram.png", hist);
				imwrite("./images\\cam" + oss.str() + "\\line.png", draw_line);
				imwrite("./images\\cam" + oss.str() + "\\Results_data" + oss.str() + " .png", plot_data);
				//�摜�\���p
				waitKey(0);
			}
		}
		//�ԋp�l�ɑ��
		result[list.size() - 1] = analog_value;
		//���X�g�̍X�V
		list.pop_back();
	}
	return 0;
}