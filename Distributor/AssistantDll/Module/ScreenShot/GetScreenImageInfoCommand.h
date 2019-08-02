#pragma once


namespace ND91Assistant
{
// ��ȡ��Ļ��Ϣ
class ScreenImageData;
class GetScreenImageInfoCommand :public BaseCommand
{
public:
	GetScreenImageInfoCommand(const DeviceData* pDevice);
	virtual ~GetScreenImageInfoCommand(void);

	// Summary  : �����������
	// Returns	: void
	// Parameter: ScreenImageData * pScreenImage
	void  SetParam(ScreenImageData*  pScreenImage);

	// ִ������
	virtual void Execute();

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
	ScreenImageData*  _pScreenImage;
};

}
