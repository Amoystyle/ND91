#pragma  once

#include "AssistantDll.h"
#include <string>
#include <GdiPlus.h>

#include "Module/iPod/IosiPodData.h"
#include "Module/iPod/IosCBDStruct.h"
#include "Module/iPod/iPodDbBase.h"
#include "Module/iPod/PlaylistDB.h"


using namespace std;
using namespace Gdiplus;
namespace ND91Assistant
{
// 	enum
// 	{
// 		Msg_Status_init = 102400,		//���账��
// 		Msg_Status_Begin_Sync,			//���账��
// 		Msg_Status_Syncing,				//���账��
// 		Msg_Status_Require_plist,			//����plist��Ϣ���յ�����Ϣʱ����Ҫ����ND_SendPlistAddr������plist��Ϣ��10s��δ�յ���Ч��Ϣ�������س�ʱ��
// 		Msg_Status_Sync_finish,				//ͬ�����
// 		Msg_Status_Sync_error,				//ͬ������
// 		Msg_Status_Sync_timeout,		//���账��
// 		Msg_Status_Copy_Music,				//��ǰ����������Ƶ�ļ�		e.g. e:\music\18. Secret (����).mp3
// 		Msg_Status_Copy_Music_error,		//��ǰ����������ļ�		e.g. e:\music\18. Secret (����).mp3
// 		Msg_Status_Get_Plist_timeout,		//����plist�ļ���ʱ
// 		Msg_Status_iTunesPrefsID,		//���账��
// 		Msg_Status_dll_load_fail,			//dll����ʧ��			e.g. itunes.dll
// 		Msg_Status_AssetMediaID,		//���账��	
// 		Msg_Status_Copy_Progress,			//��ǰ�������ȣ���KBΪ��λ
// 	};

	//iPod4���ֹ�����
	class CLASSINDLL_CLASS_DECL ciPodManager : public iPodDbBase
	{	
		// iPod������
		//���ݿ������ļ��� ������'\'��β��
	protected:
		ciPodManager(wstring cdbFilePath);
		~ciPodManager();
	public:
		//����
		static ciPodManager* GetInstance();
		static	 void   DestroyInstance();
		static void Init(wstring cdbFilePath,int VersionNUM);
	
		// �½�����
		CIosCDBTrackData* InsertNewTrack(wstring FileName);

		//    ɾ������
		void DeleteTrack(CIosCDBTrackData *pTrackData);

		//  �޸����� 
		// _alltracklist�б��л�ã���ú�ֻ�����޸��ַ�������ֵ,ID�����ݲ����޸� 
		//�޸Ľӿ� AlbumArtistName ����CIosCDBTrackData ��������������
		void ModifyTrack(CIosCDBTrackData *pTrackData ,wstring ArtWorkFilePath);
		
		//   ���������б�
		void CreatePlayList(wstring PlayListName);

		//���ݲ����б���ɾ�������б�
		void DeletePlayList(wstring PlayListName);

		//���ݲ����б�IDɾ�������б�
		void DeletePlayList(__int64 PlayListId);

		//���ݾɲ����б����޸Ĳ����б�	
		void ModifyPlayList(wstring PlayListOldName,wstring PlayListNewName);

		//�޸Ĳ����б���
		void ModifyPlayList(__int64 PlayListId,wstring PlayListNewName);

		// ��ӵ������б�
		void AddToPlayList(__int64 trackid,wstring PlayListName);

		//�Ӳ����б����Ƴ�һ�׸���
		void RemoveFromPlayList(__int64 trackid,wstring PlayListName);

		const CIosiPodData* GetPiPodCDBData() const { return piPodCDBDataList; }

		CIosCDBTrackData* GetTrackByID(__int64 pid){return piPodCDBDataList->FindTrackDataByID64(pid);}

		wstring GetDevFilePathByID(__int64 PlayListId);

		void testDB(){TestDb3();}
		//���ݸ���pid������ͼƬ������һ���ļ��ļ���Ϊtitlt+"_"+pid+".jpg"
		BOOL GetAlbumCover(__int64 TrackID,wstring TrackTitle);
		//���¼��ز����б�
		void ReloadPlayListDB();
		//����CBD�ļ�������
		void TestPackCDBFile();
	private:

		//����ר�����ݿ�id
		__int64 find_Album_PID(int  trackid);
		//�������������ݿ�id
		__int64 find_artist_PID(int  trackid);
		//�������ͼƬ�ı����ʽ
		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
		bool FindTrackByMp3Info(IiPodMediaParser * mediainfo);
	private:
			//����ʵ��
		  static ciPodManager* m_pInstance;
		  CIosiPodData* piPodCDBDataList;	//CBD�ļ�����������	
		  wstring _cdbFilePath;				//�ļ�·��

		  int m_Hashtype;					//У������
		  GdiplusStartupInput _gdiplusStartupInput;
		  ULONG_PTR _gdiplusToken;
		  cIosPlayListDB* pIosPlayListDB;

	};
}