#pragma once

namespace ND91Assistant
{
class AndroidMusicManager;
class AudioData;

// ��ȡ���и�������
class LoadTracksCommand :public TableQueryCommand
{
public:
	LoadTracksCommand(const DeviceData* pDevice, AndroidMusicManager* pmusic);
	virtual ~LoadTracksCommand(void);

	// ִ������
	virtual void Execute();

	// Summary  : ��ȡ���и���
	// Returns	: const deque<AudioData*>* ��������ָ��
	const deque<AudioData*>* GetAudios(){return &_audios;}

	int GetTrackCounts();

protected:
	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

private:

	deque<AudioData*>  _audios;//��������

	AndroidMusicManager*      _pMusic;

	int		_trackCount;
	bool	_getCount;
};

}
