#pragma once

namespace ND91Assistant
{
// ��õ�ǰ��ֽ����
class GetWallpaperCommand :public BaseCommand
{
public:
	GetWallpaperCommand(const DeviceData* pDevice);
	virtual ~GetWallpaperCommand(void);

	// ִ������
	virtual void Execute();

	wstring GetSavePath(){return _savePath;}

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

	wstring   _savePath;

};

}