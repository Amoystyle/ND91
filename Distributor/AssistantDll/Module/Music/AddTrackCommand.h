#pragma once

namespace ND91Assistant
{
class AndroidMusicManager;
class AudioData;

// ��Ӹ���
class AddTrackCommand :public BaseCommand
{
public:
	AddTrackCommand(const DeviceData* pDevice, AndroidMusicManager* pMusic);
	virtual ~AddTrackCommand(void);

	// Summary  : �����������
	// Returns	: void 
	// Parameter: wstring localpath��������·��
	// Parameter: wstring remotepath���������ϴ����ϴ���ĸ����ֻ���·��
	void SetParam(wstring localpath, wstring remotepath);

	// ִ������
	virtual void Execute();

	AudioData* GetReturnAudio(){return _pReturnAudio;}

protected:

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

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
	AndroidMusicManager*  _pMusic;

	wstring        _LocalPath;

	wstring        _RemotePath;

	AudioData*     _pReturnAudio;
};

}
