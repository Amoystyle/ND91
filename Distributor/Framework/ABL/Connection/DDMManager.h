#pragma once

#include "PhoneCenterOut.h"

typedef int (*FunPhoneCenter_Start)(EnumConnectMethod connectMethod,EnumConnectSuccessSign suceessSign, EventChanged _event,void *pOwn);
typedef int (*FunPhoneCenter_Connect)(void* key);
typedef int (*FunPhoneCenter_Exit)();

class ConnectionManager;
class DDMManager
	: public BaseCSLock
{
public:
	DDMManager(ConnectionManager* mgr);
	~DDMManager();

	//************************************
	// Method:    Init
	// FullName:  DDMManager::Init
	// Access:    public 
	// Returns:   bool
	// Qualifier: 初始化PhoneCenter，并开启设备扫描
	//************************************
	bool Init();

	//************************************
	// Method:    ReConnect
	// FullName:  DDMManager::ReConnect
	// Access:    public 
	// Returns:   void
	// Qualifier: 连接指定的设备
	// Parameter: void * key
	//************************************
	void ReConnect(void* key);

	//************************************
	// Method:    Release
	// FullName:  DDMManager::Release
	// Access:    public 
	// Returns:   void
	// Qualifier: 退出PhoneCenter
	//************************************
	void Release();

private:
	//************************************
	// Method:    ReloadLib
	// FullName:  DDMManager::ReloadLib
	// Access:    public 
	// Returns:   bool
	// Qualifier: 重新加载PhoneCenter
	//************************************
	bool ReloadLib();

	//************************************
	// Method:    LoadPhoneCenter
	// FullName:  DDMManager::LoadPhoneCenter
	// Access:    public 
	// Returns:   bool
	// Qualifier: 加载PhoneCenter.dll， 并初始化相关接口
	//************************************
	bool LoadPhoneCenter();

	//************************************
	// Method:    FreePhoneCenter
	// FullName:  DDMManager::FreePhoneCenter
	// Access:    public 
	// Returns:   bool
	// Qualifier: 释放资源
	//************************************
	bool FreePhoneCenter();

	//************************************
	// Method:    PhoneCenterCallBack
	// FullName:  DDMManager::PhoneCenterCallBack
	// Access:    private static 
	// Returns:   void
	// Qualifier: PhoneCenter回调
	// Parameter: EnumEvent eve
	// Parameter: void * pParam
	// Parameter: void * pOwn
	//************************************
	static void PhoneCenterCallBack(EnumEvent eve, void* pParam, void*pOwn);
private:
	ConnectionManager* _pMrg;

	HINSTANCE _hPhoneCenterLibrary;

	FunPhoneCenter_Start _pFunPhoneCenter_Start;
	FunPhoneCenter_Connect _pFunPhoneCenter_Connect;
	FunPhoneCenter_Exit _pFunPhoneCenter_Exit;
};