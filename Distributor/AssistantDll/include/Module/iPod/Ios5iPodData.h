#pragma  once
#include "AssistantDll.h"
#include "IosiPodData.h"
#pragma warning(disable : 4251)
using namespace std;
namespace ND91Assistant
{
	//ipod5���ֹ���������
	class CLASSINDLL_CLASS_DECL cIPodData5 
	{
	public:
			cIPodData5(wstring DBFilePath);
			~cIPodData5();
	public:
		//��������
		size_t GetTrackCount() const{return _alltracklist.size();}
		//��õ�����������
		CIosCDBTrackData* GetTrackData(size_t pos) const{return pos>=_alltracklist.size()?NULL:_alltracklist[pos];}
		//�����б�1����
		size_t GetPlayListCount() const{return _playlist.size();}
		//���������б�1����
		CIosCDBPlayList* GetPlayListData (size_t pos) const{return pos>=_playlist.size()?NULL:_playlist[pos];}
		
	private:
			void LoadData();
			 void GetAllPlayList5(vector<CIosCDBPlayList*>* pVecPlayList);
	private:
			vector<CIosCDBTrackData*> _alltracklist;	//���и����б�
			vector<CIosCDBPlayList*>	_playlist;		//�����б��б�
			wstring _DBFilePath;			
	};
}