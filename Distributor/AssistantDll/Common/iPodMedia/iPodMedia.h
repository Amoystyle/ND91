#pragma once

#include <string>
#include <fstream>
#include "iPodMp3Paeser.h"

namespace ND91Assistant
{
	//ipod֧�ֵ�ý������������
	//�����ļ��Զ�����һ�����͵�ý���ʽ
	class ciPodMedia 
	{ 
	public:
		ciPodMedia(wstring path);
		~ciPodMedia();
		IiPodMediaParser* GetInstance(){return _MediaData;}
	private:
		//ý����������ָ��
		IiPodMediaParser* _MediaData;
	};
}