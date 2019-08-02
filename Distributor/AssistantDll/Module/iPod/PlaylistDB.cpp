#include "stdafx.h"
#ifdef MODULE_IPOD

#include "Module/iPod/PlaylistDB.h"
#include "containerDB.h"
#include "Module/iPod/iPodDbBase.h"
#include "ItemToContainerDB.h"
#include "common/sqlaccess/SqliteAccess.h"
#include "Module/Common/IosPList/IosPlistHelper.h"

template<class T>
void DeleteVector(vector<T*>* p)
{
	for(size_t i = 0; i < p->size(); ++i)
		delete (*p)[i];

	p->clear();
}
cIosPlayListDB::cIosPlayListDB(wstring _iTunesPath)
:_DbFileName(_iTunesPath)
{
	bDbActive = false;	
//	OpenPlsDB();
}
cIosPlayListDB::~cIosPlayListDB()
{
	ClosePlsDB();
}
void cIosPlayListDB::OpenPlsDB()
{
	if(bDbActive == false )
	{	
		if(sql.Open(_DbFileName  + L"\\iTunes Library.itlp\\Library.itdb"))	
		{
			iPodDbBase::CreateiPhoneKeyFunction(sql.GetDB());
			bDbActive = true;
		}
	}
}
void cIosPlayListDB::ClosePlsDB()
{
	if(bDbActive)
	{
		sql.Close();
		bDbActive=false;
	}
}
wstring cIosPlayListDB::addTrackToPlist(__int64 trackid,wstring PlayListName )
{
	OpenPlsDB();
	unsigned __int64 PlayListId =GetPlayPidByName(PlayListName);
	wchar_t buffer[32];
	_ui64tow_s(PlayListId, buffer, 31, 10);
 	wstring strXmlFile = _DbFileName + L"\\Playlist_" + buffer + L".plist";
	if(!CFileOperation::IsFileExist(strXmlFile.c_str()))
	{
		ClosePlsDB();
		return L"";
	}
 
 	wstring strAlbumIdentifier = L"MusicAlbumIDs";
 	string stTrackIdentifier = "trackPersistentIDs";
 
 	IosPlistHelper plistHelper(strXmlFile, eXmlType_UTF8);
 	XML_NODE_INDEX index;
 	index.AddChild(0);
 
 	rapidxml::xml_node<char> * pUserNode = plistHelper.SearchKeyNode(index, stTrackIdentifier,"array");	
 	if (pUserNode)
 	{
		//pUserNode = pUserNode->next_sibling();
		
		_i64tow_s(trackid, buffer, 31, 10);
		plistHelper.AddElement(pUserNode,	XML_ELEMENT(L"integer",buffer));
 		plistHelper.WriteToFile(strXmlFile);
 	}
	ClosePlsDB();
	return strXmlFile;
}
__int64 cIosPlayListDB::GetPlayPidByName(wstring PlayListName)
{
	__int64 ret = 0;
	BaseTable<DContainerDB> Container(&sql);
	wstring condition = L" name= '";
	condition+=PlayListName;
	condition+=L"'";
	Container.Select(condition);
	if(Container.GetRowCount() > 0)
	{
		DContainerDB* pContainer = Container.GetDataRow(0);
		ret = pContainer->Pid;
	}
	Container.Clear();	
	return ret;
}

void cIosPlayListDB::addTrackToPlayListDB(__int64 trackid,wstring PlayListName)
{
	OpenPlsDB();
	__int64 ContainerID = 0;
	BaseTable<DContainerDB> Container(&sql);
	wstring condition = L" name= '";
	condition+=PlayListName;
	condition+=L"'";
	Container.Select(condition);
	if(Container.GetRowCount() > 0)
	{
		DContainerDB* pContainer = Container.GetDataRow(0);
		ContainerID = pContainer->Pid;
	}
	Container.Clear();

	BaseTable<DItemToContainer> TItemToContainer(&sql);
	DItemToContainer* pItemToContainer = new DItemToContainer();
	pItemToContainer->item_pid = trackid;
	pItemToContainer->container_pid = ContainerID;
	TItemToContainer.Insert(pItemToContainer);
	SAFE_DELETE(pItemToContainer);	
	ClosePlsDB();
}

void cIosPlayListDB::addTrackToPlayListDB(__int64 trackid,__int64 ContainerID)
{
	OpenPlsDB();
	BaseTable<DItemToContainer> TItemToContainer(&sql);
	DItemToContainer* pItemToContainer = new DItemToContainer();
	pItemToContainer->item_pid = trackid;
	pItemToContainer->container_pid = ContainerID;
	TItemToContainer.Insert(pItemToContainer);
	SAFE_DELETE(pItemToContainer);
	ClosePlsDB();
}

void cIosPlayListDB::addTrackToPlayListDB(CONST vector<__int64>* trackid,__int64 ContainerID )
{
	OpenPlsDB();
	BaseTable<DItemToContainer> TItemToContainer(&sql);
	vector<DItemToContainer*> pVecItem;
	for(size_t it = 0;it<trackid->size();it++ )
	{	
		DItemToContainer* pItemToContainer = new DItemToContainer();
		pItemToContainer->item_pid = trackid->at(it);
		pItemToContainer->container_pid = ContainerID;
		pVecItem.push_back(pItemToContainer);
	}	
	TItemToContainer.Insert(&pVecItem);
	DeleteVector(&pVecItem);
	ClosePlsDB();
}

__int64 cIosPlayListDB::addTrackToTotalPlayListDB(__int64 trackid )
{

	OpenPlsDB();
	__int64 ContainerID = 0;
	BaseTable<DContainerDB> Container(&sql);
	wstring condition = L" name_order=100 and media_kinds=1 and is_hidden = 1 ";
	Container.Select(condition);
	if(Container.GetRowCount() > 0)
	{
		DContainerDB* pContainer = Container.GetDataRow(0);
		ContainerID = pContainer->Pid;
	}
	Container.Clear();

	BaseTable<DItemToContainer> TItemToContainer(&sql);
	DItemToContainer* pItemToContainer = new DItemToContainer();
	pItemToContainer->item_pid = trackid;
	pItemToContainer->container_pid = ContainerID;
	TItemToContainer.Insert(pItemToContainer);
	SAFE_DELETE(pItemToContainer);
	ClosePlsDB();
	return ContainerID;
}
void cIosPlayListDB::RemoveTrackFromPlayListDB(__int64 trackid,wstring PlayListName )
{
	
	OpenPlsDB();
	__int64 ContainerID = 0;
	BaseTable<DContainerDB> Container(&sql);
	wstring condition = L" name= '";
	condition+=PlayListName;
	condition+=L"'";
	Container.Select(condition);
	if(Container.GetRowCount() > 0)
	{
		DContainerDB* pContainer = Container.GetDataRow(0);
		ContainerID = pContainer->Pid;
	}
	Container.Clear();

	BaseTable<DItemToContainer> TItemToContainer(&sql);

	wostringstream woss;
	woss<<" item_pid = " << trackid << " and container_pid = " << ContainerID;
	TItemToContainer.Delete(woss.str());
	ClosePlsDB();

}

void cIosPlayListDB::RemoveTrackFromPlayListDB(__int64 trackid,__int64 ContainerID )
{
	OpenPlsDB();
	BaseTable<DItemToContainer> TItemToContainer(&sql);
	wostringstream woss;
	woss<<" item_pid = " << trackid << " and container_pid = " << ContainerID;
	TItemToContainer.Delete(woss.str());
	ClosePlsDB();
}

void cIosPlayListDB::RemoveTrackFromPlayListDB(CONST vector<__int64>* trackid,__int64 ContainerID )
{
	OpenPlsDB();
	BaseTable<DItemToContainer> TItemToContainer(&sql);
	vector<DItemToContainer*> pVecItem;
	for(size_t it = 0;it<trackid->size();it++ )
	{	
		DItemToContainer* pItemToContainer = new DItemToContainer();
		pItemToContainer->item_pid = trackid->at(it);
		pItemToContainer->container_pid = ContainerID;
		pVecItem.push_back(pItemToContainer);
	}	
	TItemToContainer.Delete(&pVecItem);
	DeleteVector(&pVecItem);
	ClosePlsDB();
}

__int64  cIosPlayListDB::InsertSqlDbPlayList(wstring PlayListName )
{

	OpenPlsDB();
	__int64 ContainerID = 0;
	BaseTable<DContainerDB> Container(&sql);
//	ContainerID = Container.MaxPid(L"pid")+1;
 	int ret = 1;	
 	do{//创建一个数据库中没有的新ID
 		ContainerID=iPodDbBase::Create64BitID();
 		Container.Select(ContainerID,L"Pid");		
 		ret =Container.GetRowCount();
 		Container.Clear();
 	}while( ret != 0);

	DContainerDB*  pContainer = new DContainerDB();
	pContainer->Pid = ContainerID;
	pContainer->name = PlayListName;
	pContainer->media_kinds=1;
	Container.Insert(pContainer);
	SAFE_DELETE(pContainer);
	ClosePlsDB();
	return ContainerID;
}
void cIosPlayListDB::DeleteSqlDbPlayList(__int64 PlayListId )
{
	OpenPlsDB();
	BaseTable<DContainerDB> Container(&sql);

	wostringstream woss;
	woss<<" pid = " << PlayListId;
	Container.Delete(woss.str());
	ClosePlsDB();
}
void cIosPlayListDB::DeleteSqlDbPlayList(wstring PlayListName)
{
	OpenPlsDB();
	BaseTable<DContainerDB> Container(&sql);
	wstring condition = L" name = '";
	condition+=PlayListName;
	condition+=L"'";
	Container.Delete(condition);
	ClosePlsDB();
}
void cIosPlayListDB::UpdateSqlDbPlayList(__int64 PlayListId,wstring PlayListNewName)
{ 
	OpenPlsDB();
	BaseTable<DContainerDB> Container(&sql);

	DContainerDB*  pContainer = new DContainerDB();
	pContainer->Pid = PlayListId;
	pContainer->name = PlayListNewName;
	Container.Update(pContainer);
	SAFE_DELETE(pContainer);
	ClosePlsDB();
}
void cIosPlayListDB::UpdateSqlDbPlayList(wstring PlayListOldName,wstring PlayListNewName )
{ 
	OpenPlsDB();
	BaseTable<DContainerDB> Container(&sql);

	__int64 PlayListId = 0;
	wstring condition = L" name = '";
	condition+=PlayListOldName;
	condition+=L"'";
	Container.Select(condition);
	if(Container.GetRowCount() > 0)
	{
		DContainerDB* pContainerRet = Container.GetDataRow(0);
		PlayListId = pContainerRet->Pid;
	}
	else
	{
		ClosePlsDB();
		return;
	}
	Container.Clear();

	DContainerDB*  pContainer = new DContainerDB();
	pContainer->Pid = PlayListId;
	pContainer->name = PlayListNewName;
	Container.Update(pContainer);
	SAFE_DELETE(pContainer);
	ClosePlsDB();
}

void cIosPlayListDB::GetAllPlayList(vector<CIosCDBPlayList*>* pVecPlayList)
{
	OpenPlsDB();
	pVecPlayList->clear();
	BaseTable<DContainerDB> Container(&sql);	
	BaseTable<DItemToContainer> TItemToContainer(&sql);
	Container.SelectAll();
	for(int i=0;i<Container.GetRowCount();i++)
	{
		DContainerDB* pContainer = Container.GetDataRow(i);
		CIosCDBPlayList* item = new CIosCDBPlayList();
		item->ID(pContainer->Pid);
		item->Listname(pContainer->name);
		//播放列表里的歌曲们
		wostringstream woss;
		woss<< " container_pid = " << pContainer->Pid;
		TItemToContainer.Select(woss.str());
		for(int j=0;j<TItemToContainer.GetRowCount();j++)
		{
			DItemToContainer * pItemToContainer = TItemToContainer.GetDataRow(j);
			__int64 itemid = pItemToContainer->item_pid;
			item->PushTrackid(itemid);
		}
		TItemToContainer.Clear();
		pVecPlayList->push_back(item);
	}
	Container.Clear();
	ClosePlsDB();
}




#endif