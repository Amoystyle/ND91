#pragma once

#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "Common/BinaryBufferReader.h"
#include "AssistantDll.h"
#include <string>
using namespace std;

namespace ND91Assistant
{
	// ogg�ļ�ͷ����
	enum OggHeaderType
	{
		FreshPaquetInContinuedLogicalBitstream = 0,
		ContinuedPaquetInContinuedLogicalBitstream = 1,
		FreshPaquetInNewLogicalBitstream = 2,
		FreshPaquetInLastLogicalBitstream = 4,
		ContinuedPaquetInLastLogicalBitstream = 5,
		StartAndEndLogicalBitstream = 6
	};

	class OggParser : public MediaBase
	{ 
	public:
		OggParser(const wstring path);
		~OggParser(void);

		// ��ȡý���ļ�����ʱ��
		virtual	long GetMediaDuration();

		// �ж��Ƿ�����ĳ�ָ�ʽ�ļ�
		bool ParserFormat();

	private:

		// �����ļ���ȡogg�ļ���Ԫ������
		bool ReadData( bool onlyLast );

		bool IsLastInLogicalBitstream();


	private:

		OggHeaderType	_oggType;					 						

		/*__int64*/int64_t			_abGranulePostion;			// ogg�ض���ʶ����
		bool			_multiplexedStreamDetected;	// ��ȡĳ��ʶ�Ƿ�ɹ�

		int				_samples;					// ������
		int				_sampleRate;				// ���������

		unsigned char*	_segMents;					// ogg�ļ�ͷ����

		int				_pos;						// binarybuf��ȡȫ��λ��
		int				_firstBufLen;				// ����ͷ����(ÿ�ζ�ȡ�Ĳ�һ��)


	};
}
