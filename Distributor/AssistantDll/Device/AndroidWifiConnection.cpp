#include "stdafx.h"

#include "AndroidWifiConnection.h"
#include "Common/BaseSocket.h"
#include "ConnectionPool.h"
#include "BaseIO.h"

#include "Module/System/GetIMEICommand.h"
#include "Module/System/CheckDeamonCommand.h"
#include "Module/System/GetPropertyCommand.h"
#include "Module/System/SystemManager.h"
#include "Module/System/ReloadDeamonInfoCommand.h"
#include "Module/System/GetMacAddressCommand.h"
#include "Monitor/MonitorThread.h"
#include "Common/version.h"
#include "Module/AndroidDefine.h"

AndroidWifiConnection::AndroidWifiConnection(DeviceData* pDeviceData)
:_pDeviceData(pDeviceData)
{

}



AndroidWifiConnection::~AndroidWifiConnection()
{

}

bool ND91Assistant::AndroidWifiConnection::PreActivate()
{
    return true;
}
bool ND91Assistant::AndroidWifiConnection::Activate()
{
    //��ʼ�����ӳ� - ����Ҫ����1�����Ӳ�ִ��һ��������ܽ��յ�Android���������ص����� (��ȷ��)
    if (!_pDeviceData->_pConnectionPool)
    {
		//_pConnectionPool�ڴ�˵���������￪�٣���DeviceData->ReleaseAll()������
        _pDeviceData->_pConnectionPool = new ConnectionPool(&(_pDeviceData->_DeviceInfo));
        _pDeviceData->_pConnectionPool->init(1, 20);
    }

    //WIFI�����ʱ���ȡӲ����Ϣ - ������Ҫ������ǰ����ʹ��֮ǰ��Ҫ��ʼ�����ӳ�
    GetAndroidProperty();
    //IMEI�л�ȡʧ�ܵ������������ͨ���ػ��ٴλ�ȡ
    if (_pDeviceData->_DeviceInfo._deviceIMEI.length() == 0)
    {
        BaseIO *pIO = _pDeviceData->CreateIO();
        GetIMEICommand cmd(_pDeviceData);
        cmd.Execute();
        _pDeviceData->_DeviceInfo._deviceIMEI = cmd.GetIMEI();
    }

	if (_pDeviceData->_DeviceInfo._deviceResolution.empty())
	{
		BaseIO *pIO = _pDeviceData->CreateIO();
		SystemManager sys(_pDeviceData);
		_pDeviceData->_DeviceInfo._bRoot			= sys.GetIsRoot();
		_pDeviceData->_DeviceInfo._deviceResolution	= sys.GetDeviceResolution();	
		SAFE_DELETE(pIO);
	}

    //����ػ��汾
    {
        BaseIO *pIO = _pDeviceData->CreateIO();
        CheckDeamonCommand cmd(_pDeviceData);
        cmd.Execute();
        CVersion ver(cmd.getVersion());

        if (ver.NeedUpdateAndroidDeamon())        // ��Ҫ�����ػ�
        {
            return false;
        }
    }
    return true;
}

bool ND91Assistant::AndroidWifiConnection::Connect()
{
    return false;
}

void ND91Assistant::AndroidWifiConnection::Disconnect()
{
    //�������ӳ�
    if (_pDeviceData->_pConnectionPool)
    {
        //delete _pDeviceData->_pConnectionPool;
        //_pDeviceData->_pConnectionPool = NULL;
        if (_pDeviceData->_pConnectionPool)
            _pDeviceData->_pConnectionPool->ReleaseAll();
    }
    //ֹͣ����߳�
    if (_pDeviceData->_pMonitorThread)
    {
        _pDeviceData->_pMonitorThread->StopThread(); 
        delete _pDeviceData->_pMonitorThread;
        _pDeviceData->_pMonitorThread = NULL;
    }
}

const DeviceInfo* ND91Assistant::AndroidWifiConnection::GetDeviceInfo()
{
    return &(_pDeviceData->_DeviceInfo);
}


void ND91Assistant::AndroidWifiConnection::GetAndroidProperty()
{
    BaseIO *pIO = _pDeviceData->CreateIO();
    GetPropertyCommand cmd(_pDeviceData);
    cmd.Execute();
    const DeviceInfo *pInfo = cmd.GetDeviceInfo();

    _pDeviceData->_DeviceInfo._deviceIMEI             = pInfo->_deviceIMEI;          
    _pDeviceData->_DeviceInfo._manufacturer           = pInfo->_manufacturer;       
    _pDeviceData->_DeviceInfo._deviceHWModel          = pInfo->_deviceHWModel;       
    _pDeviceData->_DeviceInfo._deviceProductVersion   = pInfo->_deviceProductVersion;
    _pDeviceData->_DeviceInfo._language               = pInfo->_language;  
	_pDeviceData->_DeviceInfo._deviceCoreVersion	  = pInfo->_deviceCoreVersion;
	_pDeviceData->_DeviceInfo._deviceBaseVersion	  = pInfo->_deviceBaseVersion;
	_pDeviceData->_DeviceInfo._deviceInsdeVersion	  = pInfo->_deviceInsdeVersion;

//     ReloadDeamonInfoCommand deamoInfoCmd(_pDeviceData);
//     deamoInfoCmd.Execute();
//     _pDeviceData->_DeviceInfo._deviceWiFiMac = CCodeOperation::UTF_8ToUnicode(deamoInfoCmd.GetMacAddress());

    GetMacAddressCommand macAddressCmd(_pDeviceData);
    macAddressCmd.Execute();
    _pDeviceData->_DeviceInfo._deviceWiFiMac = macAddressCmd.GetMacAddress();
}