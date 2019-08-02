// 
// iTunnes ����/��Ƶ�ļ����ݽṹ��
//
#pragma  once

#include <string>
#include <vector>
#include <map>
#include "AssistantDll.h"
#include "Module/iPod/IosCBDStruct.h"

#include <functional> 
#include "Module/iPod/hashAB.h"

using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
	class cIosPlayListDB;
    class IiPodMediaParser;
	//�洢�����ɵ�64λ���ݿ�ID����ר�����������ƽṹ
	typedef struct STU_CREATE_DBID
	{
		__int64 TrackID;
		__int64 AlbumID;
		__int64 ArtistID;
		__int64 ComposerID;
		__int64 AlbumArtistID;
		int ArtWorkID;
		wstring AlbumArtist;
		wstring DevFilePath;
	}STU_CREATE_DBID;
	//��������������ģ������
	typedef enum eItemListType
	{
		ITUNES_LIST_TRACKS,
		ITUNES_LIST_PLAYLIST,
		ITUNES_LIST_ALBUMS,
		ITUNES_LIST_ARTIST,
	};

	//ר��
	class CLASSINDLL_CLASS_DECL CIosCDBAlbumData
	{
	public:
		int Id() const { return _id; }
		void Id(int val) { _id = val; }
		std::wstring AlbumName() const { return _albumName; }
		void AlbumName(std::wstring val) { _albumName = val; }
		wstring AlbumArtis() const { return _albumArtis; }
		void AlbumArtis(wstring val) { _albumArtis = val; }
		__int64 DBID() const { return _DBID; }
		void DBID(__int64 val) { _DBID = val; }
		std::wstring AlbumSort() const { return _albumSort; }
		void AlbumSort(std::wstring val) { _albumSort = val; }
		//����CDBstruce�б�����ʹ�ù��������ɾ�������޸ı����ݵ�ʱ�����������ܿ����ҵ����޸�
		vector<int> ItemKey;
	private:
		int		_id;								//���		
		wstring _albumName;							//ר������		
		wstring _albumArtis;						//ר��������	
		wstring _albumSort;							//����ʽ		
		__int64 _DBID;								//���ݿ�ID
		__int32 _AlbumArtisID;						//ר��������ID
		__int64 _AlbumArtisDBID;					//ר�����������ݿ�ID

	};

	//ר��������
	class CLASSINDLL_CLASS_DECL CIosCDBAlbumArtistData
	{
	public:
		int Id() const { return _id; }
		void Id(int val) { _id = val; }
		std::wstring Artistname() const { return _artistname; }
		void Artistname(std::wstring val) { _artistname = val; }
		int DBID() const { return _DBID; }
		void DBID(int val) { _DBID = val; }
		//����CDBstruce�б�����ʹ�ù��������ɾ�������޸ı����ݵ�ʱ�����������ܿ����ҵ����޸�
		vector<int> ItemKey;
	private:
		int		_id;							//���		
		wstring _artistname;					//��������
		int _DBID;								//���ݿ�ID		
	};

	//������Ϣ
	class  CLASSINDLL_CLASS_DECL CIosCDBTrackData
	{
	public:		
		//��ø�������ר������
		//Bitmap* GetArtWork();
		std::wstring Title() const { return _title; }
		void Title(std::wstring val) { _title = val; }
		std::wstring Pcpath() const { return _pcpath; }
		void Pcpath(std::wstring val) { _pcpath = val; }
		std::wstring Devicepath() const { return _devicepath; }
		void Devicepath(std::wstring val) { _devicepath = val; }
		int Timer() const { return _timer; }
		void Timer(int val) { _timer = val; }
		int Albumid() const { return _albumid; }
		void Albumid(int val) { _albumid = val; }
		int Id() const { return _id; }
		void Id(int val) { _id = val; }
		int Artist() const { return _artist; }
		void Artist(int val) { _artist = val; }
		bool Lryic() const { return _lryic; }
		void Lryic(bool val) { _lryic = val; }
		wstring Type() const { return _type; }
		void Type(wstring val) { _type = val; }
		__int64 Dbid2() const { return _dbid2; }
		void Dbid2(__int64 val) { _dbid2 = val; }
		__int64 Dbid() const { return _dbid; }
		void Dbid(__int64 val) { _dbid = val; }
		bool Isvisable() const { return _isvisable; }
		void Isvisable(bool val) { _isvisable = val; }
		bool HasArtWork() const { return _hasArtWork; }
		void HasArtWork(bool val) { _hasArtWork = val; }
		int Composer() const { return _Composer; }
		void Composer(int val) { _Composer = val; }
		std::wstring AlbumStr() const { return _AlbumStr; }
		void AlbumStr(std::wstring val) { _AlbumStr = val; }
		std::wstring ArtistStr() const { return _ArtistStr; }
		void ArtistStr(std::wstring val) { _ArtistStr = val; }
		std::wstring ComposerStr() const { return _ComposerStr; }
		void ComposerStr(std::wstring val) { _ComposerStr = val; }
		std::wstring CategoryStr() const { return _CategoryStr; }
		void CategoryStr(std::wstring val) { _CategoryStr = val; }
		std::wstring GroupingStr() const { return _GroupingStr; }
		void GroupingStr(std::wstring val) { _GroupingStr = val; }
		std::wstring DescriptionStr() const { return _DescriptionStr; }
		void DescriptionStr(std::wstring val) { _DescriptionStr = val; }
		std::wstring CommentStr() const { return _CommentStr; }
		void CommentStr(std::wstring val) { _CommentStr = val; }
		std::wstring GenreStr() const { return _GenreStr; }
		void GenreStr(std::wstring val) { _GenreStr = val; }
		std::wstring FiletypeStr() const { return _FiletypeStr; }
		void FiletypeStr(std::wstring val) { _FiletypeStr = val; }
		std::wstring EQSetting() const { return _EQSetting; }
		void EQSetting(std::wstring val) { _EQSetting = val; }

		__int64 AlbumArtistDBId() const { return _AlbumArtistDBId; }
		void AlbumArtistDBId(__int64 val) { _AlbumArtistDBId = val; }
		__int64 ArtistDBId() const { return _ArtistDBId; }
		void ArtistDBId(__int64 val) { _ArtistDBId = val; }
		__int64 AlbumDBId() const { return _AlbumDBId; }
		void AlbumDBId(__int64 val) { _AlbumDBId = val; }
		__int64 ComposerDBId() const { return _ComposerDBId; }
		void ComposerDBId(__int64 val) { _ComposerDBId = val; }
		std::wstring AlbumArtistStr() const { return _AlbumArtistStr; }
		void AlbumArtistStr(std::wstring val) { _AlbumArtistStr = val; }
		std::wstring FileSizeStr() const { return _FileSizeStr; }
		void FileSizeStr(std::wstring val) { _FileSizeStr = val; }
		std::wstring YearStr() const { return _YearStr; }
		void YearStr(std::wstring val) { _YearStr = val; }
		bool IsVideo() const { return _isVideo; }
		void IsVideo(bool val) { _isVideo = val; }

		vector<int> ItemKey;
	private:
		wstring _title;					//��������			
		wstring _pcpath;				//�����ڵ����ϵĴ��·��		
		wstring _devicepath;			//�������豸�ϵĴ��·��	
		wstring _AlbumStr;				//ר������		
		wstring _ArtistStr;				//����������		
		wstring _ComposerStr;			//����������		
		wstring _CategoryStr;			//����		
		wstring _GroupingStr;			//Ⱥ��		
		wstring _DescriptionStr;		//����		
		wstring _CommentStr;			//ע��		
		wstring _GenreStr;				//�������		
		wstring	_FiletypeStr;			//�ļ�����		
		wstring _EQSetting;				//������
		wstring _AlbumArtistStr;		//ר��������
		wstring _FileSizeStr;			//�ļ���С
		wstring _YearStr;				//���
		//wstring _PodcastEnclosureURL;	//��̨��ַ
		//wstring _PodcastRSSURL;			//��̨rss��ַ
		//wstring _ChapterStr;			//�½�(��Ƶ����)
		int _timer;						//����ʱ��		
		int _albumid;					//����ר��ID			
		int _id;						//���		
		int  _artist;					//������	
		int _Composer;					//����
		bool _lryic;				//�Ƿ��и��		
		wstring _type;			//�ļ�����		
		bool _hasArtWork;				//�Ƿ��з���		
		bool _isvisable;				//�Ƿ�ɼ�		
		bool _isVideo;

		__int64 _dbid;						//���ݿ�ID		
		__int64 _dbid2;						//���ݿ��ӦID2	
		__int64 _ComposerDBId;				//�������ݿ�ID
		__int64 _AlbumDBId;					//ר�����ݿ�ID
		__int64 _ArtistDBId;				//���������ݿ�ID		
		__int64 _AlbumArtistDBId;			//ר�����������ݿ�ID
	};

	//����������
	class CLASSINDLL_CLASS_DECL CIosCDBTrackArtist
	{
	public:
		
		int Id() const { return _id; }
		void Id(int val) { _id = val; }
		std::wstring Artistname() const { return _artistname; }
		void Artistname(std::wstring val) { _artistname = val; }
	//	vector<int> Traklist() const { return _traklist; }
	//	void Traklist(vector<int> val) { _traklist = val; }
		__int64 Dbid() const { return _dbid; }
		void Dbid(__int64 val) { _dbid = val; }
		//����CDBstruce�б�����ʹ�ù��������ɾ�������޸ı����ݵ�ʱ�����������ܿ����ҵ����޸�
		vector<int> ItemKey;
	private:
		int		_id;					//������		
		wstring _artistname;			//��������		
	//	vector<int>	_traklist;			//ӵ�е���Ʒ�б�	
		__int64 _dbid;					//���ݿ�ID
		
	};

	//�����б�
	class CLASSINDLL_CLASS_DECL CIosCDBPlayList
	{
	public:
		//�����б�����
		std::wstring Listname() const { return _listname; }
		//�����б�����
		void Listname(std::wstring val) { _listname = val; }
		//�����ĸ���
		const vector<__int64>* TrackList() const { return &_trackList; }
		__int64 GetTrackPid(int pos){return _trackList.at(pos);}
		//���Ӹ���
		void PushTrackid(__int64 pid){_trackList.push_back(pid);}
		//ɾ������
		void EraseTrackid(int pos){_trackList.erase(_trackList.begin() + pos);}
		//�����ĸ���
		//void TrackList(vector<__int64> val) { _trackList = val; }
		//�����б�ID
		__int64 ID() const { return _ID; }
		//�����б�ID
		void ID(__int64 val) { _ID = val; }
		//����CDBstruce�б�����ʹ�ù��������ɾ�������޸ı����ݵ�ʱ�����������ܿ����ҵ����޸�
		vector<int> ItemKey;
		//plist�ļ�·��
		std::wstring Plistpath() const { return plistpath; }
		//plist�ļ�·��
		void Plistpath(std::wstring val) { plistpath = val; }
	private:
		
		wstring _listname;					//�б�����		
		vector<__int64> _trackList;				//ӵ����Ʒ�б�	
		wstring plistpath;		
		__int64 _ID;							
	};

		//iPod�ļ�����ӿ�
	class CLASSINDLL_CLASS_DECL CIosiPodData
	{
		 friend class ciPodManager;
		 friend class ciPodManager5;
	public:
		CIosiPodData();
		~CIosiPodData();

	private:
		//************************************
		// Method:    ����cdb�ļ�
		// Returns:   void
		// Parameter: wstring CDBfilePath �ļ�·��
		//************************************
		void LoadCDBDataFromFile(wstring CDBfilePath,cIosPlayListDB* pIosPlayListDB);
		//************************************
		// Method:    ����cdb�ļ� 
		// Returns:   void
		// Parameter: wstring CDBfilePath ·��
		//************************************
		void SaveCDBDataToFile(wstring CDBfilePath);		
	
		//************************************
		// Method:    ����һ�������ļ�
		// Returns:   CIosCDBTrackData����
		// Parameter:wstring  filename ·��
		//************************************
		CIosCDBTrackData* InsertTrack(wstring filename,IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb);

		//************************************
		// Method:    �Ƴ�һ�������ļ�
		// Returns:   void
		// Parameter: CIosCDBTrackData * ptrackData ָ�������ļ�����ָ��
		//************************************
		void RemoveTrack(CIosCDBTrackData * ptrackData);

		//************************************
		// Method:    �޸�һ�������ļ���Ϣ
		// Returns:   void
		// Parameter: CIosCDBTrackData * ptrackData ָ�������ļ�����ָ��
		//************************************
		void ModifyTrack(CIosCDBTrackData * ptrackData,STU_CREATE_DBID * pstudb);
	
		//����mhsdͷ
		CMhsd* CreateMhsdHead(int type,int bodylen);
		//���������ϴ��ֻ���ʱ����Ҫ�����ֻ��������ļ���
		void UpdateDeviceMusicPath();
		//�����ݿ�õ���Ϣ
		wstring getInfoFromSqlDB(int infotype);

		//************************************
		// Method:    �������32λID
		// Returns:   __int32 ID��
		//************************************
		__int32 Create32BitID();

		//************************************
		// Method:    ����һ���µ�ר��
		// Returns:   int ר��ID
		//************************************
		int CreateNewAlbum(wstring AlbumName,STU_CREATE_DBID * pstudb);
		//************************************
		// Method:    ����һ��ר��������
		// Returns:   int
		//************************************
		int CreateNewArtist(wstring ArtistName,STU_CREATE_DBID * pstudb);
		//************************************
		// Method:    ����&��ӱ�����ID
		// Returns:   int
		// Parameter: wstring ArtistName �ݳ���		
		//************************************
		int FindArtistID(wstring ArtistName,STU_CREATE_DBID * pstudb);
		//************************************
		// Method:    ����&���ר��
		// Returns:   int
		// Parameter: wstring AlbumName ר����
		//************************************
		int FindAlbumID(wstring AlbumName,STU_CREATE_DBID * pstudb);
		//************************************
		// Method:    �������Mhod����
		// Returns:   CbaseMhod* Mhodָ��
		// Parameter: int type Mhod����
		// Parameter: wstring body ���������
		//************************************
		CMhod* AddMhod(int type,wstring body);

		//************************************
		// Method:    ����&�������
		// Returns:   int
		// Parameter: wstring ComposerName
		//************************************
		int FindComposerID(wstring ComposerName,STU_CREATE_DBID * pstudb);

		//************************************
		// Method:    ���������б�
		// Returns:   void
		// Parameter: BinaryBufferReader * pReader �����ƶ�ָ��
		//Parameter: _playlist �б�
		//************************************
		void ReadPlayList(BinaryBufferReader *pReader,vector<CIosCDBPlayList*>* _playlist);
		//************************************
		// Method:    ����ר���б�
		// Returns:   void
		// Parameter: BinaryBufferReader * pReader �����ƶ�ָ��
		//************************************
		void ReadAlbums(BinaryBufferReader *pReader);
		//************************************
		// Method:    ���������б�
		// Returns:   void
		// Parameter: BinaryBufferReader * pReader �����ƶ�ָ��
		//************************************
		void ReadTracks(BinaryBufferReader *pReader,vector<CIosCDBTrackData*>* alltracklist);
		//************************************
		// Method:    �����������б�
		// Returns:   void
		// Parameter: BinaryBufferReader * pReader �����ƶ�ָ��
		//************************************
		void ReadArtist(BinaryBufferReader *pReader);
		//ɾ��track��Ӧ��ר��
		void DeleteAlbum(int Albumid);
		//ɾ��track��Ӧ�ı�����
		void DeleteArtist(int ArtistID);
		//ɾ��track��Ӧ�ĸ���
		void DeleteTrack(int trackId);
		//�޸�Mhod����
		int ChangeMhodBody(CMhod* pMhod,wstring Newvalue);
		//���ݸ���idȡ�����ݿ�id
		__int64 GetTrackDbidByid(int id);

		static int zlib_inflate( unsigned char * outbuf, const unsigned char *zdata, unsigned int compressed_size, unsigned int * uncompressed_size );
		void __uncompress_itdb( const char * pSrc, int iSrcSize, char ** ppDest, int * piDestSize );
		void __compress_itdb(const char* srcbody,int size,char ** ppDes,int * retsize);
		//���Ӳ����б�
		void AddPlayList(wstring playListName,__int64 pid);
		//ɾ�������б�
		void RemovePlayList(__int64 PlayListID);
		//�����б�ɾ������
		void RemoveTrackFromPlayList(__int64 PlayListID,__int64 trackID);
		//��Ӳ����б�����
		void AddTrackToPlayList(__int64 PlayListID,__int64 trackID);
		//����pid���id
		int GetTrackIdByPid(__int64 PID);
		//�޸Ĳ����б���
		void ModifyPlayList(__int64 PlayListID,wstring NewName);
		//���¼��ز����б�
		void ReLoadPlayListDB(cIosPlayListDB* pIosPlayListDB);
	private:
		vector<CIosCDBAlbumData*> _albumlist;		//ר���б�
		vector<CIosCDBTrackData*> _alltracklist;	//���и����б�
		vector<CIosCDBTrackArtist*> _artist;		//�������б�
		vector<CIosCDBPlayList*>	_playlist1;		//�����б��б�
		vector<CIosCDBPlayList*>	_playlist2;		//�����б��б�
		vector<CIosCDBPlayList*>	_playlist3;		//�����б��б�
		vector<CIosCDBPlayList*>	_playlistDB;		//�����б��б�
		vector<CIosCDBTrackData*>	_alltracklist1;		//λ�ø����б�

	public:
		//��������
		size_t GetTrackCount() const{return _alltracklist.size();}
		//��õ�����������
		CIosCDBTrackData* GetTrackData(size_t pos) const{return pos>=_alltracklist.size()?NULL:_alltracklist[pos];}
		//ר������
		size_t GetAlbumCount() const{return _albumlist.size();}
		//����ר������
		CIosCDBAlbumData* GetAlbumData (size_t pos) const{return pos>=_albumlist.size()?NULL:_albumlist[pos];}
		//�����Ҹ���
		size_t GetArtistCount() const{return _artist.size();}
		//�������������� 
		CIosCDBTrackArtist* GetArtistData(size_t pos) const{return pos>=_artist.size()?NULL:_artist[pos];}
		//�����б�1����
		size_t GetPlayList1Count() const{return _playlist1.size();}
		//���������б�1����
		CIosCDBPlayList* GetPlayList1Data (size_t pos) const{return pos>=_playlist1.size()?NULL:_playlist1[pos];}
		//�����б�2����
		size_t GetPlayList2Count() const{return _playlist2.size();}
		//���������б�2����
		CIosCDBPlayList* GetPlayList2Data (size_t pos) const{return pos>=_playlist2.size()?NULL:_playlist2[pos];}
		//�����б�3����
		size_t GetPlayList3Count() const{return _playlist3.size();}
		//���������б�3����
		CIosCDBPlayList* GetPlayList3Data(size_t pos) const{return pos>=_playlist3.size()?NULL:_playlist3[pos];}
		//���ݿⲥ���б����
		size_t GetPlayListDBCount() const{return _playlistDB.size();}
		//�������ݿⲥ���б�����
		CIosCDBPlayList* GetPlayListDBData(size_t pos) const{return pos>=_playlistDB.size()?NULL:_playlistDB[pos];}
		//����64λ���ݿ�id�ҵ���������
		CIosCDBTrackData* FindTrackDataByID64(__int64 pid);
		
	private:		
		int m_HashType;
		map<int,cdbBase*>	_AllItem;				//�������ж������ļ��ṹ����Ա����д���ļ�
		CMhbd  m_Mhbd;			//ѹ��������ļ�ͷ		
		map<int,int> _MhsdList;		//Mhsdѭ���ṹ���һ�㱣��λ��	

		unsigned char SrcParam1[16];//����ԭʼ��У����Ϣ
		unsigned char SrcParam[12];

		HashAB _hashab;
	};

	class CLASSINDLL_CLASS_DECL TrackIDEqu:public unary_function<CIosCDBTrackData*,bool>
	{
	public:
		__int64 ID;
		explicit   TrackIDEqu(const __int64 s):ID(s){}
		bool     operator   ()(const CIosCDBTrackData* e){return e->Dbid()==ID;}
	};
}