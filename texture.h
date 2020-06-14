#pragma once
#include "pch.h"

#define BITMAP_ID 0x4D42
#define Height 16
#define Width 16
GLubyte image[Height][Width][3];// Í¼ÏñÊý¾Ý
GLuint texture[4];


// ¶ÁÎÆÀíÍ¼Æ¬
unsigned char* loadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
void texLoad(int i, const char* filename);
void initTexture();
