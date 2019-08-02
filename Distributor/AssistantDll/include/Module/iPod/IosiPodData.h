// 
// iTunnes 音乐/视频文件数据结构类
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
	//存储新生成的64位数据库ID，跟专辑表演者名称结构
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
	//数据项所属数据模块类型
	typedef enum eItemListType
	{
		ITUNES_LIST_TRACKS,
		ITUNES_LIST_PLAYLIST,
		ITUNES_LIST_ALBUMS,
		ITUNES_LIST_ARTIST,
	};

	//专辑
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
		//保存CDBstruce中本数据使用过的子项，当删除或者修改本数据的时候所有子项能快速找到并修改
		vector<int> ItemKey;
	private:
		int		_id;								//序号		
		wstring _albumName;							//专辑名称		
		wstring _albumArtis;						//专辑表演者	
		wstring _albumSort;							//排序方式		
		__int64 _DBID;								//数据库ID
		__int32 _AlbumArtisID;						//专辑表演者ID
		__int64 _AlbumArtisDBID;					//专辑表演者数据库ID

	};

	//专辑表演者
	class CLASSINDLL_CLASS_DECL CIosCDBAlbumArtistData
	{
	public:
		int Id() const { return _id; }
		void Id(int val) { _id = val; }
		std::wstring Artistname() const { return _artistname; }
		void Artistname(std::wstring val) { _artistname = val; }
		int DBID() const { return _DBID; }
		void DBID(int val) { _DBID = val; }
		//保存CDBstruce中本数据使用过的子项，当删除或者修改本数据的时候所有子项能快速找到并修改
		vector<int> ItemKey;
	private:
		int		_id;							//序号		
		wstring _artistname;					//艺术家名
		int _DBID;								//数据库ID		
	};

	//歌曲信息
	class  CLASSINDLL_CLASS_DECL CIosCDBTrackData
	{
	public:		
		//获得歌曲所属专辑封面
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
		wstring _title;					//歌曲标题			
		wstring _pcpath;				//歌曲在电脑上的存放路径		
		wstring _devicepath;			//歌曲在设备上的存放路径	
		wstring _AlbumStr;				//专辑名称		
		wstring _ArtistStr;				//表演者名称		
		wstring _ComposerStr;			//作曲者名称		
		wstring _CategoryStr;			//分类		
		wstring _GroupingStr;			//群组		
		wstring _DescriptionStr;		//描述		
		wstring _CommentStr;			//注释		
		wstring _GenreStr;				//歌曲风格		
		wstring	_FiletypeStr;			//文件类型		
		wstring _EQSetting;				//均衡器
		wstring _AlbumArtistStr;		//专辑表演者
		wstring _FileSizeStr;			//文件大小
		wstring _YearStr;				//年份
		//wstring _PodcastEnclosureURL;	//电台地址
		//wstring _PodcastRSSURL;			//电台rss地址
		//wstring _ChapterStr;			//章节(视频类型)
		int _timer;						//播放时长		
		int _albumid;					//所属专辑ID			
		int _id;						//序号		
		int  _artist;					//艺术家	
		int _Composer;					//作曲
		bool _lryic;				//是否有歌词		
		wstring _type;			//文件类型		
		bool _hasArtWork;				//是否有封面		
		bool _isvisable;				//是否可见		
		bool _isVideo;

		__int64 _dbid;						//数据库ID		
		__int64 _dbid2;						//数据库对应ID2	
		__int64 _ComposerDBId;				//作曲数据库ID
		__int64 _AlbumDBId;					//专辑数据库ID
		__int64 _ArtistDBId;				//表演者数据库ID		
		__int64 _AlbumArtistDBId;			//专辑表演者数据库ID
	};

	//歌曲表演者
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
		//保存CDBstruce中本数据使用过的子项，当删除或者修改本数据的时候所有子项能快速找到并修改
		vector<int> ItemKey;
	private:
		int		_id;					//存放序号		
		wstring _artistname;			//艺术家名		
	//	vector<int>	_traklist;			//拥有的作品列表	
		__int64 _dbid;					//数据库ID
		
	};

	//播放列表
	class CLASSINDLL_CLASS_DECL CIosCDBPlayList
	{
	public:
		//播放列表名称
		std::wstring Listname() const { return _listname; }
		//播放列表名称
		void Listname(std::wstring val) { _listname = val; }
		//包含的歌曲
		const vector<__int64>* TrackList() const { return &_trackList; }
		__int64 GetTrackPid(int pos){return _trackList.at(pos);}
		//增加歌曲
		void PushTrackid(__int64 pid){_trackList.push_back(pid);}
		//删除歌曲
		void EraseTrackid(int pos){_trackList.erase(_trackList.begin() + pos);}
		//包含的歌曲
		//void TrackList(vector<__int64> val) { _trackList = val; }
		//播放列表ID
		__int64 ID() const { return _ID; }
		//播放列表ID
		void ID(__int64 val) { _ID = val; }
		//保存CDBstruce中本数据使用过的子项，当删除或者修改本数据的时候所有子项能快速找到并修改
		vector<int> ItemKey;
		//plist文件路径
		std::wstring Plistpath() const { return plistpath; }
		//plist文件路径
		void Plistpath(std::wstring val) { plistpath = val; }
	private:
		
		wstring _listname;					//列表名称		
		vector<__int64> _trackList;				//拥有作品列表	
		wstring plistpath;		
		__int64 _ID;							
	};

		//iPod文件管理接口
	class CLASSINDLL_CLASS_DECL CIosiPodData
	{
		 friend class ciPodManager;
		 friend class ciPodManager5;
	public:
		CIosiPodData();
		~CIosiPodData();

	private:
		//************************************
		// Method:    加载cdb文件
		// Returns:   void
		// Parameter: wstring CDBfilePath 文件路径
		//************************************
		void LoadCDBDataFromFile(wstring CDBfilePath,cIosPlayListDB* pIosPlayListDB);
		//************************************
		// Method:    保存cdb文件 
		// Returns:   void
		// Parameter: wstring CDBfilePath 路径
		//************************************
		void SaveCDBDataToFile(wstring CDBfilePath);		
	
		//************************************
		// Method:    增加一个音乐文件
		// Returns:   CIosCDBTrackData对象
		// Parameter:wstring  filename 路径
		//************************************
		CIosCDBTrackData* InsertTrack(wstring filename,IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb);

		//************************************
		// Method:    移除一个音乐文件
		// Returns:   void
		// Parameter: CIosCDBTrackData * ptrackData 指向音乐文件数据指针
		//************************************
		void RemoveTrack(CIosCDBTrackData * ptrackData);

		//************************************
		// Method:    修改一个音乐文件信息
		// Returns:   void
		// Parameter: CIosCDBTrackData * ptrackData 指向音乐文件数据指针
		//************************************
		void ModifyTrack(CIosCDBTrackData * ptrackData,STU_CREATE_DBID * pstudb);
	
		//增加mhsd头
		CMhsd* CreateMhsdHead(int type,int bodylen);
		//新增音乐上传手机的时候需要更新手机上音乐文件名
		void UpdateDeviceMusicPath();
		//从数据库得到信息
		wstring getInfoFromSqlDB(int infotype);

		//************************************
		// Method:    随机生成32位ID
		// Returns:   __int32 ID号
		//************************************
		__int32 Create32BitID();

		//************************************
		// Method:    创建一个新的专辑
		// Returns:   int 专辑ID
		//************************************
		int CreateNewAlbum(wstring AlbumName,STU_CREATE_DBID * pstudb);
		//************************************
		// Method:    创建一个专辑表演者
		// Returns:   int
		//************************************
		int CreateNewArtist(wstring ArtistName,STU_CREATE_DBID * pstudb);
		//************************************
		// Method:    查找&添加表演者ID
		// Returns:   int
		// Parameter: wstring ArtistName 演唱者		
		//************************************
		int FindArtistID(wstring ArtistName,STU_CREATE_DBID * pstudb);
		//************************************
		// Method:    查找&添加专辑
		// Returns:   int
		// Parameter: wstring AlbumName 专辑名
		//************************************
		int FindAlbumID(wstring AlbumName,STU_CREATE_DBID * pstudb);
		//************************************
		// Method:    生成添加Mhod对象
		// Returns:   CbaseMhod* Mhod指针
		// Parameter: int type Mhod类型
		// Parameter: wstring body 保存的数据
		//************************************
		CMhod* AddMhod(int type,wstring body);

		//************************************
		// Method:    查找&添加作曲
		// Returns:   int
		// Parameter: wstring ComposerName
		//************************************
		int FindComposerID(wstring ComposerName,STU_CREATE_DBID * pstudb);

		//************************************
		// Method:    读出播放列表
		// Returns:   void
		// Parameter: BinaryBufferReader * pReader 二进制读指针
		//Parameter: _playlist 列表
		//************************************
		void ReadPlayList(BinaryBufferReader *pReader,vector<CIosCDBPlayList*>* _playlist);
		//************************************
		// Method:    读出专辑列表
		// Returns:   void
		// Parameter: BinaryBufferReader * pReader 二进制读指针
		//************************************
		void ReadAlbums(BinaryBufferReader *pReader);
		//************************************
		// Method:    读出歌曲列表
		// Returns:   void
		// Parameter: BinaryBufferReader * pReader 二进制读指针
		//************************************
		void ReadTracks(BinaryBufferReader *pReader,vector<CIosCDBTrackData*>* alltracklist);
		//************************************
		// Method:    读出表演者列表
		// Returns:   void
		// Parameter: BinaryBufferReader * pReader 二进制读指针
		//************************************
		void ReadArtist(BinaryBufferReader *pReader);
		//删除track对应的专辑
		void DeleteAlbum(int Albumid);
		//删除track对应的表演者
		void DeleteArtist(int ArtistID);
		//删除track对应的歌曲
		void DeleteTrack(int trackId);
		//修改Mhod内容
		int ChangeMhodBody(CMhod* pMhod,wstring Newvalue);
		//根据歌曲id取得数据库id
		__int64 GetTrackDbidByid(int id);

		static int zlib_inflate( unsigned char * outbuf, const unsigned char *zdata, unsigned int compressed_size, unsigned int * uncompressed_size );
		void __uncompress_itdb( const char * pSrc, int iSrcSize, char ** ppDest, int * piDestSize );
		void __compress_itdb(const char* srcbody,int size,char ** ppDes,int * retsize);
		//增加播放列表
		void AddPlayList(wstring playListName,__int64 pid);
		//删除播放列表
		void RemovePlayList(__int64 PlayListID);
		//播放列表删除歌曲
		void RemoveTrackFromPlayList(__int64 PlayListID,__int64 trackID);
		//添加播放列表到歌曲
		void AddTrackToPlayList(__int64 PlayListID,__int64 trackID);
		//根据pid获得id
		int GetTrackIdByPid(__int64 PID);
		//修改播放列表名
		void ModifyPlayList(__int64 PlayListID,wstring NewName);
		//重新加载播放列表
		void ReLoadPlayListDB(cIosPlayListDB* pIosPlayListDB);
	private:
		vector<CIosCDBAlbumData*> _albumlist;		//专辑列表
		vector<CIosCDBTrackData*> _alltracklist;	//所有歌曲列表
		vector<CIosCDBTrackArtist*> _artist;		//表演者列表
		vector<CIosCDBPlayList*>	_playlist1;		//播放列表列表
		vector<CIosCDBPlayList*>	_playlist2;		//播放列表列表
		vector<CIosCDBPlayList*>	_playlist3;		//播放列表列表
		vector<CIosCDBPlayList*>	_playlistDB;		//播放列表列表
		vector<CIosCDBTrackData*>	_alltracklist1;		//位置歌曲列表

	public:
		//歌曲个数
		size_t GetTrackCount() const{return _alltracklist.size();}
		//获得单个歌曲数据
		CIosCDBTrackData* GetTrackData(size_t pos) const{return pos>=_alltracklist.size()?NULL:_alltracklist[pos];}
		//专辑个数
		size_t GetAlbumCount() const{return _albumlist.size();}
		//单个专辑数据
		CIosCDBAlbumData* GetAlbumData (size_t pos) const{return pos>=_albumlist.size()?NULL:_albumlist[pos];}
		//艺术家个数
		size_t GetArtistCount() const{return _artist.size();}
		//单个艺术家数据 
		CIosCDBTrackArtist* GetArtistData(size_t pos) const{return pos>=_artist.size()?NULL:_artist[pos];}
		//播放列表1个数
		size_t GetPlayList1Count() const{return _playlist1.size();}
		//单个播放列表1数据
		CIosCDBPlayList* GetPlayList1Data (size_t pos) const{return pos>=_playlist1.size()?NULL:_playlist1[pos];}
		//播放列表2个数
		size_t GetPlayList2Count() const{return _playlist2.size();}
		//单个播放列表2数据
		CIosCDBPlayList* GetPlayList2Data (size_t pos) const{return pos>=_playlist2.size()?NULL:_playlist2[pos];}
		//播放列表3个数
		size_t GetPlayList3Count() const{return _playlist3.size();}
		//单个播放列表3数据
		CIosCDBPlayList* GetPlayList3Data(size_t pos) const{return pos>=_playlist3.size()?NULL:_playlist3[pos];}
		//数据库播放列表个数
		size_t GetPlayListDBCount() const{return _playlistDB.size();}
		//单个数据库播放列表数据
		CIosCDBPlayList* GetPlayListDBData(size_t pos) const{return pos>=_playlistDB.size()?NULL:_playlistDB[pos];}
		//根据64位数据库id找到歌曲数据
		CIosCDBTrackData* FindTrackDataByID64(__int64 pid);
		
	private:		
		int m_HashType;
		map<int,cdbBase*>	_AllItem;				//保存所有读出的文件结构子项，以便后面写入文件
		CMhbd  m_Mhbd;			//压缩包外的文件头		
		map<int,int> _MhsdList;		//Mhsd循环结构体第一层保存位置	

		unsigned char SrcParam1[16];//保存原始的校验信息
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