#pragma once
#include "DownLoadSystemSetInterface.h"


class DownLoadSystemSetModel
{
public:
	DownLoadSystemSetModel(DownLoadSystemSetInterface *pInterface);
	~DownLoadSystemSetModel();

	bool Init();//初始化

	void SaveDownLoadPathSetting();//保存下载路径设置

	void SaveSetting();	//保存下载设置界面其他设置

private:
	DownLoadSystemSetInterface *_pInterface;

};
