#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
// ��OMS��ȡ������ͼƬ
class GetContactPhotoCommand :public BaseCommand
{
public:
	GetContactPhotoCommand(const DeviceData* pDevice, int contactId);
	virtual ~GetContactPhotoCommand(void);

	// ִ������
	virtual void Execute();

	// Summary  : ��ȡͷ��ͼƬ���浽���ص�·��
	// Returns	: std::wstring
	wstring  GetPhotoPath(){return _contantPhotoLocalPath;}

private:
	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·����
	virtual std::wstring GetIosCommandFilePath(){ return L""; }

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·����
	virtual std::wstring GetIosResultFilePath(){ return L""; }

	// ����ios�ļ�
	virtual std::wstring GetIosCommandFileContent(){ return L""; }

	// ����ios�豸�ķ����ļ���������ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename){}

private:

	int			_contactId;					// ������������id

	wstring		_contantPhotoLocalPath;		// �����ȡ����ͷ�񵽱��ص�·��

};

}
