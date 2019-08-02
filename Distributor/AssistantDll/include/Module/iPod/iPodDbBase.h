#pragma  once

#include "AssistantDll.h"
#include "IosiPodData.h"
#include "Common/SqlAccess/BaseTable.h"

#ifndef _CSHARPDLL
#include "Core/DeviceData.h"
#endif

using namespace std;
namespace ND91Assistant
{
    class SqliteAccess;
    class DLocation;
    class DItem;

	//iPod���ֹ������ݿ������
	class CLASSINDLL_CLASS_DECL iPodDbBase
	{
		friend class ciPodManager;
		friend class cIosPlayListDB;
	public:
			iPodDbBase(wstring sqlDbPath);
			~iPodDbBase();

		//************************************
		// Method:    �������64λID
		// Returns:   __int64 ID��
		//************************************
		static __int64 Create64BitID();

		//�õ�һ�������mp3�ļ���
		static string gen_file_name( );

	private:

//�����������ݲ������ݿ�
		void InsertSqlDBNewTrack(STU_CREATE_DBID * pstudb,IiPodMediaParser * pMP3Data);
		//���ݿ������������
		wstring UpdateSqlDbTrack(CIosCDBTrackData * ptrackData,STU_CREATE_DBID* pDBID,wstring ArtWorkFilePath,wstring ArtWorkDBPath);
		//���ݿ�ɾ����������
		wstring DeleteSqlDbtrack(CIosCDBTrackData * ptrackData,int& ArtWorkID);	
		//�����ݿ�����Ϣ
		wstring getInfoFromSqlDB(int infotype);

		//����ר�����������ݿ�ID
		__int64 find_Album_artist_PID(SqliteAccess *sql,__int64 trackpid);
		//����½�mp3�ļ����Ƿ�����
		bool CheckNewName(wstring filename,BaseTable<DLocation>* plocaltion);
		//�������ݿ���ID
		void GetAndCreateAllDBID(IiPodMediaParser* pMP3data,STU_CREATE_DBID * pstudb);
		//ȡ���ļ���
		wstring GetTrackPath(__int64 pid);
		//ɾ��Localtion�������
		wstring LocationDBDeltrack(__int64 pid);
		//ɾ��ר�����Ӧ��¼����ר�������߱�����
		void DeleteAlbumArtistDbRow(__int64 pid,SqliteAccess * sql);
		//�������ƻ��������ID��û�о��½�һ��
		__int64 GetComposerIdByName(SqliteAccess* pSql,wstring ComperName);
		//�������ƻ��ר��ID��û�о��½�һ��
		__int64 GetAlbumIDByName(SqliteAccess* pSql,wstring AlbumName,STU_CREATE_DBID * pstudb);
		//�������ƻ��ר��������ID��û�о��½�һ��
		__int64 GetAlbumArtistIdByName(SqliteAccess* pSql,wstring AlbumArtistName);
		//�������ƻ��������ID��û�о��½�һ��
		__int64 GetArtistIdByName(SqliteAccess* pSql,wstring ArtistName);
		//�������ݿ���������Ϣ
		__int64 UpdateComposerDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData);
		//����������
		__int64 UpdateArtistDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData);
		//����ר��
		__int64 UpdateAlbumDB(SqliteAccess* pSql,CIosCDBTrackData * ptrackData,STU_CREATE_DBID* pDBID);
		//����ר��������
		__int64 UpdateAlbumArtistDB(SqliteAccess* pSql,__int64 oldAlbumid,wstring AlbumArtistName);

		//�����������ݿ�id
		__int64 find_composer_PID(SqliteAccess *sql,__int64  trackpid);
		//��������������
		static void CreateiPhoneKeyFunction(CppSQLite3DB* db);

		__int64 find_artist_PID(CIosiPodData* piPodCDBDataList,int  trackid);
		//���ݸ���pid��÷���ͼƬid
		int GetArtworkid(__int64 pid);

		virtual __int64 find_Album_PID(int  trackid) = 0;
		virtual	__int64 find_artist_PID(int  trackid) = 0;

		void TestDb3();

		wstring GetNewDevFileName();

	private:
		wstring _sqlDbPath;				//pc��itunes���õ����ݿ�����·��

	};

}