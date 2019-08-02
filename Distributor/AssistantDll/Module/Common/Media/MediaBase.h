#pragma once

#include "IMediaParser.h"

#define AAC  L"aac"
#define GP3  L"3gp"
#define MP4  L"mp4"
#define AVI  L"avi"
#define OGG	 L"ogg"
#define REAL L"rmvb"
#define WAV  L"wav"
#define WMV  L"wmv"

const int CHUNK_SIZE = 4;

namespace ND91Assistant
{
    class MediaData;
	class MediaBase : public IMediaParser
	{ 
	public:
		MediaBase( const wstring path );
		~MediaBase(void);

		// ��ȡý���ļ�����ʱ��
		virtual long GetMediaDuration();

		virtual bool ParserFormat(){return false;}

        MediaData* GetMediaData() { return _media; }

	protected:

		int GetInt(unsigned char* buf, int begin);

		MediaData*			_media;				// ��Ŷ�ý���ļ�Ԫ�ص�������

		wstring				_path;	

	};
}
