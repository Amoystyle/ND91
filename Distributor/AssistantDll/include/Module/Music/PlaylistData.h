#pragma once

#include "PlayItemData.h"

namespace ND91Assistant
{
class DeviceData;
class BinaryBufferReader;

#define SYSTEM_PlAYLIST_NAMES L"my favorite,recent play,recent download"

//  �����б�
class CLASSINDLL_CLASS_DECL PlaylistData : public BaseData
{
	friend class LoadPlaylistCommand;
	friend class PlaylistOperationCommand;
public:
	PlaylistData(const DeviceData* pDevice);
	virtual ~PlaylistData(void);

	PlaylistData(PlaylistData* playlist);

	int  Id(){return _id;}//�����б�id

	wstring Name(){return _name;}//�����б���

    void SetName(wstring name) { _name = name;};

	// Summary  : �Ƿ�ϵͳ��
	// Returns	: bool
	bool IsSystem();

	// Summary  : ���ز����б���
	// Returns	: bool
	bool Load();

	// Summary  : �����������б�
	// Returns	: bool
	// Parameter: wstring name
	bool Rename(wstring name);

	// Summary  : ��Ӹ���
	// Returns	: bool
	// Parameter: int trackId
	bool AddTrack(int trackId);

	// Summary  : �Ƴ�
	// Returns	: bool
	// Parameter: PlayItemData * item
	bool RemoveItem(PlayItemData* item);

	// Summary  : ��ղ����б�
	// Returns	: bool
	bool RemoveAllItems();

	// Summary  : �ƶ������б���
	// Returns	: bool
	// Parameter: PlayItemData * itemҪ�ƶ�����
	// Parameter: int newIndex
	bool MoveItem(PlayItemData* item, int newIndex);

	// Summary  : ������в����б���
	// Returns	: deque<PlayItemData>*
	deque<PlayItemData>* GetPlaylistItems();


	// Summary  : �������Ƴ�����Ӳ����б����Ƴ������ø���id�����в����б���
	// Returns	: void
	// Parameter: int trackId����id
	void RemoveItemByTrackId(int trackId);

	// Summary  : ��Ӳ����б������
	// Returns	: void
	// Parameter: PlayItemData item
	void AddPlayListItem(PlayItemData item);

private:
	// Summary  : ��������
	// Returns	: bool
	// Parameter: BinaryBufferReader & reader
	bool ParseAndroidData(BinaryBufferReader& reader);

private:

	int						_id;		// �����б�id

	wstring					_name;		// �����б�����

	deque<PlayItemData>		_PlayItems; //�����б���

	bool                    _hasLoad;

	const DeviceData*		_pDevice;
};

}
