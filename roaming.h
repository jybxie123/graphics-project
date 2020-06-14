#pragma once
#include "glut.h"
#include <iostream>
//支持zoom功能
class roaming
{
	//平移方向
	enum ROAMING_PAN_MOVEMENT
	{
		PAN_UP,
		PAN_DOWN,
		PAN_FORWARD,
		PAN_BACKWARD,
		PAN_LEFT,
		PAN_RIGHT
	};
	//鼠标滚轮操作
	enum SCROLL_STATE
	{
		SCROLL_DOWN,
		SCROLL_UP,
		SCROLL_PUSH//鼠标中键按下
	};
	//常量
	static const GLfloat 	MAX_SPEED,//移动速度
				MIN_SPEED,
				MAX_SENSITIVITY,//鼠标灵敏度
				MIN_SENSITIVITY,
				DEFAULT_FOVY,//默认视野二面角
				MAX_SCROLL_SENSITIVITY,//鼠标滚轮灵敏度
				MIN_SCROLL_SENSITIVITY,
				MAX_ZOOM,//缩放比例
		                MIN_ZOOM,
				MAX_PITCH,//x轴旋转角
				MIN_PITCH,
				TOTAL_YAW;//最大y轴旋转角

public:
	roaming();
	~roaming();

	//由按键回调函数调用
	void HandleKeyPress(ROAMING_PAN_MOVEMENT);
	//由鼠标操作回调函数调用
	void HandleMouseScroll(SCROLL_STATE, GLfloat);
	//由鼠标捕捉回调函数调用
	void HandleMouseMove(GLfloat, GLfloat);
	//由判断鼠标是否在窗口内回调函数调用
	void HandleMouseInWindow(bool);
	//设置移动速度
	void SetSpeed(GLfloat);
	//设置鼠标灵敏度
	void SetSensitivity(GLfloat);
	//设置鼠标滚轮灵敏度
	void SetScrollSensitivity(GLfloat);

private:
	GLfloat eye[3], forward[3], up[3], side[3];
	GLfloat speed, sensitivity, zoomRate, scrollSensitivity;
	GLfloat pitch, yaw;
	bool ifInWindow;

private:
	//更新lookat
	void UpdateLookAt();
	//更新glperspecive
	void UpdatePerspect();
};
