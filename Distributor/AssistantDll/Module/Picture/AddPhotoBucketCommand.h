#pragma once

namespace ND91Assistant
{
class AndroidPhotoBucket;
// ���ͼ�����
class AddPhotoBucketCommand :public BaseCommand
{
public:
	AddPhotoBucketCommand(const DeviceData* pDevice);
	virtual ~AddPhotoBucketCommand(void);

	// Summary  : �����������
	// Returns	: void
	// Parameter: AndroidPhotoBucket * PhotoBucketҪ��ӵķ���
	void SetParam(AndroidPhotoBucket* PhotoBucket);

	// ִ������
	virtual void Execute();

	// Summary  : �ɹ�ʧ�ܱ�־
	// Returns	: bool
	bool  Success(){return _success;}

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

	AndroidPhotoBucket*     _pPhotoBucket;  // ������Ҫ��ӵ�ͼ�����
    bool                 _success;       // �ɹ�ʧ�ܱ�־
};

}
