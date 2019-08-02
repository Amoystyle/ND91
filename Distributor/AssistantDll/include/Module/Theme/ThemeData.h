#pragma once
#include "Module/BaseData.h"
#include <string>
#include <deque>
#include <map>

using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
// ��������
enum ThemeType
{
	ThemeType_APK,// �������������
	ThemeType_PandaHome,// ��è���������
	ThemeType_SmartHome,// ��׿���������
	ThemeType_SWBoard, //SpringBoard or WinterBoard
	ThemeType_Unkonw
};

//Ӧ�ó���ͼ��
struct ThemeAppIcon 
{
	wstring  _themeId;	// ����ID
	wstring  _appId;	//  Ӧ�ó���Id
	wstring  _iconPath;	// Ӧ�ó���ͼ��
	wstring  _LocalIconPath;
	ThemeAppIcon()
	{
		_themeId	= L"";	// ����ID
		_appId		= L"";	//  Ӧ�ó���Id
		_iconPath	= L"";	// Ӧ�ó���ͼ��
		_LocalIconPath = L"";
	}
};

// ȫ�������Ҫ��Ϣ
class ThemeBriefInfo
{
public:
	ThemeBriefInfo()
	{
		_currentThemeId =L"";
		_count=0;
	}
	~ThemeBriefInfo(){}

	wstring _currentThemeId; // ��ǰ����ID
	int		_count;			 // �������
	deque<wstring>  _themeIdList;//����ID�б� 
};

// ����
class CLASSINDLL_CLASS_DECL ThemeData : public BaseData
{
public:
	ThemeData(void);
	virtual ~ThemeData(void);

	void clone(ThemeData* pTheme);
	
	wstring       _id; // ����ID

	wstring       _name; // ��������

	wstring       _en_name;// ����Ӣ����

	ThemeType     _themeType;  // ��������

	wstring       _version; // ����汾

	int			  _textSize; // ���������С

	wstring       _textColor; // ������ɫ

	wstring       _textBackColor;// �ı�����ɫ

	int           _pTextSize;  

	wstring       _pTextColor;

	wstring       _pTextBackColor;

	wstring      _wallPaperPath;  // �����ֽ

	wstring      _panelWallPaperPath;// 

	wstring		 _strThemeFolderOnDev; //�����·�����豸��

	wstring		 _strPreviewPathOnPC; //Ԥ��ͼ��·����PC��

	map<ThemeAppIconType, ThemeAppIcon> _themeIconList;
	//deque<ThemeAppIcon>  _themeIconList;// Ӧ�ó���ͼ���б�

	wstring      _localWallPaperPath;   // ���غ�洢�����صı�ֽ·��

	wstring      _localPanelWallPaperPath;// ���غ�洢�����ص�Сϻ�ӱ�ֽ·��

	wstring      _addAptLocalPath;		// 
};

}
