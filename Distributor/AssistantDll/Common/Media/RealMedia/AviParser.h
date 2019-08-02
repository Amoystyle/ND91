#pragma once

#include <string>
#include <fstream>
#include "MediaBase.h"
#include "Common/BinaryBufferReader.h"

using namespace std;

namespace ND91Assistant
{
	// avi�ļ�ͷ�ṹ��
	struct AviMainHeader
	{
		int fcc;					// ����Ϊ��avih��
		int cb;						// �����ݽṹ�Ĵ�С�������������8���ֽڣ�fcc��cb������
		int dwMicroSecPerFrame;		// ��Ƶ֡���ʱ�䣨�Ժ���Ϊ��λ��
		int dwMaxBytesPerSec;		// ���AVI�ļ������������
		int dwPaddingGranularity;	// ������������
		int dwFlags;				// AVI�ļ���ȫ�ֱ�ǣ������Ƿ����������
		int dwTotalFrames;			// ��֡��
		int dwInitialFrames;		// Ϊ������ʽָ����ʼ֡�����ǽ�����ʽӦ��ָ��Ϊ0��
		int dwStreams;				// ���ļ����������ĸ���
		int dwSuggestedBufferSize;	// �����ȡ���ļ��Ļ����С��Ӧ���������Ŀ飩
		int dwWidth;				// ��Ƶͼ��Ŀ�������Ϊ��λ��
		int dwHeight;				// ��Ƶͼ��ĸߣ�������Ϊ��λ��
		int dwReserved1;   			// ����1
		int dwReserved2;   			// ����2
		int dwReserved3;   			// ����3
		int dwReserved4;   			// ����4
	};

	// avi�ļ�������
    class MediaData;
	class AviParser : public MediaBase
	{ 
	public:
		AviParser( const wstring path );
		~AviParser(void);

		// ��ȡý���ļ�����ʱ��
		virtual	long GetMediaDuration();

		// �ж��Ƿ�����ĳ�ָ�ʽ�ļ�
		bool ParserFormat();

	private:

		// Summary  : �����ļ�������ͷ
		// Returns  : unsigned char*
		// Parameter: wstring path
		std::string ParserHeader(BinaryBufferReader* binarybuf, int headSize);

		AviMainHeader	_aviHeader;		// �ļ�ͷ���ݷ�װ�Ľṹ��		

		int				_pos;			// ȫ��BinaryBufferReaderָ�������λ��

	};
}
