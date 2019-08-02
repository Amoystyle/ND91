#pragma once

#include "Module/BaseCommand.h"
#include "Common/greta/RegexprHelper.h"

namespace ND91Assistant
{
class GetSpaceInfoCommand :public BaseCommand
{
public:
	GetSpaceInfoCommand(const DeviceData* pDevice);
	virtual ~GetSpaceInfoCommand();

    // ִ������
    virtual void Execute();

	void  setPath(const wstring& apath){_Path = apath;}

	deque<SpaceInfo> GetDirInfos(){return _SpaceInfos;}

    // ����Android�������ݰ�
    virtual std::string GetAndroidBodyData();

    // ����Android���ؽ�����ݰ�
    virtual void SetAndroidReturnData(std::string data);

    // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
    virtual std::wstring GetIosCommandFilePath() { return L""; }

    // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
    virtual std::wstring GetIosResultFilePath() { return L""; }

    // ����ios�ļ��������ļ����������߸���ɾ���ļ�
    virtual std::wstring GetIosCommandFileContent() { return L""; }

    // ���������ļ���������ɾ���ļ�
    virtual  void SetIosReturnFile(std::wstring filename) { }

private:

	//�������ؽ��
	void Analyze(const string& result);

	//��������Ŀ¼�ռ���Ϣ
	SpaceInfo AnalyzeSpaceInfo(const string& result);

	wstring  _Path; //�����·��

	deque<SpaceInfo>  _SpaceInfos;//���ؽ��

    RegexprHelper _rh;

};

}
