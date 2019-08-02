#pragma once

// 取得目录的公共方法
class SINGLETON_API DataPath
{
public:
    // 【用户数据目录】：我的文档\CompanyName\ProductName\ 
    static wstring GetDataDir();

    // 【临时数据目录】：系统临时目录\ProductName\ 
    static wstring GetTempDir();

    // 获得唯一的临时数据目录（调用者负责删除）
    static wstring GetUniqueTempDir();

private:
    static wstring _dataPath;   // 用户数据目录
    static wstring _tempPath;   // 临时数据目录
};