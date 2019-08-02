#include "pch_module.h"

#ifdef MODULE_THEME

#include "Module/Theme/ThemeData.h"

ThemeData::ThemeData(void)
{

	_id = L""; // ����ID

	_name = L""; // ��������

	_en_name = L"";// ����Ӣ����

	_themeType = ThemeType_Unkonw;  // ��������

	_version = L""; // ����汾

	_textSize = 12; // ���������С

	_textColor = L""; // ������ɫ

	_textBackColor = L"";// �ı�����ɫ

	_pTextSize = 12;  

	_pTextColor = L"";

	_pTextBackColor = L"";

	_wallPaperPath = L"";// �����ֽ

	_panelWallPaperPath = L"";// 

	_localWallPaperPath = L"";

	_localPanelWallPaperPath = L"";

	_addAptLocalPath = L"";

	_strThemeFolderOnDev = L"";

	_strPreviewPathOnPC = L"";
}

ThemeData::~ThemeData(void)
{
}

void ND91Assistant::ThemeData::clone( ThemeData* pTheme )
{
	RETURN_IF(!pTheme);

	this->_id = pTheme->_id; // ����ID

	this->_name = pTheme->_name; // ��������

	this->_en_name = pTheme->_en_name;// ����Ӣ����

	this->_themeType = pTheme->_themeType;  // ��������

	this->_version = pTheme->_version; // ����汾

	this->_textSize = pTheme->_textSize; // ���������С

	this->_textColor = pTheme->_textColor; // ������ɫ

	this->_textBackColor = pTheme->_textBackColor;// �ı�����ɫ

	this->_pTextSize = pTheme->_pTextSize;  

	this->_pTextColor = pTheme->_pTextColor;

	this->_pTextBackColor = pTheme->_pTextBackColor;

	this->_wallPaperPath = pTheme->_wallPaperPath;// �����ֽ

	this->_panelWallPaperPath = pTheme->_panelWallPaperPath;// 

	for(map<ThemeAppIconType, ThemeAppIcon>::iterator it = pTheme->_themeIconList.begin(); it!= pTheme->_themeIconList.end(); it++)
	{
		this->_themeIconList.insert(pair<ThemeAppIconType, ThemeAppIcon>((*it).first, (*it).second));
	}
	this->_strThemeFolderOnDev = pTheme->_strThemeFolderOnDev;
}

#endif