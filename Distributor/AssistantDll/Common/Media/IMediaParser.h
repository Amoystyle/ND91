#pragma once

#include <string>

using namespace std;
	

namespace ND91Assistant
{
	class IMediaParser
	{ 
	public:

		// ��ȡý���ļ�����ʱ��
		virtual long GetMediaDuration() = 0;


	};
}
