///////////////////////////////////////////////////////////
//  UsbConnection.cpp
//  Implementation of the Class UsbConnection
//  Created on:      01-����-2011 9:43:04
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "UsbConnection.h"

using namespace ND91Assistant;

UsbConnection::UsbConnection()
{

}



UsbConnection::~UsbConnection()
{

}





std::vector<DeviceInfo> UsbConnection::ScanUsbBus()
{

	std::vector<DeviceInfo> devices;

	return  devices;
}


/**
 * ����0��ʾ�ɹ�����0��ʾ�������
 */
int UsbConnection::Connect(DeviceInfo di)
{

	return 0;
}


void UsbConnection::Disconnect(DeviceInfo di)
{

}