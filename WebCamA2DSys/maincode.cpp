#include "Control.h"

void response(float *result);
vector<VideoCapture> getCamNum(vector<VideoCapture> list);

int main(void) {
	//�A�i���O�l��ԋp
	float result[CAM_MAX_NUM];
	//�J�������l������ϐ�
	vector<VideoCapture> list;
	//���J�������擾���X�g�ɕێ�
	list = getCamNum(list);

	//cout << "camnum" << list.size() << endl;

	while (1) {
		CControl *Algorithm = new CControl();
		Algorithm->Start//(list,result,DEBUG_ON);//�摜�\��
						(list,result, DEBUG_OFF);//�摜��\��
		response(result);
		delete Algorithm;
	}
	return 0;
}

//�A�i���O�l��`��
void response(float *result) {
	for (int i = 0; i < CAM_MAX_NUM; i++) {
		cout << "Analog[" << i << "]:" << result[i] << endl;
	}
	printf("\n");
}

//�J�����䐔�F���p
vector<VideoCapture> getCamNum(vector<VideoCapture> list) {
	//�J������`0-4��usb�|�[�g�ԍ�������
	cv::VideoCapture cam0(0);
	cv::VideoCapture cam1(1);
	cv::VideoCapture cam2(2);
	cv::VideoCapture cam3(3);
	cv::VideoCapture cam4(4);
	//�J�����m�F
	if (cam0.isOpened())	list.push_back(cam0);
	if (cam1.isOpened())	list.push_back(cam1);
	if (cam2.isOpened())	list.push_back(cam2);
	if (cam3.isOpened())	list.push_back(cam3);
	if (cam4.isOpened())	list.push_back(cam4);

	return list;
}