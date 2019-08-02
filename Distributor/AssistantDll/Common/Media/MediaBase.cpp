#include "stdafx.h"

MediaBase::MediaBase( const wstring path )
: _path(path)
{		
	_media = new MediaData;
}

MediaBase::~MediaBase(void)
{
	SAFE_DELETE(_media);
}


int ND91Assistant::MediaBase::GetInt( unsigned char* buf, int begin )
{
	if (sizeof((char*)buf) < 4)
		return 0;

	int num = (int)buf[begin + 3] + ((int)buf[begin + 2] << 8) + ((int)buf[begin + 1] << 16) + ((int)buf[0] << 24);

	return num;
}

long ND91Assistant::MediaBase::GetMediaDuration()
{
	RETURN_VALUE_IF ( _path.empty(), 0);

	wstring fileExt = CFileOperation::GetFileExt(_path);
	fileExt = CStrOperation::toLowerW(fileExt);

	MediaBase* base = NULL;
	
	if ( AAC == fileExt )
	{
		base = new AacParser(_path);
	}
	else if ( AVI == fileExt )
	{
		base = new AviParser(_path);
	}
	else if ( GP3 == fileExt )
	{
		base = new Gp3Parser(_path);
	}
	else if ( WAV == fileExt )
	{
		base = new WavParser(_path);
	}
	else if ( WMV == fileExt )
	{
		base = new WmvParser(_path);
	}
	else if ( MP4 == fileExt )
	{
		base = new Mp4Parser(_path);
	}
	else if ( OGG == fileExt )
	{
		base = new OggParser(_path);
	}
	else if ( REAL == fileExt )
	{
		base = new RealParser(_path);
	}

	int duration = base->GetMediaDuration();

	SAFE_DELETE(base);

	return duration;
}