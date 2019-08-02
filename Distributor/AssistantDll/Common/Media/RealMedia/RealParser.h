#pragma once

#include <string>
#include "MediaBase.h"
#include "MediaData.h"
#include "RealData.h"
#include "Common/BinaryBufferReader.h"

using namespace std;

namespace ND91Assistant
{
	class RealParser : public MediaBase
	{ 
	public:
		RealParser(const wstring path);
		~RealParser(void);

		// ��ȡý���ļ�����ʱ��
		virtual	long GetMediaDuration();

		// �ж��Ƿ�����ĳ�ָ�ʽ�ļ�
		bool ParserFormat();

  private:
		
		// ��װ��real�ļ�ͷ�Աȵ����ݼ���
		void GetInitData();

		// �����ļ�ȡ��ʱ��
		unsigned int Decode( FIELDDEF* field, BinaryBufferReader* binaryBuf );

		unsigned int ReadUInt32BE( BinaryBufferReader* binaryBuf );

		unsigned short ReadUInt16BE( BinaryBufferReader* binaryBuf );

		// �������͵õ����ݳ���
		int GetSize( std::string type );

	private:

		vector<RealData*>		_recordef;		// real�ļ����ݵĴ洢

	};
}
