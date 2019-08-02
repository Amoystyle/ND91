#include "pch_module.h"

#ifdef MODULE_VIDEO

#include "Module/Video/VideoOperationCommand.h"
#include "Module/Video/VideoData.h"

#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/RealMedia/AacParser.h"
#include "Module/Common/Media/RealMedia/AviParser.h"
#include "Module/Common/Media/RealMedia/Gp3Parser.h"
#include "Module/Common/Media/RealMedia/WavParser.h"
#include "Module/Common/Media/RealMedia/WmvParser.h"
#include "Module/Common/Media/RealMedia/Mp4Parser.h"
#include "Module/Common/Media/RealMedia/OggParser.h"
#include "Module/Common/Media/RealMedia/RealParser.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

VideoOperationCommand::VideoOperationCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = MEDIA;

	_pVideo      = NULL;     //����������������Ƶ
	_newName     = L"";      //�����������������������

	_localpath   = L"";      //�����������Ƶʱ�ı���·��
	_remotePath  = L"";      //�����������Ƶʱ���ϴ����ֻ��˵�·��

	_pVideoList  = NULL;     //��������ȡ��Ƶ���洫�����Ƶ�б�

	_pReturnVideo= NULL;     // ����ֵ����ӵ���Ƶ
	_size        = 0;        // ����ֵ���ռ��С
	_success     = false;    // ����ֵ���ɹ�ʧ��
}

VideoOperationCommand::~VideoOperationCommand(void)
{
	SAFE_DELETE(_pReturnVideo);
}

void ND91Assistant::VideoOperationCommand::Execute()
{
	_success   = false;
	DefaultExecute();
}

std::string ND91Assistant::VideoOperationCommand::GetAndroidBodyData()
{
	BinaryBufferWriter bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteChar(_type);
	switch(_type)
	{
		case VIDEO_ADD:
			{
				// TODO::��ȡ������Ƶ�ļ���Ϣ
				bufwriter.WriteStringAsUTF8(_remotePath);//
				bufwriter.WriteStringAsUTF8(CFileOperation::GetFileNameNoExt(_remotePath));//tittle
				bufwriter.WriteStringAsUTF8(L"");//artist
				bufwriter.WriteStringAsUTF8( CFileOperation::GetFileName(CStrOperation::trimRightW(CFileOperation::GetFileDir(_remotePath),L"/")));//album
				bufwriter.WriteInt64(CFileOperation::GetFileSizeW(_localpath));//size��С
				int duration = 0;
				wstring mime = GetVideoMime(_localpath, duration);
				bufwriter.WriteStringAsUTF8(mime);//mime
				// ʱ����ר����
				bufwriter.WriteInt(duration*1000);//durationʱ��
			}
			break;
		case VIDEO_REMOVE:
			bufwriter.WriteInt(_pVideo->_id);
			break;
		case VIDEO_RENAME:
			{
				bufwriter.WriteInt(_pVideo->_id);
				bufwriter.WriteStringAsUTF8(_newName);
			}
			break;
		case VIDEO_GET_IMAGE:
			{
				bufwriter.WriteInt(_pVideoList->size());
				for (size_t i =0; i < _pVideoList->size(); i++)
				{
					bufwriter.WriteStringAsUTF8(_pVideoList->at(i)->_remotepath);
				}
			}
			break;
		default:
			break;
	}
	return bufwriter.GetBuffer();
}

void ND91Assistant::VideoOperationCommand::SetAndroidReturnData( std::string data )
{
    BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead_Top16(&reader))
		return;
	if(reader.IsEnd())
		return;

	switch(_type)
	{
	case VIDEO_ADD:
		{
			_pReturnVideo = new VideoData();
			_pReturnVideo->ParseAndroidData(reader);
		}
		break;
	case VIDEO_REMOVE:
	case VIDEO_RENAME:
		{
			string okay;
			reader.ReadString(okay,4);
			if (okay =="OKAY")
				_success = true;
		}
		break;
	case VIDEO_GET_IMAGE:
		{
            // ���Ӵ����ж�
            int nOffset = reader.GetOffset();
            string okay;
            reader.ReadString(okay,4);
            if (okay == "FAIL")
                break;
            reader.SetOffset(nOffset);
            wstring strDir = _pDevice->GetPath()->GetLocalVideoImagePath();
			CFileOperation::MakeLocalDir(strDir);
			for (size_t i = 0; i < _pVideoList->size(); i++)
			{
				VideoData* pvideo = _pVideoList->at(i);
				int length = 0;
				reader.ReadInt32(length);
				if (length > 0)
				{
					string data;
					reader.ReadString(data,length);

					pvideo->_imageLocalPath = strDir + CFileOperation::GetFileNameNoExt(pvideo->_remotepath)+L".png";
					CFileOperation::WriteBinFile(pvideo->_imageLocalPath, data.c_str(), data.length());
				}
			}
		}
		break;
	case VIDEO_GET_TOTAL_SIZE:
		{
			reader.ReadInt64(_size);
		}
		break;
	default:
		break;
	}

}

VideoData* ND91Assistant::VideoOperationCommand::AddVideo( wstring localpath, wstring remotepath )
{
	_type      = VIDEO_ADD;
	_localpath = localpath;
	_remotePath= remotepath;
	SAFE_DELETE(_pReturnVideo);
	Execute();
	return _pReturnVideo;
}

bool ND91Assistant::VideoOperationCommand::RemoveVideo( VideoData* pVideo )
{
	RETURN_FALSE_IF(!pVideo);
	_type      = VIDEO_REMOVE;
	_pVideo    = pVideo;
	Execute();
	return     _success;
}

bool ND91Assistant::VideoOperationCommand::RenameVideo( VideoData* pVideo, wstring newName )
{
	RETURN_FALSE_IF(!pVideo);
  _type		= VIDEO_RENAME;
  _pVideo	= pVideo;
  _newName	= newName;
  Execute();
  return	_success;
}

void ND91Assistant::VideoOperationCommand::GetVideoImages( const deque<VideoData*>* pVideoList )
{
	RETURN_IF(!pVideoList || pVideoList->size()<1);
	_type		= VIDEO_GET_IMAGE;
	_pVideoList = pVideoList;
	Execute();
}

__int64 ND91Assistant::VideoOperationCommand::GetTotalSize()
{
	_type = VIDEO_GET_TOTAL_SIZE;
	_size = 0;
	Execute();
	return _size;
}
wstring VideoOperationCommand::GetVideoMime(wstring localpath, int& duration)
{
	int pos = localpath.find_last_of(L".");
	wstring extw = pos>-1?localpath.substr(pos+1):L"";
	extw = CStrOperation::toLowerW(extw);
	wstring mime = L"";
	MediaBase* base = NULL;
	if (extw == L"mp4" || extw == L"m4v")
	{
		mime = L"video/mp4";
		base = new Mp4Parser(localpath);
	}
	else if (extw == L"3gp")
	{
		mime = L"video/3gpp";
		base = new Gp3Parser(localpath);
	}
	else if (extw == L"wmv")
	{
		mime = L"video/x-ms-wmv";
		base = new WmvParser(localpath);
	}
	if(mime.empty())
	{
		base = new Mp4Parser(localpath);
		if(base->ParserFormat())
			mime = L"video/mp4"; 
		else
		{
			SAFE_DELETE(base);
			base = new Gp3Parser(localpath);
			if(base->ParserFormat())
				mime = L"video/3gpp";
			else
			{
				SAFE_DELETE(base);
				base = new WmvParser(localpath);
				if(base->ParserFormat())
					mime = L"video/x-ms-wmv";
				else
					SAFE_DELETE(base);	
			}
		}
	}
	if (base)
		duration = base->GetMediaDuration();
	return mime;
}

#endif