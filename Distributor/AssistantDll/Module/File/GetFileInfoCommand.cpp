#include "stdafx.h"

#include "GetFileInfoCommand.h"
#include "Core/DeviceData.h"
#include "Module/AndroidDefine.h"

GetFileInfoCommand::GetFileInfoCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	// Android
    _nAndroidChannelID  = FILEMANAGER;
    _nAndroidCommandID  = GET_FILEINFO;
}

GetFileInfoCommand::~GetFileInfoCommand(void)
{
}

void ND91Assistant::GetFileInfoCommand::Execute()
{
    RETURN_IF(_pDevice->GetDevInfo()->_deviceType == Ios);
    DefaultExecute();
}

std::string ND91Assistant::GetFileInfoCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufWrite;
	bufWrite.Clear();
	bufWrite.WriteInt(_nAndroidChannelID);
	bufWrite.WriteChar(_nAndroidCommandID);
    bufWrite.WriteStringAsUTF8(_remotePath);
	return bufWrite.GetBuffer();
}

void ND91Assistant::GetFileInfoCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());
    RETURN_IF(!parseAndroidPackageHead_Top16(&reader));

    int flag = 0;
    reader.ReadInt32(flag);
    RETURN_IF(flag != 1);

    _fileInfo._path = _remotePath;
    _fileInfo._name = CFileOperation::GetFileName(_remotePath);

    int nSize = 0;
    string strUTF8;
    reader.ReadInt32(nSize);
    reader.ReadString(strUTF8, nSize);
    _fileInfo._linkTo = CCodeOperation::UTF_8ToUnicode(strUTF8);

    reader.ReadInt64(_fileInfo._size);

    int64_t date = 0;
    reader.ReadInt64(date);
    _fileInfo._date = date*1000;

    uint32_t st_mode = 0;
    reader.ReadUInt32(st_mode);

    reader.ReadBool(_fileInfo._hasSubFolders);
}