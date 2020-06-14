#include <cmath>
#include "roaming.h"
#include "MyException.h"

//need test
const GLfloat roaming::MAX_SPEED = 0.5f;
const GLfloat roaming::MIN_SPEED = 0.5f;
const GLfloat roaming::MAX_SENSITIVITY = 0.0f;
const GLfloat roaming::MIN_SENSITIVITY = 1.0f;
const GLfloat roaming::DEFAULT_FOVY = 60.0f;
const GLfloat roaming::MAX_SCROLL_SENSITIVITY = 1.0f;
const GLfloat roaming::MIN_SCROLL_SENSITIVITY = 0.0f;
const GLfloat roaming::MAX_ZOOM = 2.0f;
const GLfloat roaming::MIN_ZOOM = 0.5f;
const GLfloat roaming::MAX_PITCH = 89.99f;
const GLfloat roaming::MIN_PITCH = -89.99f;
const GLfloat roaming::TOTAL_YAW = 360.0f;
const float roaming::PI_DIV_360 = (3.1415926 + 0.1)/ 180;

roaming::roaming()
{
	//eye: x, y, z
	eye[0] = 0.0f, eye[1] = 0.0f, eye[2] = 2.0f;
	//forward: the vector of sight
	forward[0] = 0.0f, forward[1] = 0.0f, forward[2] = -1.0f;
	//up: the upward vector
	up[0] = 0.0f, up[1] = 1.0f, up[2] = 0.0f;
	//side forward x up
	side[0] = 1.0f, side[1] = 0.0f, side[2] = 0.0f;
	//speed, sensitivity, zoomRate, scrollSensitivity
	speed = 0.05f;
	sensitivity_x = 180.0f / 480.0f;
	sensitivity_y = 360.0f / 640.0f;
 	zoomRate = 1.0f;
	scrollSensitivity = 0.01f;
	//pitch, yaw
	pitch = 0.0f;
	yaw = 0.0f;
	//ifInWindow
	ifInWindow = true;
	//wwid, whei
	wwid = 640, whei = 480;
}
roaming::~roaming()
{

}
void roaming::HandleKeyPress(roaming::ROAMING_PAN_MOVEMENT panType)
{
	switch (panType)
	{
	case PAN_UP://space
	{
		eye[1] += speed * up[1];
		break;
	}
	case PAN_DOWN://shift
	{
		eye[1] -= speed * up[1];
		break;
	}
	case PAN_FORWARD://w
	{
		eye[0] += speed * forward[0];
		eye[1] += speed * forward[1];
		eye[2] += speed * forward[2];
		break;
	}
	case PAN_BACKWARD://s
	{
		eye[0] -= speed * forward[0];
		eye[1] -= speed * forward[1];
		eye[2] -= speed * forward[2];
		break;
	}
	case PAN_LEFT:
	{
		eye[0] -= speed * side[0];
		eye[1] -= speed * side[1];
		eye[2] -= speed * side[2];
		break;
	}
	case PAN_RIGHT:
	{
		eye[0] += speed * side[0];
		eye[1] += speed * side[1];
		eye[2] += speed * side[2];
		break;
	}
	default:
	{
		break;
	}
	};

	UpdateLookAt();
}
void roaming::HandleMouseScroll(SCROLL_STATE scrollType)
{
	switch (scrollType)
	{
	case SCROLL_UP:
	{
		zoomRate -= scrollSensitivity;
		break;
	}
	case SCROLL_DOWN:
	{
		zoomRate += scrollSensitivity;
		break;
	}
	case SCROLL_PUSH:
	{
		//need fix
		zoomRate = 1.0f;
		break;
	}
	}

	if (zoomRate < MIN_ZOOM)
		zoomRate = MIN_ZOOM;
	if (zoomRate > MAX_ZOOM)
		zoomRate = MAX_ZOOM;

	UpdatePerspect(wwid, whei);
}
void roaming::HandleMouseMove(GLfloat xoffset, GLfloat yoffset)
{
	//the mouse is outside the window
	if (!ifInWindow)
		return;

	xoffset = 0.5 * wwid - xoffset;
	yoffset = 0.5 * whei - yoffset;

	xoffset *= sensitivity_x * PI_DIV_360;
	yoffset *= sensitivity_y * PI_DIV_360;
	
	pitch = yoffset;
	yaw = xoffset;

	//update forward
	forward[0] = -1 * sin(yaw) * cos(pitch);
	forward[1] = sin(pitch);
	forward[2] = -1 * cos(yaw) * cos(pitch);
	//the vector has been normalized
	//update side
	side[0] = cos(yaw);
	side[1] = 0.0f;
	side[2] = -sin(yaw);

	UpdateLookAt();
}
void roaming::HandleMouseInWindow(bool ifInW)
{
	ifInWindow = ifInW;
}
void roaming::SetSpeed(GLfloat speedIn)
{
	try 
	{
		if (speedIn < MIN_SPEED || speedIn >  MAX_SPEED)
			throw EXC_SPEED_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	speed = speedIn;
}
void roaming::SetSensitivity(int index, GLfloat senIn)
{
	try
	{
		if (senIn < MIN_SENSITIVITY || senIn > MAX_SENSITIVITY)
			throw EXC_SENSITIVITY_OUT_OF_RANGE;
		if (index != 0 && index != 1)
			throw EXC_LIGHT_INDEX_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	if (index == 0)
		sensitivity_x = senIn;
	else if (index == 1)
		sensitivity_y = senIn;
}
void roaming::SetScrollSensitivity(GLfloat senIn)
{
	try
	{
		if (senIn < MIN_SCROLL_SENSITIVITY || senIn > MAX_SCROLL_SENSITIVITY)
			throw EXC_SCROLL_SENSITIVITY_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	scrollSensitivity = senIn;
}
void roaming::UpdateLookAt()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],
			  eye[0] + forward[0], eye[1] + forward[1], eye[2] + forward[2],
			  up[0], up[1], up[2]);
}
void roaming::UpdatePerspect(GLint wwidth, GLint wheight)
{
	if (wheight == 0) wheight = 1;
	wwid = wwidth; whei = wheight;

	sensitivity_x = 360.0f / wwidth;
	sensitivity_y = 180.0f / wheight;

	glViewport(0, 0, wwidth, wheight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//handle for exception
	gluPerspective(DEFAULT_FOVY*zoomRate, (GLdouble)wwidth/(GLdouble)(wheight), 0.1f, 300.0f);
	
	glMatrixMode(GL_MODELVIEW);
}
