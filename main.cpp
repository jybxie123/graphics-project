#include"pch.h"
#include "texture.h"
#include "light.h"
#include "roaming.h"
#include "screen_shot.h"
#include "model.h"

float pai = 3.1415926535;


float DegreesToRadians(float the_degree)
{
	float result;
	result = the_degree / 180 * pai;
	return result;
}


// 纹理标示符数组，保存两个纹理的标示符
// 描述: 通过指针，返回filename 指定的bitmap文件中数据。
// 同时也返回bitmap信息头.（不支持-bit位图）


float fRotate;//可以用于视角转动
bool bWire = false;//线框模式
int wHeight = 0;
int wWidth = 0;


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




