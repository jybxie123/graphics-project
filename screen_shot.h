#pragma once
#include <string>
#include <iostream>

//Ϊ�˷������Ȳ�������ע��
//�Ͻ�ʵ�鱨��ʱ��ȡӢ��ע��
class ScreenShot
{
public:
	ScreenShot();
	~ScreenShot();

	//��ʼ��ͼ����ȡĬ������
	void BeginScreenShot();
	//��ʼ��ͼ�������ļ���
	void BeginScreenShot(std::string);
	//��õ�ǰ��ͼ����
	int GetNumOfFile() const;
	//���ý�ͼ���
	void SetNumOfFile(int);

private:
	void BeginScreenShot_(std::string);

private:
	int numOfFile;
};