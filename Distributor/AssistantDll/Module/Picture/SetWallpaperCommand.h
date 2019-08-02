#pragma once

namespace ND91Assistant
{

// ��Ϊ��ֽ
class SetWallpaperCommand :public BaseCommand
{
public:
	SetWallpaperCommand(const DeviceData* pDevice);
	virtual ~SetWallpaperCommand(void);

	// Summary  : ����ͼƬ��Ϊ��ֽ
	// Returns	: bool
	// Parameter: wstring localPath
	bool SetLocalImageAsWallPaper(wstring localPath);

	// Summary  : �ֻ���ͼƬ��Ϊ��ֽ
	// Returns	: bool
	// Parameter: wstring remotePath
	bool SetRemoteImageAsWallPaper(wstring remotePath);

protected:

	// ִ������
	virtual void Execute();

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

	wstring GetVisibleName(wstring remote);

	enum  SetType
	{
		SetType_RemotePath,// ���ļ�
		SetType_ImageData  // ������
	};

	SetType     _type;		// ����
	wstring     _localpath;	// ����������·��
	wstring     _remotepath;// �������ֻ���·��

	bool        _success;	// ���ؽ�����ɹ�ʧ��

};

}
