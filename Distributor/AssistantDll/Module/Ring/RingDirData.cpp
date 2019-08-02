#include "pch_module.h"

#ifdef MODULE_RING

#include "Module/Ring/RingDirData.h"
#include "Module/Ring/AddRingCommand.h"
#include "Module/Ring/AndroidRingData.h"
#include "Module/Ring/RingOperationCommand.h"

#include "Module/Common/Media/RealMedia/MP3Data.h"
#include "Module/Common/Media/RealMedia/WmvParser.h"
#include "Module/Common/Media/RealMedia/Mp4Parser.h"
#include "Module/Common/Media/RealMedia/OggParser.h"
#include "Module/Music/MusicPlayCommand.h"
#include "Core/International.h"

#define  SYSTEM_RING_URI  L"content://media/internal/audio/media/"
#define  CUSTOM_RING_URI  L"content://media/external/audio/media/"

RingDirData::RingDirData(const DeviceData* pDevice, RingGroupType type, wstring  fullpath)
{

	_pDir = new DirData(pDevice);
	_pDir->_name = fullpath;
	_pDir->SetHasLoad(true);
	DirectoryOperationCommand cmd(pDevice);
	if(!cmd.Exist(_pDir->_name))
		cmd.MakeDir(CStrOperation::trimRightW(_pDir->_name,L"/"));
	_type = type;
	_pCurRing = NULL;
	_iosCurrentRingTitle = L"Marimba";
}

RingDirData::~RingDirData(void)
{
	SAFE_DELETE(_pDir);
}

RingData* ND91Assistant::RingDirData::Insert(const RingData* pRing )
{
    return dynamic_cast<RingData*>(_pDir->Insert((NodeData*)pRing,false));
}


// int ND91Assistant::RingDirData::AddIosRings(deque<wstring> localRingPaths,ProgressObserver* pObserver)
// {
// 	RETURN_FALSE_IF( _pDir->_pDeviceData->GetDevInfo()->_deviceType != Ios );
// 	RETURN_FALSE_IF(localRingPaths.empty());
// 	return AddRings(localRingPaths,pObserver);
// }
// 
// int ND91Assistant::RingDirData::AddAndroidRings( RingData::MediaFileKind kind, deque<wstring> localRingPaths,ProgressObserver* pObserver)
// {
// 	RETURN_FALSE_IF( _pDir->_pDeviceData->GetDevInfo()->_deviceType != Android );
// 	RETURN_FALSE_IF(localRingPaths.empty());
// 	return AddRings(localRingPaths,pObserver, kind);
// }


int ND91Assistant::RingDirData::AddRings(deque<wstring> localringPaths, ProgressObserver* pObserver, RingData::MediaFileKind kind)
{
    // Ios平台强制为RingData::Null
    if (_pDir->_pDeviceData->GetDevInfo()->_deviceType == Ios)
        kind = RingData::Null;

	UploadFileCommand cmd(_pDir->_pDeviceData);
    cmd.Attach(pObserver);

    int sizeSum = 0;
    for (size_t i = 0; i < localringPaths.size(); i++)
        sizeSum += CFileOperation::GetFileSizeW(localringPaths[i]);
    // 设置所有任务总量
    cmd.SetAllTaskSum(sizeSum);
    // 设置任务总数
    cmd.SetTaskNumberSum(localringPaths.size());

    size_t count = 0;
    for(size_t i =0; i < localringPaths.size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        // 不支持的铃声类型跳过
		MediaFileType type = IsSurpportedMediaFormat(localringPaths[i]);
        if( type == MEDIA_AUDIO_UNKNOWN)
            continue;

		if ( _pDir->_pDeviceData->GetDevInfo()->_deviceType == Android  )
		{
			cmd.setfilename(localringPaths[i], _pDir->GetFullPath()+CFileOperation::GetFileName(localringPaths[i]));
			cmd.Execute();

			switch(cmd.GetUploadResult())
			{
			case Transfer_Success:
				{
					//上传成功添加铃声
					AddRingCommand  addcmd(_pDir->_pDeviceData);
					addcmd.SetParam(kind, localringPaths[i], cmd.GetDestFilePath(),type);
					addcmd.Execute();
					if(addcmd.GetAddedRing())
					{
						_pDir->Insert(addcmd.GetAddedRing());
						count++;
					}
					//cmd.DoneSuccess();
				}
				break;
			case Transfer_Cancel:
				{
					return (int)count;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			AddRingCommand  addcmd(_pDir->_pDeviceData);
			addcmd.SetParam(kind, localringPaths[i], _pDir->GetFullPath()+CFileOperation::GetFileName(localringPaths[i]),type);
			addcmd.Execute();
			if(addcmd.GetAddedRing())
			{
				_pDir->Insert(addcmd.GetAddedRing());
				count++;
			}
		}

        cmd.IncreaseTaskNumberComplete();
        cmd.SendProgress();

        cmd.SetCurrentTaskSum(1);
        cmd.SetCurrentTaskName(CFileOperation::GetFileName(localringPaths[i]));

        cmd.AddTaskProgress(1);
        cmd.SendProgress();
    }
    return (int)count;
}

MediaFileType RingDirData::IsSurpportedMediaFormat(wstring path)
{
	int  pos =  path.find_last_of(L".");
	wstring AndroidmediaFormat = L"mp3,wma,m4a,ogg,wav";
	wstring IosmediaFormat = L"mp3,wma,m4a,m4r";
	bool support = false;
	MediaFileType type = MEDIA_AUDIO_UNKNOWN;
	if(pos>0)
	{
		wstring tmp = path.substr(pos+1, path.length()-1-pos);
		tmp = CStrOperation::toLowerW(tmp);
		if(tmp == L"mp3")
			type = MEDIA_AUDIO_MP3;
		else if(tmp == L"wma")
			type = MEDIA_AUDIO_WMA;
		else if(tmp == L"m4a")
			type = MEDIA_AUDIO_M4A;
		else  if(tmp == L"ogg" && _pDir->_pDeviceData->GetDevInfo()->_deviceType == Android)
			type = MEDIA_AUDIO_OGG;
        else  if(tmp == L"wav" && _pDir->_pDeviceData->GetDevInfo()->_deviceType == Android)
            type = MEDIA_AUDIO_WAV;
		else if(tmp == L"m4r" && _pDir->_pDeviceData->GetDevInfo()->_deviceType == Ios)
			type = MEDIA_AUDIO_M4R;
	}
	else
	{
		if(MP3Data::IsValidMp3(path))
			type = MEDIA_AUDIO_MP3;
		else if(WmvParser(path).ParserFormat())
			type = MEDIA_AUDIO_WMA;
		else if(Mp4Parser(path).ParserFormat())
			type = MEDIA_AUDIO_M4A;
		else if (_pDir->_pDeviceData->GetDevInfo()->_deviceType == Android && OggParser(path).ParserFormat())
			type = MEDIA_AUDIO_OGG;
	}
	return type;
}


void ND91Assistant::RingDirData::StopRing()
{
#ifdef MODULE_MUSIC
	MusicPlayCommand cmd(_pDir->_pDeviceData);
	cmd.Stop();
#endif
}

bool ND91Assistant::RingDirData::RemoveRing(RingData* pRing)
{
	RETURN_FALSE_IF(!pRing || pRing->_IsSystemRing);
    RingOperationCommand cmd(_pDir->_pDeviceData);
	DirectoryOperationCommand dircmd(_pDir->_pDeviceData);

	bool suc = dircmd.DeleteFileLink(pRing->GetFullPath());
    if (_pCurRing && _pCurRing->_remotePath == pRing->_remotePath)
		_pCurRing = NULL;
    RETURN_FALSE_IF( ! cmd.RemoveRing(pRing) );
  	_pDir->RemoveFromChildrens(pRing);
	return true;
}

bool ND91Assistant::RingDirData::DownLoadRing( RingData* pRing, wstring destPath )
{
	RETURN_FALSE_IF(!pRing || destPath.empty());

	DownloadFileCommand cmd(_pDir->_pDeviceData);
	return cmd.Download(destPath, pRing->_remotePath);

}

deque<AndroidRingData*> ND91Assistant::RingDirData::GetAndroidRings(TypeReturnRings type)
{
    deque<NodeData*> nodelist = _pDir->GetChildrens();

	deque<AndroidRingData*> ringlist;
	for(size_t i = 0; i < nodelist.size(); i++)
	{
		bool IsSystem = (dynamic_cast<AndroidRingData*>(nodelist[i])->_IsSystemRing);
		if((type == RETURN_SYSTEM && !IsSystem) || (type == RETURN_CUSTOM && IsSystem))
			continue;
		ringlist.push_back(dynamic_cast<AndroidRingData*>(nodelist[i]));
	}
	return ringlist;
}

deque<RingData*> ND91Assistant::RingDirData::GetRings(TypeReturnRings type)
{
	deque<NodeData*> nodelist = _pDir->GetChildrens();

	deque<RingData*> ringlist;
	for(size_t i = 0; i < nodelist.size(); i++)
	{
		bool IsSystem = (dynamic_cast<RingData*>(nodelist[i])->_IsSystemRing);
		if((type == RETURN_SYSTEM && !IsSystem) || (type == RETURN_CUSTOM && IsSystem))
			continue;
		ringlist.push_back(dynamic_cast<RingData*>(nodelist[i]));
	}
	return ringlist;
}

void ND91Assistant::RingDirData::SetCurrentRing( wstring ringUri )
{
	if (_pDir->_pDeviceData->GetDevInfo()->_deviceType == Ios)
	{
		std::string guid = CCodeOperation::UnicodeToGB2312(ringUri);
		if ( (int)guid.find(STRSYSTEM) >= 0 )
		{
			_iosCurrentRingTitle = International::GetIosRingNameByGuid(ringUri);

			if (_iosCurrentRingTitle.empty())
				_iosCurrentRingTitle = ringUri.substr(7);

			return;
		}
		else if ( (int)guid.find(ITUNES) >= 0 )
		{
			guid = guid.substr(7);
			deque<NodeData*> nodelist = _pDir->GetChildrens();
			for(size_t i = 0; i < nodelist.size(); i++)
			{
				RingData* ring = dynamic_cast<RingData*>(nodelist[i]);
				if(guid == ring->_Guid)
				{
					_iosCurrentRingTitle = ring->_title;
					return;
				}
			}
		}
		else
            _iosCurrentRingTitle = CCodeOperation::GB2312ToUnicode("您的iphone未越狱，我们无法获取当前来电铃声");
	}
	else if (_pDir->_pDeviceData->GetDevInfo()->_deviceType == Android)
	{
		bool isSystem = false;
		int pos = ringUri.find_last_of(L"/");
		wstring tempuri = pos>-1?CStrOperation::toLowerW(ringUri.substr(0, pos+1)):L"";
		//int curId = pos >-1?_wtoi(ringUri.substr(pos+1).c_str()):-1;
		int curId = pos >-1?atoi(CCodeOperation::WstringToString(ringUri.substr(pos+1)).c_str()):-1;
		if(tempuri == SYSTEM_RING_URI)
			isSystem = true;
		else if(tempuri == CUSTOM_RING_URI)
			isSystem = false;
		SetCurrentAndroidRing(curId, isSystem);
	}
}

void ND91Assistant::RingDirData::SetCurrentAndroidRing( int ringId, bool isSystem)
{
	RETURN_IF(_pCurRing && _pCurRing->_IsSystemRing ==isSystem && ringId == _pCurRing->_Id);

	deque<NodeData*> nodelist = _pDir->GetChildrens();
	for(size_t i = 0; i < nodelist.size(); i++)
	{
		RingData* ring = dynamic_cast<RingData*>(nodelist[i]);
		if(ringId ==ring->_Id && isSystem == ring->_IsSystemRing)
		{
			_pCurRing = dynamic_cast<RingData*>(nodelist[i]);
			return;
		}
	}
}

RingData* ND91Assistant::RingDirData::ImportRing( wstring localPath, ProgressObserver* pObserver,
													wstring remotePath, int mediaKind, wstring title )
{
	// Ios平台强制为RingData::Null
	int kind = mediaKind;
	if (_pDir->_pDeviceData->GetDevInfo()->_deviceType == Ios)
		kind = RingData::Null;

	MediaFileType type = IsSurpportedMediaFormat(localPath);
	if( type == MEDIA_AUDIO_UNKNOWN)
		return NULL;

	UploadFileCommand cmd(_pDir->_pDeviceData);
	cmd.Attach(pObserver);
	wstring remote = remotePath;
	if(remotePath.empty())
		remote = _pDir->GetFullPath()+CFileOperation::GetFileName(localPath);

	DirectoryOperationCommand dircmd(_pDir->_pDeviceData);
	bool exist = dircmd.Exist(remote);
	bool music = ((kind&RingData::Music)==RingData::Music);

	if(!exist && _pDir->_pDeviceData->GetDevInfo()->_deviceType == Android)
	{ 
		cmd.setfilename(localPath, remote);
		cmd.Execute();
		if (cmd.GetUploadResult() != Transfer_Success)
			return NULL;
	}
	RingData* pRing = FindRing(remote, title);
	if(pRing) return pRing;

	//上传成功添加铃声
	AddRingCommand  addcmd(_pDir->_pDeviceData);
	addcmd.SetParam(kind, localPath, remote, type, title);
	addcmd.Execute();
	if(addcmd.GetAddedRing())
	{
		_pDir->Insert(addcmd.GetAddedRing());
		deque<NodeData*> nodes = _pDir->GetChildrens();
		return (RingData*)(_pDir->GetChildrens()[nodes.size()-1]);
	}
	return NULL;
}

RingData* ND91Assistant::RingDirData::FindRing( wstring remotePath, wstring title )
{
	deque<NodeData*> nodelist = _pDir->GetChildrens();
	for(size_t i = 0; i < nodelist.size(); i++)
	{
		RingData* ring = dynamic_cast<RingData*>(nodelist[i]);
		if (_pDir->_pDeviceData->GetDevInfo()->_deviceType == Ios)
		{
			if ( title == ring->_title )
				return ring;					
		}
		else if( _pDir->_pDeviceData->GetDevInfo()->_deviceType == Android )
		{
			if ( remotePath == ring->GetFullPath() ) 
				return ring;
		}
	}
	return NULL;
}



#endif