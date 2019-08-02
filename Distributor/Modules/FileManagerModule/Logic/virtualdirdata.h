#pragma once


#include "virtualnamedirdata.h"

// 业务逻辑基类，提供基本的业务操作
class VirtualDirData : public DirData ,public RootData
{
public:
	VirtualDirData(const DeviceData* pDevice);
	~VirtualDirData();

public:
	bool Rename(wstring dest);

	// 取得所有子节点（复制数组，请勿放在循环中）
	deque<NodeData*> GetChildrens();

	void InstallIos();		//ios设备快速入口
	void InstallAndroid();	//安卓设备快速入口

private:

#pragma warning(disable : 4251)     // 禁用此警告
	deque<NodeData*> _childNodes;	// 子节点
#pragma warning(default : 4251)     // 恢复

	const DeviceData* _pDevice;

	VirtualNameDirData *_flash_Diskdata;
	VirtualNameDirData *_mydocumentData;
	VirtualNameDirData *_backupData;
	VirtualNameDirData *_userappData;
	VirtualNameDirData *_cydiaappData;
	VirtualNameDirData *_photo_data;

	DirData *_parent;
};