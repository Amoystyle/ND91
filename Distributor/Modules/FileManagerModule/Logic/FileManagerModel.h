#pragma once

#include "FileManagerInterface.h"
#include "FileManagerHistory.h"
#include "virtualdirdata.h"
#include "Base/BaseThread.h"
#include "Common/ProgressObserver.h"

enum Refresh_Node_flag
{
	RefreshNode,		//只刷新节点
	RefreshNode_And_SelectList,	//刷新节点后选中行
};

enum FM_Thread
{
	Action_FM_LoadRootData,		// 加载节点
	Action_FM_Download,         // 文件管理：下载
	Action_FM_UploadFile,       // 文件管理：上传文件
	Action_FM_UploadDir,        // 文件管理：上传文件夹
	Action_FM_Delete,           // 文件管理：删除
	Action_FM_Paste,            // 文件管理：粘贴
	Action_FM_Load_App,		    // 文件管理：加载应用程序名
	Action_FM_Ctreat_Dir,	    // 文件管理：预创建dir目录
};

struct Param_typ
{
	void		*_p;
	wstring		_str;
	bool		_bool;
	int			_int;
	vector<void*> _ver_p;
};

// 文件管理模块业务逻辑类
class FileManagerModel :BaseCSLock
{
public:
    FileManagerModel(FileManagerInterface* pInterface,wstring id);
    virtual ~FileManagerModel(void);


	unsigned long Main();

	//界面第一次加载时调用，临时处理首次加载阻塞问题。
	void LoadDefaultData();

	//是否已加载过，判断依据：_pRoot && _pVirtualRoot
	bool IsDefaultLoaded();

    void RefreshData();

    bool LoadData();

    NodeData* GetNode(wstring path);        // 取指定目录的文件节点
    void RefreshSpaceInfo(wstring path);    // 刷新空间信息(参数为当前路径)

    void AddHistory(wstring path);          // 增加浏览历史记录
    void GoNext();                          // 前进
    void GoPrev();                          // 后退
    wstring AddFolder(DirData* pDir);          // 新建文件夹

    void Delete(vector<NodeData*>* pDeleteNodes,DirData* pCurDir);                   // 删除
    void UploadFile(vector<wstring>* pFiles, DirData* pCurDir);      // 上传文件
    void UploadDir(wstring dir, DirData* pCurDir);                   // 上传目录
    // bShowFinishMsg ：是否显示完成信息
    // bWaitFinish : 是否等到下载完成
    bool Download(wstring destDir, vector<NodeData*>* pNodes, bool bShowFinishMsg=true, bool bWaitFinish=false);

    // 下载当前选中文件到临时目录并打开（对零字节文件不做处理）
    void DownloadAndOpenFile(NodeData* pNode);

    // Summary  : 粘贴
    // Returns  : void
    // Parameter: vector<wstring>* pPasteNodes  // 待粘贴的节点
    // Parameter: bool bCut  // 剪切还是拷贝
    // Parameter: DirData* pCurDir  // 当前目录
    void Paste(vector<wstring>* pPasteNodes, bool bCut, DirData* pCurDir);

    // 是否用户目录
    bool IsUserDir(const wchar_t* pPath );

    // 获取SDCard路径
    wstring GetSDCardPath();
    
    // 获取SDCard状态
    SDCardState GetSDCardState();
	
	//应用程序创建线程把guid转为应用程序名
	void RestoreAppNodeName(DirData* pTreeNode);

	//更新应用程序名线程
	void SetRestortAppName(bool flag);

	//设置节点刷新状态
	bool GetTreadState();

	//关闭历史记录按钮以及灰态向前、向后、上一级按钮
	void SetHistoryEnable(bool val);

	void CreateDirThread();

	//获取应用程序刷新接口是否在运行如果在运行则不刷新
	bool GetAppThreadFlag(){return _thread_is_run;};		
	
	vector<wstring> _refresh_node_ver;	//刷新数据使用的vector

    bool IsLastDownloadSuccess() {return _bLastDownloadSuccess;}

	VirtualDirData* GetVirtualRoot(){ return _pVirtualRoot; }

	DirData* GetRoot(){ return _pRoot; }

	//设备退出
	void Release();

	DEVICE_OS_TYPE GetDeviceOsType();

	void HideProWnd(int Requestid);		//关闭进度条窗口通用消息发送
private:
    // 进度对话框处理
	static DWORD WINAPI DoDownload(LPVOID lpParameter);
    static DWORD WINAPI DoUploadFile(LPVOID lpParameter);
    static DWORD WINAPI DoUploadDir(LPVOID lpParameter);
    static DWORD WINAPI DoDelete(LPVOID lpParameter);
//     bool DoPaste( BaseParam* pParam );
 	static DWORD WINAPI DoRestoreAppName(LPVOID lpParameter);
 	static DWORD WINAPI DoCreateDir(LPVOID lpParameter);
    // 设定导航按钮是否可用
    void EnableButtons(wstring path);

	//发送刷新请求到界面
	void SendRefreshToUI(Refresh_Node_flag flag);

	//上传并删除一个文件测试目标文件夹是否有权限
	bool DirIsPermissions(DirData* pCurDir);


    FileManagerInterface*   _pUI;       // 所有需要的界面操作
    FileManagerHistory      _history;   // 路径浏览历史记录
    DirData*                _pRoot;     // 记住根目录
	VirtualDirData*			_pVirtualRoot;		//快速入口
	bool					_restoreAppName;	//是否已经重置应用程序名
	bool					_nodeRefresh;		//false:加载线程正常操作，true:节点被重新加载需要退出线程
	bool					_thread_is_run;		//线程是否在运行
	HWND					_uihwnd;	//ui句柄发送消息用
	bool					_addHistory;	//添加history的开关
	CRITICAL_SECTION            _lock;      // 列表是否加载判断的锁
	HANDLE					_handle;		//预创建dir线程
    bool                    _bLastDownloadSuccess; //上次下载是否成功
	SpaceInfo				_si;		//获取手机空间
	bool					_check_exit;	//设备是否退出
	wstring					_deviceID;

	BackUpAndRestoreObserver *_ob;
	ProgressRecvMsg *_progress;


	FM_Thread				_nAction;			//当前需要运行的线程
	Param_typ				_downloadThread;	//下载线程参数 Action_FM_Download
	Param_typ				_uploadFileThread;	//上传文件线程参数	Action_FM_UploadFile
	Param_typ				_uploadDirThread;	//上传文件夹		Action_FM_UploadDir
	Param_typ				_deleteThread;		//删除	Action_FM_Delete
	Param_typ				_pasteThread;		//粘贴	Action_FM_Paste
	Param_typ				_loadAppThread;		//加载应用程序

	static DWORD WINAPI LoadRootData(LPVOID lpParameter);

    // 文件已存在时的行为：重命名
    class RenameFile : public ProgressObserver
    {
        void GetPrompt(MESSAGE_CODE msgCode) {};
        void GetProgress(ProgressData pData) {};
        void Complete() {};
        EXIST_ACTION GetExistAction(std::wstring name)
        {
            return RENAME_ALL;
        };
    };

};
