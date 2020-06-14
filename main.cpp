#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include "glut.h"
#include <gl/glext.h>
#include <vector>
#include <assert.h>
#include <fstream>
using namespace std;

#define BITMAP_ID 0x4D42
#define Height 16
#define Width 16
float pai = 3.1415926535;
GLubyte image[Height][Width][3];// 图像数据
GLuint texture[4];

float DegreesToRadians(float the_degree)
{
	float result;
	result = the_degree / 180 * pai;
	return result;
}

class vertex {
public:
	float x;
	float y;
	float z;
public:
	vertex(float xi, float yi, float zi) {
		this->x = xi;
		this->y = yi;
		this->z = zi;
	}
	vertex(vertex* aim)
	{
		this->x = aim->x;
		this->y = aim->y;
		this->z = aim->z;
	}
	vertex()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	~vertex() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	float length()  //用于表示向量时计算向量的长度
	{
		return sqrt(x * x + y * y + z * z);
	}
	void set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void normalize()  //用于表示向量时将向量标准化
	{
		this->x = this->x / length();
		this->y = this->y / length();
		this->z = this->z / length();
	}
	vertex operator+(const vertex v)  //表示向量时的向量加法
	{
		vertex newv;
		newv.x = this->x + v.x;
		newv.y = this->y + v.y;
		newv.z = this->z + v.z;
		return newv;
	}
	vertex operator*(float f)  //表示向量时的向量数乘
	{
		vertex newv;
		newv.x = this->x * f;
		newv.y = this->y * f;
		newv.z = this->z * f;
		return newv;
	}
};

class voxel {
public:
	string name;//可以用于个性化地描述这个体素到底是什么。
	int type;//用来记录体素的类型，如立方体，球等等。
	float scale[3];//记录放缩变换
	float rotate;//记录旋转变换
	int texture_state;//可以用于记录使用了什么texture，甚至可以直接得到该物体的材质。为0则说明没有texture。

	//leave texture message here.
public:
	voxel()
	{
		this->type = 0;// unknown type;
		this->name = "no_description";
		this->scale[0] = 0;
		this->scale[1] = 0;
		this->scale[2] = 0;
		this->rotate = 0;
		this->texture_state = 0;
	}
	voxel(string name, float scale_param[], int texture, int rotate)
	{
		int i;
		this->type = 0;
		this->name = name;
		for (i = 0; i < 3; i++)
			this->scale[i] = scale_param[i];
		this->rotate = rotate;
		this->texture_state = texture;
	}
	~voxel()
	{
		int i;
		this->name = "null";
		for (i = 0; i < 3; i++)
			this->scale[i] = 0;
		this->texture_state = 0;
	}
	virtual bool ifcrash(float, float, float) = 0;
};

voxel* positionlist[50];  //总物品列表，记录所有物体的信息，用于碰撞检测时遍历检测，物体数最大50，可改动
int nownum = 0; //下一个加入物品列表的物品序号

class triangle :public voxel
{
public:
	int index[3];
	vertex* vertices[3];
public:
	triangle(string name, float scale_param[], int texture, float rotate, vertex* v[]) :voxel(name, scale_param, texture, rotate)
	{
		this->type = 10;//10是三角形
		int i;
		for(i = 0;i<3;i++)
			vertices[i] = new vertex(v[i]);
	}
	triangle() :voxel()
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			vertices[i] = new vertex();
			index[i] = 0;
		}
			
	}
	/*~triangle()
	{
		int i;
		for (i = 0; i < 3; i++)
			delete vertices[i];
	}*/
	void draw_triangle()
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(vertices[0]->x, vertices[0]->y, vertices[0]->z);
		glVertex3f(vertices[1]->x, vertices[1]->y, vertices[1]->z);
		glVertex3f(vertices[2]->x, vertices[2]->y, vertices[2]->z);
		glEnd();
	}
	bool ifcrash(float camx, float camy, float camz)
	{
		return false;
	}
};

class cubic :public voxel
{
public:
	vertex* cubic_center;
	cubic(string name, float scale_param[], int texture, float rotate, vertex* posi) :voxel(name, scale_param, texture, rotate)
	{
		int i;
		this->type = 1;//type == 1,说明是立方体
		/*
		this->name = name;
		for (i = 0; i < 3; i++)
			this->scale[i] = scale_param[i];
		this->texture_state = texture;*/
		this->cubic_center = new vertex(posi);//为什么不直接指向的原因，如果两个体，调用同一个点，当两个物体都被析构的时候，这个点会被delete两次，就会出问题。
	}
	~cubic()//不能显式地调用父类的析构函数
	{
		delete this->cubic_center;
	}
	void drawcubic();
	void drawcubic_tex();
	bool ifcrash(float camx, float camy, float camz)
	{
		if (camx< this->cubic_center->x + this->scale[0] / 2 && camx > this->cubic_center->x - this->scale[0] / 2
			&& camy< this->cubic_center->y + this->scale[1] / 2 && camy > this->cubic_center->y - this->scale[1] / 2
			&& camz< this->cubic_center->z + this->scale[2] / 2 && camz > this->cubic_center->z - this->scale[2] / 2)
		{
			return true;
		}
		else return false;
	}
};

class sphere :public voxel
{
public:
	vertex* center;
	float radius;
	sphere(string name, int type, float scale_param[], int texture, float rotate, vertex* center_posi, float r) : voxel(name, scale_param, texture, rotate)
	{
		this->type = 2;
		this->center = new vertex(center_posi);
		this->radius = r;
	}
	~sphere()
	{
		delete this->center;
	}
	void drawsphere()
	{
		positionlist[nownum] = this;
		nownum++;
		glPushMatrix();
		glTranslatef(this->center->x, this->center->y, this->center->z);
		glScalef(this->scale[0], this->scale[1], this->scale[2]);
		glutSolidSphere(this->radius, 90, 90);
		glPopMatrix();
	}
	bool ifcrash(float camx, float camy, float camz)
	{
		if ((camx - center->x) * (camx - center->x)
			+ (camy - center->y) * (camy - center->y)
			+ (camz - center->z) * (camz - center->z) < radius * radius)
		{
			return true;
		}
		else return false;
	}
};


class cylinder :public voxel
{
public:
	vertex* bottom_center;  //顶圆圆心
	vertex* top_center;  //底圆圆心
	float radius;   //圆半径
	int edges;   //几棱柱
	cylinder(string name, float scale_param[], int texture, float rotate, vertex* aim1, vertex* aim2, float r, int e) :voxel(name, scale_param, texture, rotate)
	{
		this->type = 3;
		this->bottom_center = new vertex(aim1);
		this->top_center = new vertex(aim2);
		this->radius = r;
		this->edges = e;
		if (e < 3) e = 3;
	}
	~cylinder()
	{
		delete this->bottom_center;
		delete this->top_center;
	}
	void draw()
	{
		static GLUquadricObj* quad_obj = NULL;
		if (quad_obj == NULL)quad_obj = gluNewQuadric();
		glPushMatrix();
		gluCylinder(quad_obj, 1.0, 1.0, 2.0, 12, 3);
		glPopMatrix();
	}
	bool iscrash(float camx, float camy, float camz)
	{
		vertex dir(top_center->x - bottom_center->x, top_center->y - bottom_center->y, top_center->z - bottom_center->z);  //轴方向
		dir.normalize();
		vertex bot_check(camx - bottom_center->x, camy - bottom_center->y, camz - bottom_center->z);
		vertex top_check(camx - top_center->x, camy - top_center->y, camz - top_center->z);
		float inbot, intop;
		inbot = bot_check.x * dir.x + bot_check.y * dir.y + bot_check.z * dir.z;
		intop = top_check.x * dir.x + top_check.y * dir.y + top_check.z * dir.z;
		if ((inbot < 0 && intop < 0) || (inbot > 0 && intop > 0))  //相机在圆柱两个面之外
		{
			return false;
		}
		else
		{
			vertex mul(dir.y * bot_check.z - dir.z * bot_check.y,
				dir.z * bot_check.x - dir.x * bot_check.z,
				dir.x * bot_check.y - dir.y * bot_check.z); //叉积
			if (mul.length() < radius) return true;  //相机到轴线距离小于半径
			else return false;
		}
	}
};

class prism :public voxel
{
public:
	vertex* bottom_center;
	vertex* top_center;
	float radius;
	int edges;   //几棱锥
	prism(string name, float scale_param[], int texture, float rotate, vertex* aim1, vertex* aim2, float r, int e) :voxel(name, scale_param, texture, rotate)
	{
		this->type = 4;
		this->bottom_center = new vertex(aim1);
		this->top_center = new vertex(aim2);
		this->radius = r;
		this->edges = e;
		if (e < 3) e = 3;
	}
	~prism()
	{
		delete this->bottom_center;
		delete this->top_center;
	}

	void draw()
	{
		positionlist[nownum] = this;
		nownum++;
		float phi;  //当前角度
		vertex v3, v4;
		vertex thex(0, 0, 0), they(0, 0, 0);  //圆平面上的两个正交单位向量
		vertex dir(top_center->x - bottom_center->x, top_center->y - bottom_center->y, top_center->z - bottom_center->z);  //轴方向
		if (abs(dir.x) < 0.001 && (abs(dir.y) < 0.001))  //轴向为z轴方向
		{
			thex.set(1, 0, 0); they.set(0, 1, 0);
		}
		else if (abs(dir.z) < 0.001 && (abs(dir.y) < 0.001))  //轴向为x轴方向
		{
			thex.set(0, 1, 0); they.set(0, 0, 1);
		}
		else if (abs(dir.z) < 0.001 && (abs(dir.x) < 0.001))  //轴向为y轴方向
		{
			thex.set(1, 0, 0); they.set(0, 0, 1);
		}
		else if (abs(dir.z) < 0.001) //轴向在xy平面
		{
			they.set(0, 0, 1);
			thex.set(dir.y, -dir.x, 0);
			thex.normalize();
		}
		else if (abs(dir.x) < 0.001) //轴向在yz平面
		{
			they.set(1, 0, 0);
			thex.set(0, -dir.z, dir.y);
			thex.normalize();
		}
		else if (abs(dir.y) < 0.001) //轴向在xz平面
		{
			they.set(0, 1, 0);
			thex.set(dir.z, 0, -dir.x);
			thex.normalize();
		}
		else
		{
			thex.set(-dir.z * dir.z / dir.x, -dir.z * dir.z / dir.y, dir.z);
			thex.normalize();
			they.set(1, (dir.x * thex.z - thex.x * dir.z) / (thex.y * dir.z - thex.z * dir.y)
				, (dir.x * thex.y - dir.y * thex.x) / (dir.y * thex.z - dir.z * thex.y));
			they.normalize();
		}

		for (phi = DegreesToRadians(0); phi <= DegreesToRadians(360 - 360.0 / edges); phi += DegreesToRadians(360.0 / edges))  //画一个edges条棱的正棱锥
		{
			v3 = *bottom_center + thex * sin(phi) + they * cos(phi);
			v4 = *bottom_center + thex * sin(phi + DegreesToRadians(360.0 / edges)) + they * cos(phi + DegreesToRadians(360.0 / edges));

			glShadeModel(GL_SMOOTH);
			glBegin(GL_TRIANGLES);

			glVertex3f(bottom_center->x, bottom_center->y, bottom_center->z);
			glVertex3f(v3.x, v3.y, v3.z);
			glVertex3f(v4.x, v4.y, v4.z);
			glEnd();
		}
	}

	bool iscrash(float camx, float camy, float camz)
	{
		vertex dir(top_center->x - bottom_center->x, top_center->y - bottom_center->y, top_center->z - bottom_center->z);  //轴方向
		dir.normalize();
		vertex bot_check(camx - bottom_center->x, camy - bottom_center->y, camz - bottom_center->z);
		vertex top_check(camx - top_center->x, camy - top_center->y, camz - top_center->z);
		float inbot, intop;
		inbot = bot_check.x * dir.x + bot_check.y * dir.y + bot_check.z * dir.z;
		intop = top_check.x * dir.x + top_check.y * dir.y + top_check.z * dir.z;
		if ((inbot < 0 && intop < 0) || (inbot > 0 && intop > 0))  //在圆柱两个面之外
		{
			return false;
		}
		else
		{
			vertex mul(dir.y * bot_check.z - dir.z * bot_check.y,
				dir.z * bot_check.x - dir.x * bot_check.z,
				dir.x * bot_check.y - dir.y * bot_check.z); //叉积
			if (mul.length() < radius) return true;
			else return false;
		}
	}
};



class object
{
public:
	vector <vertex*> verts;
	vector <triangle*> tris;
	object()
	{
		verts = vector<vertex*>(0);
		tris = vector<triangle*>(0);
	}
	object(int vcount, int fcount)
	{
		int i;
		for (i = 0; i < vcount; i++)
		{
			vertex* newv = new vertex();
			this->verts.push_back(newv);
		}
		for (i = 0; i < fcount; i++)
		{
			triangle* newt = new triangle();
			this->tris.push_back(newt);
		}
	}
	~object()
	{
		int i;
		for (i = 0; i < this->verts.size(); i++)delete this->verts[i];
		for (i = 0; i < this->tris.size(); i++)delete this->tris[i];
		for (i = 0; i < this->verts.size(); i++)this->verts.pop_back();
		for (i = 0; i < this->tris.size(); i++)this->tris.pop_back();
	}
	object* operator+(object* addend)
	{
		object* addition = new object(this->verts.size() + addend->verts.size(), this->tris.size() + addend->tris.size());
		int i;
		for (i = 0; i < addition->verts.size(); i++)
		{

		}
		for (i = 0; i < addition->tris.size(); i++)
		{

		}
	}
};
// 纹理标示符数组，保存两个纹理的标示符
// 描述: 通过指针，返回filename 指定的bitmap文件中数据。
// 同时也返回bitmap信息头.（不支持-bit位图）


float fRotate;//可以用于视角转动
bool bWire = false;//线框模式

int wHeight = 0;
int wWidth = 0;

// 读纹理图片
unsigned char *loadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;// bitmap文件头
	unsigned char* bitmapImage;// bitmap图像数据
	int imageIdx = 0;// 图像位置索引
	unsigned char tempRGB;// 交换变量

	// 以“二进制+读”模式打开文件filename 
	// 需要用到fopen_s
	errno_t err;
	err = fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL) {
		printf("file not open\n");
		return NULL;
	}
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}
	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	// 由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}

// 加载纹理的函数
void texLoad(int i, const char * filename)
{
	unsigned char* bitmapData;// 纹理数据
	BITMAPINFOHEADER bitmapInfoHeader;// bitmap信息头

	if (filename == "Spot.bmp") {
		BITMAPINFOHEADER bitmapInfoHeader1;// bitmap信息头
		unsigned char* bitmapData1;
		unsigned char* bitmapData2;
		int imageIdx = 0;
		bitmapData1 = loadBitmapFile(filename, &bitmapInfoHeader);
		bitmapData2 = loadBitmapFile("Crack.bmp", &bitmapInfoHeader1);

		bitmapData = new unsigned char[bitmapInfoHeader.biSizeImage];
		for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx++) {
			bitmapData[imageIdx] = bitmapData1[imageIdx] * 0.5 + bitmapData2[imageIdx] * 0.5;
		}
	}
	else {
		bitmapData = loadBitmapFile(filename, &bitmapInfoHeader);
	}

	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0,//mipmap层次(通常为，表示最上层) 
		GL_RGB,//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData);//bitmap数据指针  
}

// 定义纹理的函数
void initTexture()
{
	glGenTextures(4, texture); // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
	texLoad(0, "Monet.bmp");
	texLoad(1, "Sand.bmp");
	texLoad(2, "Spot.bmp");

	// 下面生成自定义纹理

	// 生成红黑相间的图像
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			int x;
			if ((i < 4 || (i >= 8 && i < 12)) && (j < 4 || (j >= 8 && j < 12)) || (i >= 4 && i < 8 || (i >= 12 && i < 16)) && (j >= 4 && j < 8 || (j >= 12 && j < 16))) x = 0;
			else x = 255;
			image[i][j][0] = (GLubyte)x;
			image[i][j][1] = 0;
			image[i][j][2] = 0;
		}
	}
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //设置像素存储模式控制所读取的图像数据的行对齐方式.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大过滤，线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小过滤，线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S方向重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//T方向重复
}

void cubic::drawcubic()
{
	glPushMatrix();
	glTranslatef(this->cubic_center->x, this->cubic_center->y, this->cubic_center->z);
	glScalef(this->scale[0], this->scale[1], this->scale[2]);
	glRotatef(this->rotate, 0, 0, 1);
	glutSolidCube(1.0);
	glPopMatrix();
}

void cubic::drawcubic_tex() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[this->texture_state-1]);//选择纹理texture[status]
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//设置纹理不受光照影响
	glPushMatrix();
	vertex* x[8];
	x[0] = new vertex(this->cubic_center->x + this->scale[0] / 2, this->cubic_center->y + this->scale[1] / 2, this->cubic_center->z + this->scale[2] / 2);
	x[1] = new vertex(this->cubic_center->x + this->scale[0] / 2, this->cubic_center->y + this->scale[1] / 2, this->cubic_center->z - this->scale[2] / 2);
	x[2] = new vertex(this->cubic_center->x + this->scale[0] / 2, this->cubic_center->y - this->scale[1] / 2, this->cubic_center->z + this->scale[2] / 2);
	x[3] = new vertex(this->cubic_center->x + this->scale[0] / 2, this->cubic_center->y - this->scale[1] / 2, this->cubic_center->z - this->scale[2] / 2);
	x[4] = new vertex(this->cubic_center->x - this->scale[0] / 2, this->cubic_center->y + this->scale[1] / 2, this->cubic_center->z + this->scale[2] / 2);
	x[5] = new vertex(this->cubic_center->x - this->scale[0] / 2, this->cubic_center->y + this->scale[1] / 2, this->cubic_center->z - this->scale[2] / 2);
	x[6] = new vertex(this->cubic_center->x - this->scale[0] / 2, this->cubic_center->y - this->scale[1] / 2, this->cubic_center->z + this->scale[2] / 2);
	x[7] = new vertex(this->cubic_center->x - this->scale[0] / 2, this->cubic_center->y - this->scale[1] / 2, this->cubic_center->z - this->scale[2] / 2);

	GLfloat vertexs[8][3] = { // 长方体的八个顶点
		{ x[0]->x, x[0]->y, x[0]->z },
		{ x[1]->x, x[1]->y, x[1]->z },
		{ x[5]->x, x[5]->y, x[5]->z },
		{ x[4]->x, x[4]->y, x[4]->z },
		{ x[2]->x, x[2]->y, x[2]->z },
		{ x[3]->x, x[3]->y, x[3]->z },
		{ x[7]->x, x[7]->y, x[7]->z },
		{ x[6]->x, x[6]->y, x[6]->z }
	};

	GLint flat[6][4] = { // 长方体六个面的四个顶点
		{ 0, 1, 2, 3 },
		{ 4, 5, 6, 7 },
		{ 0, 1, 5, 4 },
		{ 2, 3, 7, 6 },
		{ 3, 0, 4, 7 },
		{ 1, 2, 6, 5 }
	};

	GLint textcoor[4][2] = {
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },
		{ 0, 1 }
	};

	glTranslatef(this->cubic_center->x, this->cubic_center->y, this->cubic_center->z);
	glScalef(this->scale[0], this->scale[1], this->scale[2]);
	glRotatef(this->rotate, 0, 0, 1);


	glBegin(GL_QUADS); // 设置正方形绘制模式
	int i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(textcoor[j]);
			glVertex3fv(vertexs[flat[i][j]]);
		}
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D); //关闭纹理texture[status2]
}

object* readobj(string filename)
{
	FILE* file = fopen(filename.c_str(), "r");
	assert(file != NULL);
	int vcount = 0; int fcount = 0;
	while (1) {
		int c = fgetc(file);
		if (c == EOF) {
			break;
		}
		else if (c == 'v') {
			assert(fcount == 0); float v0, v1, v2;
			fscanf(file, "%f %f %f", &v0, &v1, &v2);
			vcount++;
		}
		else if (c == 'f') {
			int f0, f1, f2;
			fscanf(file, "%d %d %d", &f0, &f1, &f2);
			fcount++;
		} // otherwise, must be whitespace
	}
	fclose(file);
	// make arrays
	int i;
	object* aim = new object(vcount, fcount);
	// read it again, save it
	file = fopen(filename.c_str(), "r");
	assert(file != NULL);
	int new_vcount = 0; int new_fcount = 0;
	while (1) {
		int c = fgetc(file);
		if (c == EOF) {
			break;
		}
		else if (c == 'v') {
			assert(new_fcount == 0); float v0, v1, v2;
			fscanf(file, "%f %f %f", &v0, &v1, &v2);
			aim->verts[new_vcount]->x = v0;
			aim->verts[new_vcount]->y = v1;
			aim->verts[new_vcount]->z = v2;
			new_vcount++;
		}
		else if (c == 'f') {
			assert(vcount == new_vcount);
			int f0, f1, f2;
			fscanf(file, "%d %d %d", &f0, &f1, &f2);
			// indexed starting at 1...
			assert(f0 > 0 && f0 <= vcount);
			assert(f1 > 0 && f1 <= vcount);
			assert(f2 > 0 && f2 <= vcount);
			aim->tris[new_fcount]->vertices[0] = aim->verts[f0 - 1];
			aim->tris[new_fcount]->vertices[1] = aim->verts[f1 - 1];
			aim->tris[new_fcount]->vertices[2] = aim->verts[f2 - 1];
			aim->tris[new_fcount]->index[0] = f0 - 1;
			aim->tris[new_fcount]->index[1] = f1 - 1;
			aim->tris[new_fcount]->index[2] = f2 - 1;
			//answer->addObject(new_fcount, t);
			new_fcount++;
		} // otherwise, must be whitespace
	}
	//delete verts;
	assert(fcount == new_fcount);
	assert(vcount == new_vcount);
	fclose(file);
	return aim;
}

void drawobj(object* paim, vertex* posi, float scale[], float rotate,float color[])
{
	glPushMatrix();
	unsigned int i;
	int j;
	//glBegin(GL_TRIANGLES);
	glTranslatef(posi->x, posi->y, posi->z);
	glScalef(scale[0], scale[1], scale[2]);
	glRotatef(rotate, 1, 0, 0);
	glColor3f(color[0],color[1],color[2]);
	for (i = 0; i < paim->tris.size(); i++)
	{
		paim->tris[i]->draw_triangle();
	}
	/*for (i = 0; i < face_indices.size(); i++)
	{
		for (j = 0; j < 3; j++)
		{
			int vi = face_indices[i][j];
			int ni = face_indices[i][j + 3];//Normal index
			//glNormal3fv(&normals[ni][0]);
			//glVertex3fv(&vertices[vi][0]);
			//等比例将兔子大小缩小
			GLfloat glTemp[3];
			GLfloat glScale = 2.0f;
			glTemp[0] = normals[ni][0] / glScale;
			glTemp[1] = normals[ni][1] / glScale;
			glTemp[2] = normals[ni][2] / glScale;
			glNormal3fv(&glTemp[0]);
			glTemp[0] = vertices[vi][0] / glScale;
			glTemp[1] = vertices[vi][1] / glScale;
			glTemp[2] = vertices[vi][2] / glScale;
			glVertex3fv(&glTemp[0]);
		}
	}*/
	//glEnd();
	glPopMatrix();
}

void writeobj(object* obj)
{
	ofstream out("output.obj");
	int i,vcount, fcount;
	vcount = obj->verts.size();
	fcount = obj->tris.size();
	for (i = 0; i < vcount; i++)
	{
		out<<"v "<< obj->verts[i]->x<<" " << obj->verts[i]->y <<" "<< obj->verts[i]->z <<endl;
	}
	for (i = 0; i < fcount; i++)
	{
		out << "f " << obj->tris[i]->index[0] + 1 << " " << obj->tris[i]->index[1] + 1 << " " << obj->tris[i]->index[2] + 1<<endl;
	}
	
}

GLint tableList = 0;  //用显示列表，list！！！ 
//每次只需在GenTableList时构建positionlist，否则每次都要重新记录所有物品的位置，及其慢

GLint GenTableList()
{
	int i;
	for (i = 0; i < nownum; i++)
	{
		positionlist[i] = NULL;
	}
	nownum = 0;
	//--------↑先清空场景--------------//

	GLint lid = glGenLists(1);
	glNewList(lid, GL_COMPILE);

	//---------↓在这里画所有想画的东西
	{
		vertex* posi = new vertex(2, 4, 0);
		float scale_parameter[3] = { 1,8,3 };
		cubic testc("test", scale_parameter, 2, 20, posi);
		testc.drawcubic_tex();

		vertex* posi2 = new vertex(-2, 4, 0);
		cubic testc2("test", scale_parameter, 2, 0, posi2);
		testc2.drawcubic_tex();

		vertex* posi3 = new vertex(0, 4, 0);
		float scale_parameter2[3] = { 0.2,0.2,0.2 };
		float color[3] = { 0.5,0.5,1 };
		object* aim;
		aim = readobj("diamond.obj");
		if (aim != NULL)
		{
			drawobj(aim, posi3, scale_parameter2, 0, color);
			writeobj(aim);
			//if (aim->tris[0] == NULL)cout << "miss" << endl;
		}
	}
	//---------↑在这里画所有想画的东西

	glEndList();
	return lid;
}


bool checkcrash(float camx, float camy, float camz)  //总碰撞检测函数，每次移动相机时都会调用
{
	int i;
	for (i = 0; i < nownum; i++)
	{
		if (positionlist[i]->ifcrash(camx, camy, camz))  //遍历positionlist，如果有任意一个物品碰撞，则碰撞
			return true;
	}
	return false;
}


void updateView(int width, int height)
{
	glViewport(0, 0, width, height);//设置视窗大小  

	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影   
	glLoadIdentity();//初始化矩阵为单位矩阵      

	float whRatio = (GLfloat)width / (GLfloat)height;//设置显示比例    
	gluPerspective(45.0f, whRatio, 0.1f, 100.0f); //透视投影       
	glMatrixMode(GL_MODELVIEW);//设置矩阵模式为模型  
}

void reshape(int width, int height)
{
	if (height == 0)//如果高度为0  
	{
		height = 1;//让高度为1（避免出现分母为0的现象）  
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth); //更新视角  
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = { 0, 0, 1 };
float center[] = { 0, 0, 0 };

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'o': {bWire = !bWire; break; }
	case 'l': {exit(0); break; }
	case 'a': {//todo, hint: eye[] and center[] are the keys to solve the problems
		eye[0] -= 0.05;
		center[0] -= 0.05;
		break;
	}
	case 'd': {//todo
		eye[0] += 0.1;
		center[0] += 0.1;
		break;
	}
	case 'w': {//todo
		eye[2] -= 0.1;
		center[2] -= 0.1;
		break;
	}
	case 's': {//todo
		eye[2] += 0.1;
		center[2] += 0.1;
		break;
	}
	case 'q': {//todo
		fRotate += 0.5;
		break;
	}
	case 'e': {//todo
		fRotate -= 0.5;
		break;
	}
	}

	updateView(wHeight, wWidth);
}


void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓存和深度缓存   
	glLoadIdentity();//初始化矩阵为单位矩阵    

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//设置多边形绘制模式：正反面，线型  
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//设置多边形绘制模式：正反面，填充      
	}

	glEnable(GL_DEPTH_TEST);//开启深度测试    
	glEnable(GL_LIGHTING);//开启光照模式   

	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5,5,5,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);//光源位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);//定义白色 
	glEnable(GL_LIGHT0);//开启第0号光源   
	//全景变换
	glRotatef(fRotate, 0, 1.0f, 0);//旋转
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);//缩放

	glCallList(tableList);//绘制场景

	glutSwapBuffers(); //交换缓冲区
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	initTexture();

	tableList = GenTableList();

	glutMainLoop();
	return 0;
}




