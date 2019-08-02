#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "CheckDeamonCommand.h"
#include "Module/AndroidDefine.h"

CheckDeamonCommand::CheckDeamonCommand(const DeviceData* pDevice)
: BaseCommand(pDevice)
{
	_nAndroidChannelID = DEAMONMANAGER;
	_nAndroidCommandID = GET_DEMON_VERSION;
}


CheckDeamonCommand::~CheckDeamonCommand()
{

}

std::string ND91Assistant::CheckDeamonCommand::GetAndroidBodyData()
{
    _BodyWriter.Clear();
    _BodyWriter.WriteInt(_nAndroidChannelID);
    _BodyWriter.WriteInt(_nAndroidCommandID);
    return _BodyWriter.GetBuffer();
}

void ND91Assistant::CheckDeamonCommand::SetAndroidReturnData(std::string data)
{
    BinaryBufferReader reader(data.c_str(), data.length());
    //�����е㲻���ϰ�ͷ�淶������ҵ��IDֵ,���Բ�Ҫʹ�� parseAndroidPackageHead
    reader.ReadBuffer(_pReturnHead, 4);
    reader.ReadShort(_nReturnFlag);
    reader.ReadShort(_nReturnLongConnectSign);
    reader.ReadInt32(_nReturnBodySize);
    reader.ReadInt32(_nReturnChannelID);
    _nReturnCommonType = GET_DEMON_VERSION; //�����ֵû�з���
    reader.ReadString(_strVersion);
}

void ND91Assistant::CheckDeamonCommand::Execute()
{
    DefaultExecute();
}



#endif