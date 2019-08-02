#include "stdafx.h"

#include "AndroidUsbConnection.h"

#include "AndroidAdbHelper.h"
#include "AdbStartDeamonCommand.h"
#include "AndroidShowDeamonUICommand.h"
#include "AdbForwardTcpCommand.h"
#include "AdbGetPropertyCommand.h"
#include "AdbKillProgressCommand.h"

#include "Common/BaseSocket.h"
#include "ConnectionPool.h"
#include "BaseIO.h"
#include "Common/version.h"

#include "Module/Application/InstallCommand.h"
#include "Module/AndroidDefine.h"
#include "Monitor/MonitorThread.h"
#include "Module/System/GetMacAddressCommand.h"
#include "Module/System/GetIMEICommand.h"
#include "Module/System/CheckDeamonCommand.h"
#include "Module/System/SystemManager.h"

AndroidUsbConnection::AndroidUsbConnection(DeviceData* pDeviceData)
:_pDeviceData(pDeviceData)
{
	_hThread=NULL;
}

AndroidUsbConnection::~AndroidUsbConnection()
{

}

bool ND91Assistant::AndroidUsbConnection::PreActivate()
{
    //启动ADB服务
    AndroidAdbHelper aah;

    //启动ADB服务
    aah.startServer();

    if (_pDeviceData->_nAndroidDeamonMode)
	{
		//启动守护
		if (!StartDeamon())
		{
			//安装守护
            try
            {
				InstallDeamon();
			}
            catch(...)
            {
            }
			//安装完立即启动不成功
			Sleep(200);
			int retrycount=0;
			while (!StartDeamon())
			{
				if(retrycount==2)//启动10次还是失败
					return false;
				retrycount++;
				Sleep(200);					
			}
		}

	}

    //设置端口映射
    if (_pDeviceData->_DeviceInfo._nPort == 0)
        _pDeviceData->_DeviceInfo._nPort = CBaseSocket::getActiveSocketPort();

    AdbForwardTcpCommand forwardCommand(_pDeviceData->_DeviceInfo._strSerialNumber,_pDeviceData->_DeviceInfo._ios_deviceIdentifier, _pDeviceData->_DeviceInfo._nPort, 5900);
    forwardCommand.Execute();

    return true;
}

bool AndroidUsbConnection::Activate()
{
	bool ret=true;
	LOG->WriteDebug(L"Begin AndroidUsbConnection Activate");
    //初始化连接池 - 至少要启用1个连接并执行一个命令才能接收到Android心跳包返回的数据 (不确定)	
    if (_pDeviceData->_nAndroidDeamonMode && !_pDeviceData->_pConnectionPool)
    {
		//_pConnectionPool内存说明：在这里开辟，在DeviceData->ReleaseAll()中销毁
        _pDeviceData->_pConnectionPool = new ConnectionPool(&(_pDeviceData->_DeviceInfo));
        _pDeviceData->_pConnectionPool->init();
    }

	LOG->WriteDebug(L"Begin AndroidUsbConnection _pConnectionPool init");
    //IMEI有获取失败的情况，在这里通过守护再次获取
   /* if (_pDeviceData->_nAndroidDeamonMode && _pDeviceData->_DeviceInfo._deviceIMEI.length() == 0)
    {
        BaseIO *pIO = _pDeviceData->CreateIO();
        GetIMEICommand cmd(_pDeviceData);
        cmd.Execute();
        _pDeviceData->_DeviceInfo._deviceIMEI = cmd.GetIMEI();
		SAFE_DELETE(pIO);

		LOG->WriteDebug(L"Begin AndroidUsbConnection GetIMEI ok");
    }*/

    //检查守护版本
	if (_pDeviceData->_nAndroidDeamonMode)
    {
        BaseIO *pIO = _pDeviceData->CreateIO();
        CheckDeamonCommand cmd(_pDeviceData);
        cmd.Execute();
        CVersion ver(cmd.getVersion());

		LOG->WriteDebug(L"Begin AndroidUsbConnection CheckDeamonCommand ok");
        if ( ver.NeedUpdateAndroidDeamon() )    // 需要升级守护 ？
        {
			try
			{
               ret = InstallDeamon();

				LOG->WriteDebug(L"Begin AndroidUsbConnection InstallDeamon ok");

				////启动守护
				//CVersion romver(CCodeOperation::UnicodeToUTF_8(_pDeviceData->GetDevInfo()->_deviceProductVersion));
				//if(romver._nVer1>=4 && romver._nVer2>=0 )
				//{
				//	AndroidShowDeamonUICommand showDeamonUIcommand(_pDeviceData->_DeviceInfo._strSerialNumber);
				//	showDeamonUIcommand.Execute();
				//}

			} catch(...){
				ret =false;
			}   
			ret = StartDeamon();

//             AdbStartDeamonCommand startDeamonCommand(_pDeviceData->_DeviceInfo._strSerialNumber);
//             startDeamonCommand.Execute();
//             if (!startDeamonCommand.Success())
//             {
//                 AdbStartDeamonCommand startDeamonCommand2(_pDeviceData->_DeviceInfo._strSerialNumber,2);
//                 startDeamonCommand2.Execute();
// 				 if (!startDeamonCommand2.Success())
// 					 ret = false;
//             }            
        }
		SAFE_DELETE(pIO);
    }

	//IMEI有获取失败的情况，在这里通过守护再次获取
	/*if (_pDeviceData->_nAndroidDeamonMode && _pDeviceData->_DeviceInfo._deviceIMEI.length() == 0)
	{
		BaseIO *pIO = _pDeviceData->CreateIO();
		GetIMEICommand cmd(_pDeviceData);
		cmd.Execute();
		_pDeviceData->_DeviceInfo._deviceIMEI = cmd.GetIMEI();
		SAFE_DELETE(pIO);
		LOG->WriteDebug(L"Begin AndroidUsbConnection GetIMEI ok!");
	}*/

	//移到initdevicedata里面供单独调用
	/*if (_pDeviceData->_DeviceInfo._deviceResolution.empty())
	{
		BaseIO *pIO = _pDeviceData->CreateIO();
		SystemManager sys(_pDeviceData);
		_pDeviceData->_DeviceInfo._bRoot			= sys.GetIsRoot();
		_pDeviceData->_DeviceInfo._deviceResolution	= sys.GetDeviceResolution();	
		SAFE_DELETE(pIO);
		LOG->WriteDebug(L"Begin AndroidUsbConnection GetDeviceResolution again!");
	}

    GetMacAddressCommand macAddressCmd(_pDeviceData);
    macAddressCmd.Execute();
    _pDeviceData->_DeviceInfo._deviceWiFiMac = macAddressCmd.GetMacAddress();

	LOG->WriteDebug(L"Begin AndroidUsbConnection GetMacAddressCommand ok!");*/

    return ret;
}

bool ND91Assistant::AndroidUsbConnection::Connect()
{
	LOG->WriteDebug(L"Begin AndroidUsbConnection Connect ..");
    //启动ADB服务 启动的时间稍长，是否可以考虑放到 DeviceManager 初始化中启动
    AndroidAdbHelper aah;
    bool ret = aah.startServer();
	LOG->WriteDebug(L"Begin AndroidAdbHelper Connect startServer ok.");
    //USB模式下，可直接获取设备属性
	//移到initdeviceinfo里面处理
    // GetAndroidProperty();
	//LOG->WriteDebug(L"Begin AndroidAdbHelper Connect GetAndroidProperty ok.");
    return ret;
}

void ND91Assistant::AndroidUsbConnection::Disconnect()
{
    //撤销连接池
    //SAFE_DELETE(_pDeviceData->_pConnectionPool); //关闭连接池
    if (_pDeviceData->_pConnectionPool)
        _pDeviceData->_pConnectionPool->ReleaseAll();

    //停止监控线程
    if (_pDeviceData->_pMonitorThread)
    {
        _pDeviceData->_pMonitorThread->StopThread(); 
        delete _pDeviceData->_pMonitorThread;
        _pDeviceData->_pMonitorThread = NULL;
    }
    //停止Adb
    //AndroidAdbHelper aah;
    //aah.stopServer();

}
void ND91Assistant::AndroidUsbConnection::InitDeviceData()
{	
	
//	 _hThread = CreateThread(NULL,0,InitDeviceOtherData,(LPVOID)_pDeviceData,0,NULL);
	  GetAndroidProperty();	

//	  WaitForSingleObject(_hThread,5000);
}

 DWORD WINAPI ND91Assistant::AndroidUsbConnection::InitDeviceOtherData(/*LPVOID lpParameter*/)
{
	//DeviceData* pdevdata = (DeviceData*)lpParameter;
	if (_pDeviceData->_nAndroidDeamonMode && _pDeviceData->_DeviceInfo._deviceIMEI.length() == 0)
	{
		BaseIO *pIO = _pDeviceData->CreateIO();
		GetIMEICommand cmd(_pDeviceData);
		cmd.Execute();
		_pDeviceData->_DeviceInfo._deviceIMEI = cmd.GetIMEI();
		SAFE_DELETE(pIO);
		LOG->WriteDebug(L"Begin AndroidUsbConnection GetIMEI ok!");
	}

	if (_pDeviceData->_DeviceInfo._deviceResolution.empty())
	{
		BaseIO *pIO = _pDeviceData->CreateIO();
		SystemManager sys(_pDeviceData);
		_pDeviceData->_DeviceInfo._bRoot			= sys.GetIsRoot();
		_pDeviceData->_DeviceInfo._deviceResolution	= sys.GetDeviceResolution();	
		SAFE_DELETE(pIO);
		LOG->WriteDebug(L"Begin AndroidUsbConnection GetDeviceResolution again!");
	}

	GetMacAddressCommand macAddressCmd(_pDeviceData);
	macAddressCmd.Execute();
	_pDeviceData->_DeviceInfo._deviceWiFiMac = macAddressCmd.GetMacAddress();

	LOG->WriteDebug(L"Begin AndroidUsbConnection GetMacAddressCommand ok!");
	return 0;
}

const DeviceInfo* ND91Assistant::AndroidUsbConnection::GetDeviceInfo()
{
    return &(_pDeviceData->_DeviceInfo);
}


void ND91Assistant::AndroidUsbConnection::GetAndroidProperty()
{
    AdbGetPropertyCommand cmd(_pDeviceData->_DeviceInfo._strSerialNumber);
    cmd.Execute();
    const DeviceInfo *pInfo = cmd.GetDeviceInfo(); 

    _pDeviceData->_DeviceInfo._deviceIMEI				= pInfo->_deviceIMEI;          
    _pDeviceData->_DeviceInfo._manufacturer				= pInfo->_manufacturer;       
    _pDeviceData->_DeviceInfo._deviceHWModel			= pInfo->_deviceHWModel;       
    _pDeviceData->_DeviceInfo._deviceProductVersion		= pInfo->_deviceProductVersion;
    _pDeviceData->_DeviceInfo._language					= pInfo->_language;     
	_pDeviceData->_DeviceInfo._platform					= pInfo->_platform;
	_pDeviceData->_DeviceInfo._deviceCoreVersion		= pInfo->_deviceCoreVersion;
	_pDeviceData->_DeviceInfo._deviceBaseVersion		= pInfo->_deviceBaseVersion;
	_pDeviceData->_DeviceInfo._deviceInsdeVersion		= pInfo->_deviceInsdeVersion;

}

bool ND91Assistant::AndroidUsbConnection::StartDeamon()
{
    bool bRec = false;
    int nRetry = 0;
LOG->WriteDebug(L"Begin StartDeamon ");

	  //杀掉守护再启动
	   StopDeamon();


    while (nRetry < 3)
    {
        AdbStartDeamonCommand startDeamonCommand(_pDeviceData->_DeviceInfo._strSerialNumber);
        startDeamonCommand.Execute();
		LOG->WriteDebug(L"Begin startDeamonCommand Execute ..");
        if (!startDeamonCommand.Success())
        {
				LOG->WriteDebug(L"Begin startDeamonCommand Execute fail");
            AdbStartDeamonCommand startDeamonCommand2(_pDeviceData->_DeviceInfo._strSerialNumber,2);
            startDeamonCommand2.Execute();
            if (startDeamonCommand2.Success())
            {
				LOG->WriteDebug(L"Begin startDeamonCommand2 Execute sucess");
                bRec = true;
                break;
            }
        }

        if (startDeamonCommand.Success())
        {
            bRec = true;
            break;
        }
       
      //  Sleep(500);			        
        nRetry ++;
    }

    return bRec;
}

bool ND91Assistant::AndroidUsbConnection::InstallDeamon()
{
#ifdef MODULE_APP
    bool bRec = true;
    AdbPSCommand psCmd(_pDeviceData->_DeviceInfo._strSerialNumber);
    psCmd.Execute();

    wstring strDeamonName;
    wstring strDeamonPath;
    wstring strPandaName;
    wstring strPandaPath;

    OPTION->Get(GlobalOption::STR_DEAMON_NAME, strDeamonName);
    OPTION->Get(GlobalOption::STR_DEAMON_PATH, strDeamonPath);
    OPTION->Get(GlobalOption::STR_PANDASPACE_NAME, strPandaName);
    OPTION->Get(GlobalOption::STR_PANDASPACE_PATH, strPandaPath);

//    if (!psCmd.isProcessExist(strDeamonName))
	{
		bRec = InstallCommand::SimpleInstallApp(_pDeviceData, strDeamonPath, strDeamonName, APP_TYPE_APK);
		if ( bRec )
		{
			//启动守护
			CVersion romver(CCodeOperation::UnicodeToUTF_8(_pDeviceData->GetDevInfo()->_deviceProductVersion));
			if(romver._nVer1>=4 && romver._nVer2>=2 )
			{
				AndroidShowDeamonUICommand showDeamonUIcommand(_pDeviceData->_DeviceInfo._strSerialNumber);
				showDeamonUIcommand.Execute();
			}
		}
	}

    if (!bRec)
        return bRec;
	
    if (!psCmd.isProcessExist(strPandaName) && CFileOperation::IsFileExist(strPandaPath.c_str()))
	{
		bRec = InstallCommand::SimpleInstallApp(_pDeviceData, strPandaPath, strPandaName, APP_TYPE_APK);
	}

    //刚安装完，休息一下
#ifdef WIN32        
        Sleep(500);
#else
				sleep(0.5);
#endif	
    return bRec;
#else
    return false;
#endif
}

bool ND91Assistant::AndroidUsbConnection::StopDeamon()
{
    AdbPSCommand cmd(_pDeviceData->_DeviceInfo._strSerialNumber);
    cmd.Execute();
    int nPID = cmd.GetProgressPID(ADB_DEMON_PACKAGE_NAME);
    if (nPID)
    {
        AdbKillProgressCommand killcmd(_pDeviceData->_DeviceInfo._strSerialNumber, nPID);
        killcmd.Execute();
    }
    return true;
}
