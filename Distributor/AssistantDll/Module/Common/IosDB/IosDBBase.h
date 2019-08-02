#pragma once

/*********************************************************************
数据库操作基类
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

    bool CreateDBFile(wstring dbFilePath);                          // 创建数据库文件
    bool CreateFile(wstring dbFilePath);                            // 创建文件
    bool CreateDB(string strSql);                                   // 创建数据库

    // Summary  : 同步到IOS设备
    // Returns  : bool 是否成功
    // Parameter: const DeviceData * pDevice 设备信息
    bool SyncToIos(const DeviceData* pDevice);

    // Summary  : 加载数据库
    // Returns  : bool 是否成功
    // Parameter: const DeviceData * pDevice 设备信息
    bool Reload(const DeviceData* pDevice);

    // Summary  : 获取ID字符串(类似(1,2,3,4))
    // Returns  : string ID字符串
    // Parameter: vector<int> rowidVec ID容器
    string GetRowidStr(vector<int> rowidVec);

    // Summary  : 获取IOS设备的产品序列
    // Returns  : std::wstring 产品序列
    wstring GetSerial();

    // Summary  : 执行SELECT语句并返回rowid
    // Returns  : int rowid
    // Parameter: const char * sql select语句
    int ExecuteAndGetId(const char* sql);


    // Summary  : 获取临时数据库文件在本地的路径
    // Returns  : std::wstring 文件路径
    wstring GetTempDBFilePath();

private:
    // Summary  : 获取联络人临时数据库文件在本地的路径
    // Returns  : std::wstring 文件路径
    wstring GetContactTempDBFilePath();

    // Summary  : 复制数据库文件到备份目录
    void CopyDBFileToBackupFolder();

    // Summary  : 设置文件所有者
    // Returns  : void
    // Parameter: const DeviceData * pDevice 设备信息
    // Parameter: wstring path 文件路径
    void SetDBFileOwn(const DeviceData* pDevice, wstring path);
public:
    wstring _dbPath;                     // 数据库路径
    IosDBPath _iosDBPath;                // 数据库路径
};

}
