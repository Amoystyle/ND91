#pragma once

#include "Module/File/FileData.h"

#define STRKEY		"key"
#define STRING		"string"
#define RINGTONE	"ringtone"
#define DICT		"dict"
#define ITUNES		"itunes:"
#define STRSYSTEM	"system:"	



namespace ND91Assistant
{    
    class CLASSINDLL_CLASS_DECL RingData : public FileData
    {
    public:
        RingData();
        virtual ~RingData();

		wstring GetFullPath() const;
		void Assign(const RingData* pData);

		// ����һ�����࣬��ָ�����ڵ㣨�¶���ָ���ɵ����߸���
		virtual NodeData* Clone(DirData* pParent) const = 0;

		enum MediaFileKind
		{
			Null = 0,
			Ring = 1,
			Notif = 2,
			Alarm = 4,
			Mail = 8,
			Calendar = 16,
			Reminder = 32,
			Fetion = 64,
			Music = 128,
			All = 255	
		};

		int				_Id;			//
		int				_duration;		// ����ʱ��
		wstring			_artist;		// ������/�ݳ���
		wstring			_album;			// ר��
		string          _Guid;          // IOS�������
		bool			_IsSystemRing;	// �Ƿ�ϵͳ����
		int				_ringType;		// ��������, MediaFileKind��ֵ�����?
		wstring         _title;		    // ��������
		wstring         _remotePath;	// ȫ·��         
    };
}
