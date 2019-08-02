#include "stdafx.h"

#ifdef MODULE_IPOD

#include "Ios5ItemDB.h"

DItem5::DItem5()
{
	item_pid=0;
	media_type=0;	
	title_order=0;	
	title_order_section=0;
	item_artist_pid=0;	
	item_artist_order=0;
	item_artist_order_section=0;
	series_name_order=0;
	series_name_order_section=0;
	album_pid=0;		
	album_oder=0;
	album_oder_section=0;
	album_artist_pid=0;	
	album_artist_order=0;
	album_artist_order_section=0;
	composer_pid=0;
	compser_order=0;
	compser_order_sectionn=0;
	genre_id=0;
	genre_order=0;
	genre_order_section=0;
	disk_number=0;
	track_number=0;
	location_kind_id=0;	
	episode_sort_id=0;
	store_link_id=0;
	category_id=0;
	base_location_id=0;	

}
wstring DItem5::GetTableName()
{
	return L"item";
}
DItem5* DItem5::FromValue(const vector<const char*>* pValues)
{
	DItem5* p = new DItem5();
	int i = 0;	
	p->item_pid = _atoi64(pValues->at(i++));		
	p->media_type = atoi(pValues->at(i++));
	p->title_order = _atoi64(pValues->at(i++));	 
	p->title_order_section = atoi(pValues->at(i++));
	p->item_artist_pid = _atoi64(pValues->at(i++));	
	p->item_artist_order = _atoi64(pValues->at(i++));
	p->item_artist_order_section = atoi(pValues->at(i++));
	p->series_name_order = _atoi64(pValues->at(i++));
	p->series_name_order_section = atoi(pValues->at(i++));
	p->album_pid = _atoi64(pValues->at(i++));
	p->album_oder = _atoi64(pValues->at(i++));
	p->album_oder_section = atoi(pValues->at(i++));
	p->album_artist_pid = _atoi64(pValues->at(i++));	
	p->album_artist_order = _atoi64(pValues->at(i++));
	p->album_artist_order_section = atoi(pValues->at(i++));
	p->composer_pid = _atoi64(pValues->at(i++));
	p->compser_order = _atoi64(pValues->at(i++));
	p->compser_order_sectionn = atoi(pValues->at(i++));
	p->genre_id = _atoi64(pValues->at(i++));
	p->genre_order = _atoi64(pValues->at(i++));
	p->genre_order_section = atoi(pValues->at(i++));
	p->disk_number = atoi(pValues->at(i++));
	p->track_number = atoi(pValues->at(i++));
	p->location_kind_id = _atoi64(pValues->at(i++));	
	p->episode_sort_id = _atoi64(pValues->at(i++));
	p->store_link_id = _atoi64(pValues->at(i++));
	p->category_id = _atoi64(pValues->at(i++));
	p->base_location_id = _atoi64(pValues->at(i++));
	return p;
}

void DItem5::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"item_pid");
	vector.push_back(L"media_type");	
	vector.push_back(L"title_order");	
	vector.push_back(L"title_order_section");
	vector.push_back(L"item_artist_pid");	
	vector.push_back(L"item_artist_order");
	vector.push_back(L"item_artist_order_section");
	vector.push_back(L"series_name_order");
	vector.push_back(L"series_name_order_section");
	vector.push_back(L"album_pid");		
	vector.push_back(L"album_order");
	vector.push_back(L"album_order_section");
	vector.push_back(L"album_artist_pid");	
	vector.push_back(L"album_artist_order");
	vector.push_back(L"album_artist_order_section");
	vector.push_back(L"composer_pid");
	vector.push_back(L"composer_order");
	vector.push_back(L"composer_order_section");
	vector.push_back(L"genre_id");
	vector.push_back(L"genre_order");
	vector.push_back(L"genre_order_section");
	vector.push_back(L"disc_number");
	vector.push_back(L"track_number");
	vector.push_back(L"location_kind_id");	
	vector.push_back(L"episode_sort_id");
	vector.push_back(L"store_link_id");
	vector.push_back(L"category_id");
	vector.push_back(L"base_location_id");	

}
const vector<wstring>* const DItem5::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(item_pid));		 //歌曲pid
	pVector->push_back(Int32ToStr(media_type));			 //媒体类型
	pVector->push_back(Int64ToStr(title_order));	 //标题排序
	pVector->push_back(Int32ToStr(title_order_section));
	pVector->push_back(Int64ToStr(item_artist_pid));	//表演者pid
	pVector->push_back(Int64ToStr(item_artist_order));
	pVector->push_back(Int32ToStr(item_artist_order_section));
	pVector->push_back(Int64ToStr(series_name_order));
	pVector->push_back(Int32ToStr(series_name_order_section));
	pVector->push_back(Int64ToStr(album_pid));			//专辑pid
	pVector->push_back(Int64ToStr(album_oder));
	pVector->push_back(Int32ToStr(album_oder_section));
	pVector->push_back(Int64ToStr(album_artist_pid));	//专辑表演者pid
	pVector->push_back(Int64ToStr(album_artist_order));
	pVector->push_back(Int32ToStr(album_artist_order_section));
	pVector->push_back(Int64ToStr(composer_pid));		//作曲家pid
	pVector->push_back(Int64ToStr(compser_order));
	pVector->push_back(Int32ToStr(compser_order_sectionn));
	pVector->push_back(Int64ToStr(genre_id));			//风格
	pVector->push_back(Int64ToStr(genre_order));
	pVector->push_back(Int32ToStr(genre_order_section));
	pVector->push_back(Int32ToStr(disk_number));
	pVector->push_back(Int32ToStr(track_number));
	pVector->push_back(Int64ToStr(location_kind_id));	//音乐类型（本地还是网络）
	pVector->push_back(Int64ToStr(episode_sort_id));
	pVector->push_back(Int64ToStr(store_link_id));
	pVector->push_back(Int64ToStr(category_id));
	pVector->push_back(Int64ToStr(base_location_id));	//基础库pid
	return pVector;
}
wstring DItem5::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" item_id = "<<item_pid;
	return woss.str();
}


#endif