#pragma once
#include "../cul.h"
#include <map>
#include <string>

// 为方便使用提供OPTION宏定义
#define OPTION GlobalOption::GetInstance()

// 选项存取类，单例，支持所有数据类型
// 注意：对指针类型，只能保存指针本身
class CUL_CLASS_DECL GlobalOption
{
public:

    // 选项类型：每个选项都必须在这里添加一个值
    // 为避免数据类型错误，每个类型名称以数据类型开头，如INT_
    enum OPTION_TYPE {
        STR_LOG_FILE_PATH,              // log文件路径			
        INT_MAX_LOG_FILE_SIZE,          // log文件的最大值
        //IOS
        INT_IOS_CHECK_DEAMON_RUNNING,   // IOS监测守护时候运行的时间间隔
        STR_IOS_WIFI_CONNECT_PASSWORD,  // WIFI连接的默认密码
        STR_IOS_PHOTO_DATABASE_PATH,	// IOS设备图库图片初始化photo database文件路径
        //Driver
        STR_DRIVER_INI_PATH,			// 驱动信息配置文件路径
        //Android
        STR_ANDROID_DEAMON_PCPATH,      // Android守护的PC路径
        INT_DEAMON_SILENCE_LIMIT,       // 守护心跳包间隔时长(ms)，超过则认为设备断开
        INT_DEAMON_PORT,                // 守护使用的端口
        STR_DEAMON_NAME,                // 守护（PC连接模块）在手机中的package名称
        STR_DEAMON_PATH,                // 守护（PC连接模块）的文件位置 （完整路径）
        STR_PANDASPACE_NAME,            // 守护（界面-原熊猫空间）在手机中的package名称
        STR_PANDASPACE_PATH,            // 守护（界面-原熊猫空间）的文件位置 （完整路径） 
        STR_ADB_FILE_APTH,              // ADB.EXE文件位置（完整路径）
        STR_DLLPATH,					// Assistant.dll ndiPhoneNet.dll等路径
        STR_ANDROID_NPK_TEMP_PATH,      // NPK安装过程缓存目录
        INT_ANDROID_IMPORT_CONTACT_COUNT, // 安卓单条导入联系人命令允许的数量上限
        INT_ANDROID_IMPORT_CALLDATA_COUNT,// 安卓单条导入通话记录命令允许的数量上限
        INT_ANDROID_IMPORT_MESSAGE_COUNT, // 安卓单条导入短消息命令允许的数量上限
        INT_ANDROID_DEAMON_MODE,		// 守护模式 0表示不开启守护模式 1表示开启守护模式
        INT_IOS_DEAMON_MODE,		// 守护模式 0表示不开启守护模式 1表示开启守护模式
        INT_FILETASK_COUNTMODE,    // 计算文件夹上传/下载的任务数模式 0表示不计算文件夹 1表示计算文件夹
        STR_PCNAME,                // 计算机名字
        STR_IOS_CONNECT_SHOWTEXT,  // Ios连接确认文字
        INT_IOS_CONNECT_TIMEOUT,   // Ios连接确认超时时间
        STR_91HELPERDEAMON_LOG,     //NoDeamon守护地址
        INT_LOG_SWITCH,				//1关0开
    };

    static GlobalOption* GetInstance();
    static void Release();

    // string 类型
    void Set(OPTION_TYPE key, const char* value);
    void Set(OPTION_TYPE key, std::string value);
    bool Get(OPTION_TYPE key, std::string& value) const;

    // wstring 类型
    void Set(OPTION_TYPE key, const wchar_t* value);
    void Set(OPTION_TYPE key, std::wstring value);
    bool Get(OPTION_TYPE key, std::wstring& value) const;

    // 设置选项
    template<class T>
    void Set(OPTION_TYPE key, T value)
    {
        int size = sizeof(T);

        char* pBuffer = new char[size];
        //memcpy_s(pBuffer, size, &value, size);
        memcpy(pBuffer, &value, size);

        _datas[key] = std::string(pBuffer, size);
        delete[] pBuffer;
    }

    // 读取选项
    // 若该选项不存在（未设置），则返回false
    template<class T>
    bool Get(OPTION_TYPE key, T& value) const
    {
        std::string s;

        if ( ! Get(key, s) )
            return false;

        //int size = sizeof(T);
        //memcpy_s(&value, size, s.c_str(), s.length());
        memcpy(&value, s.c_str(), s.length());

        return true;
    }

private:
    GlobalOption();
    virtual ~GlobalOption();

    // 存放单例变量
    static GlobalOption* _pInstance;

#pragma warning(disable : 4251)

    // 存储每个选项及其对应的值
    std::map<OPTION_TYPE, std::wstring> _wstrings;  // 存储wstring类型数据
    std::map<OPTION_TYPE, std::string> _datas;      // 存储其他类型数据，包括string类型
};
