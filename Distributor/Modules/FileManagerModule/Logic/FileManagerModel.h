#pragma once

#include "FileManagerInterface.h"
#include "FileManagerHistory.h"
#include "virtualdirdata.h"
#include "Base/BaseThread.h"
#include "Common/ProgressObserver.h"

enum Refresh_Node_flag
{
	RefreshNode,		//ֻˢ�½ڵ�
	RefreshNode_And_SelectList,	//ˢ�½ڵ��ѡ����
};

enum FM_Thread
{
	Action_FM_LoadRootData,		// ���ؽڵ�
	Action_FM_Download,         // �ļ���������
	Action_FM_UploadFile,       // �ļ������ϴ��ļ�
	Action_FM_UploadDir,        // �ļ������ϴ��ļ���
	Action_FM_Delete,           // �ļ�����ɾ��
	Action_FM_Paste,            // �ļ�����ճ��
	Action_FM_Load_App,		    // �ļ���������Ӧ�ó�����
	Action_FM_Ctreat_Dir,	    // �ļ�����Ԥ����dirĿ¼
};

struct Param_typ
{
	void		*_p;
	wstring		_str;
	bool		_bool;
	int			_int;
	vector<void*> _ver_p;
};

// �ļ�����ģ��ҵ���߼���
class FileManagerModel :BaseCSLock
{
public:
    FileManagerModel(FileManagerInterface* pInterface,wstring id);
    virtual ~FileManagerModel(void);


	unsigned long Main();

	//�����һ�μ���ʱ���ã���ʱ�����״μ����������⡣
	void LoadDefaultData();

	//�Ƿ��Ѽ��ع����ж����ݣ�_pRoot && _pVirtualRoot
	bool IsDefaultLoaded();

    void RefreshData();

    bool LoadData();

    NodeData* GetNode(wstring path);        // ȡָ��Ŀ¼���ļ��ڵ�
    void RefreshSpaceInfo(wstring path);    // ˢ�¿ռ���Ϣ(����Ϊ��ǰ·��)

    void AddHistory(wstring path);          // ���������ʷ��¼
    void GoNext();                          // ǰ��
    void GoPrev();                          // ����
    wstring AddFolder(DirData* pDir);          // �½��ļ���

    void Delete(vector<NodeData*>* pDeleteNodes,DirData* pCurDir);                   // ɾ��
    void UploadFile(vector<wstring>* pFiles, DirData* pCurDir);      // �ϴ��ļ�
    void UploadDir(wstring dir, DirData* pCurDir);                   // �ϴ�Ŀ¼
    // bShowFinishMsg ���Ƿ���ʾ�����Ϣ
    // bWaitFinish : �Ƿ�ȵ��������
    bool Download(wstring destDir, vector<NodeData*>* pNodes, bool bShowFinishMsg=true, bool bWaitFinish=false);

    // ���ص�ǰѡ���ļ�����ʱĿ¼���򿪣������ֽ��ļ���������
    void DownloadAndOpenFile(NodeData* pNode);

    // Summary  : ճ��
    // Returns  : void
    // Parameter: vector<wstring>* pPasteNodes  // ��ճ���Ľڵ�
    // Parameter: bool bCut  // ���л��ǿ���
    // Parameter: DirData* pCurDir  // ��ǰĿ¼
    void Paste(vector<wstring>* pPasteNodes, bool bCut, DirData* pCurDir);

    // �Ƿ��û�Ŀ¼
    bool IsUserDir(const wchar_t* pPath );

    // ��ȡSDCard·��
    wstring GetSDCardPath();
    
    // ��ȡSDCard״̬
    SDCardState GetSDCardState();
	
	//Ӧ�ó��򴴽��̰߳�guidתΪӦ�ó�����
	void RestoreAppNodeName(DirData* pTreeNode);

	//����Ӧ�ó������߳�
	void SetRestortAppName(bool flag);

	//���ýڵ�ˢ��״̬
	bool GetTreadState();

	//�ر���ʷ��¼��ť�Լ���̬��ǰ�������һ����ť
	void SetHistoryEnable(bool val);

	void CreateDirThread();

	//��ȡӦ�ó���ˢ�½ӿ��Ƿ������������������ˢ��
	bool GetAppThreadFlag(){return _thread_is_run;};		
	
	vector<wstring> _refresh_node_ver;	//ˢ������ʹ�õ�vector

    bool IsLastDownloadSuccess() {return _bLastDownloadSuccess;}

	VirtualDirData* GetVirtualRoot(){ return _pVirtualRoot; }

	DirData* GetRoot(){ return _pRoot; }

	//�豸�˳�
	void Release();

	DEVICE_OS_TYPE GetDeviceOsType();

	void HideProWnd(int Requestid);		//�رս���������ͨ����Ϣ����
private:
    // ���ȶԻ�����
	static DWORD WINAPI DoDownload(LPVOID lpParameter);
    static DWORD WINAPI DoUploadFile(LPVOID lpParameter);
    static DWORD WINAPI DoUploadDir(LPVOID lpParameter);
    static DWORD WINAPI DoDelete(LPVOID lpParameter);
//     bool DoPaste( BaseParam* pParam );
 	static DWORD WINAPI DoRestoreAppName(LPVOID lpParameter);
 	static DWORD WINAPI DoCreateDir(LPVOID lpParameter);
    // �趨������ť�Ƿ����
    void EnableButtons(wstring path);

	//����ˢ�����󵽽���
	void SendRefreshToUI(Refresh_Node_flag flag);

	//�ϴ���ɾ��һ���ļ�����Ŀ���ļ����Ƿ���Ȩ��
	bool DirIsPermissions(DirData* pCurDir);


    FileManagerInterface*   _pUI;       // ������Ҫ�Ľ������
    FileManagerHistory      _history;   // ·�������ʷ��¼
    DirData*                _pRoot;     // ��ס��Ŀ¼
	VirtualDirData*			_pVirtualRoot;		//�������
	bool					_restoreAppName;	//�Ƿ��Ѿ�����Ӧ�ó�����
	bool					_nodeRefresh;		//false:�����߳�����������true:�ڵ㱻���¼�����Ҫ�˳��߳�
	bool					_thread_is_run;		//�߳��Ƿ�������
	HWND					_uihwnd;	//ui���������Ϣ��
	bool					_addHistory;	//���history�Ŀ���
	CRITICAL_SECTION            _lock;      // �б��Ƿ�����жϵ���
	HANDLE					_handle;		//Ԥ����dir�߳�
    bool                    _bLastDownloadSuccess; //�ϴ������Ƿ�ɹ�
	SpaceInfo				_si;		//��ȡ�ֻ��ռ�
	bool					_check_exit;	//�豸�Ƿ��˳�
	wstring					_deviceID;

	BackUpAndRestoreObserver *_ob;
	ProgressRecvMsg *_progress;


	FM_Thread				_nAction;			//��ǰ��Ҫ���е��߳�
	Param_typ				_downloadThread;	//�����̲߳��� Action_FM_Download
	Param_typ				_uploadFileThread;	//�ϴ��ļ��̲߳���	Action_FM_UploadFile
	Param_typ				_uploadDirThread;	//�ϴ��ļ���		Action_FM_UploadDir
	Param_typ				_deleteThread;		//ɾ��	Action_FM_Delete
	Param_typ				_pasteThread;		//ճ��	Action_FM_Paste
	Param_typ				_loadAppThread;		//����Ӧ�ó���

	static DWORD WINAPI LoadRootData(LPVOID lpParameter);

    // �ļ��Ѵ���ʱ����Ϊ��������
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
