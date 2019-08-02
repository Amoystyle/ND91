#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{

class PlaylistData;
class PlayItemData;
// �����б�����������
class PlaylistOperationCommand :public BaseCommand
{
public:
	PlaylistOperationCommand(const DeviceData* pDevice);
	virtual ~PlaylistOperationCommand(void);

	// Summary  : ��Ӳ����б�
	// Returns	: PlaylistData* ��ӵĲ����б�
	// Parameter: wstring name ����
	PlaylistData* AddPlaylist(wstring  name);

	// Summary  : �Ƴ������б�
	// Returns	: bool �ɹ�ʧ��
	// Parameter: PlaylistData * playlist Ҫ�Ƴ��Ĳ����б�
	bool RemovePlaylist(PlaylistData* playlist);

	// Summary  : �����������б�
	// Returns	: bool �ɹ�ʧ��
	// Parameter: PlaylistData * playlist Ҫ�������Ĳ����б�
	// Parameter: string name ������
	bool RenamePlaylist(PlaylistData* playlist, wstring name);

	// Summary  : ��Ӹ����������б�
	// Returns	: bool �ɹ�ʧ��
	// Parameter: PlaylistData * playlist Ҫ��ӵ��Ĳ����б�
	// Parameter: AudioData * track Ҫ��ӵĸ���
	bool AddTrackToPlaylist(PlaylistData* playlist, int trackId);

	// Summary  : �Ӳ����б��Ƴ�һ��
	// Returns	: bool �ɹ�ʧ��
	// Parameter: PlayItemData * item Ҫ�Ƴ����б���
	bool RemoveFromPlaylist(PlayItemData* item);

	// Summary  : �ƶ������б���
	// Returns	: bool �ɹ�ʧ��
	// Parameter: PlayItemData * item Ҫ�ƶ����б���
	// Parameter: int newIndex Ҫ�ƶ�������λ��
	bool MovePlaylistItem(PlayItemData* item, int newIndex);


	virtual void Execute();

protected:

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// Summary  : �����������ݰ�
	// Returns	: void
	// Parameter: std::string data
	virtual void SetAndroidReturnData(std::string data);

	// Summary  : �����������ݰ�ͷ
	// Returns	: bool
	// Parameter: BinaryBufferReader * pReader
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// ����ios�ļ��������ļ�����ȫ·��, unicode���������߸���ɾ���ļ�
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename) { }

private:

	enum CommandType
	{
		NOTYPE,	
        ADD_PLAYLSIT = 5,			// ��Ӳ����б�
		RENAME_PLAYLIST = 6,		// �����������б�
		REMOVE_PLAYLIST = 7,		// �Ƴ������б�
		ADD_TRACK_TO_PLAYLIST = 8,  // ��Ӹ����������б�
		MOVE_PLAYLIST_ITEM = 9,		// �ƶ������б���
		REMOVE_FROM_PLAYLIST = 10,  // �Ӳ����б����Ƴ������б���
	};

	CommandType		_cmdType;         // ��������

	PlaylistData*	_pSourcePlaylist; // ������ԴҪ�����Ĳ����б�

	PlayItemData*	_pSourcePlayItem; // ������ԴҪ�����Ĳ����б���

	wstring         _Name;			  // ������Ҫ�޸�Ϊ��������

	int             _newIndex;        // ������Ҫ�޸�Ϊ��������

	int             _trackId;		  // ������Ҫ��ӵ������б�ĸ���id


	PlaylistData*	_pReturnPlaylist; // ���ɵĲ����б�

	bool            _success;		  // �����ɹ�ʧ�ܱ�־

};

}