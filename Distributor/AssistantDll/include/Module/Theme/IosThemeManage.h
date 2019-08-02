#pragma once

#include "ThemeManager.h"

namespace ND91Assistant
{
#define Ios_SummerBoard_App_Folder		L"/Applications/SMBPrefs.app"
#define Ios_WinterBoard_App_Folder		L"/Applications/WinterBoard.app"
#define Ios_PandaHome_App_Folder		L"/Applications/PandaHome.app"
#define Ios_PandaHome_Dylib				L"/Library/MobileSubstrate/DynamicLibraries/SBPandaHome.dylib"
#define Ios_WSBoard_Theme_Path			L"/Library/Themes"
//#define Ios_PandaHome_Theme_Path		L"/private/var/mobile/Media/PandaHome/theme"// �ɵ�Ŀ¼
#define Ios_PandaHome_Theme_Path		L"/private/var/stash/ThemeData"// �µ�Ŀ¼
#define Ios_PandaHome_Theme_DownLoad	L"/private/var/mobile/Media/PandaHome/themeDownload"
#define Ios_Preview_Image_Name			L"91ThemePreview"
//#define Ios_PandaHome_Setting_File		L"/private/var/mobile/Media/PandaHome/theme.plist" // �ɵ�
#define Ios_PandaHome_Setting_File		L"/private/var/stash/PandaHome/ThemeDownload/theme.plist" // �µ�
#define Ios_WinterBoard_Setting_File	L"/private/var/mobile/Library/Preferences/com.saurik.WinterBoard.plist"
#define Ios_SummerBoard_Setting_File	L"/var/root/Library/Preferences/com.apptapp.SummerBoard.plist"
#define Ios_Weather_Info_File			L"/Applications/Weather.app/Info.plist"

	/// �豸�ͺ�,iphone1�� ����iphone2��iphong4��ipad��
	/// ����Ŀǰ���ֵĲ���̫ϸ�������Ϊ���������Ǵ�ֵ��
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
		wstring _strEnglist; //Ӣ����
		wstring _strChinese; //���ļ�����
		wstring _strTaiwan;  //���ķ�����

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

		// Summary  : ��ʼ���ֻ���������Ҫ���ж�������Ҫ��װӦ��
		// Returns  : ND91Assistant::eThemeInitCode �����س�ʼ��������Ϣ
		virtual eThemeInitCode InitEnvironment();

		// Summary  : ���������б�
		// Returns  : void
		virtual bool Load();

		// Summary  : ��ȡ������ϸ��Ϣ
		// Returns  : ThemeData* ������Ϣ�������������ͷ�
		// Parameter: wstring strThemeId ������ID
		virtual bool GetThemeDetailInfo(ThemeData* pThemeData);

		// Summary  : ��������Ԥ��ͼ
		// Returns  : bool ���ɹ�����TRUE
		// Parameter: wstring strFullNameOnPc ��Ԥ��ͼ�ڱ��ص����ƣ���·��)
		// Parameter: ThemeData * ��������Ϣ
		virtual bool LoadPreviewPhoto(ThemeData* pThemeData);

		// Summary  : ��ȡ��ǰ����
		// Returns  : ThemeData* �����ص�ǰ����
		virtual ThemeData* GetCurTheme();

		// Summary  : ������������
		// Returns  : bool �ɹ�����TRUE
		// Parameter: ThemeData * pThemeData ��������Ϣ
		// Parameter: wstring strNewName ����������
		virtual bool Rename(ThemeData* pThemeData, wstring strNewName);

		// Summary  : ���õ�ǰ����
		// Returns  : bool �ɹ�����TRUE
		// Parameter: ThemeData * pThemeData ��������Ϣ
		// Parameter: eThemeInitCode & eEnvironmentErrorCode ���ֻ�����������
		virtual bool DeleteTheme(ThemeData* pThemeData);

		// Summary  : �������
		// Returns  : bool �ɹ�����TRUE
		// Parameter: wstring strLocalFile ������·�����ļ���
        // Parameter: ProgressObserver* pObserver ����������
		// Parameter: eThemeInitCode & eEnvironmentErrorCode ���ֻ�����������
		virtual ThemeData* AddLocalTheme(wstring strLocalFile, ProgressObserver* pObserver = NULL, wstring strPreviewFile = L"");

		// Summary  : ������������
		// Returns  : bool �ɹ�����TRUE
		// Parameter: ThemeData * pThemeData ��������Ϣ
		virtual wstring BackupTheme(ThemeData* pThemeData, wstring SavePath);

		// Summary  : ���õ�ǰ����
		// Returns  : bool �ɹ�����TRUE
		// Parameter: ThemeData * pThemeData ��������Ϣ
		// Parameter: eThemeInitCode & eEnvironmentErrorCode ���ֻ�����������
		virtual bool SetCurTheme(ThemeData* pThemeData);

		// Summary  : ������ǰ����
		// Returns  : bool �ɹ�����TRUE
		virtual bool RevocationTheme();

		// Summary  : �ж��Ƿ�Ϊ����Ŀ¼��PC�ˣ�
		// Returns  : bool ������Ŀ¼�򷵻�TRUE
		// Parameter: wstring strPath ��PC�ϵ�Ŀ¼
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

		// �ж�ͼƬ��PixelFormat �Ƿ��� �����쳣�� PixelFormat ֮��
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
