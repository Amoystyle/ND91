#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "MusicPlayCommand.h"
#include "Module/AndroidDefine.h"

MusicPlayCommand::MusicPlayCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = MEDIA;
}

MusicPlayCommand::~MusicPlayCommand(void)
{
}

bool ND91Assistant::MusicPlayCommand::Play( wstring musicPath )
{
	_type		= EPlayMusic;
	_remotepath = musicPath;
	Execute();
	return _success;
}

bool ND91Assistant::MusicPlayCommand::Stop()
{
	_type = EStop;
	Execute();
	return _success;
}

void ND91Assistant::MusicPlayCommand::Execute()
{
	_success = false;
	DefaultExecute();
}

bool ND91Assistant::MusicPlayCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	if(_nReturnChannelID != _nAndroidChannelID)
		return false;
	return true;
}

std::string ND91Assistant::MusicPlayCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  writer;
	writer.WriteInt(MEDIA);
	writer.WriteChar((char)_type);
	switch(_type)
	{
		case EPlayMusic:
			{
			writer.WriteString(CCodeOperation::UnicodeToUTF_8(_remotepath));
			float a = 1.0;
			writer.WriteBuffer((char*)&a, sizeof(float));
			}
			break;
		case EStop:
			break;
		default:
			break;
	}
	return writer.GetBuffer();
}

void ND91Assistant::MusicPlayCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
	switch(_type)
	{
		case EPlayMusic:
		case EStop:
			{
				int action;
				reader.ReadInt32(action);
				reader.ReadBool(_success);
			}
			break;
		default:
			break;
	}
}


#endif