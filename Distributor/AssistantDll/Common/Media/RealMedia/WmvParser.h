#pragma once

#include <string>
#include <fstream>
#include "MediaBase.h"
#include "MediaData.h"
#include "Common/BinaryBufferReader.h"

using namespace std;

namespace ND91Assistant
{

	class WmvParser : public MediaBase
	{ 
	public:
		WmvParser( const wstring path );
		~WmvParser(void);

		// ��ȡý���ļ�����ʱ��
		virtual	long GetMediaDuration();

		// �ж��Ƿ�����ĳ�ָ�ʽ�ļ�
		bool ParserFormat();

	private:

		// ��ȡý���ļ�����ʱ��
		long Read_ASF_File_Properties_Object(BinaryBufferReader* binaryBuf);        

		// ��ȡ�ļ���ʽ		
		bool Read_ASF_Codec_List_Object(BinaryBufferReader* binaryBuf);									

		// ��ȡý���ļ�������Ԫ��(��ʱû��)
		void ReadASF_Extended_Content_Description_Object();							

		// �ж��ļ��Ƿ�ϸ�
		bool Verification();								   						

		// bufתGUID string����
		std::string BufferToGuidString(unsigned char* buf);							


		int					_pos;			// BinaryBuffer������λ��
		__int32				_header_num;	// �ļ�ָ��ͷ���ֽ���
		__int64				_bufsize;		// �̶������ֽ���


	};
}
