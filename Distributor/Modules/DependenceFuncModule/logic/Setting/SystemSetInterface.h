#pragma once

class SystemSetInterface
{
public:
	virtual void SetCheck(wstring name,bool flag) = 0;		//�����Ƿ񿪻�����
	virtual void SetRadio(wstring name) = 0;	//����radioѡ��

	virtual bool GetCheck(wstring name) = 0;		//��ȡ�Ƿ񿪻�����
	virtual bool GetRadio( wstring name ) = 0;		//��ȡradioѡ��
};
