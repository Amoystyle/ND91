#pragma once

#include "Module/Ring/RingData.h"

namespace ND91Assistant
{
//�������ĸ��ֲ���
class RingOperationCommand :
	 public BaseCommand
{
public:
	RingOperationCommand(const DeviceData* pDevice);
	virtual ~RingOperationCommand(void);

	// ��������
	bool SettingRing(RingData* ring, RingData::MediaFileKind kind);

	// ����������
	bool RenameRing(RingData* ring, wstring tittle);

	// �Ƴ�����
	bool RemoveRing(RingData* ring);

	// �޸���������
	bool ChangeRingType(RingData* ring, RingData::MediaFileKind kind);

	// �滻ϵͳ����
	bool ReplaceIosRing(RingData* ring, wstring filepath);

	// ��������
	bool DownLoadIosRing(RingData* ring, wstring destpath);	

	// ��ȡIOS��ǰ����������GUID
	wstring GetIosCurrentRing();

	bool  DoneSuccess(){return _success;}

protected:

	// ִ������
	virtual void Execute();

	// ����Android���ص����ݰ�ͷ -- 20���ֽ�
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// ����ios�ļ��������ļ�����ȫ·��, unicode���������߸���ɾ���ļ�
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename) { }
#pragma  endregion

private:
	//EStopRing  ������������������������������RingData
	//ERenameRing/EReplaceRing ������title����
	//ESetRing��EChangeRingType������MediaFileKind����
	void setParam(RingData* ring, RingData::MediaFileKind kind = RingData::Null, wstring title = L"");

	bool DeleteRing();
	bool ChangeRingname();
	bool SetIosRing();
	bool SetIosRingWallerFile();
	bool ReplaceRing();
	bool DownLoadRing();
	wstring GetIosRing();

	DirectoryOperationCommand*  GetDirCmd();
	UploadFileCommand*			GetUpLoadCmd();
	DownloadFileCommand*		GetDownCmd();

	enum RingOpareType
	{
		ENone,
		ERemoveRing		= 1,				//�Ƴ�����
		ESetRing		= 2,				//��������
		ERenameRing		= 3,				//����������
		EReplaceRing	= 4,        		//�滻����
		EDownLoadRing	= 5,				//��������
		EGetCurrentRing	= 6,				//��ȡIOS��ǰ��������
		EChangeRingType = 16				//�޸���������
	};

	RingData*			_ring;	 		//����
    int					_kind;	 		//���� ���ͣ������������޸���������ʱ��
	wstring				_title;			//���⣬rename/replace��
	wstring				_ringid;		//ios����������GUID	
	RingOpareType       _type;			//��������

	DirectoryOperationCommand*	_pDirCmd;
	UploadFileCommand*			_pUpload;
	DownloadFileCommand*		_pDownCmd;

	bool                _success;		//�ɹ�ʧ��

	wstring   _strTempRingList;       	//PC Suite�µ�RingList�ļ�
	wstring   _strIosRingList;        	//�ֻ����ϴ��RingList�ļ���Ŀ¼
	wstring   _strTempRingSet;        	//PC Suite����������List�ļ�
	wstring   _strIosRingSetList;     	//�ֻ��˴����������List�ļ�
	wstring   _strTempRingWallerSet;  	//PC Suite��������ֽ����ini�ļ�
	wstring   _strIosRingWallerSet;   	//�ֻ���������ֽ����ini�ļ�
	wstring   _strTempSystemRingList; 	//PC Suite��ϵͳ��������List�ļ�
	wstring   _strIosSystemRingList;  	//�ֻ���ϵͳ��������List�ļ�
};

}
