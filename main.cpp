// glutEx1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include "glut.h"
#include <gl/glext.h>
using namespace std;

#define BITMAP_ID 0x4D42
#define TEX_HEIGHT 256 
#define TEX_WIDTH 256 

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;
GLuint texture[4];
void Draw_Leg();
GLubyte tex[TEX_HEIGHT][TEX_WIDTH][3];

// �����ʾ�����飬������������ı�ʾ��
// ����: ͨ��ָ�룬����filename ָ����bitmap�ļ������ݡ�
// ͬʱҲ����bitmap��Ϣͷ.����֧��-bitλͼ��
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader)
{
	FILE* filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char* bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������

	// �ԡ�������+����ģʽ���ļ�filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];//bitmapInfoHeader->biSizeImage
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 2;
		imageIdx <= bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx - 2];
		bitmapImage[imageIdx - 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}

//��������ĺ�����
void texload(int i, char* filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap��Ϣͷ
	unsigned char* bitmapData;                                       // ��������

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapData);	//bitmap����ָ��  
}



void Draw_Triangle() // This function draws a triangle with RGB colors,with textures
{
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);  //ѡ������texture[0]
	glPushMatrix();
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);	//�ر�����texture[0]



	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);  //ѡ������texture[1]
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3i(3, -2, 4);
	glTexCoord2i(1, 0); glVertex3i(3, -2, 3);
	glTexCoord2i(0, 0); glVertex3i(-3, -2, 3);
	glTexCoord2i(0, 1); glVertex3i(-3, -2, 4);
	glTexCoord2i(1, 1); glVertex3i(3, 2, 4);
	glTexCoord2i(1, 0); glVertex3i(3, 2, 3);
	glTexCoord2i(0, 0); glVertex3i(-3, 2, 3);
	glTexCoord2i(0, 1); glVertex3i(-3, 2, 4);
	glTexCoord2i(1, 1); glVertex3i(3, -2, 4);
	glTexCoord2i(1, 0); glVertex3i(3, -2, 3);
	glTexCoord2i(0, 0); glVertex3i(3, 2, 3);
	glTexCoord2i(0, 1); glVertex3i(3, 2, 4);
	glTexCoord2i(1, 1); glVertex3i(-3, -2, 4);
	glTexCoord2i(1, 0); glVertex3i(-3, -2, 3);
	glTexCoord2i(0, 0); glVertex3i(-3, 2, 3);
	glTexCoord2i(0, 1); glVertex3i(-3, 2, 4);
	glTexCoord2i(1, 1); glVertex3i(3, -2, 4);
	glTexCoord2i(1, 0); glVertex3i(3, 2, 4);
	glTexCoord2i(0, 0); glVertex3i(-3, 2, 4);
	glTexCoord2i(0, 1); glVertex3i(-3, -2, 4);
	glEnd();
	glTranslatef(0, 0, 3.5);
	glScalef(6, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);	//�ر�����texture[1]

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);  //ѡ������texture[1]
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3i(3, 2, 3);
	glTexCoord2i(1, 0); glVertex3i(2, 2, 3);
	glTexCoord2i(0, 0); glVertex3i(2, 2, 0);
	glTexCoord2i(0, 1); glVertex3i(3, 2, 0);
	glTexCoord2i(1, 1); glVertex3i(3, 1, 3);
	glTexCoord2i(1, 0); glVertex3i(2, 1, 3);
	glTexCoord2i(0, 0); glVertex3i(2, 1, 0);
	glTexCoord2i(0, 1); glVertex3i(3, 1, 0);
	glTexCoord2i(1, 1); glVertex3i(3, 2, 3);
	glTexCoord2i(1, 0); glVertex3i(3, 1, 3);
	glTexCoord2i(0, 0); glVertex3i(3, 1, 0);
	glTexCoord2i(0, 1); glVertex3i(3, 2, 0);
	glTexCoord2i(1, 1); glVertex3i(2, 2, 3);
	glTexCoord2i(1, 0); glVertex3i(2, 1, 3);
	glTexCoord2i(0, 0); glVertex3i(2, 1, 0);
	glTexCoord2i(0, 1); glVertex3i(2, 2, 0);
	glEnd();
	glTranslatef(2.5, 1.5, 1.5);
	Draw_Leg();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);	//�ر�����texture[1]

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);  //ѡ������texture[1]
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3i(-3, 2, 3);
	glTexCoord2i(1, 0); glVertex3i(-2, 2, 3);
	glTexCoord2i(0, 0); glVertex3i(-2, 2, 0);
	glTexCoord2i(0, 1); glVertex3i(-3, 2, 0);
	glTexCoord2i(1, 1); glVertex3i(-3, 1, 3);
	glTexCoord2i(1, 0); glVertex3i(-2, 1, 3);
	glTexCoord2i(0, 0); glVertex3i(-2, 1, 0);
	glTexCoord2i(0, 1); glVertex3i(-3, 1, 0);
	glTexCoord2i(1, 1); glVertex3i(-3, 2, 3);
	glTexCoord2i(1, 0); glVertex3i(-3, 1, 3);
	glTexCoord2i(0, 0); glVertex3i(-3, 1, 0);
	glTexCoord2i(0, 1); glVertex3i(-3, 2, 0);
	glTexCoord2i(1, 1); glVertex3i(-2, 2, 3);
	glTexCoord2i(1, 0); glVertex3i(-2, 1, 3);
	glTexCoord2i(0, 0); glVertex3i(-2, 1, 0);
	glTexCoord2i(0, 1); glVertex3i(-2, 2, 0);
	glEnd();
	glTranslatef(-2.5, 1.5, 1.5);
	Draw_Leg();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);	//�ر�����texture[1]

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);  //ѡ������texture[1]
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3i(3, -2, 3);
	glTexCoord2i(1, 0); glVertex3i(2, -2, 3);
	glTexCoord2i(0, 0); glVertex3i(2, -2, 0);
	glTexCoord2i(0, 1); glVertex3i(3, -2, 0);
	glTexCoord2i(1, 1); glVertex3i(3, -1, 3);
	glTexCoord2i(1, 0); glVertex3i(2, -1, 3);
	glTexCoord2i(0, 0); glVertex3i(2, -1, 0);
	glTexCoord2i(0, 1); glVertex3i(3, -1, 0);
	glTexCoord2i(1, 1); glVertex3i(3, -2, 3);
	glTexCoord2i(1, 0); glVertex3i(3, -1, 3);
	glTexCoord2i(0, 0); glVertex3i(3, -1, 0);
	glTexCoord2i(0, 1); glVertex3i(3, -2, 0);
	glTexCoord2i(1, 1); glVertex3i(2, -2, 3);
	glTexCoord2i(1, 0); glVertex3i(2, -1, 3);
	glTexCoord2i(0, 0); glVertex3i(2, -1, 0);
	glTexCoord2i(0, 1); glVertex3i(2, -2, 0);
	glEnd();
	glTranslatef(2.5, -1.5, 1.5);
	Draw_Leg();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);	//�ر�����texture[1]

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);  //ѡ������texture[1]
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3i(-3, -2, 3);
	glTexCoord2i(1, 0); glVertex3i(-2, -2, 3);
	glTexCoord2i(0, 0); glVertex3i(-2, -2, 0);
	glTexCoord2i(0, 1); glVertex3i(-3, -2, 0);
	glTexCoord2i(1, 1); glVertex3i(-3, -1, 3);
	glTexCoord2i(1, 0); glVertex3i(-2, -1, 3);
	glTexCoord2i(0, 0); glVertex3i(-2, -1, 0);
	glTexCoord2i(0, 1); glVertex3i(-3, -1, 0);
	glTexCoord2i(1, 1); glVertex3i(-3, -2, 3);
	glTexCoord2i(1, 0); glVertex3i(-3, -1, 3);
	glTexCoord2i(0, 0); glVertex3i(-3, -1, 0);
	glTexCoord2i(0, 1); glVertex3i(-3, -2, 0);
	glTexCoord2i(1, 1); glVertex3i(-2, -2, 3);
	glTexCoord2i(1, 0); glVertex3i(-2, -1, 3);
	glTexCoord2i(0, 0); glVertex3i(-2, -1, 0);
	glTexCoord2i(0, 1); glVertex3i(-2, -2, 0);
	glEnd();
	glTranslatef(-2.5, -1.5, 1.5);
	Draw_Leg();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);	//�ر�����texture[1]

}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);
	} else {
		glOrtho(-3 ,3, -3, 3,-100,100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}

	case 'a': {
		eye[0] -= 0.2f;
		center[0] -= 0.2f;
		break;
			  }
	case 'd': {
		eye[0] += 0.2f;
		center[0] += 0.2f;
		break;
			  }
	case 'w': {
		eye[1] -= 0.2f;
		center[1] -= 0.2f;
		break;
			  }
	case 's': {
		eye[1] += 0.2f;
		center[1] += 0.2f;
		break;
			  }
	case 'z': {
		eye[2] -= 0.2f;
		center[2] -= 0.2f;
		break;
			  }
	case 'c': {
		eye[2] += 0.2f;
		center[2] += 0.2f;
		break;
			  }
	}

	updateView(wHeight, wWidth);
}

void generateTex() {
	for (int i = 0; i < TEX_HEIGHT; i++) {
		for (int j = 0; j < TEX_WIDTH; j++) {
			if ((i % 4 - 2) < 0 || (j % 4 - 2) <= 0) {
				tex[i][j][0] = 3840;
			}
			else {
				tex[i][j][0] = 0;
			}
			tex[i][j][1] = 255;
			tex[i][j][2] = 255;
		}
	}
}

// ���ø�������
//void openMultitexture() {
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture[2]);

	//glActiveTextureARB(GL_TEXTURE1_ARB);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
//}

// �رո�������
//void closeMultitexture() {
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	//glDisable(GL_TEXTURE_2D);
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glDisable(GL_TEXTURE_2D);
//}

void init()
{
	glGenTextures(4, texture);                                         // ��һ��������Ҫ���ɱ�ʾ���ĸ���, �ڶ������Ƿ��ر�ʾ��������
	texload(0, "Monet.bmp");
	texload(1, "Crack.bmp");
	texload(2, "Spot.bmp");
	//���������Զ�������
	generateTex();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	// �������ش洢ģʽ��������ȡ��ͼ�����ݵ��ж��뷽ʽ
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ���ظ�������
	//glActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	//glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	//glClientActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
}


void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// ������0��0��0�����ӵ����� (0,5,50)��Y������

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Triangle();						// Draw triangle

	if (bAnim) fRotate    += 0.2f;
	glutSwapBuffers();
}




int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple GLUT App");
	init();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 0.1);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


