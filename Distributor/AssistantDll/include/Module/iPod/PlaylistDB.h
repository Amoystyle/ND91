#pragma once
#include "Common/rapidxml/rapidxml.hpp"
#include "Common/rapidxml/rapidxml_utils.hpp"
#include "Common/rapidxml/rapidxml_print.hpp"
#include "Common/rapidxml/rapidxml_iterators.hpp"
#include <string>
#include "Common/SqlAccess/SqliteAccess.h"
using namespace std;
using namespace rapidxml;
#include "Module/iPod/IosiPodData.h"

namespace ND91Assistant
{
	//class CIosCDBPlayList;
	//class SqliteAccess;

	// �����б�DB��Library.itdb����д��
	class CLASSINDLL_CLASS_DECL cIosPlayListDB
	{
		friend class ciPodManager;
		friend class CIosiPodData;
		friend class cIPodData5;

	public:
		cIosPlayListDB(wstring _iTunesPath);
		~cIosPlayListDB();
	private:
		//��������ӵ������б��������pid�������б�pid�������б������ļ���·��
			wstring addTrackToPlist(__int64 trackid,wstring PlayListName  );

		//�����б�������б�pid
		 __int64 GetPlayPidByName(wstring PlayListName );

		//���ݲ����б�����Ӹ����������б�
		 void addTrackToPlayListDB(__int64 trackid,wstring PlayListName );

		//���ݲ����б�ID��Ӹ����������б�
		 void addTrackToPlayListDB(__int64 trackid,__int64 ContainerID);

		//���������ĸ�������������ӵ��豸�����б�
		 __int64 addTrackToTotalPlayListDB(__int64 trackid );
		 void addTrackToPlayListDB(CONST vector<__int64>* trackid,__int64 ContainerID);

		//���ݲ����б����Ӳ����б������Ƴ�һ������
		 void RemoveTrackFromPlayListDB(__int64 trackid,wstring PlayListName);

		//���ݲ�����ID�Ӳ����б������Ƴ�һ������
		 void RemoveTrackFromPlayListDB(__int64 trackid,__int64 ContainerID);
		 void RemoveTrackFromPlayListDB(CONST vector<__int64>* trackid,__int64 ContainerID);

		//���ݿ����������б�
		 __int64 InsertSqlDbPlayList(wstring PlayListName);

		//���ݿ�ɾ�������б�
		 void DeleteSqlDbPlayList(__int64 PlayListId);
		 void DeleteSqlDbPlayList(wstring PlayListName);

		//���ݿ���²����б�
		 void UpdateSqlDbPlayList(__int64 PlayListId,wstring PlayListNewName);
		 void UpdateSqlDbPlayList(wstring PlayListOldName,wstring PlayListNewName);

		//������в����б���Ϣ
		 void GetAllPlayList(vector<CIosCDBPlayList*>* pVecPlayList);
		
		 //�����б��Ƿ�����
		 bool CheckDBActive(){return true;}
		
		 //�����ݿ�
		 void OpenPlsDB();

		 //�ر����ݿ�
		 void ClosePlsDB();
		
	private:
		wstring _DbFileName;
		SqliteAccess sql;
		bool bDbActive;

	};
}