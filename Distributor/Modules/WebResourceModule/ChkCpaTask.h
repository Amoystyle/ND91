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

	//解析后的哈希值
	PROPERTYREALONLY(set<wstring>*,_itemHashValues,ItemHashValues)

	//是否退出
	PROPERTYWRITEONLY(bool,_cancel,Cancel)

	//原始hash值
	PROPERTYWRITEONLY(wstring,_strHashValue,StrHashValue)

	PROPERTY(ENUM_PLATFORM,_platform,Platfrom)

	//唯一标识和页面一一对应保证所执行的任务是对应正确的页面
	PROPERTYWRITEONLY(wstring,_id,Id)
	virtual bool Run();

	PROPERTY(UINT,_userData,UserData)

protected:
	virtual bool getCancel();
private:
	void initItemHashValues();
	
	void Compare();

};
