#pragma once

#include "ThemeManager.h"
#include "Monitor/DeamonMessageObserver.h"

namespace ND91Assistant
{
    // Android主题管理类
    class CLASSINDLL_CLASS_DECL AndroidThemeManager : public ThemeManager, public DeamonMessageObserver
    {
    public:
        AndroidThemeManager(const DeviceData* pDevice);
        ~AndroidThemeManager(void);

    public: // 继承自 DeamonMessageSubject
        // 新短信通知
        void NewMessage(const MessageData* msg) {};
        // 短信回执通知
        void MessageReceipt(const MessageReceiptData* pMsgData) {};
        // 熊猫桌面操作通知
        void PandaThemeAction(const PandaThemeActionReturn* pData);

    public: // 继承自 ThemeManager
        // Summary  : 初始化手机环境，主要是判断有无需要安装应用
        // Returns  : ND91Assistant::eThemeInitCode ：返回初始化错误信息
        virtual eThemeInitCode InitEnvironment();

        // Summary  : 加载主题列表
        // Returns  : void
        virtual bool Load(); //virtual void LoadThemes(vector<wstring>& vtThemeId) = 0; 

        // Summary  : 获取主题详细信息
        // Returns  : ThemeData* 主题信息，调用者无需释放
        // Parameter: wstring strThemeId ：主题ID
        virtual bool GetThemeDetailInfo(ThemeData* pThemeData); //virtual ThemeData* GetThemeDetailInfo(wstring strThemeId) = 0; 

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
        virtual bool DeleteTheme(ThemeData* pThemeData);

        // Summary  : 添加主题
        // Returns  : bool 成功返回TRUE
        // Parameter: wstring strLocalFile ：本地路径或文件名（IOS为文件夹，ANDROID为文件）
        // Parameter: wstring strPreviewFile ：预览图文件名
        // Parameter: ProgressObserver* pObserver 进度条设置
        virtual ThemeData* AddLocalTheme(wstring strLocalFile, ProgressObserver* pObserver = NULL, wstring strPreviewFile = L"");

        // Summary  : 导出备份主题
        // Returns  : wstring 备份后的本地文件名（IOS为文件夹，ANDROID为文件），失败返回L""
        // Parameter: ThemeData * pThemeData ：主题信息
        // Parameter: wstring SavePath 要存储到本地的目录
        virtual wstring BackupTheme(ThemeData* pThemeData, wstring SavePath);

        // Summary  : 设置当前主题
        // Returns  : bool 成功返回TRUE
        // Parameter: ThemeData * pThemeData ：主题信息
        virtual bool SetCurTheme(ThemeData* pThemeData);

        // Summary  : 撤销当前主题
        // Returns  : bool 成功返回TRUE
        virtual bool RevocationTheme();

        // Summary  : 判断是否为主题目录（PC端）
        // Returns  : bool 是主题目录则返回TRUE
        // Parameter: wstring strPath ：PC上的目录
        virtual bool CheckIsThemePath(wstring strPath);

        // 释放，在最后调用
        virtual void Release();
    private:
		enum ActionType
		{
			ENone = 0,
			EActionSetTheme = 1,
			EActionDelTheme = 2,
			EActionAddTheme = 3
		};
        PandaThemeActionReturn* _pActionReturnData;
        ActionType              _curActionType;//      _bActionReturnData;
        ThemeData*              _pCurrentTheme; //当前主题

        // 等待守护返回结果
        bool WaitForDeamonReturn(int nWaitTime = 15000);
        // 加载主题列表
        void LoadAllThemes();
        // Summary  : 加载全部主题id列表，2.0以上版本使用
        // Returns	: void
        ThemeBriefInfo LoadThemeBreifInfo();

		void LoadSmartThemes();

        // Summary  : 添加apk主题
        // Returns	: bool
        // Parameter: wstring localApkPath
        ThemeData* AddAPKTheme(wstring localApkPath);

        // Summary  : 添加apt主题
        // Returns	: bool
        // Parameter: wstring localAptPath
        ThemeData* AddAptTheme(wstring localAptPath,bool IsSmart=false);

		int AppInstallStatus(wstring pkgname, wstring minVersion);
    };
}