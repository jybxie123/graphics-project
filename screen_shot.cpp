#include <string>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include "glut.h"
#include <gl/glext.h>
#include "screen_shot.h"
#include "MyException.h"

ScreenShot::ScreenShot():numOfFile(0)
{
}
ScreenShot::~ScreenShot()
{
}
void ScreenShot::BeginScreenShot()
{
	std::string fileName("ScreenShot(");
	std::string numberPart;

	std::stringstream ss;
	ss << ++numOfFile;
	ss >> numberPart;

	fileName += numberPart;
	fileName += ')';

	try
	{
		BeginScreenShot_(fileName);
	}
	catch(MyException exc)
	{
		throw exc;
	}
}
void ScreenShot::BeginScreenShot(std::string fileName)
{
	try
	{
		BeginScreenShot_(fileName);
	}
	catch (MyException exc)
	{
		throw exc;
	}
}
int ScreenShot::GetNumOfFile() const
{
	return numOfFile;
}
void ScreenShot::SetNumOfFile(int numIn)
{
	MyException exc = EXC_WRONG_NUMOFFILE;
	
	try
	{
		if (numIn <= 0)
			throw exc;
	}
	catch (MyException exc)
	{
		throw exc;
	}

	numOfFile = numIn;
}
void ScreenShot::BeginScreenShot_(std::string fileName)
{
	//fix the filename and open the bmp file
	std::ofstream writeBMP;
	fileName += ".bmp";
	writeBMP.open(fileName.c_str(),std::ios::binary);
	//the file header and infoheader
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	//the width and height of the screen
	GLint windowHeight, windowWidth;
	windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	//test if the screen does not exist
	try
	{
		if (windowHeight == 0 || windowWidth == 0)
			throw EXC_NO_WINDOW_SHOT;
	}
	catch(MyException exc)
	{
		throw exc;
	}
	//mod 4
	int len = ((windowWidth * 3 - 1) / 4 + 1) * 4;
	len *= windowHeight;

	bitmapFileHeader.bfType = (WORD)0x4d42;
	//BM
	bitmapFileHeader.bfSize = (DWORD)(54+len);
	//No affects
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	bitmapFileHeader.bfOffBits = 0x036;
	//54 bytes offset

	bitmapInfoHeader.biSize = (DWORD)0x00000028;
	//size of infoheader
	bitmapInfoHeader.biWidth = (LONG)windowWidth;
	bitmapInfoHeader.biHeight = (LONG)windowHeight;
	bitmapInfoHeader.biPlanes = (WORD)1;
	bitmapInfoHeader.biBitCount = (WORD)0x0018;
	bitmapInfoHeader.biCompression = 0;
	bitmapInfoHeader.biSizeImage = len;
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;

	GLubyte* pData = new GLubyte[len];
	//set new space for data
	//set alignment method: 4
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//read data
	glReadPixels(0, 0, windowWidth, windowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pData);
	//write file
	//writeBMP.write((const char*)&bitmapFileHeader, sizeof(bitmapFileHeader));
	writeBMP.write((const char*)&bitmapFileHeader, sizeof(bitmapFileHeader));
	writeBMP.write((const char*)&bitmapInfoHeader, sizeof(bitmapInfoHeader));
	writeBMP.write((const char*)pData, len);

	delete[] pData;
	writeBMP.close();
}
