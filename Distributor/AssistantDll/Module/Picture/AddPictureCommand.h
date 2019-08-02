#pragma once

namespace ND91Assistant
{
class AndroidPhotoBucket;
class PictureData;
// ���ͼƬ 
class AddPictureCommand :public BaseCommand
{
public:
	AddPictureCommand(const DeviceData* pDevice);
	virtual ~AddPictureCommand(void);

	// Summary  : �����������
	// Returns	: void
	// Parameter: wstring remotePath �ֻ���·��
	// Parameter: wstring localPath ͼƬ����·��
	// Parameter: string groupId    ����ID
	// Parameter: wstring groupName ��������
	void SetParam(wstring remotePath, wstring localPath, string groupId, wstring groupName);

	// ִ������
	virtual void Execute();

	// Summary  : ��ȡ��ӵ�ͼƬ
	// Returns	: PictureData* 
	const PictureData* GetRetPicture(){return _pRetPicture;}


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

	wstring          _remotePath;		  // ������ �ֻ���·��
	wstring          _localPath;		  // ������ ͼƬ��PC��·��
	
    string          _groupId;       // ����: ����ID
    wstring         _groupName;     // ����: ��������

	PictureData*     _pRetPicture;		  // ���ؽ���� ����ӵ�ͼƬ��Ϣ

};

}
