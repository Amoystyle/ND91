#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/AddPictureCommand.h"
#include "Module/Picture/PictureData.h"
#include "Module/AndroidDefine.h"

AddPictureCommand::AddPictureCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;
	_pRetPicture =NULL;

}

AddPictureCommand::~AddPictureCommand(void)
{
	//SAFE_DELETE(_pRetPicture);
}

void ND91Assistant::AddPictureCommand::Execute()
{
	SAFE_DELETE(_pRetPicture);
	DefaultExecute();
}

std::string ND91Assistant::AddPictureCommand::GetAndroidBodyData()
{
	BinaryBufferWriter   bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteChar(0);
	bufwriter.WriteString("images");
	bufwriter.WriteInt(1);
	bufwriter.WriteChar(DataRowState_Added);
	bufwriter.WriteStringAsUTF8(_remotePath);
	bufwriter.WriteString(MyFileOperation::GetImageFileMime(_localPath));  // mimetype
	bufwriter.WriteString(_groupId);
	bufwriter.WriteStringAsUTF8(_groupName);
	return bufwriter.GetBuffer();
}

void ND91Assistant::AddPictureCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;
	if (!_pRetPicture)
		_pRetPicture = new PictureData();
	_pRetPicture->ParseAndroidData(reader,_pDevice);

}

bool ND91Assistant::AddPictureCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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

void AddPictureCommand::SetParam( wstring remotePath,
                                 wstring localPath,
                                 string groupId,
                                 wstring groupName)
{
	_remotePath = remotePath;
	_localPath  = localPath;
	_groupId = groupId;
    _groupName = groupName;
}

#endif