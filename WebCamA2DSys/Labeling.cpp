#include "Labeling.h"

CLabeling::CLabeling()
{
}

CLabeling::~CLabeling()
{
}

//���x�����O�ɂ��ڐ���̈挟�o
void CLabeling::Labeling(Mat bin, Mat& MaxLabel) {
	Mat LabelImg;
	Mat stats;
	Mat centroids;
	int nLab = cv::connectedComponentsWithStats(bin, LabelImg, stats, centroids);
	int maxArea, maxValue = 0;
	Mat dst(bin.size(), CV_8UC3);

	// ���x�����O�̕`��F
	std::vector<cv::Vec3b> colors(nLab);
	colors[0] = cv::Vec3b(0, 0, 0);
	//�ʐϒl�̏o��
	for (int i = 1; i < nLab; ++i) {
		int *param = stats.ptr<int>(i);
	}
		//ROI�̐ݒ�
		for (int i = 1; i < nLab; ++i) {
				int *param = stats.ptr<int>(i);
				int x = param[cv::ConnectedComponentsTypes::CC_STAT_LEFT];
				int y = param[cv::ConnectedComponentsTypes::CC_STAT_TOP];
				int height = param[cv::ConnectedComponentsTypes::CC_STAT_HEIGHT];
				int width = param[cv::ConnectedComponentsTypes::CC_STAT_WIDTH];
				int s = width/*(width - x) * (height - y)*/;
				if (s > maxValue) {
					maxValue = s;
					maxArea = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];
				}
		}
	//�ő啝�̈�̐F����
	for (int i = 1; i < nLab; ++i) {
		int *param = stats.ptr<int>(i);
		if (param[cv::ConnectedComponentsTypes::CC_STAT_AREA] == maxArea)
			colors[i] = cv::Vec3b(255, 255, 255);
		else
			colors[i] = cv::Vec3b(0, 0, 0);
	}
	// ���x�����O���ʂ̕`��
	for (int i = 0; i < dst.rows; ++i) {
		int *lb = LabelImg.ptr<int>(i);
		cv::Vec3b *pix = dst.ptr<cv::Vec3b>(i);
		for (int j = 0; j < dst.cols; ++j) {
			pix[j] = colors[lb[j]];
		}
	}
	//imshow("labeling->th", bin);
	MaxLabel = dst.clone();
	//cv::imwrite("./images\\after_labeling.png", dst);
}
