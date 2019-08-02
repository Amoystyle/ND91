#include "stdafx.h"

#ifdef MODULE_SYSTEM
#include "Module/AndroidDefine.h"

#include "CreateLongConnectionCommand.h"

std::string CreateLongConnectionCommand::GetAndroidDatagram(short port, wstring pcName)
{
    BinaryBufferWriter _writer;

    _writer.Clear();
    _writer.WriteBuffer("NDPC", 4);
    _writer.WriteShort(Client | NeedPostBack | CreateLongConnection);
    _writer.WriteShort(port);

    _writer.WriteInt(pcName.length() + 16);
    _writer.WriteInt(SYSTEM);
    _writer.WriteInt(1);    // �����Ӳ����汾��
                            //  = 1��pcName,timeout;
                            //  = 2: pcName,timeout,doConnectConfirm(pc�˿����Ƿ񵯳�����ȷ��)

    _writer.WriteInt(pcName.length());    // ��ʾ�ַ�������
    if (pcName.length() > 0)
    { 
        string strUTF8 = CCodeOperation::UnicodeToUTF_8(pcName);
        _writer.WriteBuffer(strUTF8.c_str(), strUTF8.length()); //pcName
    }
    _writer.WriteInt(30);   // ��ʱʱ�䣺Ҫ�ȴ��û����ֻ��ϲ���������̫��
    
    return _writer.GetBuffer();
}

void CreateLongConnectionCommand::ParseturnDatagram( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.size());

    memset(_pHead, 0, 5);
    reader.ReadBuffer(_pHead, 4);
    reader.ReadInt32(_nFlag);
    reader.ReadInt32(_nDataLength);
    reader.ReadInt32(_nCommandID);
}

bool CreateLongConnectionCommand::VerifyPackageHead()
{
    return ! strncmp(_pHead, "NDPC", 4);
}

std::wstring CreateLongConnectionCommand::ToString()
{
    std::wostringstream woss;

    woss << " receive datagram : head=" << _pHead << ", flag=" << _nFlag
        << ", length" << _nDataLength << ", command id=" << _nCommandID;

    return woss.str();
}



#endif