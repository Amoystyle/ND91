#pragma once


// 业务逻辑基类，提供基本的业务操作
class VirtualNameDirData : public DirData
{
public:
	VirtualNameDirData(const DeviceData* pDevice);
	VirtualNameDirData(DirData *dirdata);
	~VirtualNameDirData();

public:

	//获取快速列表显示名称
	wstring GetVirtualname();
	
	//设置快速列表显示名称
	void SetVirtualname(wstring name);

	//获取logo
	wstring GetLogo();
	//设置logo
	void SetLogo(wstring logo);

private:
	wstring _virtualname;		//列表中显示的名字
	wstring _logo;				//真是名称
};