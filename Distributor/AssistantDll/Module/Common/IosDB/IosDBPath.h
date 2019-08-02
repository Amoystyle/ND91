#pragma once
/*********************************************************************
根据IOS设备的版本不同获取数据库在IOS设备的路径
*********************************************************************/
#include <string>
#include "Core/DeviceData.h"
using namespace std;

namespace ND91Assistant
{
    typedef enum ISDBTYPE
    {
        ISCALLDATADB,       // 是通话记录数据库
        ISMESSAGEDB,        // 是短信数据库
        ISCONTACTDB,        // 是联系人数据库
        ISCONTACTIMAGEDB,   // 是联络人图片数据库
		ISNOTESDB,			// 是备忘录数据库
        ISCALENDERDB,       // 是日历数据库
		ISBOOKMARKDB        // 书签
    };

    class IosDBPath
    {
    public:
        IosDBPath(const DeviceData* pDevice, ISDBTYPE type);
        ~IosDBPath(void);
 
        wstring GetTempFolder();

        // 获取设备版本号
        int GetProductVersion();

        // 设备是否越狱
        bool GetJailBreaked();

        // 获取设备序列号
        wstring GetDeviceSerial(); 

        // 获取系统 类似iPad1,1
        wstring GetDeviceProductType();

        // 获取数据库类型
        ISDBTYPE GetDBType();
        const DeviceData* GetDeviceData();

        // 获取数据库在IOS设备上的路径
        wstring GetDBPath();

        // 获取数据库在IOS设备上的目录
        wstring GetDBFolder();

    private:
    #pragma region 通话记录数据库

        // 获取通话记录数据库在IOS设备上的路径
        wstring GetCallDataDBPath();

        // 获取通话记录数据库在IOS设备上的目录
        wstring GetCallDataDBFolder();

    #pragma endregion

    #pragma region 短信数据库

        // 获取短信数据库在IOS设备上的路径
        wstring GetMessageDBPath();

        // 获取短信数据库在IOS设备上的目录
        wstring GetMessageDBFolder();

    #pragma endregion

    #pragma region 联络人据库

        // 获取联络人数据库在IOS设备上的路径
        wstring GetContactDBPath();

        // 获取联络人数据库在IOS设备上的目录
        wstring GetContactDBFolder();

		// 获取备忘录数据库在IOS设备上的目录
		wstring GetNotesDBFolder();

		// 获取备忘录数据库在IOS设备上的路径
		wstring GetNotesDBPath();

    #pragma endregion

    #pragma region 联络人图片数据库

        // 获取联络人图片数据库在IOS设备上的路径
        wstring GetContactImageDBPath();

        // 获取联络人图片数据库在IOS设备上的目录
        wstring GetContactImageDBFolder();

    #pragma endregion

#pragma region 日历数据库

        // 获取日历数据库在IOS设备上的路径
        wstring GetCalenderDBPath();

        // 获取日历数据库在IOS设备上的目录
        wstring GetCalenderDBFolder();

#pragma endregion

		wstring GetBookMarkDBPath();

    private:
        int _iProductVersion;           // 版本号
        bool _bJailBreaked;             // 是否越狱 
        wstring _deviceSerial;          // 设备序列号
        wstring _deviceProductType;     // 系统 类似iPad1,1

        ISDBTYPE _type;                   // 数据库类型
        const DeviceData* _pDevice;      
    };
}
