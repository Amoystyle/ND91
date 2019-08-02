#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
	class MediaData
	{ 
	public:
		MediaData();
		~MediaData(void);

	public:
		long			_fileSize;			// �ļ���С
		long			_duration;			// ý��Ĳ���ʱ��
		int				_bitrate;
		bool			_isVideo;			// �Ƿ�����Ƶ�ļ�

        wstring         _title;             // ����
        wstring         _artist;            // ������
        wstring         _album;             // ר����
        wstring         _comment;           // ע��
        UINT            _nSampleRate;		//�������ʣ���λHz��

	};

}
