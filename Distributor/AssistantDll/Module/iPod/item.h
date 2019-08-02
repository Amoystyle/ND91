#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod����item(��Ŀ)�� ������(��D��ͷ)
	class DItem : public IDataRow
	{
	public:
		DItem();
		virtual ~DItem(){}
		__int64 pid;					//Track�����PID
		__int32 revision_level;			//δ֪
		__int32 media_kind;				//��ý������maskֵ
		__int32 is_song;				//�Ƿ�������
		__int32 is_audio_book;			//�Ƿ�����������
		__int32 is_music_video;			//�Ƿ�������¼Ӱ��
		__int32 is_movie;				//�Ƿ��ǵ�Ӱ
		__int32 is_tv_show;				//�Ƿ���TVShow
		__int32 is_ring_tone;			//�Ƿ�������
		__int32 is_tone;				//�Ƿ���Tone
		__int32 is_voic_memo;			//�Ƿ���������¼
		__int32 is_book;				//�Ƿ���Book
		__int32 is_rental;				//�Ƿ���ý������
		__int32	is_itunes_u;			//�Ƿ���iTunes_u
		__int32	is_digital_booklet;		//�Ƿ�������С���ӣ���¼ר����Ϣ�Լ���ʵȵ�
		__int32	is_podcast;				//�Ƿ��ǲ���
		__int64	date_modified;			//�޸�ʱ�䣬MACʱ��
		__int32	date_backed_up;			//δ֪
		__int32	Year;					//��������
		__int32	content_rating;			//���ݷּ�
		__int32	content_rating_level;	//���ݷּ��ȼ�
		__int32	is_compilation;			//�����Ƿ�iTunes�༭��
		__int32	is_user_disabled;		//δ֪
		__int32	remember_bookmark;		//��ǩ��¼�Ƿ�����
		__int32	exclude_from_shuffle;	//�Ƿ��shuffle�ų�
		__int32	part_of_gapless_album;	//δ֪������=0
		__int32	chosen_by_auto_fill;	//δ֪������=0
		__int32	artwork_status;			//Track�Ƿ���Cover
		__int32	artwork_cache_id;		//��¼artwork��iTunes ID
		__int32	start_time_ms;			//iTunes����������ʼʱ���ֹͣʱ��
		__int32	stop_time_ms;			//iTunes����������ʼʱ���ֹͣʱ��
		__int32	total_time_ms;			//Trackʱ��
		__int32	total_burn_time_ms;		//δ֪������=NULL
		__int32	track_number;			//����ר��λ�ñ�ţ���MP3��TRCK�ֶ����
		__int32	track_count;			//����ר�����ܵĸ�����Ŀ
		__int32	disc_number;			//����CDλ�ñ��
		__int32	disc_count;				//����CD���ܵĸ�����Ŀ
		__int32	Bpm;					//ÿ���ӽ�����Ŀ
		__int32	relative_volume;		//��������
		wstring	eq_preset;				//��Ƶƽ����
		wstring	radio_stream_status;	//δ֪
		__int32	genius_id;				//geniusҵ���й�
		__int32	genre_id;				//Track���ID
		__int32	category_id;			//δ֪
		__int64	album_pid ;				//����ר��PID
		__int64	artist_pid;				//��ʼ�PID
		__int64	composer_pid;			//������PID
		wstring	Title;					//������
		wstring	Artist;					//������
		wstring	Album;					//ר��
		wstring	album_artist;			//ר��������
		wstring	Composer;				//������
		wstring	sort_title;				//����������
		wstring	sort_artist;			//����������
		wstring	sort_album;				//ר������
		wstring	sort_album_artist;		//ר����������������
		wstring	sort_composer;			//����������
		__int32	title_order;			//sort_title�ֶ�����
		__int32	artist_order;			//sort_artist�ֶ�����
		__int32	album_order;			//sort_album�ֶ�����
		__int32	genre_order;
		__int32	composer_order;
		__int32	album_artist_order;
		__int32	album_by_artist_order;
		__int32	series_name_order;
		wstring	comment;				//ע��
		wstring	Grouping;				//����
		wstring	Description;			//����
		wstring	description_long;		//δ֪������=NULL
		wstring	collection_description; //δ֪������=NULL
		wstring	Copyright;				//δ֪������=NULL
		__int32	title_blank;			//itle�Ƿ�Ϊ�գ�Ϊ��=1������=0
		__int32	artist_blank;			//artist�Ƿ�Ϊ�գ�Ϊ��=1������=0
		__int32	album_artist_blank;		//Album_artist�Ƿ�Ϊ�գ�Ϊ��=1������=
		__int32	album_blank;			//album�Ƿ�Ϊ�գ�Ϊ��=1������=0
		__int32	composer_blank;			//composer�Ƿ�Ϊ�գ�Ϊ��=1������=0
		__int32	grouping_blank;			//grouping�Ƿ�Ϊ�գ�Ϊ��=1������=0
		__int32	is_audible_audio_book;	//�Ƿ���audible_audio_book
		__int32	title_order_section;	//Sort����ţ������Ϊ27������(A-Z 26������+����)
		__int32	artist_order_section;
		__int32	album_order_section;
		__int32	album_artist_order_section;
		__int32	composer_order_section;
		__int32	genre_order_section;
		__int32	series_name_order_section;
		__int64	album_artist_pid;		//ר���質��PID
		__int32	in_songs_collection;	//���������ղر�־

		virtual const vector<wstring>* const GetValues();

		virtual wstring PrimaryKeyCondition() const;

#pragma region ����ʵ�ֵľ�̬����, ģ����BaseTable���õ�
		static wstring GetTableName();
		static DItem* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion
	};
}