#pragma once
#include "Module/BaseCommand.h"
namespace ND91Assistant
{
// �����������
class ThemeOperateCommand : public BaseCommand
{
public:
	ThemeOperateCommand(const DeviceData* pDevice);
	virtual ~ThemeOperateCommand(void);

	// Summary  : ��Ϊ����
	// Returns	: bool
	// Parameter: string themeID
	bool SetTheme(wstring themeID,bool IsSmart=false);

	// Summary  : ɾ������
	// Returns	: bool
	// Parameter: string themeID
	bool DelTheme(wstring themeID,bool IsSmart=false);

	// Summary  : �������
	// Returns	: bool
	// Parameter: wstring aptRemotePath
	bool AddTheme(wstring aptRemotePath,bool IsSmart=false);

	// Summary  : ��ʼ��������Ϣ
	// Returns	: bool
	bool InitUpdateThemes();

	// Summary  : �ػ����������Ҫ��Ϣ��xml�ļ�
	// Returns	: bool
	bool GetThemeBriefInfo();

	bool GetSmartThemeXml();

	// Summary  : �ػ����ɵ���������ϸ��Ϣ��xml�ļ�
	// Returns	: bool
	// Parameter: string themeID
	bool GetThemeDetailInfo(wstring themeID);

	// Summary  : ��Ӳ���Ϊ��ǰ����
	// Returns	: bool
	// Parameter: wstring aptRemotePath
	bool AddThemeAndSetCurrentTheme(wstring aptRemotePath);

	// Summary  : �ػ����ɰ���ȫ��������Ϣ��xml�ļ�
	// Returns	: bool
	// Parameter: wstring path
	bool GetThemeInfos(wstring path);

	// Summary  : �ػ�����Ҫ������������ļ�
	// Returns	: bool
	// Parameter: string themeID
	bool ExportThemePackage(wstring themeID);

	virtual void Execute();

protected:

	// ����Android���ص����ݰ�ͷ -- 20���ֽ�
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

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

	enum OperationType
	{
		ESetTheme = 1,
		EDeleteTheme = 2,
		EAddTheme = 3,
		EAddThemeAndSetAsCurTheme = 4,
		EGetThemeInfos = 5,
		EInitUpdateTheme = 6,
		EGetThemeBriefInfo = 7,
		EGetThemeDetailInfo = 8,
		EExportThemePkg = 9
	};

	OperationType   _type;

	wstring    _themeId;

	wstring   _path;

	bool      _success;
};

}
