#include "stdafx.h"
#ifdef MODULE_IPOD
#include "item.h"
extern int G_DBver;
DItem::DItem()
{
    pid = 0;
    revision_level = 0;
    media_kind = 0;
    is_song = 0;
    is_audio_book = 0;
    is_music_video = 0;
    is_movie = 0;
    is_tv_show = 0;
    is_ring_tone = 0;
    is_tone = 0;
    is_voic_memo = 0;
    is_book = 0;
    is_rental = 0;
    is_itunes_u = 0;
    is_digital_booklet = 0;
    is_podcast = 0;
    date_modified = 0;
    date_backed_up = 0;
    Year = 0;
    content_rating = 0;
    content_rating_level = 0;
    is_compilation = 0;
    is_user_disabled = 0;
    remember_bookmark = 0;
    exclude_from_shuffle = 0;
    part_of_gapless_album = 0;
    chosen_by_auto_fill = 0;
    artwork_status = 1;
    artwork_cache_id = 0;
    start_time_ms = 0;
    stop_time_ms = 0;
    total_time_ms = 0;
    total_burn_time_ms = 0;
    track_number = 0;
    track_count = 0;
    disc_number = 0;
    disc_count = 0;
    Bpm = 0;
    relative_volume = 0;

    genius_id = 0;
    genre_id = 0;
    category_id = 0;
    album_pid  = 0;
    artist_pid = 0;
    composer_pid = 0;

    title_order = 0;
    artist_order = 0;
    album_order = 0;
    genre_order = 0;
    composer_order = 0;
    album_artist_order = 0;
    album_by_artist_order = 0;
    series_name_order = 0;

    title_blank = 0;
    artist_blank = 0;
    album_artist_blank = 0;
    album_blank = 0;
    composer_blank = 0;
    grouping_blank = 0;
    is_audible_audio_book = 0;
    title_order_section = 0;
    artist_order_section = 0;
    album_order_section = 0;
    album_artist_order_section = 0;
    composer_order_section = 0;
    genre_order_section = 0;
    series_name_order_section = 0;
    album_artist_pid = 0;
    in_songs_collection = 0;
}

wstring DItem::GetTableName()
{
    return L"Item";
}
DItem* DItem::FromValue(const vector<const char*>* pValues)
{
    DItem* p = new DItem();
    int i = 0;	
    p->pid= _atoi64(pValues->at(i++));
    p->revision_level = atoi(pValues->at(i++));
    p->media_kind = atoi(pValues->at(i++));
    p->is_song = atoi(pValues->at(i++));
    p->is_audio_book = atoi(pValues->at(i++));
    p->is_music_video = atoi(pValues->at(i++));
    p->is_movie = atoi(pValues->at(i++));
    p->is_tv_show = atoi(pValues->at(i++));
    p->is_ring_tone = atoi(pValues->at(i++));
	if((G_DBver>301 && G_DBver<410) || G_DBver>420)
		p->is_tone = atoi(pValues->at(i++));
    p->is_voic_memo = atoi(pValues->at(i++));
	if(G_DBver>301)
		p->is_book = atoi(pValues->at(i++));
    p->is_rental = atoi(pValues->at(i++));
    p->is_itunes_u = atoi(pValues->at(i++));
	if(G_DBver>301)
		p->is_digital_booklet = atoi(pValues->at(i++));
    p->is_podcast = atoi(pValues->at(i++));
    p->date_modified = MacToPc(atoi(pValues->at(i++)));
    p->date_backed_up = atoi(pValues->at(i++));
    p->Year = atoi(pValues->at(i++));
    p->content_rating = atoi(pValues->at(i++));
    p->content_rating_level = atoi(pValues->at(i++));
    p->is_compilation = atoi(pValues->at(i++));
    p->is_user_disabled = atoi(pValues->at(i++));
    p->remember_bookmark = atoi(pValues->at(i++));
    p->exclude_from_shuffle = atoi(pValues->at(i++));
    p->part_of_gapless_album = atoi(pValues->at(i++));
	if(G_DBver>301)
		p->chosen_by_auto_fill = atoi(pValues->at(i++));
    p->artwork_status = atoi(pValues->at(i++));
    p->artwork_cache_id = atoi(pValues->at(i++));
    p->start_time_ms = atoi(pValues->at(i++));
    p->stop_time_ms = atoi(pValues->at(i++));
    p->total_time_ms = atoi(pValues->at(i++));
    p->total_burn_time_ms = atoi(pValues->at(i++));
    p->track_number = atoi(pValues->at(i++));
    p->track_count = atoi(pValues->at(i++));
    p->disc_number = atoi(pValues->at(i++));
    p->disc_count = atoi(pValues->at(i++));
    p->Bpm = atoi(pValues->at(i++));
    p->relative_volume = atoi(pValues->at(i++));
    p->eq_preset = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->radio_stream_status = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->genius_id = atoi(pValues->at(i++));
    p->genre_id = atoi(pValues->at(i++));
    p->category_id = atoi(pValues->at(i++));
    p->album_pid  = _atoi64(pValues->at(i++));
    p->artist_pid = _atoi64(pValues->at(i++));
    p->composer_pid = _atoi64(pValues->at(i++));
    p->Title = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->Artist = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->Album = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->album_artist = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->Composer = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->sort_title = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->sort_artist = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->sort_album = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->sort_album_artist = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->sort_composer = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->title_order = atoi(pValues->at(i++));
    p->artist_order = atoi(pValues->at(i++));
    p->album_order = atoi(pValues->at(i++));
    p->genre_order = atoi(pValues->at(i++));
    p->composer_order = atoi(pValues->at(i++));
    p->album_artist_order = atoi(pValues->at(i++));
    p->album_by_artist_order = atoi(pValues->at(i++));
    p->series_name_order = atoi(pValues->at(i++));
    p->comment = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->Grouping = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->Description = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->description_long = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	if(G_DBver>301)
		p->collection_description = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->Copyright = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->title_blank = atoi(pValues->at(i++));
    p->artist_blank = atoi(pValues->at(i++));
    p->album_artist_blank = atoi(pValues->at(i++));
    p->album_blank = atoi(pValues->at(i++));
    p->composer_blank = atoi(pValues->at(i++));
    p->grouping_blank = atoi(pValues->at(i++));
	if(G_DBver>301)
		p->is_audible_audio_book = atoi(pValues->at(i++));
	if(G_DBver>=310)
	{
		p->title_order_section = atoi(pValues->at(i++));
		p->artist_order_section = atoi(pValues->at(i++));
		p->album_order_section = atoi(pValues->at(i++));
		p->album_artist_order_section = atoi(pValues->at(i++));
		p->composer_order_section = atoi(pValues->at(i++));
		p->genre_order_section = atoi(pValues->at(i++));
	}
	if(G_DBver>=400)
		p->series_name_order_section = atoi(pValues->at(i++));
	if(G_DBver>=421)	
		p->album_artist_pid = atoi(pValues->at(i++));
    p->in_songs_collection = atoi(pValues->at(i++));
    return p;
}
void DItem::GetColumns(vector<wstring>& vector)
{
    vector.push_back(L"Pid");
    vector.push_back(L"revision_level");
    vector.push_back(L"media_kind");
    vector.push_back(L"is_song");
    vector.push_back(L"is_audio_book");
    vector.push_back(L"is_music_video");
    vector.push_back(L"is_movie");
    vector.push_back(L"is_tv_show");
    vector.push_back(L"is_ringtone");
	if((G_DBver>301 && G_DBver<410) || G_DBver>420)
		vector.push_back(L"is_tone");
    vector.push_back(L"is_voice_memo");
	if(G_DBver>301)
		vector.push_back(L"is_book");
    vector.push_back(L"is_rental");
    vector.push_back(L"is_itunes_u");
	if(G_DBver>301)
		vector.push_back(L"is_digital_booklet");
    vector.push_back(L"is_podcast");
    vector.push_back(L"date_modified");
    vector.push_back(L"date_backed_up");
    vector.push_back(L"Year");
    vector.push_back(L"content_rating");
    vector.push_back(L"content_rating_level");
    vector.push_back(L"is_compilation");
    vector.push_back(L"is_user_disabled");
    vector.push_back(L"remember_bookmark");
    vector.push_back(L"exclude_from_shuffle");
    vector.push_back(L"part_of_gapless_album");
	if(G_DBver>301)
		vector.push_back(L"chosen_by_auto_fill");
    vector.push_back(L"artwork_status");
    vector.push_back(L"artwork_cache_id");
    vector.push_back(L"start_time_ms");
    vector.push_back(L"stop_time_ms");
    vector.push_back(L"total_time_ms");
    vector.push_back(L"total_burn_time_ms");
    vector.push_back(L"track_number");
    vector.push_back(L"track_count");
    vector.push_back(L"disc_number");
    vector.push_back(L"disc_count");
    vector.push_back(L"Bpm");
    vector.push_back(L"relative_volume");
    vector.push_back(L"eq_preset");
    vector.push_back(L"radio_stream_status");
    vector.push_back(L"genius_id");
    vector.push_back(L"genre_id");
    vector.push_back(L"category_id");
    vector.push_back(L"album_pid"); 
    vector.push_back(L"artist_pid");
    vector.push_back(L"composer_pid");
    vector.push_back(L"Title");
    vector.push_back(L"Artist");
    vector.push_back(L"Album");
    vector.push_back(L"album_artist");
    vector.push_back(L"Composer");
    vector.push_back(L"sort_title");
    vector.push_back(L"sort_artist");
    vector.push_back(L"sort_album");
    vector.push_back(L"sort_album_artist");
    vector.push_back(L"sort_composer");
    vector.push_back(L"title_order");
    vector.push_back(L"artist_order");
    vector.push_back(L"album_order");
    vector.push_back(L"genre_order");
    vector.push_back(L"composer_order");
    vector.push_back(L"album_artist_order");
    vector.push_back(L"album_by_artist_order");
    vector.push_back(L"series_name_order");
    vector.push_back(L"Comment");
    vector.push_back(L"Grouping");
    vector.push_back(L"Description");
    vector.push_back(L"description_long");
	if(G_DBver>301)
		vector.push_back(L"collection_description");
    vector.push_back(L"Copyright");
    vector.push_back(L"title_blank");
    vector.push_back(L"artist_blank");
    vector.push_back(L"album_artist_blank");
    vector.push_back(L"album_blank");
    vector.push_back(L"composer_blank");
    vector.push_back(L"grouping_blank");
   if(G_DBver>301)
	   vector.push_back(L"is_audible_audio_book");
	if(G_DBver>=310)
	{
		if(G_DBver>=400)
		{
			vector.push_back(L"title_order_section");
			vector.push_back(L"artist_order_section");
			vector.push_back(L"album_order_section");
			vector.push_back(L"album_artist_order_section");
			vector.push_back(L"composer_order_section");
			vector.push_back(L"genre_order_section");
		}
		else
		{
			vector.push_back(L"sort_title_section");
			vector.push_back(L"sort_artist_section");
			vector.push_back(L"sort_album_section");
			vector.push_back(L"sort_album_artist_section");
			vector.push_back(L"sort_composer_section");
			if(G_DBver>=310)
				vector.push_back(L"genre_section");
			else
				vector.push_back(L"sort_genre_section");
		}
	}
	if(G_DBver>=400)
		vector.push_back(L"series_name_order_section");
	 if(G_DBver>=421)
		 vector.push_back(L"album_artist_pid");
    vector.push_back(L"in_songs_collection");
}
const vector<wstring>* const DItem::GetValues()
{
    vector<wstring>* pVector = GetVector();

    pVector->push_back(Int64ToStr(pid));
    pVector->push_back(Int32ToStr( revision_level ));
    pVector->push_back(Int32ToStr(  media_kind ));
    pVector->push_back(Int32ToStr(  is_song ));
    pVector->push_back(Int32ToStr(  is_audio_book ));
    pVector->push_back(Int32ToStr(  is_music_video ));
    pVector->push_back(Int32ToStr(  is_movie ));
    pVector->push_back(Int32ToStr(  is_tv_show ));
    pVector->push_back(Int32ToStr(  is_ring_tone ));
	if((G_DBver>301 && G_DBver<410) || G_DBver>420)
		pVector->push_back(Int32ToStr(  is_tone ));
    pVector->push_back(Int32ToStr(  is_voic_memo ));
	if(G_DBver>301)
		pVector->push_back(Int32ToStr(  is_book ));
    pVector->push_back(Int32ToStr(  is_rental ));
    pVector->push_back(Int32ToStr( 	is_itunes_u ));
	if(G_DBver>301)
		pVector->push_back(Int32ToStr( 	is_digital_booklet ));
    pVector->push_back(Int32ToStr( 	is_podcast ));
    pVector->push_back(Int64ToStr( 	PcToMac(date_modified) ));
    pVector->push_back(Int32ToStr( 	date_backed_up ));
    pVector->push_back(Int32ToStr( 	Year ));
    pVector->push_back(Int32ToStr( 	content_rating ));
    pVector->push_back(Int32ToStr( 	content_rating_level ));
    pVector->push_back(Int32ToStr( 	is_compilation ));
    pVector->push_back(Int32ToStr( 	is_user_disabled ));
    pVector->push_back(Int32ToStr( 	remember_bookmark ));
    pVector->push_back(Int32ToStr( 	exclude_from_shuffle ));
    pVector->push_back(Int32ToStr( 	part_of_gapless_album ));
	if(G_DBver>301)
		pVector->push_back(Int32ToStr( 	chosen_by_auto_fill ));
    pVector->push_back(Int32ToStr( 	artwork_status ));
    pVector->push_back(Int32ToStr( 	artwork_cache_id ));
    pVector->push_back(Int32ToStr( 	start_time_ms ));
    pVector->push_back(Int32ToStr( 	stop_time_ms ));
    pVector->push_back(Int32ToStr( 	total_time_ms ));
    pVector->push_back(Int32ToStr( 	total_burn_time_ms ));
    pVector->push_back(Int32ToStr( 	track_number ));
    pVector->push_back(Int32ToStr( 	track_count ));
    pVector->push_back(Int32ToStr( 	disc_number ));
    pVector->push_back(Int32ToStr( 	disc_count ));
    pVector->push_back(Int32ToStr( 	Bpm ));
    pVector->push_back(Int32ToStr( 	relative_volume ));
    pVector->push_back(ConvertStr(	eq_preset ));
    pVector->push_back(ConvertStr(	radio_stream_status ));
    pVector->push_back(Int32ToStr( 	genius_id ));
    pVector->push_back(Int32ToStr( 	genre_id ));
    pVector->push_back(Int32ToStr( 	category_id ));
    pVector->push_back(Int64ToStr( 	album_pid  ));
    pVector->push_back(Int64ToStr( 	artist_pid ));
    pVector->push_back(Int64ToStr( 	composer_pid ));
    pVector->push_back(ConvertStr(Title ));
    pVector->push_back(ConvertStr(Artist ));
    pVector->push_back(ConvertStr(Album ));
    pVector->push_back(ConvertStr(album_artist ));
    pVector->push_back(ConvertStr(Composer ));
    pVector->push_back(ConvertStr(sort_title ));
    pVector->push_back(ConvertStr(sort_artist ));
    pVector->push_back(ConvertStr(sort_album ));
    pVector->push_back(ConvertStr(sort_album_artist ));
    pVector->push_back(ConvertStr(sort_composer ));
    pVector->push_back(Int32ToStr(title_order ));
    pVector->push_back(Int32ToStr( 	artist_order ));
    pVector->push_back(Int32ToStr( 	album_order ));
    pVector->push_back(Int32ToStr( 	genre_order ));
    pVector->push_back(Int32ToStr( 	composer_order ));
    pVector->push_back(Int32ToStr( 	album_artist_order ));
    pVector->push_back(Int32ToStr( 	album_by_artist_order ));
    pVector->push_back(Int32ToStr( 	series_name_order ));
    pVector->push_back(ConvertStr(comment ));
    pVector->push_back(ConvertStr(Grouping ));
    pVector->push_back(ConvertStr(Description ));
    pVector->push_back(ConvertStr(description_long ));
	if(G_DBver>301)
		pVector->push_back(ConvertStr(collection_description ));
    pVector->push_back(ConvertStr(Copyright ));
    pVector->push_back(Int32ToStr( 	title_blank ));
    pVector->push_back(Int32ToStr( 	artist_blank ));
    pVector->push_back(Int32ToStr( 	album_artist_blank ));
    pVector->push_back(Int32ToStr( 	album_blank ));
    pVector->push_back(Int32ToStr( 	composer_blank ));
    pVector->push_back(Int32ToStr( 	grouping_blank ));
	if(G_DBver>301)
		pVector->push_back(Int32ToStr( 	is_audible_audio_book ));
	if(G_DBver>=310)
	{
		pVector->push_back(Int32ToStr( 	title_order_section ));
		pVector->push_back(Int32ToStr( 	artist_order_section ));
		pVector->push_back(Int32ToStr( 	album_order_section ));
		pVector->push_back(Int32ToStr( 	album_artist_order_section ));
		pVector->push_back(Int32ToStr( 	composer_order_section ));
		pVector->push_back(Int32ToStr( 	genre_order_section ));
	}
	if(G_DBver>=400)
		pVector->push_back(Int32ToStr( 	series_name_order_section ));
	if(G_DBver>=421)
		pVector->push_back(Int64ToStr( 	album_artist_pid ));
    pVector->push_back(Int32ToStr( 	in_songs_collection ));

    return pVector;
}

wstring DItem::PrimaryKeyCondition() const
{
   wostringstream woss; 
   woss<<" Pid = "<<pid;
    return woss.str();
}


#endif