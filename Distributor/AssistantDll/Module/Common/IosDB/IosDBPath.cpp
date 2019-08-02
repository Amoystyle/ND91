#include "stdafx.h"

#include "Common/Path.h"
#include "IosDBPath.h"

#include "Device/IosPublicFunc.h"

#define CALLDATADBONIOSPATHH	        L"/private/var/wireless/Library/CallHistory/call_history.db"     // ͨ����¼���ݿ��ļ���IOS�豸�ϵ�·��(��400���ϰ汾)
#define CALLDATADBONIOSPATHL	        L"/private/var/root/Library/CallHistory/call_history.db"         // ͨ����¼���ݿ��ļ���IOS�豸�ϵ�·��(400���°汾)
#define CALLDATADBONIOSFOLDERH          L"/private/var/wireless/Library/CallHistory"                     // ͨ����¼���ݿ��ļ���IOS�豸�ϵ�Ŀ¼(��400���ϰ汾)
#define CALLDATADBONIOSFOLDERL          L"/private/var/root/Library/CallHistory"                         // ͨ����¼���ݿ��ļ���IOS�豸�ϵ�Ŀ¼(400���°汾)
#define MESSAGEDBONIOSPATH              L"/private/var/root/Library/SMS/sms.db"                          // �������ݿ��ļ���IOS�豸�ϵ�·��
#define MESSAGEDBONIOSFOLDER            L"/private/var/root/Library/SMS"                                 // �������ݿ��ļ���IOS�豸�ϵ�Ŀ¼
#define ADDRESSBOOKDBONIOSPATH	        L"/private/var/mobile/Library/AddressBook/AddressBook.sqlitedb"         // ���������ݿ��ļ���IOS�豸�ϵ�·��
#define ADDRESSBOOKDBONIOSFOLDER	    L"/private/var/mobile/Library/AddressBook"                              // ���������ݿ��ļ���IOS�豸�ϵ�Ŀ¼
#define ADDRESSBOOKIMAGEDBONIOSPATH	    L"/private/var/mobile/Library/AddressBook/AddressBookImages.sqlitedb"   // ������ͼƬ���ݿ��ļ���IOS�豸�ϵ�·��
#define PRODUCTVERSIONH                 400                                                     // IOS�豸�߰汾��
#define PRODUCTVERSIONL                 113                                                     // IOS�豸�Ͱ汾��
#define NOTESDBONIOSPATHL				L"/private/var/root/Library/Notes/notes.db"			//����¼���ݿ�·��
#define NOTESDBONIOSFOLDERL				L"/private/var/root/Library/Notes"					//����¼���ݿ�Ŀ¼
#define NOTESDBONIOSPATHH				L"/private/var/mobile/Library/Notes/notes.db"			//����¼���ݿ�·��
#define NOTESDBONIOSFOLDERH				L"/private/var/mobile/Library/Notes"					//����¼���ݿ�Ŀ¼
#define NOTESDBONIOSPATH400				L"/private/var/root/Library/Notes/notes.sqlite"			//����¼���ݿ�·��
#define CALENDERDBONIOSFOLDERH          L"/private/var/mobile/Library/Calendar"			// �������ݿ�Ŀ¼
#define CALENDERDBONIOSFOLDERL          L"/private/var/root/Library/Calendar"			// �������ݿ�Ŀ¼
#define CALENDERDBONIOSPATHH          L"/private/var/mobile/Library/Calendar/Calendar.sqlitedb"			// �������ݿ�·��
#define CALENDERDBONIOSPATHL          L"/private/var/root/Library/Calendar/Calendar.sqlitedb"			// �������ݿ�·��
#define BOOKMARKDBPATH					L"/private/var/root/Library/Safari/Bookmarks.db"        //��ǩ

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
    // ���������Ŀ¼���������ִ����ʱ���ݵ�Ŀ¼
    RETURN_FALSE_IF(!CFileOperation::MakeLocalDir(localpath));
    return localpath;
}

// ��ȡ���ݿ���IOS�豸�ϵ�·��
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

// ��ȡ���ݿ���IOS�豸�ϵ�Ŀ¼
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
// ��ȡ����¼���ݿ���IOS�豸�ϵ�·��
wstring IosDBPath::GetNotesDBPath()
{
	 if (_iProductVersion >= PRODUCTVERSIONH)
		return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSPATH400, _iProductVersion, _bJailBreaked);
	 else  if (_iProductVersion >= PRODUCTVERSIONL)
		 return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSPATHH, _iProductVersion, _bJailBreaked);
	 else
		 return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSPATHL, _iProductVersion, _bJailBreaked);
}

// ��ȡ����¼���ݿ���IOS�豸�ϵ�Ŀ¼
wstring IosDBPath::GetNotesDBFolder()
{
	if (_iProductVersion >= PRODUCTVERSIONL)
		return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSFOLDERH, _iProductVersion, _bJailBreaked);
	else
		return IosPublicFunc::CheckPathByVersion(NOTESDBONIOSFOLDERL, _iProductVersion, _bJailBreaked);
}

// ��ȡ�������ݿ���IOS�豸�ϵ�·��
wstring IosDBPath::GetMessageDBPath()
{
    return IosPublicFunc::CheckPathByVersion(MESSAGEDBONIOSPATH, _iProductVersion, _bJailBreaked);
}

// ��ȡ�������ݿ���IOS�豸�ϵ�Ŀ¼
wstring IosDBPath::GetMessageDBFolder()
{
    return IosPublicFunc::CheckPathByVersion(MESSAGEDBONIOSFOLDER, _iProductVersion, _bJailBreaked);
}

// ��ȡͨ����¼���ݿ���IOS�豸�ϵ�·��
wstring IosDBPath::GetCallDataDBPath()
{
    if (_iProductVersion >= PRODUCTVERSIONH)
        return IosPublicFunc::CheckPathByVersion(CALLDATADBONIOSPATHH, _iProductVersion, _bJailBreaked);
    else
        return IosPublicFunc::CheckPathByVersion(CALLDATADBONIOSPATHL, _iProductVersion, _bJailBreaked);
}

// ��ȡͨ����¼���ݿ���IOS�豸�ϵ�Ŀ¼
wstring IosDBPath::GetCallDataDBFolder()
{
    if (_iProductVersion >= PRODUCTVERSIONH)
        return IosPublicFunc::CheckPathByVersion(CALLDATADBONIOSFOLDERH, _iProductVersion, _bJailBreaked);
    else
        return IosPublicFunc::CheckPathByVersion(CALLDATADBONIOSFOLDERL, _iProductVersion, _bJailBreaked);
}

// ��ȡ�������ݿ���IOS�豸�ϵ�·��
wstring IosDBPath::GetCalenderDBPath()
{
    if (_iProductVersion >= PRODUCTVERSIONL)
        return IosPublicFunc::CheckPathByVersion(CALENDERDBONIOSPATHH, _iProductVersion, _bJailBreaked);
    else
        return IosPublicFunc::CheckPathByVersion(CALENDERDBONIOSPATHL, _iProductVersion, _bJailBreaked);
}

// ��ȡ�������ݿ���IOS�豸�ϵ�Ŀ¼
wstring IosDBPath::GetCalenderDBFolder()
{
    if (_iProductVersion >= PRODUCTVERSIONL)
        return IosPublicFunc::CheckPathByVersion(CALENDERDBONIOSFOLDERH, _iProductVersion, _bJailBreaked);
    else
        return IosPublicFunc::CheckPathByVersion(CALENDERDBONIOSFOLDERL, _iProductVersion, _bJailBreaked);
}

// ��ȡ���������ݿ���IOS�豸�ϵ�·��
wstring IosDBPath::GetContactDBPath()
{
    return ADDRESSBOOKDBONIOSPATH;
}

// ��ȡ���������ݿ���IOS�豸�ϵ�Ŀ¼
wstring IosDBPath::GetContactDBFolder()
{
    return ADDRESSBOOKDBONIOSFOLDER;
}

// ��ȡ������ͼƬ���ݿ���IOS�豸�ϵ�·��
wstring IosDBPath::GetContactImageDBPath()
{
    return ADDRESSBOOKIMAGEDBONIOSPATH;
}

// ��ȡ������ͼƬ���ݿ���IOS�豸�ϵ�Ŀ¼
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