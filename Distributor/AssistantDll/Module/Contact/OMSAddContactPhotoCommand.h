#pragma once

#include "Module/Contact/ContactData.h"

namespace ND91Assistant
{
// oms���������ͼƬ
class OMSAddContactPhotoCommand :public BaseCommand
{
public:
	OMSAddContactPhotoCommand(const DeviceData* pDevice);
	virtual ~OMSAddContactPhotoCommand(void);


	// Summary  : �����������
	// Returns	: void
	// Parameter: wstring localpath
	void SetParam( wstring  localpath){ _localpath = localpath;}

	// ִ������
	virtual void Execute();

	wstring  GetUri(){return _uri;}
	Photo* GetReturnPictureData(){return &_pRetPicture;}

protected:

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// ����ios�ļ��������ļ�����ȫ·��, unicode���������߸���ɾ���ļ�
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename) { }

private:

	Photo  _pRetPicture;

	wstring        _localpath;

	wstring        _uri;
};

}
