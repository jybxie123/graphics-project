#pragma once
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include "glut.h"

//实现了基本的光照模模型
//将继续拓展至实时阴影映射功能
class Light
{
	//内部枚举到gl枚举的变换map
	static const GLenum numToLightEnum[8];
	static const GLenum numToIngredient[3];
	static const GLenum numToAttenuation[3];
	//光源类型
	enum LIGHT_TYPE
	{
		AMBIENT_LIGHT_TYPE = 0,
		POINT_LIGHT_TYPE = 1,
		SPOT_LIGHT_TYPE = 2,
		DIRECTION_LIGHT_TYPE = 3
	};
	//光源成分
	enum LIGHT_INGREDIENT
	{
		AMBIENT_LIGHT = 0,
		DIFFUSE_LIGHT = 1,
		SPECULAR_LIGHT = 2
	};
	//常见颜色枚举
	enum LIGHT_COLOR_INDEX
	{
		WHITE_COLOR = 0,
		BLACK_COLOR = 1,
		GREEN_COLOR = 2,
		CYAN_COLOR = 3,
		YELLOW_COLOR = 4,
		RED_COLOR = 5,
		BLUE_COLOR = 6,
		PURPLE_COLOR = 7,
		GRAY_COLOR = 8
	};
	//光源衰减系数枚举
	enum ATTENUATION_TYPE
	{
		CONSTANT = 0,
		LINER = 1,
		QUADRATIC = 2
	};
public:
	Light();
	~Light();
	//调用glLightfv同步openGL内部状态机
	void UpDateLight(int index) const;
	//设置光源位置
	void SetLightPos(int index, GLfloat* input);
	void GetLightPos(int index, GLfloat* result) const;
	//设置光源各成分颜色
	void SetLightColor(int index, LIGHT_INGREDIENT ingre, GLfloat* content);
	void SetLightColor(int index, LIGHT_INGREDIENT ingre, LIGHT_COLOR_INDEX colorIndex);
	void GetLightColor(int index, LIGHT_INGREDIENT ingre, GLfloat* res);
	//设置光源衰减系数
	void SetLightAttenuation(int index, ATTENUATION_TYPE type, GLfloat value);
	GLfloat GetLightAttenuation(int index, ATTENUATION_TYPE type) const;
	//设置光源开关
	void SetLightOn(int index, bool value);
	bool GetLightOn(int index) const;
	//设置光源类型
	void SetLightType(int index, LIGHT_TYPE type);
	LIGHT_TYPE GetLightType(int index) const;
	//设置聚光指数
	void SetPlightExp(int index, GLfloat value);
	GLfloat GetPlightExp(int index) const;
	//设置点光源聚光截止角
	void SetPlightCutoff(int index, GLfloat value);
	GLfloat GetPlightCutoff(int index) const;;
	//设置聚光灯光照方向
	void SetPlightDirection(int index, GLfloat* value);
	void GetPlightDirection(int index, GLfloat* value) const;
	//根据传入的光源类型，设置各光源类型默认参数
	void SetDefault(int index, LIGHT_TYPE type);
private:
	GLfloat lightPos[8][4], lightColor[3][8][4],
			lightAttenuation[8][3][1], plightExponent[8][1],
			plightDirection[8][3], plightCutoff[8][1],
			lightColorIndex[9][4];
	bool lightOn[8];
	LIGHT_TYPE lightType[8];
private:
	//用于数组赋值
	inline void setEqual(GLfloat[], GLfloat[], int);
};