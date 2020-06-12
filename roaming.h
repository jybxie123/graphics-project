#pragma once
#include "glut.h"
#include <iostream>

class roaming
{
	//ƽ�Ʒ���
	enum ROAMING_PAN_MOVEMENT
	{
		PAN_UP,
		PAN_DOWN,
		PAN_FORWARD,
		PAN_BACKWARD,
		PAN_LEFT,
		PAN_RIGHT
	};
	//�����ֲ���
	enum SCROLL_STATE
	{
		SCROLL_DOWN,
		SCROLL_UP,
		SCROLL_PUSH//����м�����
	};
	//����
	static const GLfloat MAX_SPEED,//�ƶ��ٶ�
						 MIN_SPEED,
						 MAX_SENSITIVITY,//���������
					     MIN_SENSITIVITY,
						 DEFAULT_FOVY,//Ĭ����Ұ�����
						 MAX_SCROLL_SENSITIVITY,//������������
						 MIN_SCROLL_SENSITIVITY,
						 MAX_ZOOM,//���ű���
		                 MIN_ZOOM,
						 MAX_PITCH,//x����ת��
						 MIN_PITCH,
		                 TOTAL_YAW;//���y����ת��

public:
	roaming();
	~roaming();

	//�ɰ����ص���������
	void HandleKeyPress(ROAMING_PAN_MOVEMENT);
	//���������ص���������
	void HandleMouseScroll(SCROLL_STATE, GLfloat);
	//����겶׽�ص���������
	void HandleMouseMove(GLfloat, GLfloat);
	//���ж�����Ƿ��ڴ����ڻص���������
	void HandleMouseInWindow(bool);
	//�����ƶ��ٶ�
	void SetSpeed(GLfloat);
	//�������������
	void SetSensitivity(GLfloat);
	//����������������
	void SetScrollSensitivity(GLfloat);

private:
	GLfloat eye[3], forward[3], up[3], side[3];
	GLfloat speed, sensitivity, zoomRate, scrollSensitivity;
	GLfloat pitch, yaw;
	bool ifInWindow;

private:
	//����lookat
	void UpdateLookAt();
	//����glperspecive
	void UpdatePerspect();
};
