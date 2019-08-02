#include "stdafx.h"

#include "Module/File/DirData.h"

#include "DirectoryOperationCommand.h"
#include "ListDirectoryCommand.h"
#include "UploadFileCommand.h"
#include "DownloadFileCommand.h"
#include "CopyFileCommand.h"
#include "Module/File/FileHelper.h"

#include "Core/DeviceData.h"
#include "Core/International.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

DirData::DirData(const DeviceData* pDevice)
: NodeData(NodeData::DIR)
,_pDeviceData(pDevice)
{
	_eRootType   = RootData::UNKNOWN;
    _hasLoad     = false;
	InitializeCriticalSection(&_lock);
}

DirData::~DirData()
{
    ClearChilds();
	DeleteCriticalSection(&_lock);
}

void DirData::ClearChilds()
{
    for (size_t i=0; i<_childNodes.size();i++)
    {
        //delete _childNodes[i];
        SAFE_DELETE(_childNodes[i]);
    }
    _childNodes.clear();
}

bool DirData::Rename( wstring dest )
{
    DirectoryOperationCommand cmd(_pDeviceData);

    RETURN_FALSE_IF (! cmd.Rename(GetFullPath(), GetParentPath()+dest) );

    _name = dest;
    return true;
}

bool DirData::ChangeMode( string mode )
{
    DirectoryOperationCommand cmd(_pDeviceData);

    RETURN_FALSE_IF ( ! cmd.ChangeMode(GetFullPath(), mode) );

    _mode = mode;
    return true;
}

bool DirData::GetMode(string& mode, wstring& owner)
{
    DirectoryOperationCommand cmd(_pDeviceData);
    RETURN_FALSE_IF ( ! cmd.GetMode(GetFullPath(), mode, owner) );
    return true;
}

bool DirData::Move(vector<NodeData*>* pNodes, ProgressObserver* pObserver)
{
    DirectoryOperationCommand cmd(_pDeviceData);
    cmd.Attach(pObserver);

    cmd.SetTaskNumberSum(pNodes->size());
    cmd.SetAllTaskSum(0);
    cmd.SetCurrentTaskSum(0);
    cmd.SendProgress();

    for(size_t i = 0; i < pNodes->size(); ++i)
    {
        IF_CANCEL_BREAK(pObserver)

        cmd.SetCurrentTaskName(pNodes->at(i)->GetFullPath());

        RETURN_FALSE_IF( ! DoMove(pNodes->at(i), &cmd));
        cmd.IncreaseTaskNumberComplete();
        cmd.SendProgress();
    }    
    cmd.Complete();
    IF_CANCEL_RETURN_FALSE(pObserver)
    return true;
}


bool DirData::DoMove( NodeData* pNode, DirectoryOperationCommand* pCmd)
{
    RETURN_FALSE_IF( ! pNode );

    // 若pNode是本目录的上层目录，返回
    RETURN_FALSE_IF( IsParentDir(pNode) );

    // 若pNode就已经在本目录下，返回 （改为TRUE by Litz ,因为结果是正确的）
    RETURN_TRUE_IF( this == pNode->_Parent );

    wstring destName = GetFullPath() + pNode->_name;
    wstring newName = pNode->_name;
    if ( pCmd->Exist(destName) )
    {
        switch ( pCmd->AskUser(destName) )
        {
        case CANCEL:
            return false;

        case OVERWRITE:
        case OVERWRITE_ALL:
            if ( pNode->_type != NodeData::DIR )
                RETURN_FALSE_IF( ! pCmd->DeleteFileLink(destName) );
            break;

        case RENAME:
        case RENAME_ALL:
            newName = pNode->_type == DIR ? PhoneDirRename(newName) : PhoneFileRename(newName, pCmd);
            destName = GetFullPath() +  newName;
            break;
        }
    }

    return pCmd->Move(pNode->GetFullPath(), destName);
}

NodeData* DirData::Insert(const NodeData* pNode,bool replace)
{
	if (replace)
		RETURN_FALSE_IF( findChildrenbyName(pNode->_name) );

    NodeData* pNewNode = pNode->Clone(this);

    _childNodes.push_back(pNewNode);

    return pNewNode;
}

bool DirData::Refresh()
{
	MyLock lock(&_lock);

    ListDirectoryCommand lsCmd(_pDeviceData, NULL);
    return DoLoad(&lsCmd);
}

void DirData::Load()
{
	MyLock lock(&_lock);
    RETURN_IF( _hasLoad );
    Refresh();
}

bool DirData::DoLoad(ListDirectoryCommand* pCmd)
{
	MyLock lock(&_lock);
    pCmd->SetNode(this);
    pCmd->Execute();

    _hasLoad  = pCmd->DoneSuccess();
    return _hasLoad;
}

bool DirData::RecursiveLoad(int& totalSize, int& totalCounts, wstring ExtName)
{
    ListDirectoryCommand lsCmd(_pDeviceData, NULL);

    return DoRecursiveLoad(totalSize, totalCounts, &lsCmd, ExtName);
}

bool DirData::DoRecursiveLoad(int& totalSize, int& totalCounts, ListDirectoryCommand* pCmd, wstring ExtName)
{
	MyLock lock(&_lock);
    if (!_hasLoad)
    {
        RETURN_FALSE_IF ( ! DoLoad(pCmd) );
    }
    int iCountType = 0;
    OPTION->Get(GlobalOption::INT_FILETASK_COUNTMODE, iCountType);

    for(size_t i = 0; i < _childNodes.size(); i++)
    {
        if (_childNodes[i]->_type == DIR && iCountType > 0 
            || _childNodes[i]->_type == FILE 
            || _childNodes[i]->_type == LINK && iCountType > 1)
        {
            totalCounts++;
        }

        DirData* pDir = dynamic_cast<DirData*>(_childNodes[i]);
        if ( pDir ) // 是目录就递归
        {
            pDir->DoRecursiveLoad(totalSize, totalCounts, pCmd, ExtName);
        }
        else if(_childNodes[i]->_type == FILE && (ExtName.empty() || CStrOperation::endsWithW(CStrOperation::toLowerW(_childNodes[i]->_name), CStrOperation::toLowerW(ExtName)))) // 是文件就加大小
        {
            totalSize += _childNodes[i]->_size;
        }

    }
    return true;
}

bool DirData::IsExist(wstring subName)
{
    deque<NodeData*> nodes = GetChildrens();
    for (deque<NodeData*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if (CStrOperation::CompareUnCase(subName, (*it)->_name) == 0)
            return true;
    }
    return false;
}

bool DirData::MakeSubDir(wstring subDirName)
{
    DirectoryOperationCommand cmd(_pDeviceData);
    if (IsExist(subDirName))
        return false;
    return DoMakeSubDir(subDirName, &cmd);
}

bool DirData::DoMakeSubDir(wstring subDirName, DirectoryOperationCommand* pCmd)
{    
    RETURN_FALSE_IF( ! pCmd->MakeDir(GetFullPath() + subDirName));

    SetNodeAttribute(AddDir(), subDirName);
    return true;
}


NodeData* DirData::Clone( DirData* pParent ) const
{
    DirData* pNew = new DirData(_pDeviceData);

    pNew->_name       = this->_name      ;
    pNew->_mode       = this->_mode      ;
    pNew->_owner      = this->_owner     ;
    pNew->_group      = this->_group     ;
    pNew->_size       = this->_size      ;
    pNew->_modifyTime = this->_modifyTime;
    pNew->_createTime = this->_createTime;
    pNew->_Parent     = pParent;

    pNew->_hasLoad    = _hasLoad;
    pNew->_pDeviceData= _pDeviceData;

    for( size_t i = 0 ; i < this->_childNodes.size() ; ++i )
    {
        pNew->_childNodes.push_back(this->_childNodes[i]->Clone(pNew));
    }

    return pNew;
}

bool DirData::IsParentDir(const NodeData* pNode ) const
{
    const DirData* p = dynamic_cast<const DirData*>(pNode);
    if ( ! p )
        return false;

    // 逐层往上找，若有此目录则返回true
    DirData* pDir = _Parent;
    while(pDir->_Parent)
    {
        if (pDir->GetFullPath() == p->GetFullPath())
            return true;

        pDir = pDir->_Parent;
    }

    return false;
}

bool DirData::ExistInDevice() const
{
    DirectoryOperationCommand  cmd(_pDeviceData);
    return cmd.Exist(GetFullPath());
}

void DirData::RemoveFromChildrens( NodeData* pNode )
{
    for(deque<NodeData*>::iterator it = _childNodes.begin() ; it != _childNodes.end(); ++it)
    {
        if((*it)->GetFullPath() == pNode->GetFullPath())
        {
            //delete (*it);
            SAFE_DELETE(*it);
            _childNodes.erase(it);
            break;
        }
    }
}

#define NEWFOLDERNAMELENG  80
void DirData::FileNameCheck( wstring &filename )
{
    RETURN_IF(filename.length() <= NEWFOLDERNAMELENG);
    int flag1 = filename.find_last_of('\\');	//文件名开始
    int flag2 = filename.find_last_of('.');	//文件名结束
    wstring buf1,buf2,buf3;
    if (flag2 != -1)
        buf2 = filename.substr(flag2,filename.length());

    buf1 = filename.substr(flag1+1,NEWFOLDERNAMELENG);
    filename = filename.substr(0,flag1+1);
    filename = filename + buf1+_T("__")+buf2;
}

TransferResult DirData::UploadFile(vector<wstring> paths, ProgressObserver* pObserver,bool bSetAttr)
{
    RETURN_VALUE_IF( paths.empty() , Transfer_Unknown);

    if (pObserver)
        pObserver->GetPrompt(MESSAGE_UPLOAD);

    Load();

    UploadFileCommand cmd(_pDeviceData);
	cmd.SetIsSetAttr(bSetAttr);
    DirectoryOperationCommand dirCmd(_pDeviceData);

    cmd.Attach(pObserver);

    // 收集文件数据
    __int64 sizeSum = 0;
    for(size_t i = 0; i < paths.size(); ++i)
    {
        wstring filePath = paths[i].c_str();
        RETURN_VALUE_IF( ! CFileOperation::IsFileExist(filePath.c_str()) , Transfer_Unknown);

        sizeSum += CFileOperation::GetFileSizeW(filePath);
    }

    cmd.SetTaskNumberSum(paths.size());
    cmd.SetAllTaskSum(sizeSum);

    // 逐个上传，出错则返回
    for(size_t i = 0; i < paths.size(); ++i)
    {
        IF_CANCEL_BREAK(pObserver);

        TransferResult result = Transfer_Unknown;
        try
        {
            result =  DoUploadFile(paths[i], &cmd, &dirCmd);
        }
        catch (...)
        {
            return Transfer_Fail;
        }
        RETURN_VALUE_IF ( result != Transfer_Success, result);
    }

    cmd.Complete();
    Load();

    IF_CANCEL(pObserver)
        return Transfer_Cancel;

    return Transfer_Success;
}

void DirData::SetNodeAttribute( NodeData* pNode, wstring name) 
{
    pNode->_name = name;
    pNode->_mode = pNode->_type == NodeData::DIR ? "755" : "644";

    time_t now;
    time(&now);
    pNode->_createTime = pNode->_modifyTime = now;

    if ( _pDeviceData->GetDevInfo()->_deviceType == Android )
    {
        pNode->_owner = "system";
        pNode->_group = "system";
    }
    else if ( _pDeviceData->GetDevInfo()->_deviceType == Ios )
    {
        pNode->_owner = "mobile";
        pNode->_group = "mobile";
    }
}

// 上传文件
TransferResult DirData::DoUploadFile(wstring path,
                                     UploadFileCommand* pCmd,
                                     DirectoryOperationCommand* pDirCmd)
{
    //wstring destName = GetFullPath() + CFileOperation::GetFileName(path);
    wstring filename = CFileOperation::GetFileName(path);
    if (_pDeviceData->GetDevInfo()->_deviceType == Android)
        FileNameCheck(filename);
    wstring destName = GetFullPath() + filename;

    pCmd->setfilename(path, destName);
    pCmd->Execute();
    TransferResult result = pCmd->GetUploadResult();
    RETURN_VALUE_IF ( result != Transfer_Success, result);

    wstring newName = pCmd->GetDestFilePath();
    SetNodeAttribute(AddFile(), CFileOperation::GetFileName(newName));

    pCmd->IncreaseTaskNumberComplete();
    pCmd->SendProgress();
    return Transfer_Success;
}

TransferResult DirData::UploadDir(wstring path, ProgressObserver* pObserver,bool bSetAttr)
{
    RETURN_VALUE_IF( ! CFileOperation::IsDirExist(path.c_str()) , Transfer_Unknown);

    if (pObserver) 
        pObserver->GetPrompt(MESSAGE_UPLOAD);

    Load();

    UploadFileCommand cmd(_pDeviceData);
	cmd.SetIsSetAttr(bSetAttr);
    DirectoryOperationCommand dirCmd(_pDeviceData);

    cmd.Attach(pObserver);

    int nodeSum = 0;
    __int64 sizeSum = 0;
    CFileOperation::GetDirSizeW(path, nodeSum, sizeSum);

    cmd.SetAllTaskSum(sizeSum);
    cmd.SetTaskNumberSum(nodeSum);

    TransferResult ret = DoUploadDir(path, &cmd, &dirCmd);

	cmd.Complete();

	return ret;
}

TransferResult DirData::UploadDir(vector<wstring> paths, ProgressObserver* pObserver,bool bSetAttr)
{
	if (pObserver) 
		pObserver->GetPrompt(MESSAGE_UPLOAD);

	Load();

	UploadFileCommand cmd(_pDeviceData);
	cmd.SetIsSetAttr(bSetAttr);
	DirectoryOperationCommand dirCmd(_pDeviceData);

	cmd.Attach(pObserver);

	int nodeSum = 0;
	__int64 sizeSum = 0;

	for (vector<wstring>::iterator it = paths.begin(); it != paths.end(); it++)
	{
		IF_CANCEL_BREAK(pObserver);

		int nodeSum_tmp = 0;
		__int64 sizeSum_tmp = 0;

		wstring path = *it;

		if(CFileOperation::IsFileExist(path.c_str()))
		{
			nodeSum += 1;
			sizeSum += CFileOperation::GetFileSizeW(path);
		}
		else
		{
			CFileOperation::GetDirSizeW(path, nodeSum_tmp, sizeSum_tmp);
		}

		nodeSum += nodeSum_tmp;
		sizeSum += sizeSum_tmp;
	}

	RETURN_VALUE_IF(pObserver->IsCancel(), Transfer_Cancel);

	cmd.SetAllTaskSum(sizeSum);
	cmd.SetTaskNumberSum(nodeSum);

	TransferResult ret = Transfer_Unknown;
	for (vector<wstring>::iterator it = paths.begin(); it != paths.end(); it++)
	{
		IF_CANCEL_BREAK(pObserver);

		wstring path = *it;

		if(CFileOperation::IsFileExist(path.c_str()))
			ret = DoUploadFile(path, &cmd, &dirCmd);
		else
			ret = DoUploadDir(path, &cmd, &dirCmd);

		if(ret != Transfer_Success)
			break;
	}

	cmd.Complete();

	RETURN_VALUE_IF(pObserver->IsCancel(), Transfer_Cancel);

	return ret;
}

// 上传目录
TransferResult DirData::DoUploadDir(wstring path,
                                    UploadFileCommand* pFileCmd,
                                    DirectoryOperationCommand* pDirCmd)
{
    if ( *path.rbegin() == '\\' )
    {
        path.erase(--path.end()); // 去掉本地硬盘目录的结尾反斜杠
    }

    //wstring destName = GetFullPath() + CFileOperation::GetFileName(path);
    wstring filename = CFileOperation::GetFileName(path);
    if (_pDeviceData->GetDevInfo()->_deviceType == Android)
        FileNameCheck(filename);
    wstring destName = GetFullPath() + filename;

    if ( pDirCmd->Exist(destName))
    {
        switch ( pFileCmd->AskUser(destName))
        {
        case CANCEL:
			pFileCmd->Complete();
		//	Load();
            return Transfer_Cancel;
            break;
        case OVERWRITE:
            break;
        case OVERWRITE_ALL:
            break;

        case RENAME:
        case RENAME_ALL:
            destName = PhoneDirRename(destName);
            RETURN_VALUE_IF ( ! DoMakeSubDir(CFileOperation::GetFileName(destName), pDirCmd), Transfer_Fail);
            break;

        default:
            return Transfer_Unknown;
        }
    }
    else
    {
        RETURN_VALUE_IF ( ! DoMakeSubDir(CFileOperation::GetFileName(destName), pDirCmd), Transfer_Fail);
    }

    // 取得子目录节点
    NodeData* pNode = findChildrenbyName(CFileOperation::GetFileName(destName));
    DirData* pSubDir = dynamic_cast<DirData*>(pNode);
    RETURN_VALUE_IF ( ! pSubDir, Transfer_Fail);

    // 取得所有子目录和文件
    vector<wstring> childDirs, childFiles;
    CFileOperation::FindAllFolderInDir(path, childDirs);
    CFileOperation::FindAllFileInDir(path, childFiles);

    vector<wstring>::iterator it;
    for( it = childDirs.begin(); it != childDirs.end(); ++it)
    {
        TransferResult result =  pSubDir->DoUploadDir/*DoUploadFile*/(*it, pFileCmd, pDirCmd);
        RETURN_VALUE_IF ( result != Transfer_Success, result);
    }
    for( it = childFiles.begin(); it != childFiles.end(); ++it)
    {
        TransferResult result =  pSubDir->DoUploadFile(*it, pFileCmd, pDirCmd);
        RETURN_VALUE_IF ( result != Transfer_Success, result);
    }

	//pFileCmd->Complete();
	//Load();
    return Transfer_Success;   
}

NodeData* DirData::findChildrenbyName(wstring name, bool bFindLinkTarget, bool bExactMatch)
{
    Load();

    for(deque<NodeData*>::iterator it = _childNodes.begin() ; it != _childNodes.end(); ++it)
    {
		// 区分大小写  By QiuChang 2012.8.22
		if (bExactMatch && (*it)->_name != name)
            continue;

        // 不区分大小写
        if (!bExactMatch && CStrOperation::toLowerW((*it)->_name) != CStrOperation::toLowerW(name))
            continue;

        NodeData* pNode = *it;
        if ( pNode->_type == LINK && bFindLinkTarget)
        {
            LinkData* pLink = dynamic_cast<LinkData*> ( pNode );
            if ( pLink )
                pNode = pLink->GetTarget();
        }
        return pNode;
    }

	LOG->Write(Log::LOG_DEBUG, L"搜索不到指定节点：%s",name.c_str() );

    return NULL;
}

LinkData* DirData::AddLink()
{
    // 修改此方法须谨慎: 保证在 _pDeviceData 为NULL时, 不出错
    // 因为在 AFCFileSysemBase中调用到本方法时, _pDeviceData 为 NULL
    LinkData* pLink = new LinkData();
    pLink->_Parent = this;
    _childNodes.push_back(pLink);

    return pLink;
}

FileData* DirData::AddFile()
{
    // 修改此方法须谨慎: 保证在 _pDeviceData 为NULL时, 不出错
    // 因为在 AFCFileSysemBase中调用到本方法时, _pDeviceData 为 NULL
    FileData* pFile = new FileData();
    pFile->_Parent = this;
//    MyLock lock(&_lock);
    _childNodes.push_back(pFile);
    return pFile;
}

DirData* DirData::AddDir()
{
    // 修改此方法须谨慎: 保证在 _pDeviceData 为NULL时, 不出错
    // 因为在 AFCFileSysemBase中调用到本方法时, _pDeviceData 为 NULL
    DirData* pDir = new DirData(_pDeviceData);
    pDir->_Parent = this;

    _childNodes.push_back(pDir);
    return pDir;
}

std::wstring DirData::GetFullPath() const
{
    wstring path = _Parent == NULL ? _name : _Parent->GetFullPath() + _name;

	if (path == L"/")
		return path;
	return CStrOperation::trimRightW(path, L"/") + L"/";
}

TransferResult DirData::DoDownload(wstring destDir,
                                   DownloadFileCommand* pCmd )
{
    Load();

    //if (  !_waccess(destDir.c_str(), 0) )
    if ( !access(CCodeOperation::WstringToString(destDir).c_str(), 0) )
    {
        // 同名节点已存在，询问用户如何处理
        switch ( pCmd->AskUser(destDir) )
        {
        case CANCEL:
            return Transfer_Cancel;

        case OVERWRITE:
        case OVERWRITE_ALL:
            // 同名文件就删掉，同名目录就啥也不做
            if ( CFileOperation::IsFileExist(destDir.c_str()) )
            {
                CFileOperation::DeleteFile(destDir);
                RETURN_VALUE_IF ( ! CFileOperation::MakeLocalDir(destDir), Transfer_Fail );
            }
            break;

        case RENAME:
        case RENAME_ALL:
            destDir = PhoneDirRename(destDir);  // 改名
            RETURN_VALUE_IF ( ! CFileOperation::MakeLocalDir(destDir), Transfer_Fail );
            break;
        }
    }
    else
    {
        // 若目录不存在则创建，创建失败则返回
        RETURN_VALUE_IF ( ! CFileOperation::MakeLocalDir(destDir.c_str()), Transfer_Fail);
    }

    // 保证以 \ 结尾
    if ( *destDir.rbegin() != CHAR_PATH_SEPARATER)
        destDir.push_back(CHAR_PATH_SEPARATER);

    int iCountType = 0;
    OPTION->Get(GlobalOption::INT_FILETASK_COUNTMODE, iCountType);
    if (iCountType > 0)
        pCmd->IncreaseTaskNumberComplete();

    for(size_t i = 0; i < _childNodes.size(); ++i)
    {
        // 下载，不成功则返回
        wstring dest = destDir + _childNodes[i]->_name;

        TransferResult result = _childNodes[i]->DoDownload(dest, pCmd);
        RETURN_VALUE_IF ( result != Transfer_Success, result);
    }

    return Transfer_Success;
}

bool DirData::DoDelete( DirectoryOperationCommand* pCmd, ProgressObserver* pObserver)
{
    Load();

    RETURN_FALSE_IF ( !_Parent );

    // 删除所有子节点
    while( _childNodes.begin() != _childNodes.end() )
    {
		if(pObserver && pObserver->IsCancel())
			return true;
        RETURN_FALSE_IF ( ! (*_childNodes.begin())->DoDelete(pCmd, pObserver) );
    }


    pCmd->SetCurrentTaskName(GetFullPath());
    pCmd->SetCurrentTaskSum(0);

    // 删除自己
    RETURN_FALSE_IF ( ! pCmd->DeleteDir(GetFullPath()) );
    _Parent->RemoveFromChildrens(this);

    pCmd->IncreaseTaskNumberComplete();
    pCmd->SendProgress();

    return true;
}

wstring DirData::PhoneDirRename(const wstring name)
{
	int i = 0;
	wchar_t buffer[10];
	wstring dirName;
	size_t offset = name.rfind('/');
	if ( offset != string::npos )
	{
		dirName = name.substr(offset+1);
	}

	if (dirName.empty())
		dirName = name;

	Load();

	wstring destName;
	do
	{
		CStrOperation::MyItows(++i, buffer, 10);

		wstring copyName = International::GetString(FILE_COPY_NAME);
		if (name.find(copyName) != wstring::npos)
			destName = copyName + buffer + L" " + dirName.substr(copyName.length(), dirName.length());
		else
			destName = dirName + L"_" + buffer;
	}
	while(findChildrenbyName(destName) || CFileOperation::IsDirExist(destName.c_str()));

    return destName;
}

wstring DirData::PhoneDirRenameCopy(const wstring name)
{
    wstring copyName = International::GetString(FILE_COPY_NAME);
    wstring copyDirName = copyName + name;
    return PhoneDirRename(copyDirName);
}

bool DirData::IsRoot() const
{
    return NULL == _Parent;
}

wstring DirData::PhoneFileRename(const wstring name,
                                 DirectoryOperationCommand* pCmd )
{
    wstring filename;
    wstring copyName = International::GetString(FILE_COPY_NAME);
    if (name.find(copyName) != wstring::npos)
        filename = CFileOperation::GetFileNameNoExt(name);
    else
        filename = CFileOperation::GetFileNameNoExt(name) + L"_";

    // 文件名之前的路径
    wstring path;
    size_t slashOffset = name.rfind('/');
    if ( slashOffset != string::npos )
    {
        path = name.substr(0, slashOffset + 1);
    }

    // 文件名之后的扩展名
    wstring ext;
    size_t dotOffset = name.rfind('.');
    if ( dotOffset != string::npos && dotOffset > slashOffset )
    {
        ext = name.substr(dotOffset, name.size() - dotOffset);
    }

    int i = 0;
    wchar_t buffer[10];
    wstring destName;

    do
    {
        CStrOperation::MyItows(++i, buffer, 10);
        wstring copyName = International::GetString(FILE_COPY_NAME);
        if (name.find(copyName)!=wstring::npos)
            destName = path + copyName + buffer + L" " + filename.substr(copyName.length(), filename.length()) + ext;
        else
            destName = path + filename + buffer + ext;
    }
    while( pCmd->Exist(destName) );

    return destName;
}

wstring DirData::PhoneFileRenameCopy(const wstring name, DirectoryOperationCommand* pCmd)
{
    wstring filename = CFileOperation::GetFileNameNoExt(name);
    wstring copyName = International::GetString(FILE_COPY_NAME);
    wstring copyFileName = copyName + filename;
    return PhoneFileRename(CStrOperation::ReplaceW(name, filename, copyFileName), pCmd);
}

bool DirData::Copy(vector<NodeData*>* pNodes,
                   ProgressObserver* pObserver)
{
    RETURN_FALSE_IF ( pNodes->empty() );

    for(size_t i = 0; i < pNodes->size(); ++i)
    {
        NodeData* pNode = pNodes->at(i);
        RETURN_FALSE_IF(pNode->GetFullPath() == GetFullPath());
        RETURN_FALSE_IF(IsParentDir(pNode));
    }

    Load();

    // 创建命令
    DirectoryOperationCommand dirCmd(_pDeviceData);
    CopyFileCommand fileCmd(_pDeviceData);

    DirData* pParent = pNodes->at(0)->GetParent();
    if (pParent != NULL
        && pParent->GetFullPath() == GetFullPath())
        fileCmd.SetRenameCopyAllAction();

    // 统计文件数、大小，并设置到fileCmd中
    // 因为fileCmd才是需要提示进度的，dirCmd不需要
    fileCmd.Attach(pObserver);
    FileHelper::SetTaskSize(pNodes, &fileCmd,pObserver);

    for(size_t i = 0; i < pNodes->size(); ++i)
    {
        IF_CANCEL_BREAK(pObserver)
        NodeData* pNode = pNodes->at(i);
        RETURN_FALSE_IF( ! DoCopy(pNode, &dirCmd, &fileCmd) );

        // 完成任务数加 1
        fileCmd.IncreaseTaskNumberComplete();
        fileCmd.SendProgress();
    }

    fileCmd.Complete();
    fileCmd.Detach(pObserver);

    Load();
    IF_CANCEL_RETURN_FALSE(pObserver)
    return true;
}

bool DirData::DoCopy(NodeData* pNode,
                     DirectoryOperationCommand* pDirCmd,
                     CopyFileCommand* pFileCmd )
{
    if ( pNode->_type == DIR )
    {
        DirData* pDir = dynamic_cast<DirData*>(pNode);
        RETURN_FALSE_IF( ! pDir );

        return DoCopy(pDir, pDirCmd, pFileCmd);
    }
    else if ( pNode->_type == FILE )
    {
        FileData* pFile = dynamic_cast<FileData*>(pNode);
        RETURN_FALSE_IF( ! pFile );

        return DoCopy(pFile, pFileCmd);
    }
    else if ( pNode->_type == LINK)
    {
        return true;
    }
    else
        return true;   // 目录、文件之外就是链接：忽略
}

bool DirData::DoCopy(FileData* pSourceFile, CopyFileCommand* pFileCmd)
{
    wstring destName = GetFullPath() + pSourceFile->_name;    // copy 目标文件路径

    // 按常规应该在Command中设置任务进度，
    // 但CopyFileCommand不知道文件大小，因此在这里设置
    pFileCmd->SetCurrentTaskSum(pSourceFile->_size);
    pFileCmd->SetCurrentTaskName(destName);

    pFileCmd->setFileNames(pSourceFile->GetFullPath(), destName);
    pFileCmd->Execute();
    RETURN_FALSE_IF( ! pFileCmd->IsCopyDoneWell() );

    AddFile()->_name = pSourceFile->_name;
    pFileCmd->AddTaskProgress(pSourceFile->_size);
    return true;
}

bool DirData::DoCopy(DirData* pSourceDir,
                     DirectoryOperationCommand* pDirCmd,
                     CopyFileCommand* pFileCmd)
{
    RETURN_FALSE_IF ( ! pSourceDir );
    pSourceDir->Load();

    DirData* pSubDir = CheckSubDir(pDirCmd, pFileCmd, pSourceDir->_name);
    RETURN_FALSE_IF( !pSubDir);

    // 复制所有子节点
    deque<NodeData*> childs = pSourceDir->GetChildrens();
    for(size_t i = 0; i < childs.size(); ++i)
    {
        NodeData* pNode = childs[i];

        RETURN_FALSE_IF( ! pSubDir->DoCopy(pNode, pDirCmd, pFileCmd) );

        // 完成任务数加 1
        pFileCmd->IncreaseTaskNumberComplete();
        pFileCmd->SendProgress();
    }

    pFileCmd->SetCurrentTaskSum(0);
    pFileCmd->SetCurrentTaskName(pSubDir->GetFullPath());

    pSubDir->Load();
    return true;
}

DirData* DirData::CheckSubDir(DirectoryOperationCommand* pDirCmd,
                              BaseCommand* pCommand,
                              wstring name)
{
    NodeData* pNode = findChildrenbyName(name);

    if ( ! pNode )
    {
        // 节点不存在则创建
        RETURN_FALSE_IF ( ! DoMakeSubDir(name, pDirCmd) );
    }
    else
    {
        // 同名节点已存在，询问用户如何处理
        switch ( pCommand->AskUser( pNode->GetFullPath() ) )
        {
        case CANCEL:
            return NULL;

        case OVERWRITE:
        case OVERWRITE_ALL:
            // 同名文件就删掉，同名目录就啥也不做
            if ( pNode->_type != DIR )
            {
                pNode->DoDelete(pDirCmd);
                RETURN_FALSE_IF ( ! DoMakeSubDir(name, pDirCmd) );
            }
            break;

        case RENAME:
        case RENAME_ALL:
            name = PhoneDirRename(name);  // 改名
            RETURN_FALSE_IF ( ! DoMakeSubDir(name, pDirCmd) );
            break;

        case RENAME_COPY:
        case RENAME_COPY_ALL:
            name = PhoneDirRenameCopy(name);
            RETURN_FALSE_IF ( ! DoMakeSubDir(name, pDirCmd) );
            break;
        }
    }

    return dynamic_cast<DirData*>(findChildrenbyName(name));
}

deque<NodeData*> ND91Assistant::DirData::GetChildrens()
{
	MyLock lock(&_lock);
    Load();

    return _childNodes;
}

void ND91Assistant::DirData::SetRootType(RootData::ROOT_TYPE eRootType)
{
	_eRootType = eRootType;
}

bool ND91Assistant::DirData::HasSubDir()
{
    MyLock lock(&_lock);
    Load();

    deque<NodeData*>::iterator it;
    for( it = _childNodes.begin(); it != _childNodes.end(); ++it )
    {
        RETURN_TRUE_IF( (*it)->_type == NodeData::DIR );
    }
    return false;
}

