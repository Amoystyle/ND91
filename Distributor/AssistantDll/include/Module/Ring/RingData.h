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

		// 复制一个新类，并指定父节点（新对象指针由调用者负责）
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
		int				_duration;		// 铃声时常
		wstring			_artist;		// 艺术家/演唱者
		wstring			_album;			// 专辑
		string          _Guid;          // IOS铃声标记
		bool			_IsSystemRing;	// 是否系统铃声
		int				_ringType;		// 铃声类型, MediaFileKind中值的组合?
		wstring         _title;		    // 标题名称
		wstring         _remotePath;	// 全路径         
    };
}
