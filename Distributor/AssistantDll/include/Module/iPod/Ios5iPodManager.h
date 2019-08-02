#pragma  once
#include "AssistantDll.h"
#include <GdiPlus.h>
#include <string>

#include "Module/iPod/PlaylistDB.h"
#include "Module/iPod/MobileSync.h"
#include "Module/iPod/Ios5iPodData.h"
#include "Module/iPod/Ios5BaseLocation.h"


using namespace std;
namespace ND91Assistant
{
	//���ֽṹ��
// 	typedef struct
// 	{ 
// 		char music_name[32];			//music_name 
// 		char music_local_path[256];		//music_local_path
// 		char music_store_path[256];		//music_store_path 
// 	}sMusicData ;
// 	//sConnDevice�ṹ��
// 	typedef struct {
// 		int hWnd;			//���ھ����ͬ�������ᷢ��ͬ����Ϣ���ô���
// 		int AFcConnection;	//AFC���Ӿ��
// 		int OperateMode;	//�� //PsConnDevice->OperateMode����ֵ 
// 		int ReleaseFlag;	//�� //PsConnDevice->ReleaseFlag ����ֵ
// 		int music_count;	//����Ƶ����
// 		char *uuid;			//�豸UUID
// 	}sConnDevice;
// 	//Plist��Ϣ�ṹ
// 	typedef struct {
// 		int plist_size;		//plist��С
// 		int *plist_buffer;	//plist�ڴ��ַ	
// 	}sPlistInfo;
// 	//�ص�����
// 	typedef struct  {
// 		char  *str; 
// 		unsigned int msg;      
// 	}notification_callback_info;
// 
// 	typedef void(*notification_callback)(notification_callback_info *);
// 	typedef int (*SyncMusic)(sConnDevice *,sMusicData[], notification_callback , notification_callback_info **);
// 	typedef int (*SendPlistAddr)(sPlistInfo *data);
	//ipod5���ֹ�����

	class CLASSINDLL_CLASS_DECL ciPodManager5
	{
	public:

		ciPodManager5(wstring sqlDbPath,int handle);
		~ciPodManager5();
		//���ipod������
		const CIosiPodData* GetPiPodCDBData() const { return piPodCDBDataList; }

		//������ios5ģ�鲻����Щ����


		// �½�����
		CIosCDBTrackData* InsertNewTrack(wstring FileName);

		//    ɾ������
		void DeleteTrack(CIosCDBTrackData *pTrackData);

		//   ���������б�
		void CreatePlayList(wstring PlayListName);

		//���ݲ����б���ɾ�������б�
		void DeletePlayList(wstring PlayListName);

		//���ݲ����б�IDɾ�������б�
		void DeletePlayList(__int64 PlayListId);

		// �޸Ĳ����б�
		void ModifyPlayList(wstring PlayListName,wstring NewFileName);

		//���ӵ������б�
		void AddToPlayList(wstring PlayListName,vector<__int64> trackid,wstring NewName = L"");
		//�Ӳ����б����Ƴ�
		void DelFromPlayList(wstring PlayListName,vector<__int64> trackid,wstring NewName = L"" );

		//����plist�ַ���
		static string CreatePlistWithXML(wstring xmlstr);
		//����plist����
		static void SendPlistAdd(char* revision);

		static DWORD WINAPI SYNCMusicThread(LPVOID lpParameter);
		//�ϴ�CDB�ļ�
		void UpLoadCDB();

		//��õ���ʵ��ָ��
		static ciPodManager5* GetInstance();
		//����ʵ��
		static	 void   DestroyInstance();
		//��ʼ��������ʵ��
		static void Init(wstring sqlDbPath,int VersionNUM,int handle);


		const cIPodData5* GetPiPodData() const { return piPodDataList; }
	private:
		//ios5�������
		wstring CreatePlistAddMidea(IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb);
		//��������Ƿ��ظ�
		bool CheckNewName(string filename,BaseTable<DBaseLocaltion5>* pItem);
		//����plist����ͷ��
		wstring CreatePlistHead();
		//����plist����β��
		static wstring CreatePlistTail(char* revision);
		//����������Ϣ��plist����
		wstring CreatePlistAddTrackBody(IiPodMediaParser *pMP3Data,STU_CREATE_DBID * pstudb);
		//intתstring
		wstring IntToStr( int i );
		//int64תstring
		wstring Int64ToStr( __int64 i );
		//��õ�ǰʱ���ַ���yyyy-mm-ddThh:mm:ssZ��ʽ
		static wstring getNowTimeStr();
		//��ñ�����id
		__int64 GetArtistPid(wstring ArtistName,__int64 TrackPid);
		//���ר��ID
		__int64 GetAlbumPid(wstring AlbumName,__int64 TrackPid,__int64 AlbumArtistPid);
		//ͬ������
		void SyncOper(int OperateMode,sMusicData arrMusicData[]);
		//�Ƿ�յ�ר��
		bool IsAlbumEmpty(__int64 pid);
		//����ɾ�������ı�
		wstring CreatePlistDelMidea(CIosCDBTrackData *pTrackData);
		//�����²����б��ı�
		wstring CreatePlistCreateplayList(wstring playListName,__int64& pid);
		//ɾ�������б��ı�
		wstring CreatePlistDeleteplayList(wstring playListName,__int64& pid);
		
		//�༭�����б��ı�
		wstring CreatePlistModifyPlayList(__int64 TrackPID,wstring playListName,vector<__int64> trackid);

		//ɾ�������ı�����
		wstring CreatePlistDelTrackBody(CIosCDBTrackData *pTrackData);
		//���������б��ı�����
		wstring ciPodManager5::CreatePlistCreatePlayListBody(wstring name,__int64 pid);
		//�½������б�ID
		__int64 CreateNewPlayListPid();
		//ɾ�������б�����
		wstring CreatePlistDelPlayListBody(wstring name,__int64 pid);
		//���ݲ����б����ֵõ�pid
		__int64 FindPidByName(wstring playListName);
		//�޸Ĳ����б�
		//name: �޸ĺ������ pid:�����б� pid trackid:������Ŀ
		wstring CreatePlistModifyPlayListBody(wstring name,__int64 pid,vector<__int64> trackPid);
		//����������pid
		__int64 GetComposerPid(wstring ComposerName,__int64 TrackPid);
		//����ר��������
		__int64 GetAlbumArtistPid(wstring ComposerName,__int64 TrackPid);

		static void syncMusicCallBack(notification_callback_info * para);

	//	static void SyncMessage(notification_callback_info * back_info);
		BOOL GetAppVersion( wchar_t *LibName, WORD *MajorVersion, WORD *MinorVersion, WORD *BuildNumber, WORD *RevisionNumber );

		bool FindTrackByMp3Info(IiPodMediaParser * mediainfo);
	private:
		//����ʵ��
		static ciPodManager5* m_pInstance;
		static int m_Version;
		cIPodData5* piPodDataList;		
		wstring _sqlDbPath;				//pc��itunes���õ����ݿ�����·��
		 CIosiPodData* piPodCDBDataList;	//CBD�ļ�����������	
		 SqliteAccess sql;

//		SyncMusic Ios5SyncMusic;
//		SendPlistAddr Ios5SendPlistAddr;
		HINSTANCE hDLL; 
		int MessageWinHandle;
		static wstring m_plistBuffer;
		int iTunesPrvInterface;
		bool bDBActive;
		bool bSyncProcessFinsh;
		
	};
}