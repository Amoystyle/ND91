#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理item(曲目)表 数据类(以D开头)
	class DItem : public IDataRow
	{
	public:
		DItem();
		virtual ~DItem(){}
		__int64 pid;					//Track对象的PID
		__int32 revision_level;			//未知
		__int32 media_kind;				//多媒体类型mask值
		__int32 is_song;				//是否是音乐
		__int32 is_audio_book;			//是否是有声读物
		__int32 is_music_video;			//是否是音乐录影带
		__int32 is_movie;				//是否是电影
		__int32 is_tv_show;				//是否是TVShow
		__int32 is_ring_tone;			//是否是铃声
		__int32 is_tone;				//是否是Tone
		__int32 is_voic_memo;			//是否是语音记录
		__int32 is_book;				//是否是Book
		__int32 is_rental;				//是否是媒体租赁
		__int32	is_itunes_u;			//是否是iTunes_u
		__int32	is_digital_booklet;		//是否是数码小册子，记录专辑信息以及歌词等等
		__int32	is_podcast;				//是否是播客
		__int64	date_modified;			//修改时间，MAC时间
		__int32	date_backed_up;			//未知
		__int32	Year;					//初版日期
		__int32	content_rating;			//内容分级
		__int32	content_rating_level;	//内容分级等级
		__int32	is_compilation;			//歌曲是否被iTunes编辑过
		__int32	is_user_disabled;		//未知
		__int32	remember_bookmark;		//书签记录是否有用
		__int32	exclude_from_shuffle;	//是否从shuffle排除
		__int32	part_of_gapless_album;	//未知，总是=0
		__int32	chosen_by_auto_fill;	//未知，总是=0
		__int32	artwork_status;			//Track是否有Cover
		__int32	artwork_cache_id;		//记录artwork的iTunes ID
		__int32	start_time_ms;			//iTunes可以设置起始时间和停止时间
		__int32	stop_time_ms;			//iTunes可以设置起始时间和停止时间
		__int32	total_time_ms;			//Track时长
		__int32	total_burn_time_ms;		//未知，总是=NULL
		__int32	track_number;			//所在专辑位置编号，与MP3的TRCK字段相关
		__int32	track_count;			//所在专辑的总的歌曲数目
		__int32	disc_number;			//所在CD位置编号
		__int32	disc_count;				//所在CD的总的歌曲数目
		__int32	Bpm;					//每分钟节奏数目
		__int32	relative_volume;		//音量调整
		wstring	eq_preset;				//音频平衡器
		wstring	radio_stream_status;	//未知
		__int32	genius_id;				//genius业务有关
		__int32	genre_id;				//Track风格ID
		__int32	category_id;			//未知
		__int64	album_pid ;				//所属专辑PID
		__int64	artist_pid;				//歌词家PID
		__int64	composer_pid;			//作曲家PID
		wstring	Title;					//歌曲名
		wstring	Artist;					//表演者
		wstring	Album;					//专辑
		wstring	album_artist;			//专辑表演者
		wstring	Composer;				//作曲者
		wstring	sort_title;				//歌曲名排序
		wstring	sort_artist;			//表演者排序
		wstring	sort_album;				//专辑排序
		wstring	sort_album_artist;		//专辑表演者排序排序
		wstring	sort_composer;			//作曲者排序
		__int32	title_order;			//sort_title字段优先
		__int32	artist_order;			//sort_artist字段优先
		__int32	album_order;			//sort_album字段优先
		__int32	genre_order;
		__int32	composer_order;
		__int32	album_artist_order;
		__int32	album_by_artist_order;
		__int32	series_name_order;
		wstring	comment;				//注释
		wstring	Grouping;				//归类
		wstring	Description;			//描述
		wstring	description_long;		//未知，总是=NULL
		wstring	collection_description; //未知，总是=NULL
		wstring	Copyright;				//未知，总是=NULL
		__int32	title_blank;			//itle是否为空，为空=1，否则=0
		__int32	artist_blank;			//artist是否为空，为空=1，否则=0
		__int32	album_artist_blank;		//Album_artist是否为空，为空=1，否则=
		__int32	album_blank;			//album是否为空，为空=1，否则=0
		__int32	composer_blank;			//composer是否为空，为空=1，否则=0
		__int32	grouping_blank;			//grouping是否为空，为空=1，否则=0
		__int32	is_audible_audio_book;	//是否是audible_audio_book
		__int32	title_order_section;	//Sort区域号，区域分为27个区域(A-Z 26个区域+其他)
		__int32	artist_order_section;
		__int32	album_order_section;
		__int32	album_artist_order_section;
		__int32	composer_order_section;
		__int32	genre_order_section;
		__int32	series_name_order_section;
		__int64	album_artist_pid;		//专辑歌唱家PID
		__int32	in_songs_collection;	//加入音乐收藏标志

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DItem* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion
	};
}