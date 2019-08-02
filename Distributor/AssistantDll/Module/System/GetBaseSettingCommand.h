#pragma once
#include "Module/BaseCommand.h"
#include <map>

using namespace std;
namespace ND91Assistant
{

class GetBaseSettingCommand : public BaseCommand
{
public:
	GetBaseSettingCommand(const DeviceData* pDevice);
	virtual ~GetBaseSettingCommand(void);

	// Summary  : �������ƻ�ȡָ��������Ϣ
	// Returns	: std::wstring
	// Parameter: string name 
	wstring GetSettingValueByName(string name);

protected:

	virtual void Execute();

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosCommandFilePath() {return L""; }

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosResultFilePath(){return L"";}

	// ����ios�ļ��������ļ�����ȫ·��, unicode���������߸���ɾ���ļ�
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename){}

private:

	map<string, wstring>  _baseSettingInfos;
};

}
