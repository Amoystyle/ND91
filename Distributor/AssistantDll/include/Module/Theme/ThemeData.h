#pragma once
#include "Module/BaseData.h"
#include <string>
#include <deque>
#include <map>

using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
// 主题类型
enum ThemeType
{
	ThemeType_APK,// 第三方的主题包
	ThemeType_PandaHome,// 熊猫桌面主题包
	ThemeType_SmartHome,// 安卓桌面主题包
	ThemeType_SWBoard, //SpringBoard or WinterBoard
	ThemeType_Unkonw
};

//应用程序图标
struct ThemeAppIcon 
{
	wstring  _themeId;	// 主题ID
	wstring  _appId;	//  应用程序Id
	wstring  _iconPath;	// 应用程序图标
	wstring  _LocalIconPath;
	ThemeAppIcon()
	{
		_themeId	= L"";	// 主题ID
		_appId		= L"";	//  应用程序Id
		_iconPath	= L"";	// 应用程序图标
		_LocalIconPath = L"";
	}
};

// 全部主题简要信息
class ThemeBriefInfo
{
public:
	ThemeBriefInfo()
	{
		_currentThemeId =L"";
		_count=0;
	}
	~ThemeBriefInfo(){}

	wstring _currentThemeId; // 当前主题ID
	int		_count;			 // 主题个数
	deque<wstring>  _themeIdList;//主题ID列表 
};

// 主题
class CLASSINDLL_CLASS_DECL ThemeData : public BaseData
{
public:
	ThemeData(void);
	virtual ~ThemeData(void);

	void clone(ThemeData* pTheme);
	
	wstring       _id; // 主题ID

	wstring       _name; // 主题名称

	wstring       _en_name;// 主题英文名

	ThemeType     _themeType;  // 主题类型

	wstring       _version; // 主题版本

	int			  _textSize; // 主题字体大小

	wstring       _textColor; // 字体颜色

	wstring       _textBackColor;// 文本背景色

	int           _pTextSize;  

	wstring       _pTextColor;

	wstring       _pTextBackColor;

	wstring      _wallPaperPath;  // 主题壁纸

	wstring      _panelWallPaperPath;// 

	wstring		 _strThemeFolderOnDev; //主题的路径，设备端

	wstring		 _strPreviewPathOnPC; //预览图的路径，PC端

	map<ThemeAppIconType, ThemeAppIcon> _themeIconList;
	//deque<ThemeAppIcon>  _themeIconList;// 应用程序图标列表

	wstring      _localWallPaperPath;   // 下载后存储到本地的壁纸路径

	wstring      _localPanelWallPaperPath;// 下载后存储到本地的小匣子壁纸路径

	wstring      _addAptLocalPath;		// 
};

}
