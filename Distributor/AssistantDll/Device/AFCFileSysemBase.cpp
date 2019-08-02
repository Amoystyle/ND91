#include "stdafx.h"
#include "AFCFileSysemBase.h"

#include "Device/ITunesService.h"
#include "Device/CFConverter.h"
#include "Device/IosUsbConnection.h"
#include "Module/File/DirData.h"
#include "Module/File/LinkData.h"

#define BUFFER_SIZE	1048576

AFCFileSysemBase::AFCFileSysemBase(const DeviceInfo* pDevInfo)
{
	_pAFCHandle 	= pDevInfo->_ios_afcHandle;
	_pAFCHandleApp 	= pDevInfo->_ios_afcHandleApp;
	_bJailBreaked 	= pDevInfo->_ios_bJailBreaked;

    _pAFCSocket 	= pDevInfo->_ios_afcSocket;
    _pDevInfo 		= (DeviceInfo*)pDevInfo;
}

AFCFileSysemBase::AFCFileSysemBase(void* pAfcHandle, const DeviceInfo* pDevInfo)
{
    _pAFCHandle     = pAfcHandle;
    _pAFCHandleApp  = pAfcHandle;
    _bJailBreaked   = pDevInfo->_ios_bJailBreaked;

    _pAFCSocket     = pDevInfo->_ios_afcSocket;
    _pDevInfo       = (DeviceInfo*)pDevInfo;
}

AFCFileSysemBase::~AFCFileSysemBase(void)
{
}

string AFCFileSysemBase::Ios_GetRootDirectory(eFileSystemType eType)
{
	if (!_pAFCHandle)
		return "";

	switch (eType)
	{
	case eFileSystemType_Normal:
		{
			if (_bJailBreaked)
				return "/";
			else 
				return "/var/mobile/Media";
		}
		break;
	case eFileSystemType_Media:
		return "/var/mobile/Media";
		break;
	case eFileSystemType_Application:
		return "/Documents";
		break;
	}

	return "";
}

#define DIR1  L"/private/var/mobile/Media/"
#define DIR2  L"/var/mobile/Media/"

bool AFCFileSysemBase::Ios_CreateDirectory(wstring strPath)
{
	if (!_pAFCHandle)
		return false;

	bool bSuccess = false;
	if (_bJailBreaked)
	{
        const wchar_t* ptr = NULL;

		if (strPath.find(DIR1) == 0)
			ptr = strPath.c_str() + wstring(DIR1).length();
		else if (strPath.find(DIR2) == 0)
            ptr = strPath.c_str() + wstring(DIR2).length();

		if (ptr)
		{
			//Media文件夹下的目录都有AFC1服务创建,否则可能导致手机的异常
			if (CITunesMobileDevice::iTunes_AFCDirectoryCreate(_pAFCHandleApp, ptr) == 0) 
				bSuccess = true;
		}
	}

	if (!bSuccess)
	{
		if (CITunesMobileDevice::iTunes_AFCDirectoryCreate(_pAFCHandle, strPath.c_str()) == 0)
			bSuccess = true;
	}

	return bSuccess;
}

bool AFCFileSysemBase::Ios_Rename(wstring srcPath, wstring dstPath)
{
	if (!_pAFCHandle)
		return false;

	return ! CITunesMobileDevice::iTunes_AFCRenamePath(_pAFCHandle, srcPath.c_str(), dstPath.c_str());
}

bool AFCFileSysemBase::Ios_DeleteEmptyDirectory(wstring strPath)
{
	RETURN_VALUE_IF (!_pAFCHandle, false);

    RETURN_VALUE_IF( !Ios_IsDirectory(strPath), false);

    int ret = CITunesMobileDevice::iTunes_AFCRemovePath(_pAFCHandle, strPath.c_str());
    RETURN_TRUE_IF(ret == 0);

    GetAFCLastErrorMsg();
    return false;
}

bool AFCFileSysemBase::Ios_DeleteFile(wstring strPath)
{
    RETURN_FALSE_IF ( ! _pAFCHandle);

    RETURN_TRUE_IF( ! Ios_Exists(strPath));

    return 0 == CITunesMobileDevice::iTunes_AFCRemovePath(_pAFCHandle, strPath.c_str());
}

bool AFCFileSysemBase::Ios_Exists(wstring strPath)
{
    RETURN_VALUE_IF ( ! _pAFCHandle || strPath.empty(), false );

		void* szInfo = NULL;
		UINT32 nLen = 0;

#ifdef WIN32
		if (CITunesService::GetiTunesVersionType() == iTunesOld )
        return  ! CITunesMobileDevice::iTunes_AFCGetFileInfo(_pAFCHandle, strPath.c_str(), &szInfo, &nLen);
    else
        return  ! CITunesMobileDevice::iTunes_AFCFileInfoOpen(_pAFCHandle, strPath.c_str(), &szInfo);
#else
		return  ! CITunesMobileDevice::iTunes_AFCFileInfoOpen(_pAFCHandle, strPath.c_str(), &szInfo);
#endif
}

NodeData* AFCFileSysemBase::CreateNode(DirData* pDir,
                                  wstring name,
                                  std::map<string, string>& dict,
                                  eFileGetType eType)
{
    NodeData* pNode = NULL;

    string value = dict["st_ifmt"];
    if ( value.find("S_IFLNK") != string::npos )
    {
        RETURN_NULL_IF ( eFileGetType_Dirs == eType );   // 只取目录则跳过链接

        LinkData* pLink = pDir->AddLink();
        pLink->_linkTarget = CCodeOperation::GB2312ToUnicode(dict["LinkTarget"]);

        pNode = pLink;
    }
    else if (value.find("S_IFDIR") != string::npos)
    {
        RETURN_NULL_IF ( eFileGetType_Files == eType );   // 只取文件则跳过目录
        pNode = pDir->AddDir();
    }
    else
    {
        RETURN_NULL_IF ( eFileGetType_Dirs == eType );   // 只取目录则跳过文件
        pNode = pDir->AddFile();
    }

    pNode->_name = name;
    pNode->_displayName = pNode->_name;
    //GetDisplayName(pNode);
    pNode->_size = atol(dict["st_size"].c_str());

    double mTime = atof(dict["st_mtime"].c_str());
    pNode->_modifyTime = (time_t)(mTime/1000000000);
    pNode->_modifyTime -= CDealTime::GetTimeZone();

    double cTime = atof(dict["st_birthtime"].c_str());
    pNode->_createTime = (time_t)(cTime/1000000000);
    pNode->_createTime -= CDealTime::GetTimeZone();

    return pNode;
}

bool AFCFileSysemBase::Ios_GetFileInfos(DirData* pDir, eFileGetType eType)
{
    RETURN_VALUE_IF ( ! _pAFCHandle, false );

    pDir->ClearChilds();

    void* pAFCDir = NULL;
    int ret = CITunesMobileDevice::iTunes_AFCDirectoryOpen(_pAFCHandle, pDir->GetFullPath().c_str(), &pAFCDir);
    if (0 != ret)
    {
        GetAFCLastErrorMsg();
        return false;
    }

    char* pBufferUTF8 = NULL;
    CITunesMobileDevice::iTunes_AFCDirectoryRead(_pAFCHandle, pAFCDir, &pBufferUTF8);

    while (pBufferUTF8 != NULL && strlen(pBufferUTF8) > 0)
    {
        wstring name = CCodeOperation::UTF_8ToUnicode(pBufferUTF8, strlen(pBufferUTF8));
        if ( name != L"." && name != L".." )
		{
#ifdef WIN32			
			if ( CITunesService::GetiTunesVersionType() == iTunesOld )
				GetFileInfo(pDir, name, eType);
			else
				FileInfoOpen(pDir, name, eType);
#else
			FileInfoOpen(pDir, name, eType);
#endif		
		}        

        CITunesMobileDevice::iTunes_AFCDirectoryRead(_pAFCHandle, pAFCDir, &pBufferUTF8);
    }
    CITunesMobileDevice::iTunes_AFCDirectoryClose(_pAFCHandle, pAFCDir);
    return true;
}
TransferResult AFCFileSysemBase::Ios_CopyToPhoneEx(wstring sourceOnComputer, wstring destinationOnPhone, 
										 unsigned __int64 OffSetAppInExe, unsigned __int64 len,
										 ProgressSubject& subject) 
{
	RETURN_VALUE_IF ( ! _pAFCHandle, Transfer_Fail);
	RETURN_VALUE_IF ( ! CFileOperation::IsFileExist(sourceOnComputer.c_str()) &&  ! CFileOperation::IsDirExist(sourceOnComputer.c_str()),  Transfer_Fail);

	RETURN_VALUE_IF (SkipFile(sourceOnComputer), Transfer_Success);
	return DoCopyToPhone(sourceOnComputer, destinationOnPhone, subject,OffSetAppInExe,len);
}
TransferResult AFCFileSysemBase::Ios_CopyToPhone(wstring sourceOnComputer, wstring destinationOnPhone, ProgressSubject& subject)
{
	RETURN_VALUE_IF ( ! _pAFCHandle, Transfer_Fail);
	RETURN_VALUE_IF ( ! CFileOperation::IsFileExist(sourceOnComputer.c_str()) &&  ! CFileOperation::IsDirExist(sourceOnComputer.c_str()),  Transfer_Fail);

    RETURN_VALUE_IF (SkipFile(sourceOnComputer), Transfer_Success);
	return DoCopyToPhone(sourceOnComputer, destinationOnPhone, subject);
}

TransferResult AFCFileSysemBase::Ios_DownFromPhone(wstring sourceOnPhone, wstring destinationOnComputer, ProgressSubject& subject)
{
    RETURN_VALUE_IF ( ! _pAFCHandle, Transfer_Fail);
    RETURN_VALUE_IF ( ! Ios_Exists(sourceOnPhone), Transfer_Fail);

	if (CFileOperation::IsFileExist(destinationOnComputer.c_str()))
        CFileOperation::DeleteFile(destinationOnComputer);

	return DoDownFromPhone(sourceOnPhone, destinationOnComputer, subject);
}

TransferResult AFCFileSysemBase::DoCopyToPhone(wstring local_path, wstring remote_path, ProgressSubject& subject,
											   unsigned __int64 OffSetAppInExe, unsigned __int64 len)
{
    RETURN_VALUE_IF(local_path.empty(), Transfer_Fail);
    RETURN_VALUE_IF(remote_path.empty(), Transfer_Fail);

    ProgressObserver* pObserver = NULL;
    if (!subject._observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(subject._observers[0]);

	unsigned int completed = 0;
	TransferResult bRet = Transfer_Unknown;
    unsigned int unLastTick = GetTickCount();
    //unsigned int unLastTick = IosPublicFunc::MyGetTickCount();

    unsigned char *buf = (unsigned char *)malloc(sizeof(unsigned char)*(BUFFER_SIZE));
	if(! buf)
    {
        LOG->WriteError(L"Could not allocate buffer. Aborting." + local_path);
        return Transfer_Fail;
    }

	
    FILE* f = _wfopen(local_path.c_str(), L"rb");
    if ( !f )
    {
        LOG->WriteError(L"Failed to open local file: " + local_path);
        free(buf);
        return Transfer_Fail;
    }

    struct stat sb;
    if( -1 == fstat(fileno(f), &sb))
    {
        LOG->WriteError(L"Failed to get local file size: " + local_path);
        free(buf);
        fclose(f);
        return Transfer_Fail;
    }

    unsigned  int total = sb.st_size;
    subject.SetCurrentTaskName(local_path);

    afc_file_ref ref;
    afc_error_t ret = CITunesMobileDevice::iTunes_AFCFileRefOpen(_pAFCHandle, remote_path.c_str(), 3, &ref);
    if (ret)
    {
        LOG->WriteError(L"Could not create remote file: " + remote_path);

		// 此行在XP SP3、iTunes 10.3环境下造成崩溃    by QiuChang 2012.6.25
        // CITunesMobileDevice::iTunes_AFCFileRefClose(_pAFCHandle, ref); 

        free(buf);
        fclose(f);
        return Transfer_Fail;
    }

    unsigned int rsize = (total>BUFFER_SIZE ? BUFFER_SIZE:total);
	//添加从安装文件中根据偏移量读取app包
	if(OffSetAppInExe>0)
	{
		fseek(f,(long)OffSetAppInExe,0);
		if(len>0 && len < total)
					total = (int)len;
	}

	subject.SetCurrentTaskSum(total);

    while(!feof(f))
    {
        IF_CANCEL_BREAK(pObserver)

		if(completed>= total)
		{
			LOG->WriteDebug(L"completed >= total--->Break"+local_path);
			break;
		}

        rsize = (total - completed > BUFFER_SIZE ? BUFFER_SIZE:total - completed);

        fread(buf, rsize, 1, f);
        ret = CITunesMobileDevice::iTunes_AFCFileRefWrite(_pAFCHandle, ref, buf, rsize);
        completed += rsize;

        subject.AddTaskProgress(rsize);
        subject.SendProgress();

        if(ret)
        {
            LOG->WriteError(L"Could not write file: " + remote_path);
            CITunesMobileDevice::iTunes_AFCFileRefClose(_pAFCHandle, ref);
            free(buf);
            fclose(f);
            return Transfer_Fail;
        }
    }

    ret = CITunesMobileDevice::iTunes_AFCFileRefClose(_pAFCHandle, ref);
    if(!ret)
    {
        bRet = Transfer_Success;
    }
    else
    {
        LOG->WriteError(L"Failed to close remote file: " + remote_path);
    }

    fclose(f);
    free(buf);

    IF_CANCEL(pObserver)
    {
        Ios_DeleteFile(remote_path);
        bRet = Transfer_Cancel;
    }

	return bRet;
}

bool AFCFileSysemBase::Ios_IsDirectory(wstring strPath)
{
	NodeData* pNode = GetFileNode(strPath);

    bool b = pNode ? pNode->_type == NodeData::DIR : false;

	SAFE_DELETE(pNode);
	return b;
}

TransferResult AFCFileSysemBase::DoDownFromPhone(wstring remote_path, wstring local_path, ProgressSubject& subject)
{
    if(local_path.length() <= 0) return Transfer_Fail;
    if(remote_path.length() <= 0) return Transfer_Fail;

    ProgressObserver* pObserver = NULL;
    if (!subject._observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(subject._observers[0]);

    FILE *f;
    unsigned char *buf;
    afc_file_ref ref;
    unsigned int completed = 0;
    TransferResult result = Transfer_Unknown;

    buf = (unsigned char *)malloc(sizeof(unsigned char)*BUFFER_SIZE);
    if( ! buf)
    {
        LOG->WriteError(L"Could not allocate buffer. Aborting" ); 
        return Transfer_Fail;
    }

    if ( CITunesMobileDevice::iTunes_AFCFileRefOpen(_pAFCHandle, remote_path.c_str(), 1, &ref) )
    {
        LOG->WriteError(L"Failed to open remote file: " +  remote_path);
        free(buf);
        return Transfer_Fail;
    }

    unsigned int len = BUFFER_SIZE;

    subject.SetCurrentTaskSum(Ios_FileSize(remote_path));
    subject.SetCurrentTaskName(remote_path);

    if( CITunesMobileDevice::iTunes_AFCFileRefRead(_pAFCHandle, ref, buf, &len) )
    {
        LOG->WriteError(L"Failed to read from remote file: " + remote_path);
        free(buf);
        return Transfer_Fail;
    }

	CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(local_path));

    f = _wfopen(local_path.c_str(), L"w+b");
    if ( ! f) 
    {
        LOG->WriteError(L"Failed to open local file " + local_path );
        free(buf);
        return Transfer_Fail;
    }

    while(len)
    {
        IF_CANCEL_BREAK(pObserver)

        if(!fwrite(buf, len, 1, f))
        {
            LOG->WriteError(L"Failed to write to local file: " + local_path);
            break;
        }
        subject.AddTaskProgress(len);
        subject.SendProgress();

        if( CITunesMobileDevice::iTunes_AFCFileRefRead(_pAFCHandle, ref, buf, &len))
		{
			LOG->WriteError(L"Failed to read from remote file: " + remote_path);
			free(buf);
			return Transfer_Fail;
		}
    }

    fflush(f);
    fclose(f);
    result = Transfer_Success;

    if ( CITunesMobileDevice::iTunes_AFCFileRefClose(_pAFCHandle, ref)) 
    {
        LOG->WriteError(L"Failed to close remote file: " + remote_path);
    }

    free(buf);

    IF_CANCEL(pObserver)
    {
        CFileOperation::DeleteFile(local_path);
        result = Transfer_Cancel;
    }

    return result;
}

long AFCFileSysemBase::Ios_FileSize(wstring strPath)
{
	NodeData* pNode = GetFileNode(strPath);

	long l = pNode ? pNode->_size : -1;
	
	SAFE_DELETE(pNode);

	return l;
}

NodeData* AFCFileSysemBase::GetFileNode(wstring strPath)
{
	strPath = CStrOperation::trimRightW(strPath, L"/");
	DirData dir(NULL);
	size_t position = strPath.rfind('/');
	RETURN_VALUE_IF(position == wstring::npos, NULL);
	dir._name = strPath.substr(0, position);

	wstring name = strPath.substr(position+1, strPath.size() - position);

    NodeData* pNode = NULL;
#ifdef WIN32    
	if ( CITunesService::GetiTunesVersionType() == iTunesOld )
		pNode = GetFileInfo(&dir, name, eFileGetType_All);
	else
		pNode = FileInfoOpen(&dir, name, eFileGetType_All);
#else
	pNode = FileInfoOpen(&dir, name, eFileGetType_All);
#endif	

	return (pNode?pNode->Clone(NULL):NULL);
}

NodeData* AFCFileSysemBase::GetFileInfo(DirData* pDir, wstring name, eFileGetType eType)
{
    char* data = NULL;
    UINT32 data_size = 0;
    afc_error_t e = CITunesMobileDevice::iTunes_AFCGetFileInfo(_pAFCHandle,
        (pDir->GetFullPath() + name).c_str(), (void**)&data, &data_size);

    if ( e )
    {
		if  (!Ios_Exists(pDir->GetFullPath() + name))
			return NULL;

        wostringstream woss;
        woss << "CITunesMobileDevice::iTunes_AFCGetFileInfo return " << e;
        LOG_ALERT(woss.str());
        THROW_ASSISTANT_EXCEPTION(DLL_RETURN_ERROR);
    }

    string key = "";
    string value = "";
    std::map<string, string> info;

    UINT32 offset = 0;
    while (offset < data_size)
    {
        key = CStrOperation::ReadString(data + offset, data_size-offset);
        offset += key.length() + 1;

        value = CStrOperation::ReadString(data + offset, data_size-offset);
        offset += value.length() + 1;

        info[key] = value;
    }

    
    return CreateNode(pDir, name, info, eType);
}

NodeData* AFCFileSysemBase::FileInfoOpen( DirData* pDir, wstring name, eFileGetType eType )
{
    void* data = NULL;
    afc_error_t e = CITunesMobileDevice::iTunes_AFCFileInfoOpen(_pAFCHandle,
        (pDir->GetFullPath() + name).c_str(), &data);

    if ( e )
    {
		if  (!Ios_Exists(pDir->GetFullPath() + name))
			return NULL;

        wostringstream woss;
        woss << "CITunesMobileDevice::iTunes_AFCFileInfoOpen return " << e;
        LOG_ALERT(woss.str());
        THROW_ASSISTANT_EXCEPTION(DLL_RETURN_ERROR);
    }

    char* pName = NULL;
    char* pValue = NULL;
    std::map<string, string> info;
    while (true)
    {
        CITunesMobileDevice::iTunes_AFCKeyValueRead(data, &pName, &pValue);

        if (pName == NULL || strlen(pName) <= 0)
            break;

        info[pName] = pValue;
    }
    CITunesMobileDevice::iTunes_AFCKeyValueClose(data);

    return CreateNode(pDir, name, info, eType);
}

int AFCFileSysemBase::Ios_OpenFile(wstring strPath, string& buffer)
{
    return OpenRead(strPath, buffer);
}

int AFCFileSysemBase::OpenRead(wstring strPath, string& buffer)
{
    afc_file_ref ref = 0;
    afc_error_t ret = 0;
    for (int i = 0; i < 3; i++)
    {
        ret = CITunesMobileDevice::iTunes_AFCFileRefOpen(_pAFCHandle, strPath.c_str(), 1, &ref);
        if (0 == ret)
            break;

        Sleep(2500);
    }
    RETURN_FALSE_IF(0 != ret);

    unsigned int len = BUFFER_SIZE;
    unsigned char *buf = (unsigned char *)malloc(sizeof(unsigned char)*BUFFER_SIZE);
    memset(buf, 0, BUFFER_SIZE);
    CITunesMobileDevice::iTunes_AFCFileRefRead(_pAFCHandle, ref, buf, &len);
    buffer = (const char*)buf;
    free(buf);
    CITunesMobileDevice::iTunes_AFCFileRefClose(_pAFCHandle, ref);
    return buffer.length();
}

bool AFCFileSysemBase::OpenWrite(wstring strPath, string buffer)
{
    bool bRet = false;
    afc_file_ref ref = 0;
    afc_error_t ret = 0;
    for (int i = 0; i < 3; i++)
    {
        ret = CITunesMobileDevice::iTunes_AFCFileRefOpen(_pAFCHandle, strPath.c_str(), 3, &ref);
        if (0 == ret)
            break;

        Sleep(2500);
    }
    RETURN_FALSE_IF(0 != ret);

    if (0 == CITunesMobileDevice::iTunes_AFCFileRefWrite(_pAFCHandle, ref, (unsigned char *)buffer.c_str(), buffer.length()))
        bRet = true;

    CITunesMobileDevice::iTunes_AFCFileRefClose(_pAFCHandle, ref);
    return bRet;
}

bool AFCFileSysemBase::LinkPath(wstring target, wstring link, LinkMode mode)
{
    RETURN_FALSE_IF(!_pAFCHandle);

    int ret = CITunesMobileDevice::iTunes_AFCLinkPath(_pAFCHandle, (int)mode, 0, target.c_str(),link.c_str());
    RETURN_TRUE_IF(ret == 0);

    GetAFCLastErrorMsg();
    return false;
}

string AFCFileSysemBase::GetAFCLastErrorMsg()
{
    RETURN_VALUE_IF(!_pAFCHandle, "");

    void* error = CITunesMobileDevice::iTunes_AFCConnectionCopyLastErrorInfo(_pAFCHandle);

    //判断下AFC连接是否失效了.
    //IsAFCConnectionValid();
    RETURN_VALUE_IF(error == NULL, "");

    map<string, string> lastError;
    CCFConverter::GetIosData(error, lastError);
    RETURN_VALUE_IF(lastError.empty(), "");

    string errorString;
    map<string, string>::const_iterator it = lastError.find("NSLocalizedDescription");
    if (it != lastError.end())
    {
        errorString = it->second;
        if (!errorString.empty())
            LOG_DEBUG(L"V2:Last AFC Operation errorMsg : "+CCodeOperation::UTF_8ToUnicode(errorString));
    }

    //afcCode ==  -402636789 the service is not connected
    it = lastError.find("AFCCode");
    if (it != lastError.end())
        LOG_DEBUG(L"V2:Last AFC Operation errorCode :"+CCodeOperation::UTF_8ToUnicode(it->second));

    it = lastError.find("AFCDomain");
    if (it != lastError.end())
    {
        if (it->second == "AFCErrorDomain")
            LOG_DEBUG(L"Another domain encoutered");
    }

    CCFConverter::NDCFRelease(error);

    return errorString;
}

bool AFCFileSysemBase::IsAFCConnectionValid()
{
    RETURN_FALSE_IF(_pAFCHandle == NULL);

    bool isValid = CITunesMobileDevice::iTunes_AFCConnectionIsValid(_pAFCHandle);
    if (!isValid)
    {
        int retryCount = 3;
        while (retryCount < 3)
        {
            if (StartAFCFileSystem())
                return true;

            retryCount++;
            Sleep(200);
        }
        isValid = false;
    }
    else
    {
//         try
//         {
//             wstring afcTestFileName = L"netDragon.mobile.device_AFCTester";
//             OpenWrite(afcTestFileName, "0123456789");
//             if (Ios_Exists(afcTestFileName))
//             {
//                 Ios_DeleteFile(afcTestFileName);
//                 isValid = true;
//             }
//         }
//         catch (...)
//         {
//             LOG->WriteError(L"AFC Service Down");
//             isValid = false;
//             if (StartAFCFileSystem())
//                 isValid = true;
//         }
    }

    return isValid;
}

bool AFCFileSysemBase::StartAFCFileSystem()
{
    IosUsbConnection conn((DeviceInfo*)_pDevInfo);

    string temp = "com.apple.afc";
    int err = conn.StartIOSService((char*)temp.c_str(), &_pAFCSocket);
    if (err == 0)
    {
        err = CITunesMobileDevice::iTunes_AFCConnectionOpen(_pAFCSocket, 0, &_pAFCHandle);
        if (err != 0)
        {
            LOG_DEBUG(CCodeOperation::UTF_8ToUnicode(CITunesMobileDevice::iTunes_GetDeviceErrorCodeString(err)));
            CITunesMobileDevice::CloseSocket(_pAFCSocket);
            _pAFCSocket = NULL;
            return false;
        }
        return true;
    }
    else
        return false;
}

void AFCFileSysemBase::StopAFCFileSystem()
{
    if (_pAFCHandle != NULL)
    {
        CITunesMobileDevice::iTunes_AFCConnectionClose(_pAFCHandle);
        _pAFCHandle = NULL;
    }

    if (_pAFCSocket != NULL)
    {
        CITunesMobileDevice::CloseSocket(_pAFCSocket);
        _pAFCSocket = NULL;
    }
}

string AFCFileSysemBase::Ios_GetFileSHA1(wstring strPath)
{
    RETURN_FALSE_IF(!_pAFCHandle);

//     void* allocator = CCFConverter::NDCFGetAllocator(_pAFCHandle);
//     allocator = NULL;//iTunes在用这个的时候,allocator传默认的0
    void* cfString = CCFConverter::NDCFStringCreateWithCString(NULL, 
                            (unsigned char*)CCodeOperation::UnicodeToUTF_8(strPath).c_str(), 
                            0x8000100);

//     allocator = CCFConverter::NDCFGetAllocator(_pAFCHandle);
//     allocator = NULL;//iTunes在用这个的时候,allocator传默认的0
    void* op = CITunesMobileDevice::iTunes_AFCOperationCreateGetFileHash(NULL, cfString, NULL);

    if (cfString != NULL)
        CCFConverter::NDCFRelease(cfString);

    int timeOut = CITunesMobileDevice::iTunes_AFCConnectionGetIOTimeout(_pAFCHandle);
    int retCode = CITunesMobileDevice::iTunes_AFCConnectionProcessOperation(_pAFCHandle, op, timeOut);
    retCode = CITunesMobileDevice::iTunes_AFCOperationGetResultStatus(op);

    string sha1;
    if (retCode == 0)
    {
        void* resultObject = CITunesMobileDevice::iTunes_AFCOperationGetResultObject(op);
        int type = CCFConverter::NDCFGetTypeID(resultObject);
        CCFConverter::GetIosData(resultObject, sha1);
    }
    else
        GetAFCLastErrorMsg();

    if (op != NULL)
        CCFConverter::NDCFRelease(op);

    return sha1;
}

string AFCFileSysemBase::Ios_GetFileSHA1WithRange(wstring strPath, long endPosition)
{
    RETURN_FALSE_IF(!_pAFCHandle);
    RETURN_VALUE_IF(endPosition < 0, "");

    //void* allocator = CCFConverter::NDCFGetAllocator(_pAFCHandle);//这个是iTunesMobileDevice.dll里面的代码这样用
    void* cfString = CCFConverter::NDCFStringCreateWithCString(NULL, 
                                (unsigned char*)CCodeOperation::UnicodeToUTF_8(strPath).c_str(), 
                                0x8000100);

    //allocator = CCFConverter::NDCFGetAllocator(_pAFCHandle);
    //allocator = NULL;//iTunes在用这个的时候,allocator传默认的0         

    void* op = CITunesMobileDevice::iTunes_AFCOperationCreateGetFileHashWithRange(NULL, cfString, NULL, NULL, 
        endPosition, NULL, NULL);

    if (cfString != NULL)
        CCFConverter::NDCFRelease(cfString);

    int timeOut = CITunesMobileDevice::iTunes_AFCConnectionGetIOTimeout(_pAFCHandle);
    int retCode = CITunesMobileDevice::iTunes_AFCConnectionProcessOperation(_pAFCHandle, op, timeOut);
    retCode = CITunesMobileDevice::iTunes_AFCOperationGetResultStatus(op);

    string sha1;
    if (retCode == 0)
    {
        void* resultObject = CITunesMobileDevice::iTunes_AFCOperationGetResultObject(op);
        CCFConverter::GetIosData(resultObject, sha1);
    }
    else
        GetAFCLastErrorMsg();

    if (op != NULL)
        CCFConverter::NDCFRelease(op);

    return sha1;
}