#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "CheckNeedConnectConfirmCommand.h"

#include "Common/BaseSocket.h"
#include "Monitor/AndroidMonitorThread.h"
#include "Module/AndroidDefine.h"

std::string CheckNeedConnectConfirmCommand::GetAndroidDatagram()
{
    BinaryBufferWriter _writer;

    _writer.Clear();
    _writer.WriteBuffer("NDPC", 4);
    _writer.WriteShort(10);
    _writer.WriteShort(0);
    _writer.WriteInt(8);
    _writer.WriteInt(CHECKCONFIRM);
    _writer.WriteInt(1);
    
    return _writer.GetBuffer(); 
}

void CheckNeedConnectConfirmCommand::ParseturnDatagram( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.size());

    memset(_pHead, 0, 5);
    reader.ReadBuffer(_pHead, 4);
    reader.ReadInt32(_nFlag);
    reader.ReadInt32(_nDataLength);
    reader.ReadInt32(_nCommandID);
}

bool CheckNeedConnectConfirmCommand::VerifyPackageHead()
{
    return ! strncmp(_pHead, "NDPC", 4);
}

std::wstring CheckNeedConnectConfirmCommand::ToString()
{
    std::wostringstream woss;

    woss << " receive datagram : head=" << _pHead << ", flag=" << _nFlag
        << ", length" << _nDataLength << ", command id=" << _nCommandID;

    return woss.str();
}



#endif

bool ND91Assistant::CheckNeedConnectConfirmCommand::IsNeedConnectConfirm(wstring strHost, int nPort)
{
    CBaseSocket* pBaseSocket = new CBaseSocket();
    pBaseSocket->SetTimeout(5000);
    pBaseSocket->create(CCodeOperation::UnicodeToGB2312(strHost).c_str(), nPort);

    string datagram = CheckNeedConnectConfirmCommand::GetAndroidDatagram();
    pBaseSocket->sendData(datagram.c_str(), datagram.size());

    bool needConfirm = false;

    // 读取数据包
    char pPackageHead[DEMON_PACKAGE_HEAD_SIZE] = {0};
    if ( pBaseSocket->receiveDataLength(pPackageHead, DEMON_PACKAGE_HEAD_SIZE) )
    {
        string strReceive(pPackageHead, DEMON_PACKAGE_HEAD_SIZE);
        CheckNeedConnectConfirmCommand cmd;
        cmd.ParseturnDatagram(strReceive);

        int nDataLength = cmd.GetDataLength() - 4;
        if (nDataLength > 0)
        {
            char* pData = new char[nDataLength];
            if (pBaseSocket->receiveDataLength(pData, nDataLength))
            {
                BinaryBufferReader reader(pData, nDataLength);
                reader.ReadBool(needConfirm);
            }
			SAFE_DELETE(pData);
        }
    }

    pBaseSocket->close();
    SAFE_DELETE(pBaseSocket);
    return needConfirm;
}
