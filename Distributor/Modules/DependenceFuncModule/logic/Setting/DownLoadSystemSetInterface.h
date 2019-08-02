#pragma once

class DownLoadSystemSetInterface
{
public:
	virtual wstring GetIosDir() = 0;					//获取ios目录
	virtual void SetIosDir(wstring dir) = 0;			//设置ios目录

	virtual wstring GetAndroidDir() = 0;				//获取Android目录
	virtual void SetAndroidDir(wstring dir) = 0;		//设置Android目录

	virtual bool GetCheck(wstring name) = 0;			//获取checkbox属性
	virtual void SetCheck(wstring name,bool flag) = 0;	//设置checkbox属性
};
