#ifndef iTunesAuthorize_h__
#define iTunesAuthorize_h__
#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为1字节对
#include "Cer_export.h"
#pragma pack(pop)//恢复对齐状态
#define HELPER_ENCRYPT_BYTES_LEN			104 //必须为8的倍数
#define HELPER_SECURITY_LENGTH				(sizeof(DES_cblock) + (HELPER_ENCRYPT_BYTES_LEN * 2))

namespace ND91Assistant
{
    class DeviceInfo;
    class iTunesAuthorize
    {
    public:
	    iTunesAuthorize();

	    //1 服务器生成xxx.serInfo
	    //2	服务器把xxx.serInfo文件里的12行内容或者整个文件发送给客户端
	    //3 服务器把C:\ProgramData\Apple Computer\iTunes\SC Info\目录下的3个文件发送给客户端
	    //4 客户端从服务器取得3个授权文件放到用户授权目录下,如C:\ProgramData\Apple Computer\iTunes\SC Info\
	    //5	客户端从服务器取得xxx.serInfo
	    //	获取前6行做为us.m_serverNumber1,后边有值的6行作为us.m_serverNumber2

	    //调用itunes授权接口，
	    //参数：iPodDev连接的设备
	    //authorizationFromServer 是否从服务器取校验信息
	    //icInfoPath 授权的文件路径
	    //serverNumbers xxx.serInfo的文件内容每行一个item一共12个（既m_serverNumber1+m_serverNumber2）
	    //ids 91服务器已授权的iTunes账户下所有ipa软件的ApplicationDSID
        //int count ids个数
	    //szuuid 设备ID
        //iOSVersion 固件版本
        //afcConn afc1连接句柄
	    //返回-1错误处理: 1.确定iTunes版本是否支持 2.本地itunes授权 
	    //3.选对设备类型,固件要iOS5及以上版本,iOS4不支持 4.设备上已安装一个授权账号从appstore下载的软件 5.重启设备再授权
	    int Authorize(/*struct am_device *iPodDev,*/ bool authorizationFromServer, string icInfoPath, 
            vector<ND_UInt32> serverNumbers, ND_UInt64* ids, int count, /*char* szuuid, ND_UInt32 iOSVersion, ND_UInt32 afcConn*/const DeviceInfo* pDevInfoPtr);

		//itunes是否支持授权
		static long IsAssistance();

    private:
	    int setSecurityBytes(unsigned char *data); //返回后data在10秒内使用有效
		static int set_env();
    };
}
#endif // iTunesAuthorize_h__