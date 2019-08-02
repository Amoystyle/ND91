#pragma once

#include "Module/Theme/ThemeData.h"

namespace ND91Assistant
{
	typedef enum eThemeInitCode
	{
		eThemeInitCode_Success, //已安装，成功
		eThemeInitCode_Ios_UnJailBreaked, //未越狱
		eThemeInitCode_Ios_UnInstall_SummerBoard, //未装SummerBoard
		eThemeInitCode_Ios_UnInstall_WinterBoard, //未装WinterBoard
		eThemeInitCode_Ios_UnInstall_PandaHome, // 未装熊猫桌面
		eThemeInitCode_Error, //其他错误
		eThemeInitCode_Android_NotSupport,
		eThemeInitCode_Android_NotSupport_SmartHome,
		eThemeInitCode_Android_NeedInstall_SmartHome,
		eThemeInitCode_Android_NeedInstall_PandaHome_Two,
		eThemeInitCode_Android_NeedInstall_BothHome,
	};

	class CLASSINDLL_CLASS_DECL ThemeManager
	{
	public:
		ThemeManager(const DeviceData* pDevice);
		~ThemeManager(void);

		// Summary  : 初始化手机环境，主要是判断有无需要安装应用
		// Returns  : ND91Assistant::eThemeInitCode ：返回初始化错误信息
		virtual eThemeInitCode InitEnvironment() = 0;

		// Summary  : 加载主题列表
		// Returns  : void
		virtual bool Load() = 0; //virtual void LoadThemes(vector<wstring>& vtThemeId) = 0; 

		// Summary  : 获取主题详细信息
		// Returns  : ThemeData* 主题信息，调用者无需释放
		// Parameter: wstring strThemeId ：主题ID
		virtual bool GetThemeDetailInfo(ThemeData* pThemeData) = 0; //virtual ThemeData* GetThemeDetailInfo(wstring strThemeId) = 0; 

		// Summary  : 加载主题预览图
		// Returns  : bool ：成功返回TRUE
		// Parameter: wstring strFullNameOnPc ：预览图在本地的名称（含路径)
		// Parameter: ThemeData * ：主题信息
		virtual bool LoadPreviewPhoto(ThemeData* pThemeData) = 0;

		// Summary  : 获取当前主题
		// Returns  : ThemeData* ：返回当前主题
		virtual ThemeData* GetCurTheme() = 0;

		// Summary  : 重命名主题名
		// Returns  : bool 成功返回TRUE
		// Parameter: ThemeData * pThemeData ：主题信息
		// Parameter: wstring strNewName ：新主题名
		virtual bool Rename(ThemeData* pThemeData, wstring strNewName) = 0;

		// Summary  : 设置当前主题
		// Returns  : bool 成功返回TRUE
		// Parameter: ThemeData * pThemeData ：主题信息
		virtual bool DeleteTheme(ThemeData* pThemeData) = 0;

		// Summary  : 添加主题
		// Returns  : bool 成功返回TRUE
		// Parameter: wstring strLocalFile ：本地路径或文件名（IOS为文件夹，ANDROID为文件）
        // Parameter: ProgressObserver* pObserver 进度条设置
		// Parameter: wstring strPreviewFile ：预览图文件名
		virtual ThemeData* AddLocalTheme(wstring strLocalFile, ProgressObserver* pObserver = NULL, wstring strPreviewFile = L"") = 0;

		// Summary  : 导出备份主题
		// Returns  : wstring 备份后的本地文件名（IOS为文件夹，ANDROID为文件），失败返回L""
		// Parameter: ThemeData * pThemeData ：主题信息
		// Parameter: wstring SavePath 要存储到本地的目录
		virtual wstring BackupTheme(ThemeData* pThemeData, wstring SavePath) = 0;

		// Summary  : 设置当前主题
		// Returns  : bool 成功返回TRUE
		// Parameter: ThemeData * pThemeData ：主题信息
		virtual bool SetCurTheme(ThemeData* pThemeData) = 0;

		// Summary  : 撤销当前主题
		// Returns  : bool 成功返回TRUE
		virtual bool RevocationTheme() = 0;

		// Summary  : 判断是否为主题目录（PC端）
		// Returns  : bool 是主题目录则返回TRUE
		// Parameter: wstring strPath ：PC上的目录
		virtual bool CheckIsThemePath(wstring strPath) = 0;

		// Summary  : 获取主题列表
		// Returns  : vector<ThemeData*>
		virtual deque<ThemeData*> GetThemeList();

        // 释放，在最后调用
        virtual void Release() = 0;

	protected:
		void ClearThemeList();
		void AddTheme(wstring id, ThemeData* pThemeData);
        void AddTheme(ThemeData* pThemeData);
        void RemoveTheme(ThemeData* pThemeData);
		ThemeData* FindTheme(wstring id);
		ThemeData* FindTheme(ThemeType eType, wstring strThemeName);

	protected:
		const DeviceData* _pDevice;
		map<wstring, ThemeData*> _themeList;
		wstring _strPreviewImageFolder;
	};
}