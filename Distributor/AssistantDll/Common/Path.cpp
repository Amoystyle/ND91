#include "stdafx.h"

#include "Common/Path.h"

#include "Device/IosPublicFunc.h"
#include "Core/DeviceInfo.h"

Path::Path(const DeviceInfo* pInfo )
:_pInfo(pInfo)
{
    //int pid = _getpid();
    int pid = getpid();

#ifdef WIN32
    wchar_t buff[MAX_PATH] = {0};
    ::GetTempPath(MAX_PATH, buff);
#else
    wstring buff = L"/tmp/";
#endif

    wstring sn = pInfo->_strSerialNumber;
    if ( sn == L"?" )
    {
        sn = CFileOperation::GetGuidW();
    }

	_version = IosPublicFunc::GetVerNum(_pInfo->_deviceProductVersion);
    wostringstream woss;
    woss << buff << L"Assistant_" << pid << '_' << sn << /*'\\'*/CHAR_PATH_SEPARATER;
    _workFolder = woss.str();

    CFileOperation::MakeLocalDir(_workFolder);

#ifndef ASSISTANT_LITE
    _pIosPicturePath = new IosPicturePath(_workFolder, pInfo);
    _pIosRingPath = new IosRingPath(_workFolder, pInfo->_ios_bJailBreaked);
    _pAndroidPath = new AndroidPath(_workFolder);
    _pThemePath = new ThemePath(_workFolder);
	_pIosRecordingPath = new IosRecordingPath(_workFolder, pInfo->_ios_bJailBreaked);
#endif
}

Path::~Path()
{
    SAFE_DELETE(_pIosPicturePath);
    SAFE_DELETE(_pIosRingPath);
    SAFE_DELETE(_pAndroidPath);
    SAFE_DELETE(_pThemePath);
	SAFE_DELETE(_pIosRecordingPath);

    CFileOperation::DeleteDirectory(_workFolder);
}

std::wstring ND91Assistant::Path::GetLocalIconPath() const
{
	//wstring strPath = _workFolder + L"IconImage\\";
    wstring strPath = _workFolder + L"IconImage";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetLocalAppPath() const
{
	//wstring strPath = _workFolder + L"App\\";
    wstring strPath = _workFolder + L"App";
		strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetLocalIpaPath() const
{
	//wstring strPath = _workFolder + L"App\\Ipa\\";
    wstring strPath = _workFolder + L"App";
	strPath += WSTRING_PATH_SEPARATER;
	strPath += L"Ipa";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetLocalAudioPath() const
{
    //wstring strPath = _workFolder + L"Audio\\Audio\\";
    wstring strPath = _workFolder + L"Audio";
	strPath += WSTRING_PATH_SEPARATER;
	strPath += L"Audio";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetLocalAlbumPath() const
{
    //wstring strPath = _workFolder + L"Audio\\Album\\";
    wstring strPath = _workFolder;
	strPath += L"Audio";
	strPath += WSTRING_PATH_SEPARATER;
	strPath += L"Album";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetLocalPimPhotoPath() const
{
    //wstring strPath = _workFolder + L"Pim\\Photo\\";
    wstring strPath = _workFolder;
	strPath += L"Pim";
	strPath += WSTRING_PATH_SEPARATER;
	strPath += L"Photo";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetLocalPimPath() const
{
    //wstring strPath = _workFolder + L"Pim\\";
    wstring strPath = _workFolder;
	strPath += L"Pim";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetLocalScreensnapPath() const
{
    //wstring strPath = _workFolder + L"ScreenSnap\\";
    wstring strPath = _workFolder;
	strPath += L"ScreenSnap";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GlobalTemp()
{
    // 与设备无关的临时工作目录, 仅包含进程ID
    static wstring globalTemp;

    if ( globalTemp.empty() )
    {
        //int pid = _getpid();
        int pid = getpid();

#ifdef WIN32
        wchar_t buff[MAX_PATH] = {0};
        ::GetTempPath(MAX_PATH, buff);
#else
        wstring buff = L"/tmp/";
#endif

        wostringstream woss;
        woss << buff << L"Assistant_" << pid << /*'\\'*/CHAR_PATH_SEPARATER;

        globalTemp = woss.str();
    }

	CFileOperation::MakeLocalDir(globalTemp);

    return globalTemp;
}

std::wstring ND91Assistant::Path::GlobalTempFile()
{
   wstring strTempDir = GlobalTemp();
#ifdef WIN32
   wchar_t buff[MAX_PATH] = {0}; 
    ::CreateDirectory(strTempDir.c_str(), NULL);
    ::GetTempFileName(GlobalTemp().c_str(), L"temp_", MAX_PATH, buff);
    return wstring(buff);
#else
    mkdir(CCodeOperation::UnicodeToUTF_8(strTempDir).c_str(), 0777);

    char szTempFile[] = "temp_XXXXXX";
    char* pszTempFile = mktemp(szTempFile);

    string path = CCodeOperation::UnicodeToUTF_8(strTempDir) + pszTempFile;
    return wstring(CCodeOperation::UTF_8ToUnicode(path).c_str());
#endif
}

std::wstring ND91Assistant::Path::GetLocalSmsPath() const
{
    //wstring strPath = _workFolder + L"Sms\\";
    wstring strPath = _workFolder;
	strPath += L"Sms";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetLocalCallDataPath() const
{
    //wstring strPath = _workFolder + L"CallData\\";
    wstring strPath = _workFolder;
	strPath += L"CallData";
	strPath += WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

std::wstring ND91Assistant::Path::GetRandomName()
{
    time_t now;
    time(&now);
    wchar_t buffer[256] = {0};
    //swprintf_s(buffer, 256, L"%x", now);
    swprintf(buffer, 256, L"%x", now);
    return wstring(buffer);
}

std::wstring ND91Assistant::Path::IosCheckPathByVersion(wstring path) const
{
	return IosPublicFunc::CheckPathByVersion(path, _version, _pInfo->_ios_bJailBreaked);
}

std::wstring ND91Assistant::Path::GetIosClockPlistPath() const
{
	if(_version > 320)
		return IosCheckPathByVersion(L"/private/var/root/Library/Preferences/com.apple.mobiletimer.plist");
	else
		return IosCheckPathByVersion(L"/private/var/root/Library/Preferences/com.apple.springboard.plist");
}

wstring Path::GetLocalVideoPath() const
{
    //wstring strPath = _workFolder + L"Viedo\\";
    wstring strPath = _workFolder + L"Viedo" + WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}

wstring Path::GetLocalVideoImagePath() const
{
    //wstring strPath = _workFolder + L"Viedo\\Images\\";
    wstring strPath = _workFolder + L"Viedo" + WSTRING_PATH_SEPARATER + L"Images" + WSTRING_PATH_SEPARATER;
	CFileOperation::MakeLocalDir(strPath);
	return strPath;
}
