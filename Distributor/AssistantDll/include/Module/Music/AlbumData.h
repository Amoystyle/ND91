#pragma once
#include "Module/BaseData.h"
#include <deque>

using namespace std;

#pragma warning(disable : 4251)
namespace ND91Assistant
{
class AudioData;
class DeviceData;
//ר��
class CLASSINDLL_CLASS_DECL AlbumData :public BaseData
{
public:
	AlbumData();
	virtual ~AlbumData(void);

	// Summary  : ��ȡר������ͼ
	// Returns	: wstring ���ش洢·��
	// Parameter: const DeviceData * pDevice
	wstring  GetAblumImage(const DeviceData* pDevice);

	// Summary  : ����ר������
	// Returns	: bool
	// Parameter: const DeviceData * pDevice
	// Parameter: const wstring remotepath
	bool  SetAlbumImage(const DeviceData* pDevice, const wstring remotepath);

	// Summary  : �Ƴ�ר������
	// Returns	: bool
	// Parameter: const DeviceData * pDevice
	bool  RemoveAlbumImage(const DeviceData* pDevice);

	// Summary  : ��ȡר�������и���
	// Returns	: deque<AudioData*>*��������
	const deque<AudioData*>* GetTracks() {return &_audios;}

	// Summary  : ���������ר��
	// Returns	: void
	// Parameter: AudioData * track
	void  InsertTrack(AudioData* track);

	// Summary  : ��ר�����Ƴ�һ�׸���
	// Returns	: void
	// Parameter: int trackId����id
	void  RemoveTrack(int trackId);

public:

	wstring		_albumName;       // ר����
	wstring		_albumImagePath;  // ר������

private:

	// Summary  : ר���������ص����صĴ洢·��,��������ʾ
	// Returns  : �����ļ���
	// Parameter: wstring localFolder ���ص���Ŀ¼
	wstring  GetAlbumImageLocalSavePath(wstring localFolder);

private:

	deque<AudioData*>   _audios;  // �����ĸ���
	bool       _hasLoad;

	wstring  _albumImagelocalPath;
};
}
