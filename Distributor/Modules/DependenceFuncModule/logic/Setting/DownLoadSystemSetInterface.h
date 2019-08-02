#pragma once

class DownLoadSystemSetInterface
{
public:
	virtual wstring GetIosDir() = 0;					//��ȡiosĿ¼
	virtual void SetIosDir(wstring dir) = 0;			//����iosĿ¼

	virtual wstring GetAndroidDir() = 0;				//��ȡAndroidĿ¼
	virtual void SetAndroidDir(wstring dir) = 0;		//����AndroidĿ¼

	virtual bool GetCheck(wstring name) = 0;			//��ȡcheckbox����
	virtual void SetCheck(wstring name,bool flag) = 0;	//����checkbox����
};
