#pragma once
#include <vector>
#include <map>
using namespace std;

namespace ND91Assistant
{
    enum IosVersion              // 设备版本    
    {
        VERSION_OTHER,
        VERSION_1,
        VERSION_2,
        VERSION_3,
        VERSION_4,
        VERSION_43,
    };

    class IosContactDB;
    class IosAutoSort
    {
    public:
        IosAutoSort(IosContactDB *contactDB);
        ~IosAutoSort();

    public:
        // Summary  : 获取拼音库版本
        // Returns  : std::wstring 版本号
        wstring GetPinyinVersion();

        // Summary  : 安装一键排序
        // Returns  : bool true:成功 false:失败
        // Parameter: wstring pyVersion 拼音版本
        bool Patch_Install(wstring pyVersion);

        // Summary  : 卸载一键排序
        // Returns  : bool true:成功 false:失败
        bool Patch_UnInstall();

    private:

        // 获取IOS设备版本
        IosVersion GetIosVersion();

        // Summary  : 安装一键排序(1X版本)
        // Returns  : bool true:成功 false:失败
        // Parameter: wstring pyVersion 拼音版本
        bool SortPathInstall_For_1X(wstring pyVersion);

        // Summary  : 安装一键排序(2X,3X版本)
        // Returns  : bool true:成功 false:失败
        // Parameter: wstring pyVersion 拼音版本
        // Parameter: bool is2Xor3X true:2x false:3x
        bool SortPathInstall_For_2X_3X(wstring pyVersion,bool is2Xor3X);

        // Summary  : 安装一键排序(4X,43X版本)
        // Returns  : bool true:成功 false:失败
        // Parameter: bool is4Xor43X true:4X false:43X
        bool SortPatchInstall_For_4X_43X(bool is4Xor43X);

        // Summary  : 卸载一键排序(1X版本)
        // Returns  : bool true:成功 false:失败
        bool SortPatchUnInstall_For_1X();

        // Summary  : 卸载一键排序(2X,3X,4X版本)
        // Returns  : bool true:成功 false:失败
        bool SortPatchUnInstall_For_2X_3X_4X();

        // Summary  : 安装码表(1X版本)
        // Returns  : void 
        // Parameter: wstring filePath 码表文件路径
        void InstallCodeTable_1X(wstring filePath);

        // Summary  : 安装码表(2X,3X,4X版本)
        // Returns  : void
        // Parameter: wstring filePath 码表文件路径
        // Parameter: map<char, int> iPhone30SortDic 排序键对照字典
        // Parameter: bool is2Xor3X true:2x false:3x true:2x false:3x
        void InstallCodeTable_2X_3X_4X(wstring filePath,map<char, int> iPhone30SortDic,bool is2Xor3X);

        // Summary  : 获取拼音表的phoneticize值
        // Returns  : string 
        // Parameter: string pystr py.dat文件中的拼音字符串
        // Parameter: map<char, int> iPhone30SortDic  排序键对照字典
        // Parameter: int * languageIndex 拼音表的languageIndex值
        // Parameter: bool is2Xor3X true:2x false:3x
        string MakeSortSection(string pystr, map<char, int> iPhone30SortDic, int *languageIndex, bool is2Xor3X);

        // Summary  : 获取拼音表的phoneticize值(2X版本)
        // Returns  : string 
        // Parameter: string pystr py.dat文件中的拼音字符串
        // Parameter: map<char, int> iPhone30SortDic 排序键对照字典
        // Parameter: int * languageIndex 拼音表的languageIndex值
        string MakeSortSection_For_2X(string pystr, map<char, int> iPhone30SortDic, int *languageIndex);

        // Summary  : 获取拼音表的phoneticize值(3X版本)
        // Returns  : string
        // Parameter: string pystr py.dat文件中的拼音字符串
        // Parameter: map<char, int> iPhone30SortDic 排序键对照字典
        // Parameter: int * languageIndex 拼音表的languageIndex值
        string MakeSortSection_For_3X(string pystr, map<char, int> iPhone30SortDic, int *languageIndex);

        // Summary  : 检查是否存在数据
        // Returns  : bool true:存在 false: 不存在
        // Parameter: const char * sql 查找SQL语句
        bool CheckHasData(const char* sql);

        // Summary  : 获取拼音版本
        // Returns  : std::wstring 拼音版本
        // Parameter: const char * sql SQL语句
        wstring SelectPinyinVersion(const char* sql);

        // Summary  : 检查是否安装过一键排序补丁
        // Returns  : bool true:已安装 false:未安装
        bool IsPatched();

        // Summary  : 将文件内容格式转为指定的格式如 "啊", "A " -> "'啊','A '" 
        // Returns  : std::wstring  转换前的字符串
        // Parameter: wstring fileContext 转换后的字符串
        wstring PaddingPinyin(wstring fileContext);

    private:
        IosContactDB *      _contactDB;             // 联络人数据库
        wstring             _currWorkingDir;        // 当前工作目录
    };
}