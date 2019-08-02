#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "PictureOperationCommand.h"
#include "Module/AndroidDefine.h"

PictureOperationCommand::PictureOperationCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;

    _type              = Operation_None;
	_nAndroidCommandID = 0;
    _imageId = 0;
}   

PictureOperationCommand::~PictureOperationCommand(void)
{
}

void PictureOperationCommand::Execute()
{
	_success = false;
	DefaultExecute();
}

std::string PictureOperationCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteChar(_nAndroidCommandID);
	switch(_type)
	{
		case Rename_Bucket:	// 重命名
		case Remove_Bucket:	// 移除分类
			{
				bufwriter.WriteString("bucket");
				bufwriter.WriteInt(1);
				bufwriter.WriteChar(_type==Rename_Bucket?DataRowState_Modified:DataRowState_Deleted);
				bufwriter.WriteString(CStrOperation::Replace(_bucketId, "'", "''"));
				wstring displayname = _type==Rename_Bucket ? _newName : _name;
				bufwriter.WriteStringAsUTF8(CStrOperation::ReplaceW(/*_name*/displayname,L"'",L"''"));
				bufwriter.WriteStringAsUTF8(CStrOperation::ReplaceW(_path,L"'",L"''"));
				bufwriter.WriteInt(1);
			}
			break;
		case Move_Picture:	// 移动图片到指定分类
		case Delete_Picture:  // 删除图片
			{
				bufwriter.WriteString("images");
				bufwriter.WriteInt(1);
				bufwriter.WriteChar( _type==Move_Picture ? DataRowState_Modified : DataRowState_Deleted);
				bufwriter.WriteInt(_imageId);

				if(_type == Move_Picture)
				{
					bufwriter.WriteString(_bucketId);
					bufwriter.WriteStringAsUTF8(_name);
				}
			}
			break;
		default:
			break;
	}
	return bufwriter.GetBuffer();
}

void PictureOperationCommand::SetAndroidReturnData( std::string data )
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

bool PictureOperationCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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

bool PictureOperationCommand::RenamePhotoBucket(string id, wstring path, wstring name, wstring newName)
{
	_type       = Rename_Bucket;
	_bucketId   = id;
    _name       = name;
    _path       = path;
	_newName    = newName;
	Execute();
	return _success;
}

bool PictureOperationCommand::RemovePhotoBucket(string id, wstring path, wstring name)
{
	_type	    = Remove_Bucket;
    _bucketId   = id;
    _path       = path;
    _name       = name;
	Execute();
	return _success;
}

bool PictureOperationCommand::MovePicToCategory(int imageId, string bucketId, wstring name)
{
	_type       = Move_Picture;
    _imageId    = imageId;
    _bucketId   = bucketId;
    _name       = name;
	Execute();
	return _success;
}

bool PictureOperationCommand::DeletePicture(int id)
{
	_type       = Delete_Picture;
    _imageId    = id;

	Execute();
	return _success;
}

bool ND91Assistant::PictureOperationCommand::NotifyScanPhoto()
{
	_type = NotifyScan;
	_nAndroidCommandID = 10;
	Execute();
	_nAndroidCommandID = 0;
	return true;
}

#endif