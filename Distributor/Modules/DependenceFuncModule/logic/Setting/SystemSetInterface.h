#pragma once

class SystemSetInterface
{
public:
	virtual void SetCheck(wstring name,bool flag) = 0;		//设置是否开机启动
	virtual void SetRadio(wstring name) = 0;	//设置radio选择

	virtual bool GetCheck(wstring name) = 0;		//获取是否开机启动
	virtual bool GetRadio( wstring name ) = 0;		//获取radio选择
};
