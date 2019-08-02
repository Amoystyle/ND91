#pragma once

#include "ThemeManager.h"

namespace ND91Assistant
{
#define Ios_SummerBoard_App_Folder		L"/Applications/SMBPrefs.app"
#define Ios_WinterBoard_App_Folder		L"/Applications/WinterBoard.app"
#define Ios_PandaHome_App_Folder		L"/Applications/PandaHome.app"
#define Ios_PandaHome_Dylib				L"/Library/MobileSubstrate/DynamicLibraries/SBPandaHome.dylib"
#define Ios_WSBoard_Theme_Path			L"/Library/Themes"
//#define Ios_PandaHome_Theme_Path		L"/private/var/mobile/Media/PandaHome/theme"// 旧的目录
#define Ios_PandaHome_Theme_Path		L"/private/var/stash/ThemeData"// 新的目录
#define Ios_PandaHome_Theme_DownLoad	L"/private/var/mobile/Media/PandaHome/themeDownload"
#define Ios_Preview_Image_Name			L"91ThemePreview"
//#define Ios_PandaHome_Setting_File		L"/private/var/mobile/Media/PandaHome/theme.plist" // 旧的
#define Ios_PandaHome_Setting_File		L"/private/var/stash/PandaHome/ThemeDownload/theme.plist" // 新的
#define Ios_WinterBoard_Setting_File	L"/private/var/mobile/Library/Preferences/com.saurik.WinterBoard.plist"
#define Ios_SummerBoard_Setting_File	L"/var/root/Library/Preferences/com.apptapp.SummerBoard.plist"
#define Ios_Weather_Info_File			L"/Applications/Weather.app/Info.plist"

	/// 设备型号,iphone1， 还是iphone2，iphong4，ipad等
	/// 由于目前还分的不是太细，具体分为：（后面是传值）
	/// iphone 1,2,3  =====eIosDevSys_Three
	/// iphone 4  ====eIosDevSys_Four
	/// ipad ====eIosDevSys_Ipad
	typedef enum eIosDevSys
	{
		eIosDevSys_Three,
		eIosDevSys_Four,
		eIosDevSys_Ipad,
		eIosDevSys_Unknow
	};

	typedef enum eAppIconType
	{
		eAppIconType_AppStore = 0, //com.apple.AppStore
		eAppIconType_Maps,		//com.apple.Maps
		eAppIconType_Contacts, //com.apple.MobileAddressBook
		eAppIconType_Messages, //com.apple.MobileSMS
		eAppIconType_iTunes, //com.apple.MobileStore
		eAppIconType_Settings, //com.apple.Preferences
		eAppIconType_Calculator, //com.apple.calculator
		eAppIconType_Radio, //com.apple.mobile.radio
		eAppIconType_Calendar, //com.apple.mobilecal
		eAppIconType_iPod, //com.apple.mobileipod
		eAppIconType_Videos, //com.apple.mobileipod-Movie
		eAppIconType_Mail, //com.apple.mobilemail
		eAppIconType_Notes, //com.apple.mobilenotes
		eAppIconType_Phone, //com.apple.mobilephone
		eAppIconType_Safari, //com.apple.mobilesafari
		eAppIconType_Camera, //com.apple.mobileslideshow-Camera
		eAppIconType_Photos, //com.apple.mobileslideshow-Photos
		eAppIconType_Clock, //com.apple.mobiletimer
		eAppIconType_Keyboard, //com.apple.purple.KeyboardDemo
		eAppIconType_Stocks, //com.apple.stocks
		eAppIconType_Converter, //com.apple.unitconverter
		eAppIconType_Weather, //com.apple.weather
		eAppIconType_Business, //com.apple.widget.phonebook
		eAppIconType_Translator, //com.apple.widget.translation
		eAppIconType_YouTube, //com.apple.youtube
	};

	typedef struct APP_ICON_NAME
	{
		wstring _strEnglist; //英文名
		wstring _strChinese; //中文简体名
		wstring _strTaiwan;  //中文繁体名

		APP_ICON_NAME()
		{
			_strEnglist = L"";
			_strChinese = L"";
			_strTaiwan = L"";
		};

		APP_ICON_NAME(wstring strEnglist, wstring strChinese, wstring strTaiwan)
		{
			_strEnglist = strEnglist;
			_strChinese = strChinese;
			_strTaiwan = strTaiwan;
		};

	}APP_ICON_NAME;

    class ThemeManager;
    class IIosFileSystem;
	class CLASSINDLL_CLASS_DECL IosThemeManage : public ThemeManager
	{
	public:
		IosThemeManage(const DeviceData* pDevice);
		~IosThemeManage(void);

		// Summary  : 初始化手机环境，主要是判断有无需要安装应用
		// Returns  : ND91Assistant::eThemeInitCode ：返回初始化错误信息
		virtual eThemeInitCode InitEnvironment();

		// Summary  : 加载主题列表
		// Returns  : void
		virtual bool Load();

		// Summary  : 获取主题详细信息
		// Returns  : ThemeData* 主题信息，调用者无需释放
		// Parameter: wstring strThemeId ：主题ID
		virtual bool GetThemeDetailInfo(ThemeData* pThemeData);

		// Summary  : 加载主题预览图
		// Returns  : bool ：成功返回TRUE
		// Parameter: wstring strFullNameOnPc ：预览图在本地的名称（含路径)
		// Parameter: ThemeData * ：主题信息
		virtual bool LoadPreviewPhoto(ThemeData* pThemeData);

		// Summary  : 获取当前主题
		// Returns  : ThemeData* ：返回当前主题
		virtual ThemeData* GetCurTheme();

		// Summary  : 重命名主题名
		// Returns  : bool 成功返回TRUE
		// Parameter: ThemeData * pThemeData ：主题信息
		// Parameter: wstring strNewName ：新主题名
		virtual bool Rename(ThemeData* pThemeData, wstring strNewName);

		// Summary  : 设置当前主题
		// Returns  : bool 成功返回TRUE
		// Parameter: ThemeData * pThemeData ：主题信息
		// Parameter: eThemeInitCode & eEnvironmentErrorCode ：手机环境错误码
		virtual bool DeleteTheme(ThemeData* pThemeData);

		// Summary  : 添加主题
		// Returns  : bool 成功返回TRUE
		// Parameter: wstring strLocalFile ：本地路径或文件名
        // Parameter: ProgressObserver* pObserver 进度条设置
		// Parameter: eThemeInitCode & eEnvironmentErrorCode ：手机环境错误码
		virtual ThemeData* AddLocalTheme(wstring strLocalFile, ProgressObserver* pObserver = NULL, wstring strPreviewFile = L"");

		// Summary  : 导出备份主题
		// Returns  : bool 成功返回TRUE
		// Parameter: ThemeData * pThemeData ：主题信息
		virtual wstring BackupTheme(ThemeData* pThemeData, wstring SavePath);

		// Summary  : 设置当前主题
		// Returns  : bool 成功返回TRUE
		// Parameter: ThemeData * pThemeData ：主题信息
		// Parameter: eThemeInitCode & eEnvironmentErrorCode ：手机环境错误码
		virtual bool SetCurTheme(ThemeData* pThemeData);

		// Summary  : 撤销当前主题
		// Returns  : bool 成功返回TRUE
		virtual bool RevocationTheme();

		// Summary  : 判断是否为主题目录（PC端）
		// Returns  : bool 是主题目录则返回TRUE
		// Parameter: wstring strPath ：PC上的目录
		virtual bool CheckIsThemePath(wstring strPath);

		void AddDefaultIcon(eAppIconType eType, Image* pIconImage);	

        virtual void Release();

	private:
		bool IsPandaHomeEnvironment();

		void LoadPandaHomeThemes();
		void LoadWinterBoardThemes();

		void InitApplicationNameMap();
		void FreeApplicationNameMap();
		void InitNormalInfo();
		void InitThemeFilter();
		void InitDefaultIcon();
		void FreeDefaultIcon();

		bool IsInPandaHomeFilter(wstring strName);
		bool IsInWinterBoardFilter(wstring strName);

		bool GetPreviewDirect(ThemeData* pThemeData);
		bool GetPreviewFromSource(ThemeData* pThemeData);
		bool DownloadPreviewSource(wstring strPreviewSourceFolder, ThemeData* pThemeData);
		bool CreatePreviewFromSource(wstring strPreviewSourceFolder, ThemeData* pThemeData);

		// 判断图片的PixelFormat 是否在 引发异常的 PixelFormat 之中
		bool IsPixelFormatIndexed(PixelFormat pImagePixelFormat);

		wstring GetSWBoardCurThemeName();
		wstring GetPandaHomeCurThemeName();

		bool SetSummerBoardTheme(ThemeData* pThemeDataTemp);
		bool SetWinterBoardTheme(wstring strThemeName);
		bool SetPandaHomeTheme(wstring strThemeName);
		void ClearWeatherInfoSettingFile();
		void RenameIconNameByDevLang(wstring strThemeName);
		void CreateWinterBoardSettingFile(wstring strFileName, wstring strValues[], int nLen);
		void UpdateWinterBoardSettingFile(wstring strThemeName, wstring strFileName, wstring strValues[], int nLen);
		void CreatePandaHomeSettingFile(wstring strFileName, wstring strThemeName);

		bool RevocationWinterBoardTheme();
		bool RevocationPandaHomeTheme();

	private:
		IIosFileSystem* _pFileStream;
		int _nProductVer;
		eIosDevSys _eDevicePsys;
		vector<wstring> _vtPandaHomeFilter;
		vector<wstring> _vtWinterBoardFilter;
		wstring _strWSBoardThemePath;
		wstring _strPandaHomeThemePath;
		wstring _curThemePlistFilePath;
		map<eAppIconType, APP_ICON_NAME*> _mapAppIconName;
		map<eAppIconType, Image*> _mapDefaultIcon;
	};
}
