#include "StdAfx.h"
#include "Manager/ThreadManager.h"
#include "FileManagerModel.h"
#include "UI/MainView.h"
#include "Common/StrOperation.h"



FileManagerModel::FileManagerModel(FileManagerInterface* pUI,wstring id)
: _pUI(pUI)
, _history(MAX_HISTORY_COUNT)
{
    _pRoot = NULL;
	_pVirtualRoot = NULL;
	_handle = NULL;
	_restoreAppName = true;
	_nodeRefresh = false;
	_addHistory = true;
	_thread_is_run = false;
	InitializeCriticalSection(&_lock);	//��ʼ����
    _bLastDownloadSuccess = true;
	_check_exit = false;
	_deviceID = id;
}


FileManagerModel::~FileManagerModel( void )
{
	DeleteCriticalSection(&_lock);
}

bool FileManagerModel::LoadData()
{	
	WaitForSingleObject(_handle,INFINITE);
	if (_pRoot && _pVirtualRoot)
		return false;

	LOG->WriteDebug(L"����FileManagerModel::LoadData()");
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(_deviceID);
    RETURN_FALSE_IF( ! pDevice );

	int nTick = ::GetTickCount();
	while(!pDevice->IsActivate() && ::GetTickCount() - nTick < 10000)
	{
		Sleep(100);
	}

	if(!pDevice->IsActivate())
	{
		LOG->WriteDebug(L"Restore����, pDeviceinfo->IsActivate() == false");
		return false;
	}


	_pVirtualRoot = new VirtualDirData(pDevice);
    _pRoot = new DirData(pDevice);  // ������ʼ���������Զ�Ϊ��

    if ( _pRoot && _pVirtualRoot)
    {
		LOG->WriteDebug(L"��ʼ���������");
		InitRootMsg msg;
		msg.sn = pDevice->GetId();
		msg.SendToUI();
    }
    RefreshSpaceInfo(L"/");
    return true;
}

NodeData* FileManagerModel::GetNode(wstring path )
{
    NodeData* pNode = FileHelper::GetNode(_pRoot, path, false);
    if ( ! pNode )
    {
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_NotFound") + path,
            CommonUIFunc::LoadString(L"PROMPT"));
    }
    return pNode;
}

void FileManagerModel::GoNext()
{
    _history.EnableGoto(false); // ����ǰ��������������������¼

    wstring path = _history.Next();
    if ( ! _pUI->Goto(path) )
        _history.DeleteCurrent();
    EnableButtons(path);

    _history.EnableGoto(true);
}

void FileManagerModel::GoPrev()
{
    _history.EnableGoto(false); // ������˲�����������������¼

    wstring path = _history.Prev();
    if ( ! _pUI->Goto(path) )
        _history.DeleteCurrent();
    EnableButtons(path);

    _history.EnableGoto(true);
}

void FileManagerModel::AddHistory( wstring path )
{
    if ( ! path.empty() && _addHistory)
    {
        _history.Goto(path);
		EnableButtons(path);
    }
}

void FileManagerModel::EnableButtons( wstring path )
{
    _pUI->EnableButton(! _history.IsHead(), ! _history.IsTail(), path != L"/" );
}

wstring FileManagerModel::AddFolder(DirData* pDir)
{
    if (!pDir)
    return _T("");

	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(_deviceID);
	if(pDevice == NULL)
		return L"";

	//by chenxy Ŀǰ�ײ�ֻ��ͨ��adb���ֻ����в���������sdcardĿ¼�����ļ��Ĳ���������ɹ��Ǿ�ֱ�Ӹ���ʾ�ˣ��ȵײ�֧������ȥ����δ���
	if (pDevice->GetDevInfo()->_deviceType == Android)
	{
		wstring path = pDir->GetFullPath();

		wstring sdCardPath =  pDevice->GetDevInfo()->_sdcardpath;
		sdCardPath = CStrOperation::trimRightW(sdCardPath, L"/");
		bool isCreateFile = (path.find(sdCardPath)==0);
		if (!isCreateFile)
		{
			wstring msg = CommonUIFunc::LoadString(L"FM_AddFolderFailNoAuthority");
			CommonUIFunc::MessageDialog(msg,
				CommonUIFunc::LoadString(L"ERROR"));
			LOG->WriteDebug(L"android�豸��Ȩ�޴����ļ��з���ʧ��");
			return _T("");
		}
	}

    wstring name;
    wstring defaultName;
    do 
    {
        bool bCancel;
        if (defaultName.empty())
            defaultName = CommonUIFunc::LoadString(L"FM_AddFolder");
        name = CommonUIFunc::InputDialog(CommonUIFunc::LoadString(L"FM_AddFolderPrompt"),
            CommonUIFunc::LoadString(L"FM_AddFolder"),
            defaultName,
            bCancel,
			NEWFOLDERNAMELENG);
        if (bCancel || _check_exit)
			return _T("");

		bool flag = true;
		for (size_t i=0;i<name.size();i++)
		{
			if (name.at(i) != L'.')
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{		//ȫ������ . ������
			CommonUIFunc::NoticeDialog(CommonUIFunc::LoadString(_T("FM_AllSameDot")), ICON_WARNING);
			return _T("");
		}

		_pUI->MyTrim(name);

        defaultName = name;

		string str = CCodeOperation::ws2s(name);
		if (pDevice->GetDevInfo()->_deviceType == Android 
			&& name.length() > FOLDERNAMEMAX)
		{
			CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_AddFolderFailNameTooLong"),
				CommonUIFunc::LoadString(L"ERROR"));
			LOG->WriteDebug(L"�����ļ���ʧ�ܣ��ļ�������");
			continue;
		}
        else if ( name.empty() )
        {
            CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_EmptyNotAllowed"),
                CommonUIFunc::LoadString(L"PROMPT"));
			LOG->WriteDebug(L"�����ļ���ʧ�ܣ��ļ���Ϊ��");
            continue;
        }
        else if ( pDir->findChildrenbyName(name,true,false) )   // ����ͬ���ӽڵ�
        {
            CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_SameName"),
                CommonUIFunc::LoadString(L"ERROR"));
			LOG->WriteDebug(L"�����ļ���ʧ�ܣ�����ͬ���ļ���");
            continue;
        }
		else if (pDevice->GetDevInfo()->_deviceType == Android 
				&&wstring::npos==name.find_first_not_of(L"."))
		{
			CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_AllSameDot"),
				CommonUIFunc::LoadString(L"ERROR"));
			LOG->WriteDebug(L"�����ļ���ʧ�ܣ�����ȫ��Ϊ.");
			continue;
		}
        

        break;
    } while (true);

    bool res = false;
    try
    {
        res = pDir->MakeSubDir(name);
    }
    catch (...)
    {
        return _T("");
    }

    if (!res)
	{
        if (pDevice->GetDevInfo()->_deviceType == Ios)
				CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_AddFolderFailNameTooLong"),
					CommonUIFunc::LoadString(L"ERROR"));
        else
            CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_AddFolderFailNoAuthority"),
                CommonUIFunc::LoadString(L"ERROR"));

		LOG->WriteDebug(L"�����ļ��з���ʧ��");
        return _T("");
    }	
    return name;
}

void FileManagerModel::RefreshData()
{
    _pRoot = NULL;
    LoadData();
}

bool FileManagerModel::Download(wstring destDir, vector<NodeData*>* pNodes, bool bShowFinishMsg, bool bWaitFinish )
{
    RETURN_FALSE_IF(pNodes->empty());

	string request = CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());

	_downloadThread._p = pNodes;
	_downloadThread._str = destDir;
	_downloadThread._bool = bShowFinishMsg;
	_downloadThread._int = atoi(request.c_str());


 	HANDLE handle = CreateThread(NULL, 0, FileManagerModel::DoDownload, (LPVOID)this, CREATE_SUSPENDED, NULL);
	
	_progress = new ProgressRecvMsg;
	_progress->SetBtnState(true,false);
 	_progress->Init(Singleton::GetMainWnd(),NULL,handle);


	_ob = new BackUpAndRestoreObserver;	//������ʵ������
	ProgressShowMsg showmsg;
	showmsg.RequestID = atoi(request.c_str());
	showmsg.SendToUI();
	delete _progress;
	_progress = NULL;
	SAFE_DELETE(_ob);




	return true;
}

void FileManagerModel::Paste(vector<wstring>* pPasteNodes, bool bCut, DirData* pCurDir)
{
 //   RETURN_IF(pPasteNodes->empty() || pCurDir == NULL);

//     BaseParam *para = new BaseParam;
//     para->SetVoid(pPasteNodes);
//     para->SetInt((int)pCurDir);
//     para->SetBool(bCut);
//     StartActionThread(Action_FM_Paste, para, true, false);


}

void FileManagerModel::UploadFile(vector<wstring>* pFiles, DirData* pCurDir)
{
    RETURN_IF(pFiles->empty());

	string request = CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());

	_uploadFileThread._p = pFiles;
	_uploadFileThread._ver_p.clear();
	_uploadFileThread._ver_p.push_back(pCurDir);
	_uploadFileThread._int = atoi(request.c_str());

	HANDLE handle = CreateThread(NULL, 0, FileManagerModel::DoUploadFile, (LPVOID)this, CREATE_SUSPENDED, NULL);
	_progress = new ProgressRecvMsg;
	_progress->SetBtnState(true,false);
	_progress->Init(Singleton::GetMainWnd(),NULL,handle);

	_ob = new BackUpAndRestoreObserver;	//������ʵ������

	ProgressShowMsg showmsg;
	showmsg.RequestID = atoi(request.c_str());
	showmsg.SendToUI();
	delete _progress;
	SAFE_DELETE(_ob);
	_progress = NULL;
}

void FileManagerModel::UploadDir(wstring dir, DirData* pCurDir)
{
    RETURN_IF(dir.empty());
	string request = CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());

	_uploadDirThread._str = dir;
	_uploadDirThread._ver_p.clear();
	_uploadDirThread._ver_p.push_back(pCurDir);
	_uploadDirThread._int = atoi(request.c_str());

	HANDLE handle = CreateThread(NULL, 0, FileManagerModel::DoUploadDir, (LPVOID)this, CREATE_SUSPENDED, NULL);;
	_progress = new ProgressRecvMsg;
	_progress->SetBtnState(true,false);
	_progress->Init(Singleton::GetMainWnd(),NULL,handle);

	_ob = new BackUpAndRestoreObserver;	//������ʵ������

	ProgressShowMsg showmsg;
	showmsg.RequestID = atoi(request.c_str());
	showmsg.SendToUI();
	delete _progress;
	_progress = NULL;
	SAFE_DELETE(_ob);

}

void FileManagerModel::Delete( vector<NodeData*>* pDeleteNodes ,DirData* pCurDir)
{
    RETURN_IF(pDeleteNodes->empty());
	string request = CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());

	_deleteThread._ver_p.clear();
	_deleteThread._ver_p.push_back(pDeleteNodes);
	_deleteThread._ver_p.push_back(pCurDir);
	_deleteThread._int = atoi(request.c_str());

	HANDLE handle = CreateThread(NULL, 0, FileManagerModel::DoDelete, (LPVOID)this, CREATE_SUSPENDED, NULL);
	_progress = new ProgressRecvMsg;
	_progress->SetBtnState(true,false);
	_progress->Init(Singleton::GetMainWnd(),NULL,handle);

	_ob = new BackUpAndRestoreObserver;	//������ʵ������

	ProgressShowMsg showmsg;
	showmsg.RequestID = atoi(request.c_str());
	showmsg.SendToUI();
	delete _progress;
	_progress = NULL;
	SAFE_DELETE(_ob);

}

const int Mega = 1024 * 1024;
const int Giga = 1024 * 1024 * 1024;

void FileManagerModel::RefreshSpaceInfo( wstring path )
{
return;

	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(_deviceID);
	if(pDevice == NULL)
		return;

    wchar_t buffer[MAX_PATH];

    // ȡ�洢�ռ���Ϣ
    if (pDevice->GetDevInfo()->_deviceType == Android )
    {
		if (path == _T("/"))
		{//andirod��Ŀ¼�޷�ȡ�ÿռ�
			return;
		}
		
        int64_t used = 0;
        int64_t available = 0;
		for (int i=0;i<5;i++)
		{
			RETURN_IF(! pDevice->GetRootData()->GetAndroidSpaceInfo(path, used, available));
			if (used == 0 && available == 0)
			{
				Sleep(100);
				continue;
			}
			else 
				break;
		}
        
        // ������ʾ����
        const wchar_t* pTemplate = CommonUIFunc::LoadString(_T("FM_AndroidSpaceInfo"));
        RETURN_IF ( ! pTemplate );
        swprintf_s(buffer, MAX_PATH, pTemplate,
            CStrOperation::FormatFileSize(used + available).c_str(),
            CStrOperation::FormatFileSize(used).c_str(),
            CStrOperation::FormatFileSize(available).c_str());
    }
    else if (pDevice->GetDevInfo()->_deviceType == Ios)
    {
        pDevice->GetRootData()->GetSpaceInfo(RootData::PHONEDATA, &_si);
        const wchar_t* pTemplate = CommonUIFunc::LoadString(_T("FM_IosSpaceInfo"));
        swprintf_s(buffer, MAX_PATH, pTemplate,
            CStrOperation::FormatFileSize(_si._systemSize).c_str(),
            CStrOperation::FormatFileSize(_si._systemAvailable).c_str(),
            CStrOperation::FormatFileSize(_si._totalsize - _si._systemSize).c_str(),
            CStrOperation::FormatFileSize(_si._nAmountDataAvailable).c_str());
    }
    else
        return;

    _pUI->ShowSpaceInfo(buffer);
}

void FileManagerModel::DownloadAndOpenFile(NodeData* pNode)
{
    RETURN_IF( !pNode );

    // ��������ӣ���������ָ���ʵ�ʽڵ㣨�������ļ���
    if (pNode->_type == NodeData::LINK)
    {
        LinkData* pLink = dynamic_cast<LinkData*>(pNode);
        RETURN_IF ( ! pLink );

        pNode = pLink->GetTarget();
        RETURN_IF( !pNode );
        RETURN_IF( !pNode->_type != NodeData::FILE );
    }

    RETURN_IF ( 0 == pNode->_size );    // ���ļ�������
	if (pNode->_size > (1024*1024 *10))
	{
		if (!CommonUIFunc::ConfirmDialog(CommonUIFunc::LoadString(_T("FM_DownLoadMsg")),CommonUIFunc::LoadString(_T("FM_DownFileTitle")),_T(""),0,MY_MB_YESNO))
 			return;

		RETURN_IF(_check_exit);		//�豸���γ��˳�
	}

    // ���ص���ʱ�ļ���
    RenameFile rf;
    wstring tempDir = DataPath::GetTempDir();
    vector<NodeData*> nodes;
    nodes.push_back(pNode);

	TransferResult ret = Transfer_Success;
	try
	{
		ret = FileHelper::Download(&nodes, tempDir, &rf);
	}
	catch(...)
	{
		ret = Transfer_Fail;        
	}

	if ( Transfer_Success == ret )
    {
        wstring filename = pNode->GetFileToPC();
        wstring prompt = CommonUIFunc::LoadString(L"FM_FileDownTo");
        prompt.push_back('\n');
        prompt += filename;

        SHELLEXECUTEINFO sei = { sizeof(sei) };
        sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI; //SEE_MASK_INVOKEIDLIST ;
        sei.nShow = SW_SHOWNORMAL; 
        sei.lpVerb = L"open";
        sei.lpFile = filename.c_str();

        if ( ! ShellExecuteEx(&sei) )
        {
            sei.fMask = SEE_MASK_INVOKEIDLIST ;
            sei.lpVerb = L"openas";
            ShellExecuteEx(&sei);
        }
    }
    else
    {
        wstring filename = pNode->GetFileToPC();
        CFileOperation::DeleteFile(filename);
        CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_OpenFileFail"),
            CommonUIFunc::LoadString(L"PROMPT"));
    }
}

bool FileManagerModel::IsUserDir(const wchar_t* pPath )
{
    return wcsstr(pPath, L"/sdcard") 
        || wcsstr(pPath, L"/private/var/mobile/Media");
}

void FileManagerModel::HideProWnd(int Requestid)
{
	ProgressMsg progressmsg;
	progressmsg.wndHide = true;
	progressmsg.nRequestID = Requestid;
	progressmsg.SendToUI();
}

DWORD WINAPI FileManagerModel::DoDownload(LPVOID lpParameter)
{
	FileManagerModel * p = (FileManagerModel*)lpParameter;
    p->_bLastDownloadSuccess = false;
    vector<NodeData*>* pNodes = (vector<NodeData*>*)p->_downloadThread._p;
    RETURN_FALSE_IF(!pNodes);

	p->_ob->_RequestID = p->_downloadThread._int;
	p->_ob->_pegresstype = FILEMANAGER;

	TransferResult ret = Transfer_Success;
	try
	{
		ret = FileHelper::Download(pNodes, p->_downloadThread._str,p->_ob);
	}
	catch(...)
	{
		ret = Transfer_Fail;        
	}

	p->HideProWnd(p->_downloadThread._int);
    bool bShowFinishMsg = true;
    bShowFinishMsg = p->_downloadThread._bool;

    if ( Transfer_Success == ret && bShowFinishMsg)
    {
        //Notice(CommonUIFunc::LoadString(L"FM_DownlaodDone"), 2000);        
    }
    else if (Transfer_Fail == ret)
    {
        CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_DownloadFail"),
                                    CommonUIFunc::LoadString(L"FM_DownFileTitle"));
    }

    delete pNodes;
    p->_bLastDownloadSuccess = Transfer_Success == ret;
    return p->_bLastDownloadSuccess;
}

DWORD WINAPI FileManagerModel::DoUploadFile(LPVOID lpParameter)
{
	FileManagerModel *p = (FileManagerModel*)lpParameter;
    vector<wstring>* pFiles = (vector<wstring>*)p->_uploadFileThread._p;
	if (pFiles == NULL)
	{
		p->HideProWnd(p->_uploadFileThread._int);
		return false;
	}
	DirData* pCurDir = NULL;
	for (vector<void*>::const_iterator it = p->_uploadFileThread._ver_p.begin(); it != p->_uploadFileThread._ver_p.end(); it++)
		pCurDir = (DirData*)(*it);

	if (pCurDir == NULL)
	{
		p->HideProWnd(p->_uploadFileThread._int);
		return false;
	}
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(p->_deviceID);
	if(pDevice == NULL)
	{
		p->HideProWnd(p->_uploadFileThread._int);
		return false;
	}
    vector<wstring> temp = *pFiles;
    vector<wstring> tempFiles;
    vector<wstring> tempDir;

	int64_t count_size = 0;
    for (size_t i = 0; i < pFiles->size(); i++)
    {
        if (CFileOperation::IsDirExist(pFiles->at(i).c_str()))
		{
            tempDir.push_back(pFiles->at(i));
			int nodesum=0;
			__int64 sizesum=0;
			CFileOperation::GetDirSizeW(pFiles->at(i),nodesum,sizesum);
			count_size += sizesum;
		}
		else if (CFileOperation::IsFileExist(pFiles->at(i).c_str()))
		{
			tempFiles.push_back(pFiles->at(i));
			count_size += CFileOperation::GetFileSizeW(pFiles->at(i));
		}
    }
	
	bool size_flag = true;
	if (pDevice->GetDevInfo()->_deviceType == Android)
	{	//���ڲ����ֻ� ������ i9500 �ػ��޷���ȡ��Ŀ¼�ռ䣬�����޷��ϴ�

	}
	else if (pDevice->GetDevInfo()->_deviceType == Ios)
	{ 
		pDevice->GetRootData()->GetSpaceInfo(RootData::PHONEDATA, &p->_si);

		bool flag = true;
		if (pDevice->GetDevInfo()->_ios_bJailBreaked)
		{//δԽ����ȡ�ռ�ȫ��Ϊ�ֻ��ռ� 
			flag = pCurDir->IsUserSpace();
		}
		int64_t count_num = 0;
		if (flag == false)	//ϵͳ�ռ�
			count_num = p->_si._systemAvailable;
		else
			count_num = p->_si._nAmountDataAvailable;
		if (count_size > count_num)
		{
			//������С�����ϴ�
			wchar_t msg[1024] = {0};
			wsprintf(msg,_T("IOS--------�ϴ��ļ��пռ䣺%d  �豸�ܿռ䣺%d"),count_size,p->_si._nAmountDataAvailable);
			LOG->WriteDebug(msg);		
			size_flag = false;
		}
	}

	
	p->_ob->_RequestID = p->_uploadFileThread._int;
	p->_ob->_pegresstype = FILEMANAGER;
    TransferResult ret = Transfer_Unknown;
	if (size_flag)
	{
		
		if(!p->DirIsPermissions(pCurDir))
		{
			p->HideProWnd(p->_uploadFileThread._int);
			CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_DirNoPermissions"),
				CommonUIFunc::LoadString(L"FM_DirWaringTtitle"));
			p->_refresh_node_ver.clear();
			p->_refresh_node_ver = temp;
			p->SendRefreshToUI(RefreshNode_And_SelectList);
			return false;
		}

		for (size_t i = 0; i < tempDir.size(); i++)
        {
            ret = pCurDir->UploadDir(tempDir.at(i), p->_ob);
			if (p->_check_exit)
			{
				p->HideProWnd(p->_uploadFileThread._int);
				return false;
			}

		}

		if (!tempFiles.empty())
        {
            ret = pCurDir->UploadFile(tempFiles, p->_ob);
			if (p->_check_exit)
			{
				p->HideProWnd(p->_uploadFileThread._int);
				return false;
			}
		}
	}
	else
		ret = Transfer_LackOfSpace;	//�ռ䲻����ʾ
    

	p->HideProWnd(p->_uploadFileThread._int);
	
    if ( Transfer_Fail == ret )
    {
        CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_UploadFail"),
            CommonUIFunc::LoadString(L"FM_UploadFileTitle"));
    }
	else if ( Transfer_LackOfSpace == ret )
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_LackOfSpace"),
			CommonUIFunc::LoadString(L"FM_UploadFileTitle"));
	}
    else if (Transfer_Cancel == ret )
    {
        CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_OperationCancel"),
            CommonUIFunc::LoadString(L"FM_UploadFileTitle"));
    }

	p->_refresh_node_ver.clear();
	p->_refresh_node_ver = temp;
	p->SendRefreshToUI(RefreshNode_And_SelectList);
	//�ֻ��ռ䷢���仯������Ϣ
//	PostMessage(MainDlg::GetInstance()->GetHWND(), WM_DEVICE_SPACECHANGED, (WPARAM)BaseOperation::AllocateBuffer(GetCurrentDeviceID()), 0);

    return true;
}

bool FileManagerModel::DirIsPermissions(DirData* pCurDir)
{
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(_deviceID);
	if (pDevice->GetDevInfo()->_deviceType == Ios)
		return true;		//ios�����ж�
	if (!pCurDir)
	{
		LOG->WriteDebug(_T("·��Ϊ��"));
		return false;
	}

	TransferResult ret = Transfer_Unknown;
	vector <wstring> files;
	files.push_back(CONFIG(_strBackupPath)+_T("FileManager_DirIsPermissions.txt"));
	HANDLE hOpenFile = (HANDLE)CreateFileW(files.at(0).c_str(),
	GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_NEW, NULL, NULL); 
	ret = pCurDir->UploadFile(files,NULL);
	if ( Transfer_Fail == ret )
	{
		LOG->WriteDebug(_T("�ϴ��ļ�ʧ��"));
		return false;
	}
	deque<NodeData*> queue = pCurDir->GetChildrens();
	vector<NodeData*> vet;
	for (size_t i=0;i<queue.size();i++)
	{
		if (queue.at(i)->_name == _T("FileManager_DirIsPermissions.txt"))
		{
			vet.push_back(queue.at(i));
			break;
		}
	}

	FileHelper::BatchDelete(&vet,NULL);

	return true;
}



DWORD WINAPI FileManagerModel::DoUploadDir(LPVOID lpParameter)
{
	FileManagerModel *p = (FileManagerModel*)lpParameter;
    wstring dir = p->_uploadDirThread._str;
    DirData* pCurDir = NULL;
    for (vector<void*>::const_iterator it = p->_uploadDirThread._ver_p.begin(); it != p->_uploadDirThread._ver_p.end(); it++)
        pCurDir = (DirData*)(*it);
	if (pCurDir == NULL)
	{
		p->HideProWnd(p->_uploadDirThread._int);
		return false;
	}

	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(p->_deviceID);
	if(pDevice == NULL)
	{
		p->HideProWnd(p->_uploadDirThread._int);
		return false;
	}

	int nodesum=0;
	__int64	sizesum=0;
   	CFileOperation::GetDirSizeW(dir,nodesum,sizesum);
	bool size_flag = true;
	if (pDevice->GetDevInfo()->_deviceType == Android)
	{
		//���ڲ����ֻ� ������ i9500 �ػ��޷���ȡ��Ŀ¼�ռ䣬�����޷��ϴ�
	}
	else if (pDevice->GetDevInfo()->_deviceType == Ios)
	{
		pDevice->GetRootData()->GetSpaceInfo(RootData::PHONEDATA, &p->_si);
		if (sizesum > p->_si._nAmountDataAvailable)
		{
			wchar_t msg[1024] = {0};
			wsprintf(msg,_T("IOS--------�ϴ��ļ��пռ䣺%d  �豸�ܿռ䣺%d"),sizesum,p->_si._nAmountDataAvailable);
			LOG->WriteDebug(msg);
			//������С�����ϴ�
			size_flag = false;
		}
		
	}
	
	string str = CCodeOperation::ws2s(dir);
	if (str.length() > FOLDERNAMEMAX)
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_FileNameLeng"),
		CommonUIFunc::LoadString(L"ERROR"));
		p->HideProWnd(p->_uploadDirThread._int);
		return false;
	}

	if(!p->DirIsPermissions(pCurDir))
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_DirNoPermissions"),
			CommonUIFunc::LoadString(L"FM_DirWaringTtitle"));
		p->HideProWnd(p->_uploadDirThread._int);
		return false;
	}

	p->_ob->_RequestID = p->_uploadDirThread._int;
	p->_ob->_pegresstype = FILEMANAGER;

	TransferResult ret = Transfer_Unknown;
	if (size_flag)
	{
		ret = pCurDir->UploadDir(dir, p->_ob);
		RETURN_FALSE_IF (p->_check_exit);
	}
	else 
		ret=Transfer_LackOfSpace;


	p->HideProWnd(p->_uploadDirThread._int);

	if ( Transfer_Success == ret ) {
		//Notice(CommonUIFunc::LoadString(L"FM_UploadDone"), 2000);
	}
	else if ( ret == Transfer_Fail )
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_UploadFail"),
			CommonUIFunc::LoadString(L"ERROR"));
	}
	else if ( ret == Transfer_LackOfSpace )
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_LackOfSpace"),
			CommonUIFunc::LoadString(L"ERROR"));
	}
	else if ( ret == Transfer_Cancel )
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_OperationCancel"),
			CommonUIFunc::LoadString(L"PROMPT"));
	}

	p->_refresh_node_ver.clear();
	p->_refresh_node_ver.push_back(dir);

	p->SendRefreshToUI(RefreshNode_And_SelectList);

	//�ֻ��ռ䷢���仯������Ϣ
//	PostMessage(MainDlg::GetInstance()->GetHWND(), WM_DEVICE_SPACECHANGED, (WPARAM)BaseOperation::AllocateBuffer(GetCurrentDeviceID()), 0);

    return true;
}

void FileManagerModel::SendRefreshToUI(Refresh_Node_flag flag)
{
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(_deviceID);
	if(pDevice == NULL)
		return;

	RefreshNodeMsg msg;
	msg._str = pDevice->GetId();
	msg._int = flag;
	msg.SendToUI();

}

DWORD WINAPI FileManagerModel::DoDelete(LPVOID lpParameter)
{
	FileManagerModel *p = (FileManagerModel *)lpParameter;
	if(!p->DirIsPermissions((DirData*)p->_deleteThread._ver_p.at(1)))
	{
		CommonUIFunc::MessageDialog(CommonUIFunc::LoadString(L"FM_DirNoPermissions"),
			CommonUIFunc::LoadString(L"FM_DirWaringTtitle"));
		p->HideProWnd(p->_deleteThread._int);
		return false;
	}

    vector<NodeData*>* pDeleteNodes = (vector<NodeData*>*)p->_deleteThread._ver_p.at(0);
	if (!pDeleteNodes)
	{
		p->HideProWnd(p->_deleteThread._int);
		return false;
	}

	
	p->_ob->_RequestID = p->_deleteThread._int;
	p->_ob->_pegresstype = FILEMANAGER;
    if (NodeData* pNode = FileHelper::BatchDelete(pDeleteNodes, p->_ob))
        CommonUIFunc::MessageDialog(pNode->GetFullPath()+CommonUIFunc::LoadString(L"FM_DeleteFail"),
                                    CommonUIFunc::LoadString(L"ERROR"));

	p->HideProWnd(p->_deleteThread._int);
	//SAFE_DELETE(pDeleteNodes);

	p->SendRefreshToUI(RefreshNode);
	//�ֻ��ռ䷢���仯������Ϣ
//	PostMessage(MainDlg::GetInstance()->GetHWND(), WM_DEVICE_SPACECHANGED, (WPARAM)BaseOperation::AllocateBuffer(GetCurrentDeviceID()), 0);
    return true;
}

void FileManagerModel::RestoreAppNodeName(DirData* pTreeNode)
{
	if (!_restoreAppName)		//��֤�ڽڵ�仯ǰ��ڵ�ˢ��ǰֻ����һ��
		return;
	
	if (pTreeNode && pTreeNode->_name == _T("Applications"))
	{
		_loadAppThread._p = pTreeNode;

		CreateThread(NULL, 0, FileManagerModel::DoRestoreAppName, (LPVOID)this, 0, NULL);
	}
}

DWORD WINAPI FileManagerModel::DoRestoreAppName(LPVOID lpParameter)
{
	FileManagerModel *p = (FileManagerModel *)lpParameter;
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(p->_deviceID);
	if(pDevice == NULL)
		return false;

	SetCSLockExt(*p->GetLock());
	p->_thread_is_run = true;
	DirData *data = (DirData *)p->_loadAppThread._p;

	p->_restoreAppName = false;
	deque<NodeData*> pChilds = data->GetChildrens();
	deque<NodeData*>::iterator it;
	for( it = pChilds.begin(); it != pChilds.end(); ++it )
	{
		FileHelper::IosGetDisplayName(*it, pDevice);
		RefreshAppMsg msg;
		msg.sn = pDevice->GetId();
		msg.SendToUI();
	}
	
	p->_thread_is_run = false;
	    
    return true;
}

bool FileManagerModel::GetTreadState()
{
	return _thread_is_run;
}

void FileManagerModel::SetRestortAppName( bool flag )
{
	_restoreAppName = flag;
}

void FileManagerModel::SetHistoryEnable( bool val )
{
//	_history.EnableGoto(val);
	_addHistory = val;
}

void FileManagerModel::CreateDirThread()
{
	_handle = CreateThread(NULL, 0, FileManagerModel::DoCreateDir, (LPVOID)this, 0, NULL);
}

DWORD WINAPI FileManagerModel::DoCreateDir(LPVOID lpParameter)
{
	FileManagerModel *p = (FileManagerModel *)lpParameter;
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(p->_deviceID);
	if(pDevice == NULL)
		return false;

	if (pDevice->GetDevInfo()->_deviceType != Ios)
		return false;

	wstring flash_disk_str;
	wstring mydocument_str;

	const DeviceInfo *info = pDevice->GetDevInfo();
	if (info->_ios_bJailBreaked)	//���Խ��
	{
		flash_disk_str = _T("/var/mobile/Media/general_storage");
		mydocument_str = _T("/var/mobile/Media/MyDocuments");
	}
	else
	{
		flash_disk_str = _T("/general_storage");
		mydocument_str = _T("/MyDocuments");
	}


	FileHelper::CreateDir(pDevice,flash_disk_str);

	FileHelper::CreateDir(pDevice,mydocument_str);
    return true;
}

void FileManagerModel::LoadDefaultData()
{
	CreateThread(NULL, 0, FileManagerModel::LoadRootData, (LPVOID)this, 0, NULL);
}

bool FileManagerModel::IsDefaultLoaded()
{
	return _pRoot && _pVirtualRoot;
}

void FileManagerModel::Release()
{
	_check_exit = true;
}

std::wstring FileManagerModel::GetSDCardPath()
{
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(_deviceID);
	if(pDevice != NULL)
		return pDevice->GetDevInfo()->_sdcardpath; 

	return L"";
}

ND91Assistant::SDCardState FileManagerModel::GetSDCardState()
{
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(_deviceID);
	if(pDevice != NULL)
		return pDevice->GetSystemManager()->GetSDCardState(); 

	return UNMOUNTED;
}

DEVICE_OS_TYPE FileManagerModel::GetDeviceOsType()
{
	DeviceData* pDevice = FileManagerCommonFunc::FindDevice(_deviceID);
	if(pDevice != NULL)
		return pDevice->GetDevInfo()->_deviceType; 

	return Android;
}

DWORD WINAPI FileManagerModel::LoadRootData(LPVOID lpParameter)
{
	FileManagerModel *p = (FileManagerModel *)lpParameter;
	p->_pUI->ShowLoading(true);
	p->LoadData();
	p->_pUI->ShowLoading(false);
	return true;
}
