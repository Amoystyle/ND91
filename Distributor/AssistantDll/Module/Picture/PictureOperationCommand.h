#pragma once
#include "Module/BaseCommand.h"

namespace ND91Assistant
{
// ͼƬ��ͼ��������
class PictureData;
class AndroidPhotoBucket;
class PictureOperationCommand : public BaseCommand
{
public:
	PictureOperationCommand(const DeviceData* pDevice);
	virtual ~PictureOperationCommand(void);

	// Summary  : ����������
	// Returns  : �Ƿ�ɹ�
	// Parameter: string id        ����ID
	// Parameter: wstring path     ����·��
	// Parameter: wstring name     ��������
	// Parameter: wstring newName  ����������
	bool RenamePhotoBucket(string id, wstring path, wstring name, wstring newName);

	// Summary  : �Ƴ�����
    // Returns  : �Ƿ�ɹ�
    // Parameter: string id        ����ID
    // Parameter: wstring path     ����·��
    // Parameter: wstring name     ��������
	bool RemovePhotoBucket(string id, wstring path, wstring name);

	// Summary  : �ƶ�ͼƬ����һ������
    // Returns  : �Ƿ�ɹ�
    // Parameter: int imageId       ͼƬID
    // Parameter: string bucketId   �·���ID
    // Parameter: wstring name      �·�������
	bool MovePicToCategory(int imageId, string bucketId, wstring name);

	// Summary  : ɾ��ͼƬ
    // Returns  : �Ƿ�ɹ�
	// Parameter: int id ͼƬID
	bool DeletePicture(int id);

	// Summary  :
	// Returns	: bool
	bool NotifyScanPhoto();

	// Summary  : �ɹ�ʧ�ܱ�־
	// Returns	: bool
	bool  GetSuccess(){return _success;}


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
	enum OperationType
	{
		Operation_None,
		Rename_Bucket,	// ������
		Remove_Bucket,	// �Ƴ�����
		Move_Picture,	// �ƶ�ͼƬ��ָ������
		Add_Picture,    // ���ͼƬ��ָ������
		Delete_Picture,  // ɾ��ͼƬ
		NotifyScan  = 10
	};

    OperationType   _type;      // ��������

	int             _imageId;	// ��������������ͼƬ
    string          _bucketId;  // ����: ����ID
    wstring         _path;      // ����: ����·��
    wstring         _name;      // ����: ��������
	wstring         _newName;	// ����: ����������

    bool            _success;   // ���, �ɹ�ʧ�ܱ�־
};

}
