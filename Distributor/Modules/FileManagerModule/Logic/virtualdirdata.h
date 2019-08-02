#pragma once


#include "virtualnamedirdata.h"

// ҵ���߼����࣬�ṩ������ҵ�����
class VirtualDirData : public DirData ,public RootData
{
public:
	VirtualDirData(const DeviceData* pDevice);
	~VirtualDirData();

public:
	bool Rename(wstring dest);

	// ȡ�������ӽڵ㣨�������飬�������ѭ���У�
	deque<NodeData*> GetChildrens();

	void InstallIos();		//ios�豸�������
	void InstallAndroid();	//��׿�豸�������

private:

#pragma warning(disable : 4251)     // ���ô˾���
	deque<NodeData*> _childNodes;	// �ӽڵ�
#pragma warning(default : 4251)     // �ָ�

	const DeviceData* _pDevice;

	VirtualNameDirData *_flash_Diskdata;
	VirtualNameDirData *_mydocumentData;
	VirtualNameDirData *_backupData;
	VirtualNameDirData *_userappData;
	VirtualNameDirData *_cydiaappData;
	VirtualNameDirData *_photo_data;

	DirData *_parent;
};