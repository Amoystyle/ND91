#pragma once
#include "Module/BaseCommand.h"
#include <deque>
using namespace std;
namespace ND91Assistant
{
// ��ȡ�龰ģʽ����
class ProfileData;
class GetProfilesCommand :public BaseCommand
{
public:
	GetProfilesCommand(const DeviceData* pDevice);
	virtual ~GetProfilesCommand(void);

	virtual void Execute();

	deque<ProfileData*> GetProfiles(){return _profilelist;}

protected:

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

	deque<ProfileData*>   _profilelist;
};

}
