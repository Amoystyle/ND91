#pragma  once
#include "AssistantDll.h"
#include "IosiPodData.h"
#pragma warning(disable : 4251)
using namespace std;
namespace ND91Assistant
{
	//ipod5音乐管理数据类
	class CLASSINDLL_CLASS_DECL cIPodData5 
	{
	public:
			cIPodData5(wstring DBFilePath);
			~cIPodData5();
	public:
		//歌曲个数
		size_t GetTrackCount() const{return _alltracklist.size();}
		//获得单个歌曲数据
		CIosCDBTrackData* GetTrackData(size_t pos) const{return pos>=_alltracklist.size()?NULL:_alltracklist[pos];}
		//播放列表1个数
		size_t GetPlayListCount() const{return _playlist.size();}
		//单个播放列表1数据
		CIosCDBPlayList* GetPlayListData (size_t pos) const{return pos>=_playlist.size()?NULL:_playlist[pos];}
		
	private:
			void LoadData();
			 void GetAllPlayList5(vector<CIosCDBPlayList*>* pVecPlayList);
	private:
			vector<CIosCDBTrackData*> _alltracklist;	//所有歌曲列表
			vector<CIosCDBPlayList*>	_playlist;		//播放列表列表
			wstring _DBFilePath;			
	};
}