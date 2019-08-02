#pragma once


#include "Module/BaseData.h"
#include "Monitor/ProgressSubject.h"
#include "Common/BinaryBufferWriter.h"
#include "Common/BinaryBufferReader.h"
#include "Core/DeviceInfo.h"
#include "Device/BaseIO.h"
#include "Device/IosPublicFunc.h"

namespace ND91Assistant
{
	class DeviceData;
	class BaseIO;

    // 命令基类
    // 保存业务相关数据，生成命令，执行命令后解析返回结果
    class BaseCommand : public ProgressSubject
    {
    public:
        // 用设备信息和IO实例初始化
        BaseCommand(const DeviceData* pDevice);
        virtual ~BaseCommand();

        // 取得设备信息
        const DeviceInfo* GetDeviceInfo();

        // 执行命令
        virtual void Execute() = 0;

        // 文件已存在，询问用户如何处理
        EXIST_ACTION AskUser(std::wstring destName, bool isDir = false);
        void SetRenameCopyAllAction() { _lastAction = RENAME_COPY_ALL; }

        CommandExecuteResult Result() { return _result; };
		bool Success() { return _success; };
		bool ExecuteSuccess() { return _executesuccess; };
    protected:

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
		virtual std::string GetAndroidBodyData() = 0;

        // 解析返回数据包
        virtual void SetAndroidReturnData(std::string data) = 0;

        // 命令传送到IOS设备上的文件名（全路径, unicode）
		// 默认放在IOS_COMMOND_NORMAL_FOLDER路径下，并对原命令文件名用CreatDaemonCypher进行加密处理
        virtual std::wstring GetIosCommandFilePath();

        // 从IOS上获取的命令结果文件名（全路径, unicode）
		// 默认在IOS_COMMOND_NORMAL_FOLDER路径下，文件名为原命令文件名+".done"
        virtual std::wstring GetIosResultFilePath();

        // 生成ios文件内容。默认为空
        virtual std::wstring GetIosCommandFileContent();

        // 解析ios设备的返回文件（全路径, unicode）并删除文件
        virtual void SetIosReturnFile(std::wstring filename) = 0;


#pragma endregion

		// 生成Android数据包，调用者无须释放此指针，包
        std::string GetAndroidDatagram();

		// 解析Android返回的数据包头 -- 前20个字节 (标准)
		bool parseAndroidPackageHead(BinaryBufferReader* pReader);

		// 解析Android返回的数据包头 -- 前16个字节
		bool parseAndroidPackageHead_Top16(BinaryBufferReader* pReader);

		// 解析Android返回的数据包头 -- 前12个字节
		bool parseAndroidPackageHead_Top12(BinaryBufferReader* pReader);


        void DefaultExecute();

		// 取IO类指针（并非都需要IO类，需要时再创建）
		BaseIO* GetIO();
        
        
    protected:
        CommandExecuteResult _result;
		bool _success;          // 执行结果是否成功
		bool _executesuccess;   // 执行是否完成

        /*const*/ DeviceData*	_pDevice;   // 设备指针

		std::wstring _strIosCmdFileName; //IOS命令文件名，如_strIosCmdFileName = L"kill." + CFileOperation::GetGuidW(); 在子类的构造函数中赋值

#pragma region 安卓命令格式相关的变量

        // Android发送的包头: 须在每个子类的构造函数初始化
        // 只能使用AndroidDefine.h中定义的三种包头之一
		const char*	_pHead;

        // Android发送的标志：须在每个子类的构造函数初始化
        // 只能使用enum ANDROID_COMMAND_FLAG的值进行组合
		short	_nFlag;

        int     _nAndroidChannelID;         //安卓业务编号
        int     _nAndroidCommandID;         //安卓命令编号

		char	_pReturnHead[4];			// Android返回的包头
		short	_nReturnFlag;				// Android返回的标志
		short	_nReturnLongConnectSign;	// Android返回的长连接标志
		int		_nReturnChannelID;			// Android返回的业务编号
		int		_nReturnCommonType;			// Android返回的业务类型
		int		_nReturnBodySize;			// Android返回的数据体大小

#pragma endregion

	private:
		BaseIO*	_pBaseIO;		// IO类，子类须通过GetIO()获取

        EXIST_ACTION    _lastAction;    // 文件已存在时，用户上次选择的动作

	protected:
		std::wstring GetIosDoneInfoFromFileName();

	private:
		bool LookForResultFile( IIosFileSystem* pFileStream, std::wstring filePrefix, wstring& strInfo);

    };
}
