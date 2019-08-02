#include "stdafx.h"
#include "IosDBBase.h"
#include "IosCreateDBSQL.h"
#include "Device/IosWiFiFileSystem.h"
#include "Device/AFCFileSysemBase.h"

#include "Module/System/KillProcessCommand.h"
#include "Module/File/DirectoryOperationCommand.h"

#define UNIQUENUMBERCALLDATA  16  // ׷�ӵ�ͨ����¼���ݿ��ļ���ǰ׺�ִ����������
#define UNIQUENUMBERMESSAGE   17  // ׷�ӵ��������ݿ��ļ���ǰ׺�ִ����������
#define UNIQUENUMBERCONTACT   18  // ׷�ӵ���ϵ�����ݿ��ļ���ǰ׺�ִ����������
#define UNIQUENUMBERNOTES   19    // ׷�ӵ�����¼���ݿ��ļ���ǰ׺�ִ����������
#define UNIQUENUMBERCALENDER   20 // ׷�ӵ��������ݿ��ļ���ǰ׺�ִ����������

#ifdef WIN32
    #define BACKUPFOLDER                    L"D:\\�ҵ��ĵ�\\91 Mobile\\iPhone\\Backup"             // ����Ŀ¼
#else
    #define BACKUPFOLDER                    L"/tmp/91Mobile/iPhone/Backup"                          // ����Ŀ¼
#endif

#define CALLDATADBFILENAME              L"call_history.db"                                      // ͨ����¼���ݿ��ļ���
#define MESSAGEDBFILENAME               L"sms.db"                                               // �������ݿ��ļ���
#define CONTACTDBFILENAME               L"AddressBook.sqlitedb"                                 // ���������ݿ��ļ���
#define CONTACTIMAGEDBFILENAME          L"AddressBookImages.sqlitedb"                           // ������ͼƬ���ݿ��ļ���
#define CALENDERDBFILENAME              L"Calendar.sqlitedb"                                    // �������ݿ��ļ���
#define PRODUCTVERSIONH                 400                                                     // IOS�豸�߰汾��
#define PRODUCTVERSIONL                 112                                                     // IOS�豸�Ͱ汾��
#define MOBILEUSERID                    L"501"                                                   // mobile�û�ID
#define WIRLESSUSERID                   L"25"                                                    // wireless�û�ID
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

// �������ݿ��ļ�
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
    return idstr;// ���� (1,2,3)
}

wstring IosDBBase::GetTempDBFilePath()
{
    // �ļ�����ڱ��ص���ʱĿ¼
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

// ��ȡ���������ݿ���ʱ�ļ�
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
        //Ĭ�����кţ��������guid������40λ������0����
        strSerial = CFileOperation::GetGuidW();
    return strSerial;
}

// ͬ����IOS�豸
bool IosDBBase::SyncToIos(const DeviceData* pDevice)
{
    wstring sourceOnIos = _iosDBPath.GetDBPath();                                          // �ļ������IOS�豸��·��
    wstring destinationOnComputer = GetTempDBFilePath();                                   // �ļ�����ڱ��ص�·��

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

// �������ݿ�
bool IosDBBase::Reload(const DeviceData* pDevice)
{
    bool result = false;
    wstring sourceOnIos = _iosDBPath.GetDBPath();                                       // �ļ������IOS�豸��·��
    wstring destinationOnComputer = GetTempDBFilePath();                                // �ļ�����ڱ��ص�·��

    IIosFileSystem* pFileStream = NULL;
    if (pDevice->GetDevInfo()->_connectType == WIFI)
        pFileStream = new IosWiFiFileSystem(pDevice->GetDevInfo()->_ios_wifi_handle);
    else if (pDevice->GetDevInfo()->_connectType == USB)
        pFileStream = new AFCFileSysemBase(pDevice->GetDevInfo());

    if (!pFileStream)
        return false;

    // �ɹ���IOS�豸���������ݿ��ļ�
    if (pFileStream->Ios_DownFromPhone(sourceOnIos, destinationOnComputer) == Transfer_Success)
    {
        // �������ݿ��ļ�������Ŀ¼
        CopyDBFileToBackupFolder();
        result = true;
    }
    else if (!pFileStream->Ios_Exists(sourceOnIos))// ���IOS�豸�ϲ��������ݿ��ļ������ڱ��ش������ϴ�
    {
        // �ڱ��ش������ݿ��ļ�
        if (!CreateDBFile(destinationOnComputer))
        {
            SAFE_DELETE(pFileStream);
            return result;
        }

        wstring dbFolder = _iosDBPath.GetDBFolder();// ���ݿ��ļ�����IOS�豸�ϵ�Ŀ¼
        // ���Ŀ¼�������򴴽�
        if (!pFileStream->Ios_Exists(dbFolder))
            // ����Ŀ¼������Ŀ¼������
            if (pFileStream->Ios_CreateDirectory(dbFolder))
                // �������ݿ�Ŀ¼������
                SetDBFileOwn(pDevice,dbFolder);  

        // �����ݿ��ļ��ϴ���IOS�豸
        if (pFileStream->Ios_CopyToPhone(destinationOnComputer, sourceOnIos) == Transfer_Success)
            // �����ļ�������
            SetDBFileOwn(pDevice,sourceOnIos);
    }
	_dbPath = destinationOnComputer;
    SAFE_DELETE(pFileStream);
    return result;
}

// �������ݿ��ļ�������Ŀ¼
void IosDBBase::CopyDBFileToBackupFolder()
{
    wstring backupFolder = BACKUPFOLDER;                                        // ����Ŀ¼
    wstring backupPath = backupFolder + L"\\";                                  // �ļ�����·��

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

    wstring destinationOnComputer = GetTempDBFilePath();                        // �ļ�����ڱ��ص�·��
    CFileOperation::CopyFile(destinationOnComputer, backupPath, false );
}

// �����ļ�������
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

// ִ��SELECT��䲢����rowid
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