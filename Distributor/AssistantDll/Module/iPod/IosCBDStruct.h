// 
// iTunnes ����/��Ƶ�ļ����ݽṹ��
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
		MHOD_ID_DESCRIPTION = 14,// ���� Podcast Enclosure URL. Note: this is either a UTF-8 or ASCII encoded string (NOT UTF-16). Also, there is no mhod::length value for this type. Introduced in db version 0x0d.
		MHOD_ID_PODCASTURL = 15,// ���� Podcast RSS URL. Note: this is either a UTF-8 or ASCII encoded string (NOT UTF-16). Also, there is no mhod::length value for this type. Introduced in db version 0x0d.
		MHOD_ID_PODCASTRSS = 16,// ���� Chapter data. This is a m4a-style entry that is used to display subsongs within a mhit. Introduced in db version 0x0d.
		MHOD_ID_CHAPTERDATA = 17,// ���� Subtitle (usually the same as Description). Introduced in db version 0x0d.
		MHOD_ID_SUBTITLE = 18,// ���� Show (for TV Shows only). Introduced in db version 0x0d?
		MHOD_ID_TVSHOW = 19,// ���� Episode # (for TV Shows only). Introduced in db version 0x0d?
		MHOD_ID_TVEPISODE = 20,// ���� TV Network (for TV Shows only). Introduced in db version 0x0d?
		MHOD_ID_TVNETWORK = 21,// ���� Album Artist. Introduced in db version 0x13?
		MHOD_ID_ALBUMARTIST = 22,// ���� Sort key for artist.e
		MHOD_ID_SORT_ARTIST = 23,// ���� Appears to be a list of keywords pertaining to a track. Introduced in db version 0x13?
		MHOD_ID_KEYWORDS = 24,// ���� more sort keys, taking precedence over the standard entries if present
		MHOD_ID_SORT_TITLE = 27,
		MHOD_ID_SORT_ALBUM = 28,
		MHOD_ID_SORT_ALBUMARTIST = 29,
		MHOD_ID_SORT_COMPOSER = 30,
		MHOD_ID_SORT_TVSHOW = 31,
		MHOD_ID_SPLPREF = 50,// ���� settings for smart playlist
		MHOD_ID_SPLRULES = 51,// ���� rules for smart playlist
		MHOD_ID_LIBPLAYLISTINDEX = 52,// ���� Library Playlist Index
		MHOD_ID_LIBPLAYLISTJUMPTABLE=53,
		MHOD_ID_PLAYLIST = 100,
		MHOD_ID_ALBUM_ALBUM = 200,// ���� MHODs for the MHIAs
		MHOD_ID_ALBUM_ARTIST = 201,
		MHOD_ID_ALBUM_SORT_ARTIST = 202,
		MHOD_ID_ALBUM_ARTIST_MHII = 300,
	};


	//�ļ�ͷ���ṹ
	class cdbbasehead
	{
	public:
		cdbbasehead();
		void Read(BinaryBufferReader* pReader,const char* pName);
		void Write(BinaryBufferWriter* pWrite);
		char    _name[5];   // ����: 4��ASCII�ַ�
		__int32     _headSize;  // ͷ������
	};
	//cdb�ļ��ṹ����
	class CLASSINDLL_CLASS_DECL cdbBase
	{
	public:	
		cdbBase();
		~cdbBase();
		virtual	void Read(BinaryBufferReader* pReader)=0;
		virtual void Write(BinaryBufferWriter* pWrite)=0;
		virtual void release() = 0;
		cdbbasehead _head;              // ͷ��			
	};

	//CDB����ı���������CMhod���������
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
	//����CDB�ļ���ͷ��δѹ�����֣�
	class CLASSINDLL_CLASS_DECL CMhbd : public cdbBase
	{
		public:		
			CMhbd(){_nItemNum=0;}
			void Read(BinaryBufferReader* pReader);
			void Write(BinaryBufferWriter* pWrite);
			virtual void release();
		
			__int32 _nOffsetCompress;		//ѹ������λ��
			__int32 _nIsCompress;			//�Ƿ�ѹ�����û��Ϊ1����Ϊ2
			__int32 _niTunesVersion;		//iTunes�汾��
			__int32	_nItemNum;				//����Ŀ����
			__int64	_nItdID;				//CDB�ڲ�ID
			char	_OSItunes[2];			//itunesƽ̨����1-mac 2-windows
			char	_unknow1[2];
			__int64	_nItunesLibID;			//���ID��iTunesPrefs�ļ��е�IDһ�£����ID��ͬ���й����ID��iTunesPrefs�ļ��е�IDһ�£����ID��ͬ���й�
			char	_unknow2[4];
			char	hashtype[2];			//��������hash���ͣ�1 hash58 2 hash72, 3 hashAB����hashΪ0��δ֪=-1
			char	_unknow3[20];
			char	_language[2];			//����
			__int64	_pid;					//PID����sqlite�е�db_info db_pidһ��
			__int32		_TrackFlg;			//δ֪������Trackʱ=1��û�е�ʱ��=0
			__int32		_unknowflg;			//������޼�¼ʱ=0������Track�󣬱�Ϊ0x9F
			char	_hash58[20];			//
			__int32		_timeZone;			//��λs
			char	_hashtype[2];			//��ʾhashУ�鷽ʽ 4��ʾhashA 2��ʾhash72 0��ʾ���� iPod Touch4����hash72У��
			char	_hash72[46];			//Hash72У��
			char	_audiolang[2];			//��Ƶ����
			char	_subtitlelang[2];		//��Ŀ����
			char	_unknow4[7];			//0x00 19 00 0a 00 01 00
			char	_hashAB[57];			//hashABУ��
			char	_unknow5[8];
			static const int _panddingLen = 8;			//dummy space
	};
	//CDB�ļ�����δ֪�����ݴ���ṹ
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
	//CDB�ļ�ѭ����ͷ��
	class CMhsd : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		

		__int32 _nBodylen;  //������
		__int32	_type;		//1 ���� track 2 ���� playlists 3 ���� playlists 4 ���� albums 5 ���� playlists 8 ���� artists 9 ���� genius_cuid
		static const int _panddingLen = 80;
	};
	//ר���б�ṹ��
	class CMhla : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
				
		__int32 _nAlbumsNum;	//ר������
		static const int _panddingLen = 80;
	};
	//ר����Ϣ�ṹ��
	class CMhia : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nBodylen;  //������
		__int32 _nTrackNum;	//����
		__int32 _nAlbumid;	//album id����iTunes�ڲ�ID
		__int64 _nAlbumDBid; //Album id����sqlitedb��pid
		__int32 _unknow;	
		char _unknow1[8];   
		char _unknow2[8]; 
		static const int _panddingLen = 40;
	};
	//ר���������б�ṹ��
	class CMhli : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		virtual void release();
		void Write(BinaryBufferWriter* pWrite);
	
		__int32 _nArtistsNum;	//artists��������ָAlbum Artists�ĸ���
		static const int _panddingLen = 80;
	};
	//ר����������Ϣ�ṹ��
	class CMhii : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
	
		__int32 _nBodylen;		//������
		__int32 _nTrackNum;		//����
		__int32 _nArtistid;		// Artist id����iTunes�ڲ�ID
		__int64 _nArtistDBid;	// Artist id����sqlitedb��pid
		__int32 _unknow;
		static const int _panddingLen = 48;		
	};
	//�����б�ṹ��
	class CMhlt : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		virtual void release();
		void Write(BinaryBufferWriter* pWrite);
		
		__int32 _nTracksNum;	//ר������
		static const int _panddingLen = 80;
	};
	//������Ϣ�ṹ��
	class CMhit : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		//************************************
		// Method:    ���̶�ֵ����δֵ֪�ı�������������ӽ������ļ�
		// Returns:   void
		//************************************
		void FillNewStruct();

		__int32 _nBodylen;			//������ 12
		__int32 _nInfoNum;			//���� 16
		__int32 _ntrackid;			//album id����iTunes�ڲ�ID 20
		__int32 _nIsHide;			//track�Ƿ�ɼ�24
		char   _nFileType[4];			//track���ļ����ͱ�ʾ28
		char	_type1;				//CBR MP3s are type 0x00, VBR MP3s are type 0x01, AAC are type 0x00
		char	_type2;				//CBR MP3s are type 0x01, VBR MP3s are type 0x01, AAC are type 0x00 (type1 and type2 used to be one 2 byte field
		char	_compilation;
		char	_rating;
		__int32	_timeMac;			//MACʱ��� 36
		__int32	_size;				//��С40
		__int32	_longer;			//����ʱ�� ��λms44
		__int32 _nIdInAlbuum;		//ר���еı��48
		__int32	_nAlbumtrackNum;	//����ר���ĸ�������52
		__int32	_issueyear;			//�������56
		__int32 _bytespeed;			//������60
		__int32 _nSamplingrate;		//������64
		__int32 _volume;			//����68
		__int32 _begintime;			//��ʼʱ��72
		__int32 _endtime;			//����ʱ��76
		__int32	_teststring;		//����80
		__int32 _playcount;			//���Ŵ���84
		__int32 _playcount2;		//���Ŵ��� ���ֳ�ʼ��ֵ88
		__int32 _lastplaytime;		//���һ�β���ʱ��92
		__int32	_cdid;				//cd���96
		__int32 _cdNum;				//CD�ĸ���100
		__int32 _nDRMID;			//DRM�û�ID104
		__int32 _nAddTime;			//����ʱ��108
		__int32 _nBoolmarkTime;		//��ǩʱ��112
		__int64 _ndbID;				//120
		char _isSelect;				//�Ƿ�track��ѡ��121
		char _playerSpeed;			//Ӧ�ó�������122
		char _bpm[2];				//ÿ���ӽ�����124
		char _artworkcount[2];			//��ͼ��Ŀ125
		char _unknow[2];			//127
		__int32 artworksize;		//��ͼ��С131
		char _unknow2[4];			//135
		__int32 _samplerate;		//������//139
		__int32 _releasetime;		//�ͷ�ʱ��143
		char _unknow3[2];
		char _detailflag[2];			//������־147
		char _unknow4[8];				//155
		__int32 _skipcount;				//159
		__int32 _lastskipped;			//163
		char _hasartwork;				//164
		char _skipwhenshuffing;
		char _rememberplaybackposition;
		char flag4;					//167
		__int64 _dbid2;				//175
		char _haslyric;				//��ʱ�־
		char  _isMoving;			//��Ӱ��־
		char _cannotplay;			//���ܲ��ű�־
		char _unknow5;				//179
		char _unknow6[4];			//183
		__int32 _pregap;			//187
		__int64 _usecount;		//���ø���195
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
		__int32 _size2;			//track��С 303
		__int32 _unknow10;		//=0 307
		char _unknow11[8];		//0x808080808080LL 315
		char _unknow12[4];		//319
		char _unknow13[8];		//327
		char _unknow14[4];		//mediatypeΪbook��Ϊ0x00010001������Ϊ0 331
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
	//�����б��б�
	class CMhlp : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nMhodNum;	//����
		static const int _panddingLen = 80;
	};
	//�����б���Ϣ
	class CMhyp : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nBodylen;		//������ 12
		__int32 _nMhodNum;		//���� 16
		__int32 _nmhips;		//mhips���������Կ���tracks���� 20
		char _type;				//type=0 visiable��=1main 21
		char _unknow[3];   //24
		__int32 _timeMAC;		//MACʱ��� 28
		__int64 _playlistID;	//36
		__int32 _unknow1; //40
		char	_mhodcount[2];	//mhod����42
		char _Podcastflag[2]; //44
		__int32 _nSortOrder; //48
		char _unknow2[32];//80
		char _unknow3[16];//96
		static const int _panddingLen = 44;//140
		static const int _panddingLen1 = 44;
	};
	//�����б������Ϣ
	class CMhip : public cdbBase
	{
	public:		
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		virtual void release();
		
		__int32 _nBodylen;		//������
		__int32 _nMhodNum;		//����
		__int32 _Podcastgroupflag;
		__int32 _Podcastgroupid;
		__int32 _Trackid;
		__int32  _timeMAC;		//MACʱ���
		__int32 _Podcastgroupref;
		char _unknow[8];
		char _unknow1[8];
		static const int _panddingLen = 24;
	};
	//�ı���Ϣ�ṹ��ͷ
	class CMhod : public cdbBase
	{
	public:		
		CMhod();
		~CMhod();

		virtual void release();
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		
		__int32 _nBodylen;		//������
		__int32 _type;			//����
		static const int _panddingLen = 8;
		CbaseMhod* _pbody;
	};

	//һ���ı������body���ݽṹ
	class CMhodBody : public  CbaseMhod
	{
	public:		
		CMhodBody(int type);
		~CMhodBody();

		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		void release();
		__int32 _Code;			//�ַ������ͣ�=1��ʾ�ַ���UTF16���ͣ�=2��ʾUTF8����
		__int32 _size;			//�ַ�������
		__int32 _unknow;
		__int32 _unknow1;
		wstring _value;		//Body����
		wstring GetInfo(){return _value;}
	
		
	};
	//MHOD_ID_PLAYLIST����£�body���ݽṹ
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
	//102mhod���ͣ�body���ݽṹ
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
	//δ֪���ı��ṹ�崦��ṹ
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
	//��̨���ݽṹ��
/*	class CMhNodyPodCast : public CbaseMhod
	{
		CMhNodyPodCast(int type,int nLen);
		~CMhNodyPodCast();
		void Read(BinaryBufferReader* pReader);
		void Write(BinaryBufferWriter* pWrite);
		wstring GetInfo(){return _value;}
		wstring _value;
	};*/
	//�½����ݽṹ��
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