#include "stdafx.h"

#include "DirectoryOperationCommand.h"
#include "DownloadFileCommand.h"

#include "Module/File/FileHelper.h"
#include "Common/Path.h"
#include "Common/plist/PlistOperation.h"
#include "Module/Application/AppData.h"
#include "Device/AFCFileSysemBase.h"
#include "Device/IosWiFiFileSystem.h"

NodeData* FileHelper::BatchDelete( vector<NodeData*>* pNodes, ProgressObserver* pObserver )
{
    RETURN_FALSE_IF ( ! pNodes );
    RETURN_FALSE_IF ( pNodes->empty() );
    RETURN_FALSE_IF ( ! pNodes->at(0)->_Parent );

    // 创建命令 
    DirectoryOperationCommand cmd(pNodes->at(0)->_Parent->_pDeviceData);

    // 统计文件数、大小，并设置到cmd中
    cmd.Attach(pObserver);
    SetTaskSize(pNodes, &cmd,pObserver);
    cmd.SetAllTaskSum( cmd.GetLastProgress()->_taskNumberSum );    // 对于删除来说：任务总量等于任务总数

    for(size_t i = 0; i < pNodes->size(); ++i )
    {
		if(pObserver && pObserver->IsCancel())
						 return NULL;
        NodeData* pNode = pNodes->at(i);

        if ( ! pNode->DoDelete(&cmd,pObserver) )
        {
            return pNode;
        }
    }

    cmd.Complete();
    return NULL;
}

TransferResult FileHelper::Download(vector<NodeData*>* pNodes,
                                    wstring destDir,
                                    ProgressObserver* pObserver)
{
    RETURN_VALUE_IF ( ! pNodes, Transfer_Unknown);
    RETURN_VALUE_IF ( pNodes->empty(), Transfer_Unknown);
    RETURN_VALUE_IF ( ! pNodes->at(0)->_Parent, Transfer_Unknown);

    if (pObserver) 
        pObserver->GetPrompt(MESSAGE_DOWNLOAD);
    // 检查指定目录是否存在，不存在则返回
    destDir = CStrOperation::trimRightW(destDir, WSTRING_PATH_SEPARATER);    	
	//RETURN_VALUE_IF ( ! CFileOperation::MakeLocalDir(destDir.c_str()), Transfer_Unknown);
    CFileOperation::MakeLocalDir(destDir.c_str());

    // 创建命令
    DownloadFileCommand cmd(pNodes->at(0)->_Parent->_pDeviceData);

    // 统计文件数、大小，并设置到cmd中
    cmd.Attach(pObserver);
    SetTaskSize(pNodes, &cmd,pObserver);

    int iCountType = 0;
    OPTION->Get(GlobalOption::INT_FILETASK_COUNTMODE, iCountType);

    for(size_t i = 0; i < pNodes->size(); ++i)
    {
        IF_CANCEL_BREAK(pObserver)
        NodeData* pNode = pNodes->at(i);      

        wstring dest = destDir + WSTRING_PATH_SEPARATER + CStrOperation::CheckAndReplaceFileName(pNode->_name, L'_');

        // 下载，不成功则返回
        TransferResult result = pNode->DoDownload(dest, &cmd);
        RETURN_VALUE_IF (result != Transfer_Success, result);

//         // 完成任务数加 1
//         if (pNode && (pNode->_type == DIR && iCountType > 0
//                     || pNode->_type == FILE
//                     || pNode->_type == LINK && iCountType > 1))
//             cmd.IncreaseTaskNumberComplete();
        cmd.SendProgress();
    }

    cmd.Complete();
    cmd.Detach(pObserver);
    IF_CANCEL(pObserver)
        return Transfer_Cancel;
    return Transfer_Success;
}

void FileHelper::SetTaskSize(vector<NodeData*>* pNodes, ProgressSubject* pSubject,ProgressObserver* pObserver)
{
    RETURN_IF ( ! pNodes );

    int numberSum = 0;
    __int64 sizeSum = 0;
    int iCountType = 0;
    OPTION->Get(GlobalOption::INT_FILETASK_COUNTMODE, iCountType);
    for(size_t i = 0; i < pNodes->size(); ++i)
    {
		IF_CANCEL_BREAK(pObserver)
        NodeData* pNode = pNodes->at(i);

        if ( NodeData::DIR == pNode->_type )
        {
			int nSize=0;
            dynamic_cast<DirData*>(pNode)->RecursiveLoad(nSize, numberSum);
			sizeSum+=nSize;
            if (iCountType > 0)
                numberSum ++;
        }
        else if ( NodeData::FILE == pNode->_type )
        {
            sizeSum += pNode->_size;
            numberSum ++;
        }
        else
        {
            if (iCountType > 1)
                numberSum ++;
        }
    }
    pSubject->SetAllTaskSum(sizeSum);
    pSubject->SetTaskNumberSum(numberSum);
}

bool ND91Assistant::FileHelper::Delete(const DeviceData* pDevice, wstring path )
{
    RETURN_FALSE_IF ( ! pDevice );

    wstring name = CFileOperation::GetFileName(path);
    wstring parent = path.substr(0, path.size() - name.size() - 1);

    DirData parentDir(pDevice);
    parentDir._name = parent;

    NodeData* pNode = parentDir.findChildrenbyName(name);
    RETURN_FALSE_IF ( ! pNode );

    vector<NodeData*> vec;
    vec.push_back(pNode);

    return NULL == BatchDelete(&vec, NULL);
}

DirData* ND91Assistant::FileHelper::CreateDir( const DeviceData* pDevice, wstring dirInDevice )
{
    RETURN_FALSE_IF ( ! pDevice );
    RETURN_FALSE_IF ( dirInDevice.empty() );

    DirData root(pDevice);

    NodeData* pExistNode = GetNode(&root, dirInDevice);
    if ( pExistNode )
        return dynamic_cast<DirData*>(pExistNode);

    vector<wstring> dirNames = CStrOperation::parseStringsW(dirInDevice, L"/");
    DirData* pDir = &root;
    for(size_t i = 0; i < dirNames.size(); ++i)
    {
        NodeData* pNode = pDir->findChildrenbyName(dirNames[i]);
        if ( ! pNode )
        {
            RETURN_NULL_IF( ! pDir->MakeSubDir(dirNames[i])); // 不存在则创建

            pNode = pDir->findChildrenbyName(dirNames[i]);  // 再次取子目录
            RETURN_NULL_IF( ! pNode );
        }

        // 应该是目录，或指向目录的链接
        pDir = dynamic_cast<DirData*>(pNode);
        RETURN_NULL_IF( ! pDir );
    }
    
    return pDir;
}

bool FileHelper::Upload(const DeviceData* pDevice,
                                  wstring sourcePath,
                                  wstring destDir,
                                  ProgressObserver* pObserver,
                                  bool bSetAttr)
{
    return Transfer_Success ==
        Upload2(pDevice, sourcePath, destDir, pObserver, bSetAttr);
}

TransferResult FileHelper::Upload2(const DeviceData* pDevice,
                                  wstring sourcePath,
                                  wstring destDir,
                                  ProgressObserver* pObserver,
                                  bool bSetAttr)
{
    RETURN_VALUE_IF ( ! pDevice, Transfer_Fail );
    RETURN_VALUE_IF ( destDir.empty() || sourcePath.empty(), Transfer_Fail);

    DirData dir(pDevice);
    dir._name = destDir;

    if ( CFileOperation::IsDirExist(sourcePath.c_str()) )
        return dir.UploadDir(sourcePath, pObserver,bSetAttr);
    else if (CFileOperation::IsFileExist(sourcePath.c_str()))
    {
        vector<wstring> files;
        files.push_back(sourcePath);
        return dir.UploadFile(files, pObserver,bSetAttr);
    }
    else
        return Transfer_Fail;
}

NodeData* FileHelper::GetNode( DirData* pRoot, wstring path, bool bFindLinkTarget, bool bExactMatch)
{
    RETURN_NULL_IF( ! pRoot );
    RETURN_NULL_IF( path.empty() );

    vector<wstring> nodeNames = CStrOperation::parseStringsW(path, L"/");

	if (nodeNames.empty())
		return pRoot;

    DirData* pDir = pRoot;
    for(size_t i = 0; i < nodeNames.size() - 1; ++i)
    {
        NodeData* pNode = pDir->findChildrenbyName(nodeNames[i], bFindLinkTarget, bExactMatch);
		
		if (! pNode || pNode->_type == NodeData::FILE )
		{
			LOG->Write(Log::LOG_DEBUG, L"搜索不到指定节点：%s",nodeNames[i].c_str() );
			return NULL;
		}

		if (pNode->_type == NodeData::LINK)
			pDir = dynamic_cast<DirData*>((dynamic_cast<LinkData*>(pNode))->GetTarget());
		else
			pDir = dynamic_cast<DirData*>(pNode);
        RETURN_FALSE_IF( ! pDir );
    }

    return pDir->findChildrenbyName(*nodeNames.rbegin(), bFindLinkTarget, bExactMatch);
}

NodeData* FileHelper::GetRealNode(DirData* pRoot, wstring path)
{
    RETURN_NULL_IF( ! pRoot );

    vector<wstring> nodeNames = CStrOperation::parseStringsW(path, L"/");

	if (nodeNames.empty())
		return pRoot;

	DirData* pDir = pRoot;
	for(size_t i = 0; i < nodeNames.size(); ++i)
	{
		NodeData* pNode = pDir->findChildrenbyName(nodeNames[i]);
		RETURN_FALSE_IF ( ! pNode  || pNode->_type == NodeData::FILE );

		if (pNode->_type == NodeData::LINK)
			pDir = dynamic_cast<DirData*>((dynamic_cast<LinkData*>(pNode))->GetTarget());
		else
			pDir = dynamic_cast<DirData*>(pNode);
		RETURN_FALSE_IF( ! pDir );
	}

	return pDir;
}

void FileHelper::IosGetDisplayName(NodeData* pNode, const DeviceData* pDevice)
{
    RETURN_IF(pNode->_type != NodeData::DIR);

    IIosFileSystem* pFileStream = NULL;
    if ( pDevice->GetDevInfo()->_connectType == WIFI)
        pFileStream = new IosWiFiFileSystem(pDevice->GetDevInfo()->_ios_wifi_handle);
    else if (pDevice->GetDevInfo()->_connectType == USB)
        pFileStream = new AFCFileSysemBase(pDevice->GetDevInfo());
    RETURN_IF(pFileStream == NULL);

    NodeData* pTempNode = pNode;
    pTempNode->_displayName = pTempNode->_name;
    DirData* pTempDir = (DirData*)pTempNode;
    deque<NodeData*> subChilds = pTempDir->GetChildrens();
    for (size_t j = 0; j < subChilds.size(); j++)
    {
        NodeData* pFile = subChilds[j];
        if (!CStrOperation::endsWithW(pFile->_name, L".app"))
            continue;

        wstring tempDir = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"\\";
        wstring fileName = L"Info.plist";
        RETURN_IF(pFileStream->Ios_DownFromPhone(pFile->GetFullPath()+fileName, tempDir+fileName) != Transfer_Success);

        AppData* pAppData = PlistOperation::ParseIpaAppInfoPlist(tempDir+fileName);
        RETURN_IF(pAppData == NULL);

        pTempNode->_displayName = pAppData->_AppName;
        SAFE_DELETE(pAppData);
        CFileOperation::DeleteDirectory(tempDir);
        break;
    }

    SAFE_DELETE(pFileStream);
}
