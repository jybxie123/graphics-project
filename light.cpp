#include "light.h"
#include "MyException.h"

const GLenum Light::numToLightEnum[8] =
{
	GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,
	GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7
};
const GLenum Light::numToLightEnum[3] =
{
	GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR
};
const GLenum Light::numToAttenuation[3] =
{
	GL_CONSTANT_ATTENUATION,
	GL_LINEAR_ATTENUATION,
	GL_QUADRATIC_ATTENUATION
};
//defualt : light0 //, light1 x, light2 /
Light::Light()
{
	//set lightPos
	lightPos[0][0] = 5.0f, lightPos[0][1] = 5.0f, lightPos[0][2] = 5.0f, lightPos[0][3] = 0.0f;
	lightPos[1][0] = 0.0f, lightPos[1][1] = 5.0f, lightPos[1][2] = 0.0f, lightPos[1][3] = 1.0f;
	lightPos[2][0] = 0.0f, lightPos[2][1] = 5.0f, lightPos[2][2] = 0.0f, lightPos[2][3] = 1.0f;
	int i;
	for (i = 3; i <= 7; i++)
	{
		lightPos[i][0] = 0.0f, lightPos[i][1] = 0.0f,
		lightPos[i][2] = 1.0f, lightPos[i][3] = 0.0f;
	}
	//set on
	lightOn[0] = lightOn[1] = lightOn[2] = true;
	for (i = 3; i <= 7; i++)
		lightOn[i] = false;
	//set lightColorIndex
	/*
		WHITE_COLOR = 0,
		BLACK_COLOR = 1,
		GREEN_COLOR = 2,
		CYAN_COLOR = 3,
		YELLOW_COLOR = 4,
		RED_COLOR = 5,
		BLUE_COLOR = 6,
		PURPLE_COLOR = 7,
		GRAY_COLOR = 8
	*/
		//white
	lightColorIndex[0][0] = 1.0f; lightColorIndex[0][1] = 1.0f;
	lightColorIndex[0][2] = 1.0f; lightColorIndex[0][3] = 1.0f;
		//black
	lightColorIndex[1][0] = 0.0f; lightColorIndex[1][1] = 0.0f;
	lightColorIndex[1][2] = 0.0f; lightColorIndex[1][3] = 1.0f;
		//green
	lightColorIndex[2][0] = 0.0f; lightColorIndex[2][1] = 1.0f;
	lightColorIndex[2][2] = 0.0f; lightColorIndex[2][3] = 1.0f;
		//cyan
	lightColorIndex[3][0] = 0.0f; lightColorIndex[3][1] = 1.0f;
	lightColorIndex[3][2] = 1.0f; lightColorIndex[3][3] = 1.0f;
		//yellow
	lightColorIndex[4][0] = 1.0f; lightColorIndex[4][1] = 1.0f;
	lightColorIndex[4][2] = 0.0f; lightColorIndex[4][3] = 1.0f;
		//red
	lightColorIndex[5][0] = 1.0f; lightColorIndex[5][1] = 0.0f;
	lightColorIndex[5][2] = 0.0f; lightColorIndex[5][3] = 1.0f;
		//blue
	lightColorIndex[6][0] = 0.0f; lightColorIndex[6][1] = 0.0f;
	lightColorIndex[6][2] = 1.0f; lightColorIndex[6][3] = 1.0f;
		//purple
	lightColorIndex[7][0] = 1.0f; lightColorIndex[7][1] = 0.0f;
	lightColorIndex[7][2] = 1.0f; lightColorIndex[7][3] = 1.0f;
		//gray
	lightColorIndex[8][0] = 0.2f; lightColorIndex[8][1] = 0.2f;
	lightColorIndex[8][2] = 0.2f; lightColorIndex[8][3] = 1.0f;
	//set LightType
	lightType[0] = DIRECTION_LIGHT_TYPE;
	lightType[1] = POINT_LIGHT_TYPE;
	lightType[2] = SPOT_LIGHT_TYPE;
	for (i = 3; i <= 7; i++)
		lightType[i] = AMBIENT_LIGHT_TYPE;
	//setColor
	setEqual(lightColor[AMBIENT_LIGHT][0], lightColorIndex[GRAY_COLOR], 4);
	setEqual(lightColor[DIFFUSE_LIGHT][0], lightColorIndex[WHITE_COLOR], 4);
	setEqual(lightColor[SPECULAR_LIGHT][0], lightColorIndex[WHITE_COLOR], 4);

	setEqual(lightColor[AMBIENT_LIGHT][1], lightColorIndex[GRAY_COLOR], 4);
	setEqual(lightColor[DIFFUSE_LIGHT][1], lightColorIndex[RED_COLOR], 4);
	setEqual(lightColor[SPECULAR_LIGHT][1], lightColorIndex[RED_COLOR], 4);

	setEqual(lightColor[AMBIENT_LIGHT][2], lightColorIndex[GRAY_COLOR], 4);
	setEqual(lightColor[DIFFUSE_LIGHT][2], lightColorIndex[BLUE_COLOR], 4);
	setEqual(lightColor[SPECULAR_LIGHT][2], lightColorIndex[BLUE_COLOR], 4);

	GLfloat type1[4] = { 0.0f, 0.0f, 0.0f, 1.0f }, type2[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	for (i = 3; i <= 7; i++)
	{
		//set default color
		setEqual(lightColor[AMBIENT_LIGHT][i], type1, 4);
		setEqual(lightColor[DIFFUSE_LIGHT][i], type2, 4);
		setEqual(lightColor[SPECULAR_LIGHT][i], type2, 4);
	}
	//set attenuation
	GLfloat type3[3] = { 1.0f, 0.0f, 0.0f };
	for (i = 0; i <= 7; i++)
	{
		lightAttenuation[i][0][0] = type3[0];
		lightAttenuation[i][1][0] = type3[1];
		lightAttenuation[i][2][0] = type3[2];
	}
	//set spot part
	GLfloat type4[3] = { 0.0f, 0.0f, -1.0f };
	for (i = 0; i <= 7; i++)
	{
		setEqual(plightDirection[i], type4, 3);
		plightCutoff[i][0] = 180.0f;
		plightExponent[i][0] = 0.0f;
	}
	GLfloat type5[3] = { 5.0f, -5.0f, 5.0f };
	setEqual(plightDirection[2], type5, 3);
	plightCutoff[2][0] = 45.0f;
	plightExponent[2][0] = 3.0f;
}
Light::~Light()
{

}
void Light::SetLightPos(int index, GLfloat* input)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	setEqual(lightPos[index], input, 4);
	glLightfv(numToLightEnum[index], GL_POSITION, lightPos[index]);
}
void Light::GetLightPos(int index, GLfloat* result) const
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	result[0] = lightPos[index][0];
	result[1] = lightPos[index][1];
	result[2] = lightPos[index][2];
	result[3] = lightPos[index][3];
}
void Light::SetLightColor(int index, LIGHT_INGREDIENT ingre, GLfloat* content)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if (ingre < 0 || ingre > 2)
			throw EXC_WRONG_LIGHT_INGREDIENT;
	}
	catch (MyException exc)
	{
		throw exc;
	}

	setEqual(lightColor[ingre][index], content, 4);
	glLightfv(numToLightEnum[index], numToIngredient[ingre], lightColor[ingre][index]);
}
void Light::SetLightColor(int index, LIGHT_INGREDIENT ingre, LIGHT_COLOR_INDEX colorIndex)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if (ingre < 0 || ingre > 2)
			throw EXC_WRONG_LIGHT_INGREDIENT;
		else if (colorIndex < 0 || colorIndex > 8)
			throw EXC_WRONG_COLOR_INDEX;
	}
	catch (MyException exc)
	{
		throw exc;
	}

	setEqual(lightColor[ingre][index], lightColorIndex[colorIndex], 4);
	glLightfv(numToLightEnum[index], numToIngredient[ingre], lightColor[ingre][index]);
}
void Light::GetLightColor(int index, LIGHT_INGREDIENT ingre, GLfloat* res)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if (ingre < 0 || ingre > 2)
			throw EXC_WRONG_LIGHT_INGREDIENT;
	}
	catch (MyException exc)
	{
		throw exc;
	}

	res[0] = lightColor[ingre][index][0];
	res[1] = lightColor[ingre][index][1];
	res[2] = lightColor[ingre][index][2];
	res[3] = lightColor[ingre][index][3];
}
void Light::SetLightAttenuation(int index, ATTENUATION_TYPE type, GLfloat value)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if (type < 0 || type > 2)
			throw EXC_WRONG_ATTENUATION_TYPE;
		else
		{
			GLfloat func[3];
			for (int i = 0; i <= 2; i++)
				if (i == type)
					func[i] = value;
				else
					func[i] = lightAttenuation[index][i][0];
			if (func[2] < 0 || (func[2] > 0 && (func[1] * func[1] - 4 * func[2] * func[0] >= 0)) || (func[2] > 0 && (func[1] / (-2.0f * func[2]) > 0)) || (func[2] == 0 && (func[1] < 0 || func[0] < 0)))
				throw EXC_WRONG_ATTENUATION_MODEL;
		}
	}
	catch (MyException exc)
	{
		throw exc;
	}
	lightAttenuation[index][type][0] = value;
	glLightfv(numToLightEnum[index], numToAttenuation[type], lightAttenuation[index][type]);
}
GLfloat Light::GetLightAttenuation(int index, ATTENUATION_TYPE type) const
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if (type < 0 || type > 2)
			throw EXC_WRONG_ATTENUATION_TYPE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	return lightAttenuation[index][type][0];
}
void Light::SetLightOn(int index, bool value)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	lightOn[index] = value;
	if (value)
		glEnable(numToLightEnum[index]);
	else
		glDisable(numToLightEnum[index]);
}
bool Light::GetLightOn(int index) const
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	return lightOn[index];
}
void Light::SetLightType(int index, LIGHT_TYPE type)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if(type < 0 || type > 3)
			throw EXC_WRONG_LIGHT_TYPE;
	}
	catch (MyException exc)
	{
		throw exc;
	}

	lightType[index] = type;
	switch (type)
	{
	case AMBIENT_LIGHT_TYPE :
	case DIRECTION_LIGHT_TYPE :
	{
		lightPos[index][3] = 0.0f;
		break;
	}
	case POINT_LIGHT_TYPE:
	case SPOT_LIGHT_TYPE:
	{
		lightPos[index][3] = 1.0f;
		break;
	}
	}
	glLightfv(numToLightEnum[index], GL_POSITION, lightPos[index]);
}
Light::LIGHT_TYPE Light::GetLightType(int index) const
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	return lightType[index];
}
void Light::SetPlightExp(int index, GLfloat value)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	plightExponent[index][0] = value;
	glLightfv(numToLightEnum[index], GL_SPOT_EXPONENT, plightExponent[index]);
}
GLfloat Light::GetPlightExp(int index) const
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	return plightExponent[index][0];
}
void Light::SetPlightCutoff(int index, GLfloat value)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if (value < 0.0f || value > 90.0f)
			throw EXC_WRONG_CUTOFF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	plightCutoff[index][0] = value;
	glLightfv(numToLightEnum[index], GL_SPOT_CUTOFF, plightCutoff[index]);
}
GLfloat Light::GetPlightCutoff(int index) const
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	return plightCutoff[index][0];
}
void Light::SetPlightDirection(int index, GLfloat* value)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if (value[0] == 0.0f || value[1] == 0.0f && value[2] == 0.0f)
			throw EXC_WRONG_LIGHT_DIRECTION;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	setEqual(plightDirection[index], value, 3);
	glLightfv(numToLightEnum[index], GL_SPOT_DIRECTION, plightDirection[index]);
}
void Light::GetPlightDirection(int index, GLfloat* value) const
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	value[0] = plightDirection[index][0];
	value[1] = plightDirection[index][1];
	value[2] = plightDirection[index][2];
}
void Light::UpDateLight(int x) const
{
	try
	{
		if (x < 0 || x > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}

	if (lightOn[x])
	{	//if the light is on, change the state
		glLightfv(numToLightEnum[x], GL_AMBIENT, lightColor[AMBIENT_LIGHT][x]);
		glLightfv(numToLightEnum[x], GL_DIFFUSE, lightColor[DIFFUSE_LIGHT][x]);
		glLightfv(numToLightEnum[x], GL_SPECULAR, lightColor[SPECULAR_LIGHT][x]);
		glLightfv(numToLightEnum[x], GL_POSITION, lightPos[x]);
		glLightfv(numToLightEnum[x], GL_SPOT_DIRECTION, plightDirection[x]);
		glLightfv(numToLightEnum[x], GL_SPOT_CUTOFF, plightCutoff[x]);
		glLightfv(numToLightEnum[x], GL_SPOT_EXPONENT, plightExponent[x]);
		glLightfv(numToLightEnum[x], GL_CONSTANT_ATTENUATION, lightAttenuation[x][CONSTANT]);
		glLightfv(numToLightEnum[x], GL_LINEAR_ATTENUATION, lightAttenuation[x][LINER]);
		glLightfv(numToLightEnum[x], GL_QUADRATIC_ATTENUATION, lightAttenuation[x][QUADRATIC]);
		glEnable(numToLightEnum[x]);
	}
	else
		glDisable(numToLightEnum[x]);
}
void Light::SetDefault(int index, LIGHT_TYPE type)
{
	try
	{
		if (index < 0 || index > 7)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
		else if (type < 0 || type > 3)
			throw EXC_WRONG_LIGHT_TYPE;
	}
	catch (MyException exc)
	{
		throw exc;
	}

	switch (type)
	{
	case AMBIENT_LIGHT:
	{
		GLfloat type4[4] = { 0.0f, -1.0f, 0.0f };
		lightPos[index][0] = 5.0f, lightPos[index][1] = 5.0f, lightPos[index][2] = 5.0f, lightPos[index][3] = 0.0f;
		lightOn[index] = true;
		lightType[0] = AMBIENT_LIGHT_TYPE;
		setEqual(lightColor[AMBIENT_LIGHT][index], lightColorIndex[GRAY_COLOR], 4);
		setEqual(lightColor[DIFFUSE_LIGHT][index], lightColorIndex[BLACK_COLOR], 4);
		setEqual(lightColor[SPECULAR_LIGHT][index], lightColorIndex[BLACK_COLOR], 4);
		lightAttenuation[index][0][0] = 1.0f;
		lightAttenuation[index][1][0] = 0.0f;
		lightAttenuation[index][2][0] = 0.0f;
		setEqual(plightDirection[index], type4, 3);
		plightCutoff[index][0] = 180.0f;
		plightExponent[index][0] = 0.0f;
		break;
	}
	case POINT_LIGHT_TYPE:
	{
		GLfloat type4[4] = { -1.0f, -1.0f, -1.0f };
		lightPos[index][0] = 5.0f, lightPos[index][1] = 5.0f, lightPos[index][2] = 5.0f, lightPos[index][3] = 1.0f;
		lightOn[index] = true;
		lightType[0] = POINT_LIGHT_TYPE;
		setEqual(lightColor[AMBIENT_LIGHT][index], lightColorIndex[GRAY_COLOR], 4);
		setEqual(lightColor[DIFFUSE_LIGHT][index], lightColorIndex[WHITE_COLOR], 4);
		setEqual(lightColor[SPECULAR_LIGHT][index], lightColorIndex[WHITE_COLOR], 4);
		lightAttenuation[index][0][0] = 1.0f;
		lightAttenuation[index][1][0] = 0.0f;
		lightAttenuation[index][2][0] = 0.0f;
		setEqual(plightDirection[index], type4, 3);
		plightCutoff[index][0] = 180.0f;
		plightExponent[index][0] = 0.0f;
		break;
	}
	case SPOT_LIGHT_TYPE:
	{
		GLfloat type4[4] = { -1.0f, -1.0f, -1.0f };
		lightPos[index][0] = 5.0f, lightPos[index][1] = 5.0f, lightPos[index][2] = 5.0f, lightPos[index][3] = 1.0f;
		lightOn[index] = true;
		lightType[0] = SPOT_LIGHT_TYPE;
		setEqual(lightColor[AMBIENT_LIGHT][index], lightColorIndex[GRAY_COLOR], 4);
		setEqual(lightColor[DIFFUSE_LIGHT][index], lightColorIndex[WHITE_COLOR], 4);
		setEqual(lightColor[SPECULAR_LIGHT][index], lightColorIndex[WHITE_COLOR], 4);
		lightAttenuation[index][0][0] = 1.0f;
		lightAttenuation[index][1][0] = 0.0f;
		lightAttenuation[index][2][0] = 0.0f;
		setEqual(plightDirection[index], type4, 3);
		plightCutoff[index][0] = 45.0f;
		plightExponent[index][0] = 3.0f;
		break;
	}
	case DIRECTION_LIGHT_TYPE:
	{
		GLfloat type4[4] = { -1.0f, -1.0f, -1.0f };
		lightPos[index][0] = 5.0f, lightPos[index][1] = 5.0f, lightPos[index][2] = 5.0f, lightPos[index][3] = 0.0f;
		lightOn[index] = true;
		lightType[0] = DIRECTION_LIGHT_TYPE;
		setEqual(lightColor[AMBIENT_LIGHT][index], lightColorIndex[GRAY_COLOR], 4);
		setEqual(lightColor[DIFFUSE_LIGHT][index], lightColorIndex[WHITE_COLOR], 4);
		setEqual(lightColor[SPECULAR_LIGHT][index], lightColorIndex[WHITE_COLOR], 4);
		lightAttenuation[index][0][0] = 1.0f;
		lightAttenuation[index][1][0] = 0.0f;
		lightAttenuation[index][2][0] = 0.0f;
		setEqual(plightDirection[index], type4, 3);
		plightCutoff[index][0] = 180.0f;
		plightExponent[index][0] = 0.0f;
		break;
		break;
	}
	}
	UpDateLight(index);
}
inline void Light::setEqual(GLfloat* a, GLfloat* b, int c)
{
	for (int i = 0; i < c; i++)
		a[i] = b[i];
}