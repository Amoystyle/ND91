// 
// iTunnes 音乐/视频文件数据结构类
//
#pragma  once

#include <string>
#include <vector>
#include "AssistantDll.h"
#include "Common/BinaryBufferReader.h"
#include "Common/BinaryBufferWriter.h"
#pragma warning(disable : 4251)

using namespace std;
namespace ND91Assistant
{
	
	enum MHOD_TYPE{
		MHOD_ID_TITLE = 1,
		MHOD_ID_PATH = 2, 
		MHOD_ID_ALBUM = 3,
		MHOD_ID_ARTIST = 4,
		MHOD_ID_GENRE = 5,
		MHOD_ID_FILETYPE = 6,
		MHOD_ID_EQSETTING = 7, 
		MHOD_ID_COMMENT = 8,
		MHOD_ID_CATEGORY = 9,
		MHOD_ID_COMPOSER = 12,
		MHOD_ID_GROUPING = 13, 
		MHOD_ID_DESCRIPTION = 14,// —— Podcast Enclosure URL. Note: this is either a UTF-8 or ASCII encoded string (NOT UTF-16). Also, there is no mhod::length value for this type. Introduced in db version 0x0d.
		MHOD_ID_PODCASTURL = 15,// —— Podcast RSS URL. Note: this is either a UTF-8 or ASCII encoded string (NOT UTF-16). Also, there is no mhod::length value for this type. Introduced in db version 0x0d.
		MHOD_ID_PODCASTRSS = 16,// —— Chapter data. This is a m4a-style entry that is used to display subsongs within a mhit. Introduced in db version 0x0d.
		MHOD_ID_CHAPTERDATA = 17,// —— Subtitle (usually the same as Description). Introduced in db version 0x0d.
		MHOD_ID_SUBTITLE = 18,// —— Show (for TV Shows only). Introduced in db version 0x0d?
		MHOD_ID_TVSHOW = 19,// —— Episode # (for TV Shows only). Introduced in db version 0x0d?
		MHOD_ID_TVEPISODE = 20,// —— TV Network (for TV Shows only). Introduced in db version 0x0d?
		MHOD_ID_TVNETWORK = 21,// —— Album Artist. Introduced in db version 0x13?
		MHOD_ID_ALBUMARTIST = 22,// —— Sort key for artist.e
		MHOD_ID_SORT_ARTIST = 23,// —— Appears to be a list of keywords pertaining to a track. Introduced in db version 0x13?
		MHOD_ID_KEYWORDS = 24,// —— more sort keys, taking precedence over the standard entries if present
		MHOD_ID_SORT_TITLE = 27,
		MHOD_ID_SORT_ALBUM = 28,
		MHOD_ID_SORT_ALBUMARTIST = 29,
		MHOD_ID_SORT_COMPOSER = 30,
		MHOD_ID_SORT_TVSHOW = 31,
		MHOD_ID_SPLPREF = 50,// —— settings for smart playlist
		MHOD_ID_SPLRULES = 51,// —— rules for smart playlist
		MHOD_ID_LIBPLAYLISTINDEX = 52,// —— Library Playlist Index
		MHOD_ID_LIBPLAYLISTJUMPTABLE=53,
		MHOD_ID_PLAYLIST = 100,
		MHOD_ID_ALBUM_ALBUM = 200,// —— MHODs for the MHIAs
		MHOD_ID_ALBUM_ARTIST = 201,
		MHOD_ID_ALBUM_SORT_ARTIST = 202,
		MHOD_ID_ALBUM_ARTIST_MHII = 300,
	};


	//文件头部结构
	class cdbbasehead
	{
	public:
		cdbbasehead();
		void Read(BinaryBufferReader* pReader,const char* pName);
		void Write(BinaryBufferWriter* pWrite);
		char    _name[5];   // 名字: 4个ASCII字符
		__int32     _headSize;  // 头部长度
	};
	//cdb文件结构基类
	class CLASSINDLL_CLASS_DECL cdbBase
	{
	public:	
		cdbBase();
		~cdbBase();
		virtual	void Read(BinaryBufferReader* pReader)=0;
		virtual void Write(BinaryBufferWriter* pWrite)=0;
		virtual void release() = 0;
		cdbbasehead _head;              // 头部			
	};

	//CDB存放文本数据类型CMhod的主体基类
	class CbaseMhod
	{
	public:	
		CbaseMhod(int ntype);
		virtual	void Read(BinaryBufferReader* pReader)=0;
		virtual void Write(BinaryBufferWriter* pWrite)=0;
		int GetType(){return _type;}	
		virtual wstring GetInfo()=0;
		virtual void release() = 0;
		int _type;
	};
	//整个CDB文件的头（未压缩部分）
	class CLASSINDLL_CLASS_DECL CMhbd : public cdbBase
	{
		public:		
			CMhbd(){_nItemNum=0;}
			void Read(BinaryBufferReader* pReader);
			void Write(BinaryBufferWriter* pWrite);
			virtual void release();
		
			__int32 _nOffsetCompress;		//压缩结束位置
			__int32 _nIsCompress;			//是否压缩如果没有为1，有为2
			__int32 _niTunesVersion;		//iTunes版本号
			__int32	_nItemNum;				//子项目个数
			__int64	_nItdID;				//CDB内部ID
			char	_OSItunes[2];			//itunes平台类型1-mac 2-windows
			char	_unknow1[2];
			__int64	_nItunesLibID;			//这个ID和iTunesPrefs文件中的ID一致，这个ID和同步有关这个ID和iTunesPrefs文件中的ID一致，这个ID和同步有关
			char	_unknow2[4];
			char	hashtype[2];			//看起来像hash类型，1 hash58 2 hash72, 3 hashAB，无hash为0，未知=-1
			char	_unknow3[20];
			char	_language[2];			//语言
			__int64	_pid;					//PID，与sqlite中的db_info db_pid一致
			__int32		_TrackFlg;			//未知，当有Track时=1，没有的时候=0
			__int32		_unknowflg;			//当最初无记录时=0，当有Track后，变为0x9F
			char	_hash58[20];			//
			__int32		_timeZone;			//单位s
			char	_hashtype[2];			//表示hash校验方式 4表示hashA 2表示hash72 0表示其他 iPod Touch4采用hash72校验
			char	_hash72[46];			//Hash72校验
			char	_audiolang[2];			//音频语言
			char	_subtitlelang[2];		//章目语言
			char	_unknow4[7];			//0x00 19 00 0a 00 01 00
			char	_hashAB[57];			//hashAB校验
			char	_unknow5[8];
			static const int _panddingLen = 8;			//dummy space
	};
	//CDB文件里面未知的数据处理结构
	class CMhuk : public cdbBase
	{
	public:	
		CMhuk(int nvalue);
		~CMhuk();
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nBodylen;
		char* temp;
	};
	//CDB文件循环项头部
	class CMhsd : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		

		__int32 _nBodylen;  //包长度
		__int32	_type;		//1 —— track 2 —— playlists 3 —— playlists 4 —— albums 5 —— playlists 8 —— artists 9 —— genius_cuid
		static const int _panddingLen = 80;
	};
	//专辑列表结构体
	class CMhla : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
				
		__int32 _nAlbumsNum;	//专辑个数
		static const int _panddingLen = 80;
	};
	//专辑信息结构体
	class CMhia : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nBodylen;  //包长度
		__int32 _nTrackNum;	//个数
		__int32 _nAlbumid;	//album id，是iTunes内部ID
		__int64 _nAlbumDBid; //Album id，是sqlitedb的pid
		__int32 _unknow;	
		char _unknow1[8];   
		char _unknow2[8]; 
		static const int _panddingLen = 40;
	};
	//专辑表演者列表结构体
	class CMhli : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		virtual void release();
		void Write(BinaryBufferWriter* pWrite);
	
		__int32 _nArtistsNum;	//artists个数，是指Album Artists的个数
		static const int _panddingLen = 80;
	};
	//专辑表演者信息结构体
	class CMhii : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
	
		__int32 _nBodylen;		//包长度
		__int32 _nTrackNum;		//个数
		__int32 _nArtistid;		// Artist id，是iTunes内部ID
		__int64 _nArtistDBid;	// Artist id，是sqlitedb的pid
		__int32 _unknow;
		static const int _panddingLen = 48;		
	};
	//歌曲列表结构体
	class CMhlt : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		virtual void release();
		void Write(BinaryBufferWriter* pWrite);
		
		__int32 _nTracksNum;	//专辑个数
		static const int _panddingLen = 80;
	};
	//歌曲信息结构体
	class CMhit : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		//************************************
		// Method:    填充固定值或者未知值的变量，用于新添加进来的文件
		// Returns:   void
		//************************************
		void FillNewStruct();

		__int32 _nBodylen;			//包长度 12
		__int32 _nInfoNum;			//个数 16
		__int32 _ntrackid;			//album id，是iTunes内部ID 20
		__int32 _nIsHide;			//track是否可见24
		char   _nFileType[4];			//track的文件类型标示28
		char	_type1;				//CBR MP3s are type 0x00, VBR MP3s are type 0x01, AAC are type 0x00
		char	_type2;				//CBR MP3s are type 0x01, VBR MP3s are type 0x01, AAC are type 0x00 (type1 and type2 used to be one 2 byte field
		char	_compilation;
		char	_rating;
		__int32	_timeMac;			//MAC时间戳 36
		__int32	_size;				//大小40
		__int32	_longer;			//歌曲时长 单位ms44
		__int32 _nIdInAlbuum;		//专辑中的编号48
		__int32	_nAlbumtrackNum;	//所属专辑的歌曲总数52
		__int32	_issueyear;			//发行年份56
		__int32 _bytespeed;			//比特率60
		__int32 _nSamplingrate;		//采样率64
		__int32 _volume;			//音量68
		__int32 _begintime;			//开始时间72
		__int32 _endtime;			//结束时间76
		__int32	_teststring;		//调弦80
		__int32 _playcount;			//播放次数84
		__int32 _playcount2;		//播放次数 保持初始化值88
		__int32 _lastplaytime;		//最后一次播放时间92
		__int32	_cdid;				//cd编号96
		__int32 _cdNum;				//CD的个数100
		__int32 _nDRMID;			//DRM用户ID104
		__int32 _nAddTime;			//增加时间108
		__int32 _nBoolmarkTime;		//书签时间112
		__int64 _ndbID;				//120
		char _isSelect;				//是否track被选中121
		char _playerSpeed;			//应用程序速率122
		char _bpm[2];				//每分钟节拍数124
		char _artworkcount[2];			//插图数目125
		char _unknow[2];			//127
		__int32 artworksize;		//插图大小131
		char _unknow2[4];			//135
		__int32 _samplerate;		//抽样率//139
		__int32 _releasetime;		//释放时间143
		char _unknow3[2];
		char _detailflag[2];			//详述标志147
		char _unknow4[8];				//155
		__int32 _skipcount;				//159
		__int32 _lastskipped;			//163
		char _hasartwork;				//164
		char _skipwhenshuffing;
		char _rememberplaybackposition;
		char flag4;					//167
		__int64 _dbid2;				//175
		char _haslyric;				//歌词标志
		char  _isMoving;			//电影标志
		char _cannotplay;			//不能播放标志
		char _unknow5;				//179
		char _unknow6[4];			//183
		__int32 _pregap;			//187
		__int64 _usecount;		//采用个数195
		char _unknow7[4];		//199
		__int32 _postgap;		//203
		char _unknow8[4];		//207
		__int32 _mediaType;		//211
		__int32 _seasonnr;		//215
		__int32 _episodenr;		//219
		__int32 _unknowArray[7];	//247
		__int32 gaplessdata;		//251
		__int32 _unknow9;			//255
		char _gaplessTrackFlag[2];	//257
		char _gaplessAlbumFlag[2];	//259
		char _keep[28];				//286
		__int32 _alumbID;			//291
		__int64 _ID;				//299
		__int32 _size2;			//track大小 303
		__int32 _unknow10;		//=0 307
		char _unknow11[8];		//0x808080808080LL 315
		char _unknow12[4];		//319
		char _unknow13[8];		//327
		char _unknow14[4];		//mediatype为book，为0x00010001，否则为0 331
		char _keep1[20];		//351
		__int32 _mhiiLink;		//355
		__int32 _unknow15;		//359
		__int32 _unknow16;		//363
		__int32 _unknow17;		//367
		static const int _ReadEnd=20;
		static const int _panddingLen1 = 92; //479
		__int32 _artistid;		//483
		char _keep2[16];		//499
		__int32 _composerId;		//503
		static const int _panddingLen = 60; //563
		static const int _ReadEnd1 = 20; //583
	};
	//播放列表列表
	class CMhlp : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nMhodNum;	//个数
		static const int _panddingLen = 80;
	};
	//播放列表信息
	class CMhyp : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nBodylen;		//包长度 12
		__int32 _nMhodNum;		//个数 16
		__int32 _nmhips;		//mhips个数，可以看做tracks个数 20
		char _type;				//type=0 visiable，=1main 21
		char _unknow[3];   //24
		__int32 _timeMAC;		//MAC时间戳 28
		__int64 _playlistID;	//36
		__int32 _unknow1; //40
		char	_mhodcount[2];	//mhod计数42
		char _Podcastflag[2]; //44
		__int32 _nSortOrder; //48
		char _unknow2[32];//80
		char _unknow3[16];//96
		static const int _panddingLen = 44;//140
		static const int _panddingLen1 = 44;
	};
	//播放列表歌曲信息
	class CMhip : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nBodylen;		//包长度
		__int32 _nMhodNum;		//个数
		__int32 _Podcastgroupflag;
		__int32 _Podcastgroupid;
		__int32 _Trackid;
		__int32  _timeMAC;		//MAC时间戳
		__int32 _Podcastgroupref;
		char _unknow[8];
		char _unknow1[8];
		static const int _panddingLen = 24;
	};
	//文本信息结构体头
	class CMhod : public cdbBase
	{
	public:		
		CMhod();
		~CMhod();

		virtual void release();
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		
		__int32 _nBodylen;		//包长度
		__int32 _type;			//类型
		static const int _panddingLen = 8;
		CbaseMhod* _pbody;
	};

	//一般文本情况下body数据结构
	class CMhodBody : public  CbaseMhod
	{
	public:		
		CMhodBody(int type);
		~CMhodBody();

		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		void release();
		__int32 _Code;			//字符串类型，=1表示字符串UTF16类型，=2表示UTF8类型
		__int32 _size;			//字符串长度
		__int32 _unknow;
		__int32 _unknow1;
		wstring _value;		//Body内容
		wstring GetInfo(){return _value;}
	
		
	};
	//MHOD_ID_PLAYLIST情况下，body数据结构
	class CMhodBodyPLAYLIST : public  CbaseMhod
	{
	public:		
		CMhodBodyPLAYLIST(int type);
		~CMhodBodyPLAYLIST();

		void release();
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		wstring GetInfo(){return L"";}
		char _unknow[16];
		char _unknow1[2];
		char _unknow2[2];
		char _unknow3[4];
		char _unknow4[4];
		static const int _panddingLen = 0x237 -0x38 + 1;
		char _unknow5[4];
		static const int _panddingLen1 = 0x287-0x23c+1;
	};
	//102mhod类型，body数据结构
	class CMhodBody102 : public  CbaseMhod
	{
	public:		
		CMhodBody102(int type);
		~CMhodBody102();

		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		void release();
		wstring GetInfo(){return L"";}
		__int32 _unknow;
		__int32 _unknow1;
		__int32 _unknow2;
		__int32 _unknow3;
		static const int _panddingLen = 0x163 -0x28 + 1;
	};
	//未知的文本结构体处理结构
	class CMhNodyUnknow :public  CbaseMhod
	{
	public:
		CMhNodyUnknow(int type,int nLen);
		~CMhNodyUnknow();

		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		void release();

		wstring GetInfo(){return L"";}

		int BodyLen;
		char *pBody;
	};
	//电台内容结构体
/*	class CMhNodyPodCast : public CbaseMhod
	{
		CMhNodyPodCast(int type,int nLen);
		~CMhNodyPodCast();
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		wstring GetInfo(){return _value;}
		wstring _value;
	};*/
	//章节内容结构体
	/*class CMhNodyChapter : public CbaseMhod
	{
		CMhNodyChapter(int type,int nLen);
		~CMhNodyChapter();
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		wstring GetInfo(){return _value[0]->_value;}
		__int32 unknow1;
		__int32 unknow2;
		__int32 unknow3;
		__int32 _size;
		char _atomname[4];
		__int32 startpos;
		__int32 childsNum;
		__int32 unknow4;
		vector<CMhNodyChapterdate*> _value;

	};
	class CMhNodyChapterdate
	{
		__int32 size;
		char _atomname[4];
		__int32 unknow1;
		__int32 unknow2;
		__int32 unknow3;
		wstring _value;
	};

*/
}