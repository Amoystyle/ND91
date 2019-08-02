#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ios5 ipod 音乐扩展信息
	class DItemextra5 : public IDataRow
	{
	public:
		DItemextra5();
		virtual ~DItemextra5(){}
		__int64 item_pid;		 //歌曲pid
		wstring title;		
		wstring sort_title;
		int	disc_count;
		int	track_count;
		int	artwork_cache_id;
		int	location_type;
		wstring	location;
		int	extension;
		int	date_created;
		int	file_size;
		int	file_creator;
		int	file_type;
		int	num_dir_levels_file;
		int	num_dir_levels_lib;
		string integrity;
		int	media_kind;
		int	date_modified;
		int	year;
		int	content_rating;
		int	content_rating_level;
		int	is_user_disabled;
		int	remember_bookmark;
		int	exclude_from_shuffle;
		int	start_time_ms;
		int	stop_time_ms;
		int	total_time_ms;
		int	total_burn_time_ms;
		int	bpm;
		int	relative_volume;
		wstring	eq_preset;
		int	genius_id;
		wstring	comment;
		wstring	grouping;
		wstring	description;
		wstring	description_long;
		wstring	collection_description;
		int	is_ota_purchased;
		int	is_compilation;
		int	is_itunes_u;
		int	is_audible_audio_book;
		wstring	copyright;
		int	chosen_by_auto_fill;
		int	is_temporary_cloud_download;
		int	is_podcast;
		wstring	external_guid;
		wstring	feed_url;
		int	store_kind;
		int	date_purchased;
		int	date_released;
		int	account_id;
		int	key_versions;
		int	key_platform_id;
		int	key_id;
		int	store_item_id;
		int	store_artist_id;
		int	store_composer_id;
		int	store_genre_id;
		int	store_playlist_id;
		int	storefront_id;
		wstring	redownload_action_params;
		wstring	artwork_url;
		wstring	store_xid;
		wstring	store_flavor;
		int	store_saga_id;
		int	store_matched_status;
		int	store_redownloaded_status;
		int	audio_format;
		int	sample_rate;
		int	duration;
		int	gapless_heuristic_info;
		int	gapless_encoding_delay;
		int	gapless_encoding_drain;
		int	gapless_last_frame_resynch;
		int	analysis_inhibit_flags;
		int	audio_fingerprint;
		int	volume_normalization_energy;
		int	is_rental;
		int	is_demo;
		int	rental_duration;
		int	rental_playback_duration;
		int	rental_playback_date_started;
		int	rental_date_started;
		int	has_alternate_audio;
		int	has_subtitles;
		int	is_hd;
		int	season_number;
		int	audio_language;
		int	audio_track_index;
		int	audio_track_id;
		int	subtitle_language;
		int	subtitle_track_index;
		int	subtitle_track_id;
		wstring	episode_id;
		wstring	network_name;
		wstring	extended_content_rating;
		wstring	movie_info;
		int	bit_rate;
		int	pending_genius_checksum;

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DItemextra5* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
