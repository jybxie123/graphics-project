#pragma once
#include <string>
#include <iostream>

//为了方便首先采用中文注释
//上交实验报告时采取英文注释
class ScreenShot
{
public:
	ScreenShot();
	~ScreenShot();

	//开始截图，采取默认命名
	void BeginScreenShot();
	//开始截图，给定文件名
	void BeginScreenShot(std::string);
	//获得当前截图个数
	int GetNumOfFile() const;
	//设置截图序号
	void SetNumOfFile(int);

private:
	void BeginScreenShot_(std::string);

private:
	int numOfFile;
};