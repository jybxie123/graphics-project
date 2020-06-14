#include "model.h"
#include "texture.h"

extern GLuint texture[4];

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
	glBindTexture(GL_TEXTURE_2D, texture[this->texture_state - 1]);//选择纹理texture[status]
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

void drawobj(object* paim, vertex* posi, float scale[], float rotate, float color[])
{
	glPushMatrix();
	unsigned int i;
	int j;
	//glBegin(GL_TRIANGLES);
	glTranslatef(posi->x, posi->y, posi->z);
	glScalef(scale[0], scale[1], scale[2]);
	glRotatef(rotate, 1, 0, 0);
	glColor3f(color[0], color[1], color[2]);
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
	int i, vcount, fcount;
	vcount = obj->verts.size();
	fcount = obj->tris.size();
	for (i = 0; i < vcount; i++)
	{
		out << "v " << obj->verts[i]->x << " " << obj->verts[i]->y << " " << obj->verts[i]->z << endl;
	}
	for (i = 0; i < fcount; i++)
	{
		out << "f " << obj->tris[i]->index[0] + 1 << " " << obj->tris[i]->index[1] + 1 << " " << obj->tris[i]->index[2] + 1 << endl;
	}

}



