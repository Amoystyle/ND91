#pragma once

#include <string>
#include <fstream>
#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "Common/BinaryBufferReader.h"
#include "AssistantDll.h"
using namespace std;

namespace ND91Assistant
{
	// wavͷ���ݽṹ��
	struct wavRiffHeader
	{
		int RiffID;		// 'R','I','F','F'
		int fileSize;
		int fmt;			// 'W','A','V','E'
	};
	
	// wav�ļ�����
	struct wavFormat
	{
		int		FmtID;
		int		BlockSize;
		short	FormatTag;
		short	Channels;
		int		SamplesPerSec;
		int		AvgBytesPerSec;
		short	lockAlign;
		short	BitsPerSample;
	};

	class WavParser : public MediaBase
	{ 
	public:
		WavParser(const wstring path);
		~WavParser(void);

		// ��ȡý���ļ�����ʱ��
		virtual	long GetMediaDuration();

		// �ж��Ƿ�����ĳ�ָ�ʽ�ļ�
		bool ParserFormat();

	private:

		wavRiffHeader			_riffHeader;
		wavFormat				_format;

		int						_pos;			// ȫ�����ݶ�ȡλ��

	};
}
