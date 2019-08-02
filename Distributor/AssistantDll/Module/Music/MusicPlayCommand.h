#pragma once
#include "Module/BaseCommand.h"

using namespace std;
namespace ND91Assistant
{

//���֣���������
class MusicPlayCommand :public BaseCommand
{
public:
	MusicPlayCommand(const DeviceData* pDevice);
	virtual ~MusicPlayCommand(void);

	// ����
	bool Play(wstring musicPath);

	// ֹͣ����
	bool Stop();

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
	enum  CommandType
	{
		 EPlayMusic = 17,
		 EStop		= 19
	};

	CommandType  _type;

	wstring      _remotepath;

	bool         _success;
};

}
