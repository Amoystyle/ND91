#pragma once

#include "IBaseCommand.h"
namespace ND91Assistant
{
    class AndroidBaseCommand : public IBaseCommand
    {
    public:
        AndroidBaseCommand(const DeviceData* pDevice);
        ~AndroidBaseCommand();
        
        virtual void Execute();
    
        // 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
        virtual std::string GetAndroidBodyData() = 0;

        // 解析返回数据包
        virtual void SetAndroidReturnData(std::string data) = 0;


    protected:
        // 生成Android数据包，调用者无须释放此指针，包
        std::string GetAndroidDatagram();

        // 解析Android返回的数据包头 -- 前20个字节 (标准)
        bool parseAndroidPackageHead(BinaryBufferReader* pReader);

        // 解析Android返回的数据包头 -- 前16个字节
        bool parseAndroidPackageHead_Top16(BinaryBufferReader* pReader);

        // 解析Android返回的数据包头 -- 前12个字节
        bool parseAndroidPackageHead_Top12(BinaryBufferReader* pReader);

    protected:
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

    };
}