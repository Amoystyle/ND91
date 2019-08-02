#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "GetWallpaperCommand.h"

#include "Module/File/GetSDCardStateCommand.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

GetWallpaperCommand::GetWallpaperCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;
	_savePath          = L"";
}

GetWallpaperCommand::~GetWallpaperCommand(void)
{
}

void ND91Assistant::GetWallpaperCommand::Execute()
{
    CFileOperation::DeleteFile((_pDevice->GetAndroidPath()->GetWallPaperPath()+L"wallpaper"));
	_savePath = L"";
	DefaultExecute();
}

std::string ND91Assistant::GetWallpaperCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteChar(2);
	return bufwriter.GetBuffer();
}

void ND91Assistant::GetWallpaperCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader  reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead(&reader))
		return;

	char* pOk = new char[5];
	reader.ReadBuffer(pOk, 4);
	if (strncmp(pOk,"OKAY",4))
		return;
	SAFE_DELETE_ARRAY(pOk);

	_savePath = _pDevice->GetAndroidPath()->GetWallPaperPath()+L"wallpaper";
	GetSDCardStateCommand sdstatCmd(_pDevice);
	sdstatCmd.Execute();
	if(sdstatCmd.GetSDCardState() == MOUNTED)
	{
		wstring remotepath = L"";
		reader.ReadStringFromUTF8(remotepath);
		
		if (DownloadFileCommand(_pDevice).Download(
            _savePath,
            remotepath) )
		{
			//TODO:移除掉手机端文件?
			DirectoryOperationCommand dircmd(_pDevice);
			dircmd.DeleteFileLink(remotepath);
		}
	}
	else
	{
		int length =0;
		reader.ReadInt32(length);
		char* pbuf = new char[length+1];
		reader.ReadBuffer(pbuf, length);
		CFileOperation::WriteBinFile(
            _savePath,
            pbuf,length);
		SAFE_DELETE_ARRAY(pbuf);
	}
}

bool ND91Assistant::GetWallpaperCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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