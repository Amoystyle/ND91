#pragma once
#include "DownLoadSystemSetInterface.h"


class DownLoadSystemSetModel
{
public:
	DownLoadSystemSetModel(DownLoadSystemSetInterface *pInterface);
	~DownLoadSystemSetModel();

	bool Init();//��ʼ��

	void SaveDownLoadPathSetting();//��������·������

	void SaveSetting();	//�����������ý�����������

private:
	DownLoadSystemSetInterface *_pInterface;

};
