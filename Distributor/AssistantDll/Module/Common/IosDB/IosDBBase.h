#pragma once

/*********************************************************************
���ݿ��������
*********************************************************************/
#include "include/sqlite/CppSqlite3.h"
#include "IosDBPath.h"
#include "Core/DeviceData.h"
#include <vector>
using namespace std;

namespace ND91Assistant
{
class IosDBBase : public CppSQLite3DB
{
public:
    IosDBBase(IosDBPath iosDBPath);
    ~IosDBBase(void);

public:
    void SetDBPath(wstring path);

    bool OpenDB();
    void CloseDB();
    bool ExecSQL(const char *sql);

    bool CreateDBFile(wstring dbFilePath);                          // �������ݿ��ļ�
    bool CreateFile(wstring dbFilePath);                            // �����ļ�
    bool CreateDB(string strSql);                                   // �������ݿ�

    // Summary  : ͬ����IOS�豸
    // Returns  : bool �Ƿ�ɹ�
    // Parameter: const DeviceData * pDevice �豸��Ϣ
    bool SyncToIos(const DeviceData* pDevice);

    // Summary  : �������ݿ�
    // Returns  : bool �Ƿ�ɹ�
    // Parameter: const DeviceData * pDevice �豸��Ϣ
    bool Reload(const DeviceData* pDevice);

    // Summary  : ��ȡID�ַ���(����(1,2,3,4))
    // Returns  : string ID�ַ���
    // Parameter: vector<int> rowidVec ID����
    string GetRowidStr(vector<int> rowidVec);

    // Summary  : ��ȡIOS�豸�Ĳ�Ʒ����
    // Returns  : std::wstring ��Ʒ����
    wstring GetSerial();

    // Summary  : ִ��SELECT��䲢����rowid
    // Returns  : int rowid
    // Parameter: const char * sql select���
    int ExecuteAndGetId(const char* sql);


    // Summary  : ��ȡ��ʱ���ݿ��ļ��ڱ��ص�·��
    // Returns  : std::wstring �ļ�·��
    wstring GetTempDBFilePath();

private:
    // Summary  : ��ȡ��������ʱ���ݿ��ļ��ڱ��ص�·��
    // Returns  : std::wstring �ļ�·��
    wstring GetContactTempDBFilePath();

    // Summary  : �������ݿ��ļ�������Ŀ¼
    void CopyDBFileToBackupFolder();

    // Summary  : �����ļ�������
    // Returns  : void
    // Parameter: const DeviceData * pDevice �豸��Ϣ
    // Parameter: wstring path �ļ�·��
    void SetDBFileOwn(const DeviceData* pDevice, wstring path);
public:
    wstring _dbPath;                     // ���ݿ�·��
    IosDBPath _iosDBPath;                // ���ݿ�·��
};

}
