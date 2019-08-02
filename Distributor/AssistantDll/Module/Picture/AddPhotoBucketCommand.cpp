#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "AddPhotoBucketCommand.h"
#include "AndroidPhotoBucket.h"
#include "Module/AndroidDefine.h"

AddPhotoBucketCommand::AddPhotoBucketCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;
}

AddPhotoBucketCommand::~AddPhotoBucketCommand(void)
{
}

void ND91Assistant::AddPhotoBucketCommand::SetParam( AndroidPhotoBucket* PhotoBucket )
{
	_pPhotoBucket = PhotoBucket;
}

void ND91Assistant::AddPhotoBucketCommand::Execute()
{
	DefaultExecute();
}

std::string ND91Assistant::AddPhotoBucketCommand::GetAndroidBodyData()
{
	if(!_pPhotoBucket)
		return "";
	BinaryBufferWriter  bufferWriter;
	bufferWriter.WriteInt(_nAndroidChannelID);
	bufferWriter.WriteChar(0);
	bufferWriter.WriteString("bucket");
	bufferWriter.WriteInt(1); //1Ìõ
	bufferWriter.WriteChar((char)DataRowState_Added);
	bufferWriter.WriteString(CStrOperation::Replace(_pPhotoBucket->_id,"'","''"));
	bufferWriter.WriteStringAsUTF8(CStrOperation::ReplaceW(_pPhotoBucket->_name,L"'",L"''"));
	bufferWriter.WriteStringAsUTF8(CStrOperation::ReplaceW(_pPhotoBucket->_path,L"'",L"''"));
	bufferWriter.WriteInt(_pPhotoBucket->_visible?1:0);
	return bufferWriter.GetBuffer();
}

void ND91Assistant::AddPhotoBucketCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;

	char* pOk = new char[5];
	reader.ReadBuffer(pOk, 4);
	if (!strncmp(pOk,"OKAY",4))
		_success = true;
	SAFE_DELETE_ARRAY(pOk);
}

bool ND91Assistant::AddPhotoBucketCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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

#endif