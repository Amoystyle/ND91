#include "pch_module.h"
#include "AndroidBaseCommand.h"

AndroidBaseCommand::AndroidBaseCommand(const DeviceData* pDevice)
:IBaseCommand(pDevice)
{

}

AndroidBaseCommand::~AndroidBaseCommand()
{
}

void ND91Assistant::AndroidBaseCommand::Execute()
{

}

std::string ND91Assistant::AndroidBaseCommand::GetAndroidDatagram()
{
    BinaryBufferWriter writer;	//����Android�����Writer

    // ����ͷ������
    writer.WriteBuffer(_pHead, 4);
    writer.WriteShort(_nFlag);
    writer.WriteShort(_pDevice->GetDevInfo()->_nLongConnectSign);

    // ȡ���������ݲ�д��
    writer.WriteString(GetAndroidBodyData());


    return writer.GetBuffer();
}

bool ND91Assistant::AndroidBaseCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
    pReader->ReadBuffer(_pReturnHead, 4);
    pReader->ReadShort(_nReturnFlag);
    pReader->ReadShort(_nReturnLongConnectSign);
    pReader->ReadInt32(_nReturnBodySize);
    pReader->ReadInt32(_nReturnChannelID);
    pReader->ReadInt32(_nReturnCommonType);
    //������ص����ݰ���Ϣ�����ϱ�Command����Ϣ��������
    if (_nReturnChannelID != _nAndroidChannelID || _nReturnCommonType != _nAndroidCommandID)
        return false;
    _result = COMMAND_EXECUTE_SUCCESS;
    return true;
}

bool ND91Assistant::AndroidBaseCommand::parseAndroidPackageHead_Top16( BinaryBufferReader* pReader )
{
    pReader->ReadBuffer(_pReturnHead, 4);
    pReader->ReadShort(_nReturnFlag);
    pReader->ReadShort(_nReturnLongConnectSign);
    pReader->ReadInt32(_nReturnBodySize);
    pReader->ReadInt32(_nReturnChannelID);
    if (_nReturnChannelID != _nAndroidChannelID)
        return false;
    _result = COMMAND_EXECUTE_SUCCESS;
    return true;
}

bool ND91Assistant::AndroidBaseCommand::parseAndroidPackageHead_Top12( BinaryBufferReader* pReader )
{
    pReader->ReadBuffer(_pReturnHead, 4);
    pReader->ReadShort(_nReturnFlag);
    pReader->ReadShort(_nReturnLongConnectSign);
    pReader->ReadInt32(_nReturnBodySize);
    if (memcmp(_pReturnHead, "NDPC", 4) != 0)
        return false;
    _result = COMMAND_EXECUTE_SUCCESS;
    return true;
}


