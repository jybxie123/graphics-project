#include <cmath>
#include "roaming.h"
#include "MyException.h"

//need test
const GLfloat roaming::MAX_SPEED = 0.5f;
const GLfloat roaming::MIN_SPEED = 0.5f;
const GLfloat roaming::MAX_SENSITIVITY = 1.0f;
const GLfloat roaming::MIN_SENSITIVITY = 1.0f;
const GLfloat roaming::DEFAULT_FOVY = 60.0f;
const GLfloat roaming::MAX_SCROLL_SENSITIVITY = 1.0f;
const GLfloat roaming::MIN_SCROLL_SENSITIVITY = 1.0f;
const GLfloat roaming::MAX_ZOOM = 2.0f;
const GLfloat roaming::MIN_ZOOM = 0.5f;
const GLfloat roaming::MAX_PITCH = 89.99f;
const GLfloat roaming::MIN_PITCH = -89.99f;
const GLfloat roaming::TOTAL_YAW = 360.0f;

roaming::roaming()
{
	//eye: x, y, z
	eye[0] = 0.0f, eye[1] = 0.0f, eye[2] = 2.0f;
	//forward: the vector of sight
	forward[0] = 0.0f, eye[1] = 0.0f, eye[2] = -1.0f;
	//up: the upward vector
	up[0] = 0.0f, up[1] = 1.0f, up[2] = 0.0f;
	//side forward x up
	side[0] = 1.0f, side[1] = 0.0f, side[2] = 0.0f;
	//speed, sensitivity, zoomRate, scrollSensitivity
	speed = 0.05f;
	sensitivity = 1.0f;
	zoomRate = 1.0f;
	scrollSensitivity = 1.0f;
	//pitch, yaw
	pitch = 0.0f;
	yaw = 0.0f;
	//ifInWindow
	ifInWindow = true;
}
roaming::~roaming()
{

}
void roaming::HandleKeyPress(ROAMING_PAN_MOVEMENT panType)
{
	switch (panType)
	{
	case PAN_UP ://space
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
	}

	UpdateLookAt();
}
void roaming::HandleMouseScroll(SCROLL_STATE scrollType, GLfloat offset)
{
	switch (scrollType)
	{
	case SCROLL_UP:
	{
		zoomRate += offset * scrollSensitivity;
		break;
	}
	case SCROLL_DOWN:
	{
		zoomRate -= offset * scrollSensitivity;
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

	UpdatePerspect();
}
void roaming::HandleMouseMove(GLfloat xoffset, GLfloat yoffset)
{
	//the mouse is outside the window
	if (!ifInWindow)
		return;

	xoffset *= sensitivity;
	yoffset *= sensitivity;
	pitch += yoffset;
	yaw += xoffset;

	if (pitch > MAX_PITCH)
		pitch = MAX_PITCH;
	else if (pitch < MIN_PITCH)
		pitch = MIN_PITCH;
	while (yaw < 0.0f)
		yaw += TOTAL_YAW;
	while (yaw > TOTAL_YAW)
		yaw -= TOTAL_YAW;

	//update forward
	forward[0] = -1 * sin(yaw) * cos(pitch);
	forward[1] = sin(pitch);
	forward[2] = -1 * cos(yaw) * cos(pitch);
	//the vector has been normalized
	//update side
	side[0] = cos(yaw);
	side[1] = 0.0f;
	side[2] = -sin(yaw);
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
void roaming::SetSensitivity(GLfloat senIn)
{
	try
	{
		if (senIn < MIN_SENSITIVITY || senIn > MAX_SENSITIVITY)
			throw EXC_SENSITIVITY_OUT_OF_RANGE;
	}
	catch (MyException exc)
	{
		throw exc;
	}
	sensitivity = senIn;
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
	gluLookAt(eye[0], eye[1], eye[2],
			  eye[0] + forward[0], eye[1] + forward[1], eye[2] + forward[2],
			  up[0], up[1], up[2]);
}
void roaming::UpdatePerspect()
{
	GLint wwidth = glutGet(GLUT_WINDOW_WIDTH), wheight = glutGet(GLUT_WINDOW_HEIGHT);
	if (wheight == 0) wheight = 1;
	//handle for exception
	gluPerspective(DEFAULT_FOVY*zoomRate, (GLdouble)wwidth/(GLdouble)(wheight), 0.1f, 300.0f);
}