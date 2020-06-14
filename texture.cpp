#include "texture.h"

unsigned char* loadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader)
{
	FILE* filePtr;// 文件指针
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
void texLoad(int i, const char* filename)
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