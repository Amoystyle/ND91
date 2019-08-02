#include "stdafx.h"
#include "IosDBBase.h"
#include "IosCreateDBSQL.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/AFCFileSysemBase.h"

#include "Module/System/KillProcessCommand.h"
#include "Module/File/DirectoryOperationCommand.h"

#define UNIQUENUMBERCALLDATA  16  // 追加到通话记录数据库文件名前缀字串后面的数字
#define UNIQUENUMBERMESSAGE   17  // 追加到短信数据库文件名前缀字串后面的数字
#define UNIQUENUMBERCONTACT   18  // 追加到联系人数据库文件名前缀字串后面的数字
#define UNIQUENUMBERNOTES   19    // 追加到备忘录数据库文件名前缀字串后面的数字
#define UNIQUENUMBERCALENDER   20 // 追加到日历数据库文件名前缀字串后面的数字

#ifdef WIN32
    #define BACKUPFOLDER                    L"D:\\我的文档\\91 Mobile\\iPhone\\Backup"             // 备份目录
#else
    #define BACKUPFOLDER                    L"/tmp/91Mobile/iPhone/Backup"                          // 备份目录
#endif

#define CALLDATADBFILENAME              L"call_history.db"                                      // 通话记录数据库文件名
#define MESSAGEDBFILENAME               L"sms.db"                                               // 短信数据库文件名
#define CONTACTDBFILENAME               L"AddressBook.sqlitedb"                                 // 联络人数据库文件名
#define CONTACTIMAGEDBFILENAME          L"AddressBookImages.sqlitedb"                           // 联络人图片数据库文件名
#define CALENDERDBFILENAME              L"Calendar.sqlitedb"                                    // 日历数据库文件名
#define PRODUCTVERSIONH                 400                                                     // IOS设备高版本号
#define PRODUCTVERSIONL                 112                                                     // IOS设备低版本号
#define MOBILEUSERID                    L"501"                                                   // mobile用户ID
#define WIRLESSUSERID                   L"25"                                                    // wireless用户ID
#define NOTESDBNAME						L"notes.db"
#define NOTESDBNAMEH						L"notes.sqlite"

IosDBBase::IosDBBase(IosDBPath iosDBPath)
: _iosDBPath(iosDBPath)
{
    _dbPath = GetTempDBFilePath();
}

IosDBBase::~IosDBBase(void)
{
}

void IosDBBase::SetDBPath(wstring path)
{
    _dbPath = path;
}

bool IosDBBase::OpenDB()
{
    if (CFileOperation::IsFileExist(_dbPath.c_str()))
        return CppSQLite3DB::Open(CCodeOperation::UnicodeToUTF_8(_dbPath).c_str());
    return false;
}

void IosDBBase::CloseDB()
{
    return CppSQLite3DB::Close();
}

bool IosDBBase::ExecSQL(const char *sql)
{
    int change = 0;
    OpenDB();
    BeginTransaction();
    try
    {
        change = ExecDML(sql);
        Commit();
    }
    catch (CppSQLite3Exception& e)
    {
        int iError = e.ErrorCode();
        const char* szError = e.ErrorMessage();
        Rollback();
    }
    CloseDB();

    if (change > 0)
        return true;
    else
        return false;
}

// 创建数据库文件
bool IosDBBase::CreateDBFile(wstring dbFilePath)
{
    if (!CreateFile(dbFilePath))
        return false;

    string strSql = "";
    if (_iosDBPath.GetDBType() == ISCALLDATADB)
        strSql = IosCreateDBSQL::CreateCallDataDBSQL();
    else if (_iosDBPath.GetDBType() == ISMESSAGEDB)
        strSql = IosCreateDBSQL::CreateMessageDBSQL(_iosDBPath.GetProductVersion());
    else if (_iosDBPath.GetDBType() == ISCONTACTDB)
        strSql = IosCreateDBSQL::CreateContactDBSQL(_iosDBPath.GetProductVersion());
    else if (_iosDBPath.GetDBType() == ISCONTACTIMAGEDB)
        strSql = IosCreateDBSQL::CreateContactImageDBSQL(_iosDBPath.GetProductVersion());
    else if (_iosDBPath.GetDBType() == ISCALENDERDB)
        strSql = IosCreateDBSQL::CreateCalenderDBSQL(_iosDBPath.GetProductVersion());
	else if(_iosDBPath.GetDBType() == ISBOOKMARKDB)
		strSql = IosCreateDBSQL::CreateBookMarkDBSQL();

    return CreateDB(strSql);
}

bool IosDBBase::CreateFile(wstring dbFilePath)
{
    return CppSQLite3DB::Open(CCodeOperation::UnicodeToUTF_8(dbFilePath).c_str());
}

bool IosDBBase::CreateDB(string strSql)
{
    return ExecDML(strSql.c_str()) > 0 ? true : false;
}

string IosDBBase::GetRowidStr(vector<int> rowidVec)
{
    if (0 == rowidVec.size())
        return "";

    string idstr = "";
    for (size_t i = 0; i < rowidVec.size(); i++)
    {
        CppSQLite3Buffer id;
        id.Format("%d", rowidVec[i]);
        idstr += (const char*)id;
        idstr += ",";
    }

    idstr = CStrOperation::trimRight(idstr, ",");
    idstr = "(" + idstr + ")";
    return idstr;// 类似 (1,2,3)
}

wstring IosDBBase::GetTempDBFilePath()
{
    // 文件存放在本地的临时目录
    wstring destinationOnComputer = _iosDBPath.GetTempFolder();
    string strdestinationOnComputer = CCodeOperation::UnicodeToUTF_8(destinationOnComputer);

    int uUnique = 0;
    if (_iosDBPath.GetDBType() == ISCALLDATADB)
        uUnique = UNIQUENUMBERCALLDATA;
    else if (_iosDBPath.GetDBType() == ISMESSAGEDB)
        return destinationOnComputer+L"sms\\sms.db";
	else if (_iosDBPath.GetDBType() == ISNOTESDB)
		uUnique = UNIQUENUMBERNOTES;
    else if (_iosDBPath.GetDBType() == ISCALENDERDB)
        uUnique = UNIQUENUMBERCALENDER;
    else if (_iosDBPath.GetDBType() == ISBOOKMARKDB)
		return destinationOnComputer+CFileOperation::GetFileName(_iosDBPath.GetDBPath());
	else
        return GetContactTempDBFilePath();

#ifdef WIN32
		char name[256] = {0};
    ::GetTempFileNameA(strdestinationOnComputer.c_str(),"tmp",uUnique,name);
    string strName = name;
#else
		char szTempFile[] = "temp_XXXXXX";
    char* pszTempFile = mktemp(szTempFile);
    string strName = pszTempFile;
#endif

		return CCodeOperation::UTF_8ToUnicode(strName);	
}

// 获取联络人数据库临时文件
wstring IosDBBase::GetContactTempDBFilePath()
{	
    wstring serial = GetSerial();
    wstring tempOnComputerFolder = _iosDBPath.GetTempFolder();
    wstring dir = tempOnComputerFolder + L"\\" + serial + L"\\";

#ifdef WIN32
    if (!CFileOperation::IsDirExist(dir.c_str()))
        ::CreateDirectory(dir.c_str(), NULL);
#else
		if (!CFileOperation::IsDirExist(dir.c_str()))
				mkdir(CCodeOperation::UnicodeToUTF_8(dir).c_str(), 0777);
#endif        		

    wstring dbFile = L"";
    if (_iosDBPath.GetDBType() == ISCONTACTDB)
        dbFile = dir + CONTACTDBFILENAME;
    else if (_iosDBPath.GetDBType() == ISCONTACTIMAGEDB)
        dbFile = dir + CONTACTIMAGEDBFILENAME;
 
    return dbFile;
}

wstring IosDBBase::GetSerial()
{
    wstring strSerial = _iosDBPath.GetDeviceSerial();
    if (strSerial == L"")
        //默认序列号，采用随机guid，不足40位部分用0补其
        strSerial = CFileOperation::GetGuidW();
    return strSerial;
}

// 同步到IOS设备
bool IosDBBase::SyncToIos(const DeviceData* pDevice)
{
    wstring sourceOnIos = _iosDBPath.GetDBPath();                                          // 文件存放在IOS设备的路径
    wstring destinationOnComputer = GetTempDBFilePath();                                   // 文件存放在本地的路径

    IIosFileSystem* pFileStream = NULL;
    if ( pDevice->GetDevInfo()->_connectType == WIFI)
        pFileStream = new IosWiFiFileSystem(pDevice->GetDevInfo()->_ios_wifi_handle);
    else if (pDevice->GetDevInfo()->_connectType == USB)
        pFileStream = new AFCFileSysemBase(pDevice->GetDevInfo());
    
    if (!pFileStream)
        return false;
    pFileStream->Ios_CopyToPhone(destinationOnComputer,sourceOnIos);

	if(_iosDBPath.GetDBType() == ISMESSAGEDB)
	{
		KillProcessCommand cmd(pDevice, L"MobileSMS");
		cmd.Execute();

		if(CFileOperation::IsFileExist((destinationOnComputer + L"-shm").c_str()))
			pFileStream->Ios_CopyToPhone(destinationOnComputer + L"-shm", sourceOnIos+L"-shm");
		else
			pFileStream->Ios_DeleteFile(sourceOnIos+L"-shm");
		if(CFileOperation::IsFileExist((destinationOnComputer + L"-wal").c_str()))
			pFileStream->Ios_CopyToPhone(destinationOnComputer + L"-wal", sourceOnIos+L"-wal");
		else
			pFileStream->Ios_DeleteFile(sourceOnIos+L"-wal");

		vector<wstring> filepaths;
		wstring locfolder = CFileOperation::GetFileDir(destinationOnComputer);
		wstring iosFolder = CFileOperation::GetFileDir(sourceOnIos);
		CFileOperation::GetMatchFileFullName(CStrOperation::trimRightW(locfolder,L"\\"),L"",filepaths);
		for (size_t i = 0; i < filepaths.size(); i++)
		{
			wstring remotepath = CStrOperation::ReplaceW(filepaths[i], locfolder, iosFolder);
			remotepath = CStrOperation::ReplaceW(remotepath,L"\\",L"/");
			if(!pFileStream->Ios_Exists(remotepath))
				pFileStream->Ios_CopyToPhone(filepaths[i], remotepath);
		}

		DirectoryOperationCommand dircmd(pDevice);
		dircmd.ChangeMode(CFileOperation::GetFileDir(sourceOnIos),"755",L"501");
	}
    SAFE_DELETE(pFileStream);
    return true;
}

// 加载数据库
bool IosDBBase::Reload(const DeviceData* pDevice)
{
    bool result = false;
    wstring sourceOnIos = _iosDBPath.GetDBPath();                                       // 文件存放在IOS设备的路径
    wstring destinationOnComputer = GetTempDBFilePath();                                // 文件存放在本地的路径

    IIosFileSystem* pFileStream = NULL;
    if (pDevice->GetDevInfo()->_connectType == WIFI)
        pFileStream = new IosWiFiFileSystem(pDevice->GetDevInfo()->_ios_wifi_handle);
    else if (pDevice->GetDevInfo()->_connectType == USB)
        pFileStream = new AFCFileSysemBase(pDevice->GetDevInfo());

    if (!pFileStream)
        return false;

    // 成功从IOS设备上下载数据库文件
    if (pFileStream->Ios_DownFromPhone(sourceOnIos, destinationOnComputer) == Transfer_Success)
    {
        // 复制数据库文件到备份目录
        CopyDBFileToBackupFolder();
        result = true;
    }
    else if (!pFileStream->Ios_Exists(sourceOnIos))// 如果IOS设备上不存在数据库文件，则在本地创建并上传
    {
        // 在本地创建数据库文件
        if (!CreateDBFile(destinationOnComputer))
        {
            SAFE_DELETE(pFileStream);
            return result;
        }

        wstring dbFolder = _iosDBPath.GetDBFolder();// 数据库文件存在IOS设备上的目录
        // 如果目录不存在则创建
        if (!pFileStream->Ios_Exists(dbFolder))
            // 创建目录并设置目录所有者
            if (pFileStream->Ios_CreateDirectory(dbFolder))
                // 设置数据库目录所有者
                SetDBFileOwn(pDevice,dbFolder);  

        // 将数据库文件上传至IOS设备
        if (pFileStream->Ios_CopyToPhone(destinationOnComputer, sourceOnIos) == Transfer_Success)
            // 设置文件所有者
            SetDBFileOwn(pDevice,sourceOnIos);
    }
	_dbPath = destinationOnComputer;
    SAFE_DELETE(pFileStream);
    return result;
}

// 复制数据库文件到备份目录
void IosDBBase::CopyDBFileToBackupFolder()
{
    wstring backupFolder = BACKUPFOLDER;                                        // 备份目录
    wstring backupPath = backupFolder + L"\\";                                  // 文件备份路径

    if (_iosDBPath.GetDBType() == ISCALLDATADB)
        backupPath += CALLDATADBFILENAME;
    else if (_iosDBPath.GetDBType() == ISMESSAGEDB)
        backupPath += MESSAGEDBFILENAME;
    else if (_iosDBPath.GetDBType() == ISCONTACTDB)
        backupPath += CONTACTDBFILENAME;
    else if (_iosDBPath.GetDBType() == ISCONTACTIMAGEDB)
        backupPath += CONTACTIMAGEDBFILENAME;
	else if (_iosDBPath.GetDBType() == ISNOTESDB)
	{
		if(_iosDBPath.GetProductVersion() > 400)
				backupPath += NOTESDBNAMEH;
		else
				backupPath += NOTESDBNAME;
	}
    else if (_iosDBPath.GetDBType() == ISCALENDERDB)
        backupPath += CALENDERDBFILENAME;

    wstring destinationOnComputer = GetTempDBFilePath();                        // 文件存放在本地的路径
    CFileOperation::CopyFile(destinationOnComputer, backupPath, false );
}

// 设置文件所有者
void IosDBBase::SetDBFileOwn(const DeviceData* pDevice, wstring path)
{
    //SetOwnCommand setown(pDevice);
	DirectoryOperationCommand cmd(pDevice);
    if (_iosDBPath.GetDBType() == ISCALLDATADB)
    {
        if (_iosDBPath.GetProductVersion() > PRODUCTVERSIONL && _iosDBPath.GetProductVersion() < PRODUCTVERSIONH)
            //setown.SetParams(path, MOBILEUSERID);
			cmd.ChangeMode(path, "", MOBILEUSERID);
        else if (_iosDBPath.GetProductVersion() >= PRODUCTVERSIONH)
            //setown.SetParams(path, WIRLESSUSERID);     
			cmd.ChangeMode(path, "", WIRLESSUSERID);
    }
    else
    {
        if (_iosDBPath.GetProductVersion() > PRODUCTVERSIONL)
            //setown.SetParams(path, MOBILEUSERID);
			cmd.ChangeMode(path, "", MOBILEUSERID);
    }
    //setown.Execute();
}

// 执行SELECT语句并返回rowid
int IosDBBase::ExecuteAndGetId(const char* sql)
{
    int id = -1;
	CppSQLite3Query q = ExecQuery(sql);
	while (!q.Eof())
	{
		id = q.GetIntField(0);
		q.NextRow();
	}
    return id;
}