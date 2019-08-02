#pragma once
#include "CWebResourceMacros.h"
#include <set>
#include <string>
#include "NetResourceStruct.h"
using namespace std;

enum TaskStatus
{
	TaskStatusBegin,
	TaskStatusRunning,
	TaskStatusWaiting,
	TaskStatusComplete,
	TaskStatusFail,
	TaskStatusEnd
};
class CChkCpaTask
{
public:
	CChkCpaTask(void);
	virtual ~CChkCpaTask(void);

	PROPERTY(TaskStatus,_status,Status)

	//������Ĺ�ϣֵ
	PROPERTYREALONLY(set<wstring>*,_itemHashValues,ItemHashValues)

	//�Ƿ��˳�
	PROPERTYWRITEONLY(bool,_cancel,Cancel)

	//ԭʼhashֵ
	PROPERTYWRITEONLY(wstring,_strHashValue,StrHashValue)

	PROPERTY(ENUM_PLATFORM,_platform,Platfrom)

	//Ψһ��ʶ��ҳ��һһ��Ӧ��֤��ִ�е������Ƕ�Ӧ��ȷ��ҳ��
	PROPERTYWRITEONLY(wstring,_id,Id)
	virtual bool Run();

	PROPERTY(UINT,_userData,UserData)

protected:
	virtual bool getCancel();
private:
	void initItemHashValues();
	
	void Compare();

};
