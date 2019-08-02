#include "stdafx.h"
#include "DeviceController.h"
#include "DeviceContext.h"
#include "UI/LeftView.h"
#include "UI/ModulesView.h"
#include "UI/WelcomeView.h"
#include "Common/Device/DeviceMessage.h"

DeviceController::DeviceController()
{
	_pLeftView = new LeftView(this);
	_pModulesView = new ModulesView();
	_pWelcomeView = new WelcomeView();
	_pDeviceContext = NULL;
}

DeviceController::~DeviceController()
{	
}

bool DeviceController::Init(CContainerUI* pLeft,  CContainerUI* pContainer )
{
	_pLeftView->Init(pLeft);
	_pModulesView->Init(pContainer);
	_pWelcomeView->Init(_pModulesView);
	return true;
}

void DeviceController::Show()
{
	_pLeftView->Show();	
	_pModulesView->Show();	
}

void DeviceController::Hide()
{
	_pLeftView->Hide();
	_pModulesView->Hide();
}

bool DeviceController::Handle( const DeviceLinkInMsg* pMsg )
{
	_pWelcomeView->Hide();

	this->CreateContext(pMsg->id, pMsg->pPhone);

	this->_pLeftView->AddItem(pMsg->pPhone);


	LOG->WriteDebug(L"AddContext: count->"+ CStrOperation::IntToWString(_mapContext.size()));
	return true;
}

bool DeviceController::Handle( const DeviceLinkOutMsg* pMsg )
{	
	this->_pLeftView->RemoveItem(pMsg->pPhone);

	this->RemoveContext(pMsg->pPhone);

	size_t nSize = _mapContext.size();
	if( nSize <= 0 || pMsg->nDeviceCount <= 0)
	{
		_mapContext.clear();
		_pWelcomeView->Show();
	}

	LOG->WriteDebug(L"RemoveContext: count->"+ CStrOperation::IntToWString(nSize));
	return true;
}

bool DeviceController::Handle( const DeviceConnectMsg* pMsg )
{
	DeviceContext* pContext = _mapContext[pMsg->pPhone];
	if(pContext != NULL)
	{
		//只在当前选中时切换，防止界面乱跳
		if(_pDeviceContext == pContext)
			this->ChangeView(pContext->GetPhone(), DeviceModuleType_DeviceInfo);

		if(pMsg->bIsRecovery)
			this->_pLeftView->ClearSubItem(pMsg->pPhone, pMsg->deviceName, pMsg->bIsRecovery);

		this->_pLeftView->UpdateItem(pMsg->pPhone, pMsg->deviceName, !pMsg->bIsRecovery);	

		if(pMsg->bIsRecovery)
			this->_pLeftView->UpdateRecoveryText(pMsg->pPhone);

		this->InitModule(pMsg->pPhone, DeviceModuleType_Application);
	}
	return true;
}

bool DeviceController::Handle( const DeviceActivatedMsg* pMsg )
{	
	if(pMsg->nPhoneOs != ePhoneOS_Android)
	{
// 		GetAblDeviceInfoMsg ablMsg;
// 		ablMsg.deviceID = pMsg->deviceID;
// 		ablMsg.SendAndGetReturn();
// 		AblDeviceInfo* ablData = new AblDeviceInfo(&ablMsg.info);
// 		ablData->deviceID = pMsg->deviceID;
// 		RETURN_FALSE_IF ( !ablData );
// 
// 		CStaticsticsDevice msg;
// 		msg.pAblDeviceInfo = (void*)ablData;
// 		msg.Send();	

		return true;
	}

	DeviceContext* pContext = _mapContext[pMsg->pPhone];
	if(pContext != NULL)
	{
		if(pMsg->bActived)
		{
// 			GetAblDeviceInfoMsg ablMsg;
// 			ablMsg.deviceID = pMsg->deviceID;
// 			ablMsg.SendAndGetReturn();
// 			AblDeviceInfo* ablData = new AblDeviceInfo(&ablMsg.info);
// 			ablData->deviceID = pMsg->deviceID;
// 			RETURN_FALSE_IF ( !ablData );
// 
//  			CStaticsticsDevice msg;
// 			msg.pAblDeviceInfo = (void*)ablData;
//  			msg.Send();

			if(_pDeviceContext == pContext && 
				(_pDeviceContext->GetCurrentType() == DeviceModuleType_DeviceInfo || 
				 _pDeviceContext->GetCurrentType() == DeviceModuleType_ConnecGuide))
			{
				this->ChangeView(pContext->GetPhone(), DeviceModuleType_DeviceInfo);
			}

			this->_pLeftView->UpdateItem(pContext->GetPhone(), pMsg->deviceName, false);
		}
		else
		{
			//设备激活失败后。
			//1. 清除设备对应模块节点
			//2. 跳转到连接向导。
			//3. 通知激活失败。

			this->_pLeftView->ClearSubItem(pMsg->pPhone, pMsg->deviceName);

			pContext->RemoveDeviceModule();

			if(_pDeviceContext == pContext)
				this->ChangeView(pContext->GetPhone(), DeviceModuleType_ConnecGuide);


			DeviceConnectErrorMsg msg;
			msg.pPhone = pContext->GetPhone();
			msg.nError = DeviceConnectError_FailToActive;
			msg.SendToUI();	
		}
	}

	return true;
}

bool DeviceController::Handle( const NotifyDeviceKeyChangeMsg* pMsg )
{
	DeviceContext* pContext = _mapContext[pMsg->pPhone];
	if(pContext != NULL)
	{
		pContext->UpdateDeviceID(pMsg->newKey);
	}
	return true;
}

bool DeviceController::Handle( const DeviceInnerMsg* pMsg )
{
	switch(pMsg->type)
	{
		case Device_MsgType_TreeSelectChange:
			{
				void* key = (void*)pMsg->wParam;
				DeviceModuleType type = (DeviceModuleType)pMsg->lParam;
				this->ChangeView(key, type);
			}
			break;
		case Device_MsgType_Refresh:
			{
				this->Refresh();
			}
			break;
		default:
			break;
	}

	return true;
}

bool DeviceController::Handle( const DeviceConnectErrorMsg* pMsg )
{
	this->_pLeftView->UpdateErrorText(pMsg->pPhone);
	return true;
}

void DeviceController::ChangeView( void* key, DeviceModuleType type)
{
	if(_pDeviceContext != NULL && _pDeviceContext->GetPhone() != key)
	{
		_pDeviceContext->HideModule();
		_pDeviceContext = NULL;
	}
	
	DeviceContext* pContext = _mapContext[key];
	if(pContext != NULL)
	{
		pContext->ShowModule(type);
		_pDeviceContext = pContext;
	}
}

void DeviceController::CreateContext( wstring id, void* key )
{
	DeviceContext* pContext = _mapContext[key];
	if(pContext == NULL)
	{
		pContext = new DeviceContext(id, key, this->_pModulesView);
		_mapContext[key] = pContext;
	}
}

void DeviceController::RemoveContext( void* pPhone )
{
	DeviceContext* pContext = NULL;
	map<void*, DeviceContext*>::iterator it = _mapContext.find(pPhone);
	if(it != _mapContext.end())
	{
		pContext = it->second;
		_mapContext.erase(it);
	}
	if (NULL != pContext)
	{
		pContext->HideModule();
	}

	if(pContext == _pDeviceContext)
		_pDeviceContext = NULL;
	SAFE_DELETE(pContext);
}

void DeviceController::Refresh()
{
	if(_pDeviceContext == NULL)
		return;

	NotifyRefreshModuleMsg msg;
	msg.deviceID = _pDeviceContext->GetDeviceID();
	msg.PostToUI();
}

void DeviceController::InitModule( void* key, DeviceModuleType type )
{
	DeviceContext* pContext = _mapContext[key];
	if(pContext != NULL)
	{
		pContext->InitModule(type);
	}
}

