#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "SetWallpaperCommand.h"
#include "Module/File/GetSDCardStateCommand.h"
#include "Module/AndroidDefine.h"

#define WALLPAPER_PATH L"sdcard/wallpaper"

SetWallpaperCommand::SetWallpaperCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = PHOTOS;
}

SetWallpaperCommand::~SetWallpaperCommand(void)
{
}

bool ND91Assistant::SetWallpaperCommand::SetLocalImageAsWallPaper( wstring localPath )
{
	_localpath = localPath;
	GetSDCardStateCommand sdstatCmd(_pDevice);
	sdstatCmd.Execute();
	if(sdstatCmd.GetSDCardState() == MOUNTED)
	{
		wstring remote = wstring(WALLPAPER_PATH) + L"/" + CFileOperation::GetFileName(_localpath);
		DirectoryOperationCommand dirCmd(_pDevice);
		if(dirCmd.Exist(remote))
		{
			remote = GetVisibleName(remote);
			if(!_pDevice->GetDevInfo()->_sdcardpath.empty())
			{
				int pos = remote.find(L"sdcard/");
				if(pos>=0)
				{
					remote = CStrOperation::ReplaceW(remote, remote.substr(0, pos+6), _pDevice->GetDevInfo()->_sdcardpath);
				}
			}
		}

		UploadFileCommand upcmd(_pDevice);
		upcmd.setfilename(_localpath, remote);
		upcmd.Execute();
		if(upcmd.GetUploadResult() == Transfer_Success)
		{
			SetRemoteImageAsWallPaper(remote);
		}
	}
	else
	{
		_type = SetType_ImageData;
		Execute();
	}
	return _success;
}

bool ND91Assistant::SetWallpaperCommand::SetRemoteImageAsWallPaper( wstring remotePath )
{
	_remotepath = remotePath;
	_type  = SetType_RemotePath;
	Execute();
	return _success;
}

void ND91Assistant::SetWallpaperCommand::Execute()
{
	_success = false;

	DefaultExecute();

}

std::string ND91Assistant::SetWallpaperCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufwriter;

	bufwriter.WriteInt(_nAndroidChannelID);
	switch(_type)
	{
		case SetType_RemotePath:
			{
				bufwriter.WriteChar(4);
				bufwriter.WriteStringAsUTF8(_remotepath);
			}
			break;
		case SetType_ImageData:
			{
				bufwriter.WriteChar(6);
				string  data = CFileOperation::ReadFile(_localpath, true);
				bufwriter.WriteInt(data.length());
				bufwriter.WriteBuffer(data.c_str(), data.size());
			}
			break;
		default:
			break;
	}
	return bufwriter.GetBuffer();
}

void ND91Assistant::SetWallpaperCommand::SetAndroidReturnData( std::string data )
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

bool ND91Assistant::SetWallpaperCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
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


wstring SetWallpaperCommand::GetVisibleName(wstring remote)
{
	wstring path = CFileOperation::GetFileDir(remote);
	wstring fileName = CFileOperation::GetFileNameNoExt(remote);
	wstring ext = L"";
	int pos = remote.find_last_of(L".");
	if(pos>0)
		ext = remote.substr(pos);
	wstring ret = L"";
	DirectoryOperationCommand cmd(_pDevice);
	for (int i = 1; i < 10000; i++)
	{
		ret = path+fileName+L"_" +CCodeOperation::GB2312ToUnicode(CStrOperation::Int2string(i))+ext;
		if (!cmd.Exist(ret))
		{
			return ret;
		}
	}
	return ret;
}

#endif