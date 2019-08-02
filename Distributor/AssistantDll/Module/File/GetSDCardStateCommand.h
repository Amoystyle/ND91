#pragma once

#include "Module/BaseCommand.h"

using namespace  std;

namespace ND91Assistant
{
	// ��ȡSD��״̬
class GetSDCardStateCommand :
	public BaseCommand
{
public:
	GetSDCardStateCommand(const DeviceData* pDevice);
	virtual ~GetSDCardStateCommand(void);

	// Summary  : SD���Ƿ����
	// Returns	: bool
	// Parameter: const DeviceData * pDevice
	static bool IsSDCardExist(const DeviceData* pDevice);

	SDCardState GetSDCardState(){return _SDCardState;}

	// ִ������
	virtual void Execute();

protected:

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

	bool parseAndroidPackageHead( BinaryBufferReader* pReader );

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// ����ios�ļ��������ļ�����ȫ·��, unicode���������߸���ɾ���ļ�
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename) { }

	void  GetSdcardPathState();

private:

	SDCardState   _SDCardState; //SD��״̬�� MOUNTED��ʾ����


};

}
