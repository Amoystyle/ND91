#include "pch_module.h"

#ifdef MODULE_THEME

#include "Module/Theme/ThemeData.h"

ThemeData::ThemeData(void)
{

	_id = L""; // 主题ID

	_name = L""; // 主题名称

	_en_name = L"";// 主题英文名

	_themeType = ThemeType_Unkonw;  // 主题类型

	_version = L""; // 主题版本

	_textSize = 12; // 主题字体大小

	_textColor = L""; // 字体颜色

	_textBackColor = L"";// 文本背景色

	_pTextSize = 12;  

	_pTextColor = L"";

	_pTextBackColor = L"";

	_wallPaperPath = L"";// 主题壁纸

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

	this->_id = pTheme->_id; // 主题ID

	this->_name = pTheme->_name; // 主题名称

	this->_en_name = pTheme->_en_name;// 主题英文名

	this->_themeType = pTheme->_themeType;  // 主题类型

	this->_version = pTheme->_version; // 主题版本

	this->_textSize = pTheme->_textSize; // 主题字体大小

	this->_textColor = pTheme->_textColor; // 字体颜色

	this->_textBackColor = pTheme->_textBackColor;// 文本背景色

	this->_pTextSize = pTheme->_pTextSize;  

	this->_pTextColor = pTheme->_pTextColor;

	this->_pTextBackColor = pTheme->_pTextBackColor;

	this->_wallPaperPath = pTheme->_wallPaperPath;// 主题壁纸

	this->_panelWallPaperPath = pTheme->_panelWallPaperPath;// 

	for(map<ThemeAppIconType, ThemeAppIcon>::iterator it = pTheme->_themeIconList.begin(); it!= pTheme->_themeIconList.end(); it++)
	{
		this->_themeIconList.insert(pair<ThemeAppIconType, ThemeAppIcon>((*it).first, (*it).second));
	}
	this->_strThemeFolderOnDev = pTheme->_strThemeFolderOnDev;
}

#endif