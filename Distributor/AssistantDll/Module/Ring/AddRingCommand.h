#pragma once

#include "Module/Ring/RingData.h"

namespace ND91Assistant
{
	// �������
class AddRingCommand :
	public BaseCommand
{
public:
	AddRingCommand(const DeviceData* pDevice);
	virtual ~AddRingCommand(void);

	void SetParam(int kind, wstring localpath, wstring remotepath, MediaFileType type, wstring tittle=L"");

	// ִ������
	virtual void Execute();

	const RingData* GetAddedRing(){return _retRing;}

	bool  DoneSuccess(){return _success;}

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
	bool CreateRingtonesList(std::string ringname);
	string GetMimeType(const wstring path);
	int GetDuration();
	int			_kind;
	wstring		_localpath;
	wstring		_remotepath;
	wstring		_title;
	MediaFileType  _type;
	bool		_success;            //�������

	wstring		_strTempRingList;    //��ʱ�ļ����µ�RingList�ļ�

	RingData*   _retRing;


};

}
