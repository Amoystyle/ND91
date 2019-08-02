#include "stdafx.h"
#include "IosWiFiFileSystem.h"

#include "Device/IosWiFiMobileDevice.h"
#include "Module/File/DirData.h"

IosWiFiFileSystem::IosWiFiFileSystem(int nhandle)
{
	_nHandle = nhandle;
}


IosWiFiFileSystem::~IosWiFiFileSystem(void)
{
}

string IosWiFiFileSystem::Ios_GetRootDirectory(eFileSystemType eType)
{
	return "/";
}

bool IosWiFiFileSystem::Ios_Rename(wstring srcPath, wstring dstPath)
{
	if (_nHandle <= 0 || srcPath.empty()  || dstPath.empty() )
		return false;

	return CIosWiFiMobileDevice::WiFiRename(_nHandle, srcPath, dstPath);
}

bool IosWiFiFileSystem::Ios_CreateDirectory(wstring strPath)
{
	if (_nHandle <= 0 || strPath.empty() )
		return false;

	bool bRet = CIosWiFiMobileDevice::WiFiCreatePhoneDirectory(_nHandle, strPath);

	return bRet;
}


bool IosWiFiFileSystem::Ios_DeleteEmptyDirectory(wstring strPath)
{
    if (_nHandle <= 0 || strPath.empty() )
        return false;

    return CIosWiFiMobileDevice::WiFiDeleteDirectory(_nHandle, strPath, false);
}


bool IosWiFiFileSystem::Ios_DeleteFile(wstring strPath)
{
	if (_nHandle <= 0 || strPath.empty() )
		return false;

	return CIosWiFiMobileDevice::WiFiDeletePhoneFile(_nHandle, strPath);
}


bool IosWiFiFileSystem::Ios_Exists(wstring strPath)
{
	if (_nHandle <= 0 || strPath.empty() )
		return false;

    Sleep(100);
	int nRet = CIosWiFiMobileDevice::WiFiExists(_nHandle, strPath);

	if (nRet < 0)
		THROW_ASSISTANT_EXCEPTION(IOS_WIFI_C0NN_ERROR);
	else
		return (0 == nRet)?false:true;

	return false;
}


bool IosWiFiFileSystem::Ios_IsDirectory(wstring strPath)
{
	if (_nHandle <= 0 || strPath.empty() )
		return false;

	int nRet = CIosWiFiMobileDevice::WiFiIsDirectory(_nHandle, strPath);

	if (nRet < 0)
		THROW_ASSISTANT_EXCEPTION(IOS_WIFI_C0NN_ERROR);
	else
		return (0 == nRet)?false:true;
	return false;
}


long IosWiFiFileSystem::Ios_FileSize(wstring strPath)
{
	if (_nHandle <= 0 || strPath.empty() )
		return -1;

	long lRet = CIosWiFiMobileDevice::WiFiFileSize(_nHandle, strPath);

	if (lRet < 0)
		THROW_ASSISTANT_EXCEPTION(IOS_WIFI_C0NN_ERROR);
	else
		return lRet;
	return false;
}

//获取指定文件夹下的所有文件信息，但不包括子目录下的信息
bool IosWiFiFileSystem::Ios_GetFileInfos(DirData* pDir, eFileGetType eType)
{
	RETURN_VALUE_IF(_nHandle <= 0, false);

    pDir->ClearChilds();

	switch (eType)
	{
	case eFileGetType_Files:
		return Ios_GetFiles(pDir);
		break;

	case eFileGetType_Dirs:
		return Ios_GetDirectories(pDir);
		break;

	case eFileGetType_All:
        Ios_GetDirectories(pDir);
        Ios_GetFiles(pDir);
		break;

	default:
		break;
	}

	return true;
}


//上传文件
TransferResult IosWiFiFileSystem::Ios_CopyToPhone(wstring sourceOnComputer, wstring destinationOnPhone, ProgressSubject& subject)
{
    if ( _nHandle <= 0 || sourceOnComputer.empty() || destinationOnPhone.empty() )
        return Transfer_Fail;

    if (! CFileOperation::IsFileExist(sourceOnComputer.c_str()) )
        return Transfer_Fail;

    if ( SkipFile(sourceOnComputer) )    //检查此文件是否属于需要跳过的部分
        return Transfer_Success;

    return DoCopyToPhone(sourceOnComputer, destinationOnPhone, subject);
}
TransferResult IosWiFiFileSystem::Ios_CopyToPhoneEx(wstring sourceOnComputer, wstring destinationOnPhone, 
										 unsigned __int64 OffSetAppInExe, unsigned __int64 len,
										 ProgressSubject& subject)
{
	return Transfer_Success;
}

//下载文件
TransferResult IosWiFiFileSystem::Ios_DownFromPhone(wstring sourceOnPhone, wstring destinationOnComputer, ProgressSubject& subject)
{
	if ( _nHandle <= 0 || sourceOnPhone.empty() || destinationOnComputer.empty() )
		return Transfer_Fail;

	if ( ! Ios_Exists(sourceOnPhone) )
		return Transfer_Fail;

    return DoDownFromPhone(sourceOnPhone, destinationOnComputer, subject);
}

bool IosWiFiFileSystem::Ios_GetFiles(DirData* pDir)
{
    bool bRet = false;

	wstring strPath = pDir->GetFullPath();
	strPath = CStrOperation::trimRightW(strPath, L"/");
	if (strPath.length() <= 0)
		strPath = L"/";

  	int nLen =  CIosWiFiMobileDevice::WiFiGetFiles(_nHandle, strPath, NULL);
 	if (nLen > 0)
 	{
 		char* szBuff = new char[nLen + 1];
 		//ZeroMemory(szBuff, nLen+1);
        memset(szBuff, 0, nLen+1);
 		int nNum =  CIosWiFiMobileDevice::WiFiGetFiles(_nHandle, strPath, szBuff);
 		if (nNum > 0)
 		{
            ParseResult(szBuff, nLen, pDir, true);
            bRet = true;
        }
 
 		SAFE_DELETE_ARRAY(szBuff);
 	}

	return bRet;
}

void IosWiFiFileSystem::ParseResult(char* szBuff, int nLen, DirData* pDir, bool isFile)
{
    // 整理buffer并拆分
    string strBuff = CCodeOperation::UTF_8ToGB2312(szBuff, nLen);
    string trimChars("\0/", 2);
    strBuff = CStrOperation::trimRight(strBuff, trimChars);
    strBuff = CStrOperation::trimLeft(strBuff, trimChars);
    vector<string> vtTemp = CStrOperation::parseStrings(strBuff, '/');

    std::vector<string>::iterator it;
    for (it = vtTemp.begin(); it!=vtTemp.end(); it++)
    {
        NodeData* pAddNode = NULL;
        if ( isFile )
            pAddNode = pDir->AddFile();
        else
            pAddNode = pDir->AddDir();

        pAddNode->_name = CCodeOperation::GB2312ToUnicode(*it);
        pAddNode->_displayName = pAddNode->_name;
        //GetDisplayName(pAddNode);
    }
}

bool IosWiFiFileSystem::Ios_GetDirectories(DirData* pDir)
{
    bool bRet = false;

	wstring strPath = pDir->GetFullPath();
	strPath = CStrOperation::trimRightW(strPath, L"/");
	if (strPath.length() <= 0)
		strPath = L"/";

 	int nLen = CIosWiFiMobileDevice::WiFiGetDirectories(_nHandle, strPath, NULL);
    if ( nLen > 0)
    {
        char* szBuff = new char[nLen + 1];
        //ZeroMemory(szBuff, nLen+1);
        memset(szBuff, 0, nLen+1);
        int nNum = CIosWiFiMobileDevice::WiFiGetDirectories(_nHandle, strPath, szBuff);		
        if (nNum > 0)
        {
            ParseResult(szBuff, nLen, pDir, false);
            bRet = true;
        }

        SAFE_DELETE_ARRAY (szBuff);
    }
	return bRet;
}

TransferResult IosWiFiFileSystem::DoCopyToPhone(wstring local_path, wstring remote_path, ProgressSubject& subject)
{
	if (_nHandle <= 0 || local_path.empty()  || remote_path.empty() )
		return Transfer_Fail;

    ProgressObserver* pObserver = NULL;
    if (!subject._observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(subject._observers[0]);

	TransferResult bRet = Transfer_Unknown;

	//开始异步传输，返回文件句柄
	int nFileHandle = CIosWiFiMobileDevice::WiFiCopyToPhoneAsync(_nHandle, local_path, remote_path);
	
    subject.SetCurrentTaskSum(CFileOperation::GetFileSizeW(local_path));
    subject.SetCurrentTaskName(local_path);

	if (nFileHandle > 0)
	{
		int lngFileSize = 0;
		int lngTransSize = 0;
		do 
		{
            IF_CANCEL_BREAK(pObserver)

			Sleep(50); // 此处Sleep不可取消，取消后会造成文件传输被取消：赵领军2010.05.07
			//查询正在传输的文件的大小，已经传输了多少
			CIosWiFiMobileDevice::WiFiGetTransProgress(nFileHandle, lngFileSize, lngTransSize);

			// -1 表示文件还没有开始传输, 其它负值表示失败
			if (lngTransSize < 0 && lngTransSize != -1)
				THROW_ASSISTANT_EXCEPTION(IOS_WIFI_TRANSFER_ERROR);

            subject.AddTaskProgress(lngFileSize);
		    subject.SendProgress();

		} while (!(lngFileSize == lngTransSize && lngTransSize >= 0));

		bRet = Transfer_Success;

		CIosWiFiMobileDevice::WiFiCloseTransHandle(nFileHandle);
	}

    IF_CANCEL(pObserver)
    {
        Ios_DeleteFile(remote_path);
        bRet = Transfer_Cancel;
    }

	return bRet;
}

TransferResult IosWiFiFileSystem::DoDownFromPhone(wstring remote_path, wstring local_path, ProgressSubject& subject)
{
	if (_nHandle <= 0 || local_path.empty()  || remote_path.empty() )
		return Transfer_Fail;

    ProgressObserver* pObserver = NULL;
    if (!subject._observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(subject._observers[0]);

	TransferResult bRet = Transfer_Unknown;

	//开始异步传输，返回文件句柄
	CFileOperation::MakeLocalDir(CFileOperation::GetFileDir(local_path));
	int nFileHandle = CIosWiFiMobileDevice::WiFiDownFromPhoneAsync(_nHandle, remote_path, local_path);

    subject.SetCurrentTaskSum(Ios_FileSize(remote_path));
    subject.SetCurrentTaskName(remote_path);

	if (nFileHandle > 0)
	{
		int lngFileSize = 0;
		int lngTransSize = 0;
		do 
		{
            IF_CANCEL_BREAK(pObserver)

			Sleep(50);//此处Sleep不可取消，取消后会造成文件传输被取消：赵领军2010.05.07
			//查询正在传输的文件的大小，已经传输了多少
			CIosWiFiMobileDevice::WiFiGetTransProgress(nFileHandle, lngFileSize, lngTransSize);

            subject.AddTaskProgress(lngFileSize);
            subject.SendProgress();

			// -1 表示文件还没有开始传输, 其它负值表示失败
			if (lngTransSize < 0 && lngTransSize != -1)
				THROW_ASSISTANT_EXCEPTION(IOS_WIFI_TRANSFER_ERROR);

		} while (!(lngFileSize == lngTransSize && lngTransSize >= 0));

		bRet = Transfer_Success;

		CIosWiFiMobileDevice::WiFiCloseTransHandle(nFileHandle);
	}

    IF_CANCEL(pObserver)
    {
        CFileOperation::DeleteFile(local_path);
        bRet = Transfer_Cancel;
    }

	return bRet;
}

int IosWiFiFileSystem::Ios_OpenFile(wstring strPath, string& buffer)
{
    //throw "Cannot Create File Steam Under WiFi Environment";
    return 0;
}

string IosWiFiFileSystem::Ios_GetFileSHA1(wstring strPath)
{
    //throw "Need ndDaemon to implement it ";
    return "";
}

string IosWiFiFileSystem::Ios_GetFileSHA1WithRange(wstring strPath, long endPosition)
{
    //throw "Need ndDaemon to implement it ";
    return "";
}