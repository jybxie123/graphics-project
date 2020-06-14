#pragma once
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include "glut.h"

//ʵ���˻����Ĺ���ģģ��
//��������չ��ʵʱ��Ӱӳ�书��
class Light
{
	//�ڲ�ö�ٵ�glö�ٵı任map
	static const GLenum numToLightEnum[8];
	static const GLenum numToIngredient[3];
	static const GLenum numToAttenuation[3];
	//��Դ����
	enum LIGHT_TYPE
	{
		AMBIENT_LIGHT_TYPE = 0,
		POINT_LIGHT_TYPE = 1,
		SPOT_LIGHT_TYPE = 2,
		DIRECTION_LIGHT_TYPE = 3
	};
	//��Դ�ɷ�
	enum LIGHT_INGREDIENT
	{
		AMBIENT_LIGHT = 0,
		DIFFUSE_LIGHT = 1,
		SPECULAR_LIGHT = 2
	};
	//������ɫö��
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
	//��Դ˥��ϵ��ö��
	enum ATTENUATION_TYPE
	{
		CONSTANT = 0,
		LINER = 1,
		QUADRATIC = 2
	};
public:
	Light();
	~Light();
	//����glLightfvͬ��openGL�ڲ�״̬��
	void UpDateLight(int index) const;
	//���ù�Դλ��
	void SetLightPos(int index, GLfloat* input);
	void GetLightPos(int index, GLfloat* result) const;
	//���ù�Դ���ɷ���ɫ
	void SetLightColor(int index, LIGHT_INGREDIENT ingre, GLfloat* content);
	void SetLightColor(int index, LIGHT_INGREDIENT ingre, LIGHT_COLOR_INDEX colorIndex);
	void GetLightColor(int index, LIGHT_INGREDIENT ingre, GLfloat* res);
	//���ù�Դ˥��ϵ��
	void SetLightAttenuation(int index, ATTENUATION_TYPE type, GLfloat value);
	GLfloat GetLightAttenuation(int index, ATTENUATION_TYPE type) const;
	//���ù�Դ����
	void SetLightOn(int index, bool value);
	bool GetLightOn(int index) const;
	//���ù�Դ����
	void SetLightType(int index, LIGHT_TYPE type);
	LIGHT_TYPE GetLightType(int index) const;
	//���þ۹�ָ��
	void SetPlightExp(int index, GLfloat value);
	GLfloat GetPlightExp(int index) const;
	//���õ��Դ�۹��ֹ��
	void SetPlightCutoff(int index, GLfloat value);
	GLfloat GetPlightCutoff(int index) const;;
	//���þ۹�ƹ��շ���
	void SetPlightDirection(int index, GLfloat* value);
	void GetPlightDirection(int index, GLfloat* value) const;
	//���ݴ���Ĺ�Դ���ͣ����ø���Դ����Ĭ�ϲ���
	void SetDefault(int index, LIGHT_TYPE type);
private:
	GLfloat lightPos[8][4], lightColor[3][8][4],
			lightAttenuation[8][3][1], plightExponent[8][1],
			plightDirection[8][3], plightCutoff[8][1],
			lightColorIndex[9][4];
	bool lightOn[8];
	LIGHT_TYPE lightType[8];
private:
	//�������鸳ֵ
	inline void setEqual(GLfloat[], GLfloat[], int);
};