#pragma once

#include <string>
#include <fstream>
#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "AssistantDll.h"
using namespace std;

namespace ND91Assistant
{
	// aac�ļ�������
	class AacParser : public MediaBase
	{ 
	public:
		AacParser( const wstring path );
		~AacParser(void);

		// ��ȡý���ļ�����ʱ��
		virtual	long GetMediaDuration();

		// �ж��Ƿ�����ĳ�ָ�ʽ�ļ�
		bool ParserFormat();

	private:
		// Summary  : �����ļ�������ͷ
		// Returns  : unsigned char*
		// Parameter: wstring path
		unsigned char* ParserHeader();

		// aac��ADTS��ʽ����
		void ADTS_Parser(fstream* fs);

		// aac��ADIF��ʽ����
		void ADIF_Parser(unsigned char* buf);

		bool		_isADTS;	// �Ƿ���ADTS��ʽ
		bool		_isADIF;	// �Ƿ���ADIF��ʽ	

	};
}
