#pragma once
#include "SystemSetInterface.h"

#define ContactPerson _T("ContactPerson")

class SystemSetModel
{
public:
	SystemSetModel(SystemSetInterface *pInterface);
	~SystemSetModel();

	bool Init();//��ʼ��

	void SaveSetting();//��������

	void Start(bool flag);	//��������	true ����   false ������

private:
	SystemSetInterface *_pInterface;

};
