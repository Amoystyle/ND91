#pragma once

#include <string>
#include <fstream>
#include "MediaBase.h"
using namespace std;

namespace ND91Assistant
{
	class Gp3Parser : public MediaBase
	{ 
	public:
		Gp3Parser(const wstring path);
		~Gp3Parser(void);

		// ��ȡý���ļ�����ʱ��
		virtual	long GetMediaDuration();

		// �ж��Ƿ�����ĳ�ָ�ʽ�ļ�
		bool ParserFormat();

	private:

		// ��ȡ3gp�ļ��ĸ���Ԫ������
		void ParserMoov(fstream *fs, int moovSize);
		
		//	��ȡ��Ǽ������ת��int����
		int GetAtomSize(fstream *fs);

		//	��ȡ3gp�ļ����б��
		std::string GetAtomType(fstream *fs);

	};
}
