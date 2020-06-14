#pragma once
#include "pch.h"

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
		for (i = 0; i < 3; i++)
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

object* readobj(string filename);
void drawobj(object* paim, vertex* posi, float scale[], float rotate, float color[]);
void writeobj(object* obj);