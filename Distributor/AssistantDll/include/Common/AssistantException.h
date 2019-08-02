#pragma once

#include "BaseException.h"

// 宏定义：用这个宏来简化AssistantException的构造函数
#define ASSISTANT_EXCETPION(x) AssistantException(x, __FILE__ , __LINE__)
//#ifdef _DEBUG
#define THROW_ASSISTANT_EXCEPTION(x) throw AssistantException(AssistantException::x, __FILE__ , __LINE__)
// #else
// #define THROW_ASSISTANT_EXCEPTION(x) AssistantException(AssistantException::x, __FILE__ , __LINE__)   // 只写Log
// #endif

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL AssistantException: public BaseException
    {
    public:
        // 助手业务错误代码
        enum ASSISTANT_ERROR_CODE
        {
            INVALID_LOG_FILE_NAME,          // 无效的LOG文件名
            WSA_STARTUP_FAIL,               // WSAStartup()调用返回错误

            PARSE_ANDROID_DATAGRAM_FAIL,    // Android守护返回数据包格式解析错误
            UNKNOWN_DEVICE_OR_CONNECTION,   // 未实现的设备类型或连接类型
            ADB_SOCKET_ERROR,               // 网络连接错误
            INVALID_DEVICE_SERIAL_NUMBER,   // 无效的设备标识字符串
            ADB_COMMAND_RETURN_ERROR,       // adb命令返回内容格式错误
            ADB_EXE_NOT_FOUND,              // 找不到ADB.EXE程序文件

			ADB_START_SERVER_ERROR,			//启动ADB服务错误
			ADB_START_DEMON_ERROR,			//启动守护错误
			ADB_FORWORD_ERROR,				//映射端口错误
			ADB_WAKE_DEMON_ERROE,			//唤醒守护错误

            FILE_NOT_FOUND,                 // 文件操作：文件未找到
            FILE_IS_EMPTY,                  // 空文件
            OPEN_FILE_FAIL,                 // 打开文件失败
			INVALID_FILE_NAME,              // 无效的文件名
			REMOTE_FILE_ALREADY_EXIST,      // 目标文件已经存在
			REMOTE_PATH_NOT_EXIST,			// 手机端路径不存在
			REMOTE_PATH_CREATE_FAIL,        // 手机端路径创建失败
            DEVICE_NOT_ACTIVE,              // 设备未激活，不能使用

			IOS_WIFI_C0NN_ERROR,			//ios设备WIFI连接失败
			IOS_WIFI_PSW_ERROR,				//IOS设备wifi连接密码错误
			IOS_WIFI_TRANSFER_ERROR,		//IOS设备wifi传输失败

            INVALID_IO_OBJECT,              // 错误的IO对象指针。内部错误，请联系开发人员
            PROGRAM_ERROR,                  // 程序内部错误，请联系开发人员

            LS_RESULT_PARSE_FAIL,           // 安卓LS命令的结果无法识别格式，请将机型反馈开发人员

			IOS_ITUNES_APP_OPERATION_LOCK, //通过ITUNES操作APP已加锁

            STRING_NOT_SET,                 // 内部错误：未设置需要显示的字符串内容，详见International类
            DLL_RETURN_ERROR,               // DLL函数返回错误代码

			PRODUCT_VERSION_NOT_AGREE,			        // 固件版本不符合要求
            NotValideToAuthorizeByDeviceVersion,        // 设备是不适合正版授权(暂仅支持IOS4.0及以上版本)
            NotValideToAuthorizeByNotUSB,               // 连接方式不是USB
            NotValideToAuthorizeByItunesVersionTooLow,  // iTunes版本过低
            NotValideToAuthorizeByNotFoundItunes,       // 未安装iTunes
            NotValideToAuthorizeByItunesIsSimple,       // 简易版本iTunes
			IOS_UNINSTALL_APPSYNC,				// 未安装AppSync
            PRODUCT_PROHIBIT_APPINSTATLL,       // 设备禁止安装应用
            INSTALAPPFILELOST,                  // 应用程序包不完整
            INSTALAPPUNZIPERROR,                // 应用程序包无法解压
            DeviceUnSupported,                  // 此应用是ipad适用，不适合当前设备iPhone/iPod
            NoJailBreakedUnSupported,           // 非越狱设备暂不提供安装应用功能
            NoJailBreakedNoLegalSoft,           // 非越狱设备不支持安装非正版应用
            AuthorizorFaile,                    // 授权失败

			ARCHIVE_APP_ARCHIVE_ON_DEVICE_ERROR, //备份程序时，备份到手机上失败
			ARCHIVE_APP_COPY_TO_PC_ERROR, //备份程序时，拷贝到PC上失败

			IOS_RESTORE_NOTIFICATIONS_ERROR,  //Register RestoreNotifications Failed
			IOS_REGISTER_SUBSCRIBE_ERROR, //Register Subscribe Failed
			IOS_REGISTER_SUBSCRIBE_EXCEPTION_ERROR, //Register Subscribe Failed With Exception

			DRIVER_INSTALL_ERROR,			// 驱动安装错误

            PHOTO_DATABASE_FORMAT_ERROR,    // 解析photo database文件时遇到格式错误

			NOT_SUPPORT, //不支持

			CDB_FILE_FORMAT_ERROR,    // 解析CDB文件时遇到格式错误

			ADB_COMMAND_NO_NDDEVICE,       // 旧的adb没有nddevice命令
        };

        AssistantException(ASSISTANT_ERROR_CODE code, string filename, int lineNo) :
          BaseException((int)code, filename, lineNo)
          {

          }

    };
}