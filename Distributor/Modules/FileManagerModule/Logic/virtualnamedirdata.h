#pragma once


// ҵ���߼����࣬�ṩ������ҵ�����
class VirtualNameDirData : public DirData
{
public:
	VirtualNameDirData(const DeviceData* pDevice);
	VirtualNameDirData(DirData *dirdata);
	~VirtualNameDirData();

public:

	//��ȡ�����б���ʾ����
	wstring GetVirtualname();
	
	//���ÿ����б���ʾ����
	void SetVirtualname(wstring name);

	//��ȡlogo
	wstring GetLogo();
	//����logo
	void SetLogo(wstring logo);

private:
	wstring _virtualname;		//�б�����ʾ������
	wstring _logo;				//��������
};