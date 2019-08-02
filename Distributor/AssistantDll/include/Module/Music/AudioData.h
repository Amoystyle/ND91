///////////////////////////////////////////////////////////
//  AudioData.h
//  Implementation of the Class AudioData
//  Created on:      01-����-2011 9:42:49
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_03579EE0_4BC3_4e0d_9DF0_13EA0AF78F35__INCLUDED_)
#define EA_03579EE0_4BC3_4e0d_9DF0_13EA0AF78F35__INCLUDED_

#include "Module/File/FileData.h"

namespace ND91Assistant
{

	class AndroidMusicManager;
	class DeviceData;
	class AlbumData;
	class PlaylistData;
	class BinaryBufferReader;
    class RingData;
	// �������Լ�����
    class CLASSINDLL_CLASS_DECL AudioData : public FileData
    {
		friend class LoadTracksCommand;
		friend class AddTrackCommand;
    public:
        AudioData(const DeviceData* pDevice, AndroidMusicManager* pMusic);
        virtual ~AudioData();

		int Id(){return _id;}//����id

		wstring trackName()const{return _title;}//������

		wstring Artist()const{return _artist;}//������

		int		Duration() const { return _duration; }//ʱ��

		AlbumData* GetAlbum() const{ return _pOwnAlbum; }//����ר��

		wstring GetFullPath()const{return _remote;}//�����ļ�·��

		bool    IsMusic(){return _isMusic;}//�Ƿ����ֱ�־,�������򲻱����뵽��������

		// Summary  : ����
		// Returns	: bool
		bool Play();

		 // Summary  :�޸�ר��������������
		 // Returns	: void
		 // Parameter: wstring albumName
		 // Parameter: wstring artist
		 void ModifyAlbumArtistName(wstring albumName, wstring artist);

		 // Summary  :�޸ĸ�����
		 // Returns	: void
		 // Parameter: wstring name
		 void ModifyName(wstring name);

		 // Summary  : �޸�ר������
		 // Returns	: void
		 // Parameter: wstring localPath����·��
		 void ModifyAlbumImage(wstring localPath);

		 // Summary  :��ӵ������б�
		 // Returns	: bool
		 // Parameter: PlaylistData * playlist
		 bool AddToPlaylist(PlaylistData* playlist);


	private:
		// Summary  : ������������
		// Returns	: bool
		// Parameter: BinaryBufferReader & reader
		bool ParseAndroidData(BinaryBufferReader& reader);

	private:
		 int			_id;
		 bool			_isMusic;	// �Ƿ�����
		 int			_duration;	// ʱ��
		 wstring		_remote;	// ·��
		 wstring		_title;		// ����
		 wstring		_artist;	// ������

		 AlbumData*     _pOwnAlbum; // ����ר��

		 const DeviceData*  _pDevice;
		 AndroidMusicManager*		_pMusic;

    };
}

#endif // !defined(EA_03579EE0_4BC3_4e0d_9DF0_13EA0AF78F35__INCLUDED_)
