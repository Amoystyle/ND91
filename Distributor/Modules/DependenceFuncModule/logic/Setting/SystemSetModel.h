#pragma once
#include "SystemSetInterface.h"

#define ContactPerson _T("ContactPerson")

class SystemSetModel
{
public:
	SystemSetModel(SystemSetInterface *pInterface);
	~SystemSetModel();

	bool Init();//初始化

	void SaveSetting();//保存设置

	void Start(bool flag);	//开机启动	true 启动   false 不启动

private:
	SystemSetInterface *_pInterface;

};
