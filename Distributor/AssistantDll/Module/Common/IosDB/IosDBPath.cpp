#include "stdafx.h"

#include "Common/Path.h"
#include "IosDBPath.h"

#include "Device/IosPublicFunc.h"

#define CALLDATADBONIOSPATHH	        L"/private/var/wireless/Library/CallHistory/call_history.db"     // 通话记录数据库文件在IOS设备上的路径(含400以上版本)
#define CALLDATADBONIOSPATHL	        L"/private/var/root/Library/CallHistory/call_history.db"         // 通话记录数据库文件在IOS设备上的路径(400以下版本)
#define CALLDATADBONIOSFOLDERH          L"/private/var/wireless/Library/CallHistory"                     // 通话记录数据库文件在IOS设备上的目录(含400以上版本)
#define CALLDATADBONIOSFOLDERL          L"/private/var/root/Library/CallHistory"                         // 通话记录数据库文件在IOS设备上的目录(400以下版本)
#define MESSAGEDBONIOSPATH              L"/private/var/root/Library/SMS/sms.db"                          // 短信数据库文件在IOS设备上的路径
#define MESSAGEDBONIOSFOLDER            L"/private/var/root/Library/SMS"                                 // 短信数据库文件在IOS设备上的目录
#define ADDRESSBOOKDBONIOSPATH	        L"/private/var/mobile/Library/AddressBook/AddressBook.sqlitedb"         // 联络人数据库文件在IOS设备上的路径
#define ADDRESSBOOKDBONIOSFOLDER	    L"/private/var/mobile/Library/AddressBook"                              // 联络人数据库文件在IOS设备上的目录
#define ADDRESSBOOKIMAGEDBONIOSPATH	    L"/private/var/mobile/Library/AddressBook/AddressBookImages.sqlitedb"   // 联络人图片数据库文件在IOS设备上的路径
#define PRODUCTVERSIONH                 400                                                     // IOS设备高版本号
#define PRODUCTVERSIONL                 113                                                     // IOS设备低版本号
#define NOTESDBONIOSPATHL				L"/private/var/root/Library/Notes/notes.db"			//备忘录数据库路径
#define NOTESDBONIOSFOLDERL				L"/private/var/root/Library/Notes"					//备忘录数据库目录
#define NOTESDBONIOSPATHH				L"/private/var/mobile/Library/Notes/notes.db"			//备忘录数据库路径
#define NOTESDBONIOSFOLDERH				L"/private/var/mobile/Library/Notes"					//备忘录数据库目录
#define NOTESDBONIOSPATH400				L"/private/var/root/Library/Notes/notes.sqlite"			//备忘录数据库路径
#define CALENDERDBONIOSFOLDERH          L"/private/var/mobile/Library/Calendar"			// 日历数据库目录
#define CALENDERDBONIOSFOLDERL          L"/private/var/root/Library/Calendar"			// 日历数据库目录
#define CALENDERDBONIOSPATHH          L"/private/var/mobile/Library/Calendar/Calendar.sqlitedb"			// 日历数据库路径
#define CALENDERDBONIOSPATHL          L"/private/var/root/Library/Calendar/Calendar.sqlitedb"			// 日历数据库路径
#define BOOKMARKDBPATH					L"/private/var/root/Library/Safari/Bookmarks.db"        //书签

IosDBPath::IosDBPath(const DeviceData* pDevice, ISDBTYPE type)
{
    _pDevice = pDevice;
    _type = type;

    _iProductVersion = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);
    _bJailBreaked = _pDevice->GetDevInfo()->_ios_bJailBreaked;
    _deviceSerial = _pDevice->GetDevInfo()->_ios_deviceIdentifier;
    _deviceProductType = _pDevice->GetDevInfo()->_deviceProductType;
}

IosDBPath::~IosDBPath(void)
{
}

wstring IosDBPath::GetTempFolder()
{
    wstring localpath = _pDevice->GetPath()->GetWorkFolder();
    // 如果不存在目录，创建各种存放临时数据的目录
    RETURN_FALSE_IF(!CFileOperation::MakeLocalDir(localpath));
    return localpath;
}

// 获取数据库在IOS设备上的路径
wstring IosDBPath::GetDBPath()
{
    if (_type == ISCALLDATADB)
        return GetCallDataDBPath();
    else if (_type == ISMESSAGEDB)
        return GetMessageDBPath();
    else if (_type == ISCONTACTDB)
        return GetContactDBPath();
    else if (_type == ISCONTACTIMAGEDB)
        return GetContactImageDBPath();
    else if (_type == ISNOTESDB)
		return GetNotesDBPath();
    else if (_type == ISCALENDERDB)
        return GetCalenderDBPath();
	else if(_type == ISBOOKMARKDB)
		return GetBookMarkDBPath();
	else
        return L"";
}

// 获取数据库在IOS设备上的目录
wstring IosDBPath::GetDBFolder()
{
    if (_type == ISCALLDATADB)
        return GetCallDataDBFolder();
    else if (_type == ISMESSAGEDB)
        return GetMessageDBFolder();
    else if (_type == ISCONTACTDB)
        return GetContactDBFolder();
    else if (_type == ISCONTACTIMAGEDB)
        return GetContactImageDBFolder();
	else if (_type == ISNOTESDB)
		return GetNotesDBFolder();
    else if (_type == ISCALENDERDB)
        return GetCalenderDBFolder();
    else if(_type == ISBOOKMARKDB)
		return CFileOperation::GetFileDir(GetBookMarkDBPath());
	else
        return L"";
}
// 获取备忘录数据库在IOS设备上的路径
wstring IosDBPath::GetNotesDBPath()
{
	 if (_iProductVersion >= PRODUCTVERSIONH)
		return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSPATH400, _iProductVersion, _bJailBreaked);
	 else  if (_iProductVersion >= PRODUCTVERSIONL)
		 return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSPATHH, _iProductVersion, _bJailBreaked);
	 else
		 return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSPATHL, _iProductVersion, _bJailBreaked);
}

// 获取备忘录数据库在IOS设备上的目录
wstring IosDBPath::GetNotesDBFolder()
{
	if (_iProductVersion >= PRODUCTVERSIONL)
		return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSFOLDERH, _iProductVersion, _bJailBreaked);
	else
		return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSFOLDERL, _iProductVersion, _bJailBreaked);
}

// 获取短信数据库在IOS设备上的路径
wstring IosDBPath::GetMessageDBPath()
{
    return IosPublicFunc::CheckPathByVersion(MESSAGEDBONIOSPATH, _iProductVersion, _bJailBreaked);
}

// 获取短信数据库在IOS设备上的目录
wstring IosDBPath::GetMessageDBFolder()
{
    return IosPublicFunc::CheckPathByVersion(MESSAGEDBONIOSFOLDER, _iProductVersion, _bJailBreaked);
}

// 获取通话记录数据库在IOS设备上的路径
wstring IosDBPath::GetCallDataDBPath()
{
    if (_iProductVersion >= PRODUCTVERSIONH)
        return IosPublicFunc::CheckPathByVersion(CALLDATADBONIOSPATHH, _iProductVersion, _bJailBreaked);
    else
        return IosPublicFunc::CheckPathByVersion(CALLDATADBONIOSPATHL, _iProductVersion, _bJailBreaked);
}

// 获取通话记录数据库在IOS设备上的目录
wstring IosDBPath::GetCallDataDBFolder()
{
    if (_iProductVersion >= PRODUCTVERSIONH)
        return IosPublicFunc::CheckPathByVersion(CALLDATADBONIOSFOLDERH, _iProductVersion, _bJailBreaked);
    else
        return IosPublicFunc::CheckPathByVersion(CALLDATADBONIOSFOLDERL, _iProductVersion, _bJailBreaked);
}

// 获取日历数据库在IOS设备上的路径
wstring IosDBPath::GetCalenderDBPath()
{
    if (_iProductVersion >= PRODUCTVERSIONL)
        return IosPublicFunc::CheckPathByVersion(CALENDERDBONIOSPATHH, _iProductVersion, _bJailBreaked);
    else
        return IosPublicFunc::CheckPathByVersion(CALENDERDBONIOSPATHL, _iProductVersion, _bJailBreaked);
}

// 获取日历数据库在IOS设备上的目录
wstring IosDBPath::GetCalenderDBFolder()
{
    if (_iProductVersion >= PRODUCTVERSIONL)
        return IosPublicFunc::CheckPathByVersion(CALENDERDBONIOSFOLDERH, _iProductVersion, _bJailBreaked);
    else
        return IosPublicFunc::CheckPathByVersion(CALENDERDBONIOSFOLDERL, _iProductVersion, _bJailBreaked);
}

// 获取联络人数据库在IOS设备上的路径
wstring IosDBPath::GetContactDBPath()
{
    return ADDRESSBOOKDBONIOSPATH;
}

// 获取联络人数据库在IOS设备上的目录
wstring IosDBPath::GetContactDBFolder()
{
    return ADDRESSBOOKDBONIOSFOLDER;
}

// 获取联络人图片数据库在IOS设备上的路径
wstring IosDBPath::GetContactImageDBPath()
{
    return ADDRESSBOOKIMAGEDBONIOSPATH;
}

// 获取联络人图片数据库在IOS设备上的目录
wstring IosDBPath::GetContactImageDBFolder()
{
    return ADDRESSBOOKDBONIOSFOLDER;
}

int IosDBPath::GetProductVersion()
{
    return _iProductVersion;
}

bool IosDBPath::GetJailBreaked()
{
    return _bJailBreaked;
}

wstring IosDBPath::GetDeviceSerial()
{
    return _deviceSerial;
}

wstring IosDBPath::GetDeviceProductType()
{
    return _deviceProductType;
}

ISDBTYPE IosDBPath::GetDBType()
{
    return _type;
}

const DeviceData* IosDBPath::GetDeviceData()
{
    return _pDevice;
}

std::wstring ND91Assistant::IosDBPath::GetBookMarkDBPath()
{
	return IosPublicFunc::CheckPathByVersion(BOOKMARKDBPATH, _iProductVersion, _bJailBreaked);
}