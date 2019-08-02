#pragma once
#include "Module/BaseCommand.h"
#include <deque>

using namespace std;

namespace ND91Assistant
{
// ���ݿ���ʽ���ݻ�ȡ�������
class TableQueryCommand :public BaseCommand
{
public:
	TableQueryCommand(const DeviceData* pDevice);
	virtual ~TableQueryCommand(void);

    // Summary  : �����������
    // Returns	: void
    // Parameter: string tablename ����
    // Parameter: const char * * columns������
    // Parameter: string condition ��ѯ����
    // Parameter: string sort ���ؽ����ʲô����
    void SetParam(string tablename, const string columns, string condition, string sort);

	// ִ������
	virtual void Execute();

protected:

	// ����Android���ص����ݰ�ͷ -- 20���ֽ�
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosCommandFilePath() {return L""; }

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosResultFilePath(){return L"";}

	// ����ios�ļ��������ļ�����ȫ·��, unicode���������߸���ɾ���ļ�
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename){}

private:
	string			_tableName;		// ����
	string			_condition;		// ��ѯ����
	string			_sort;			// �����ʲô����
	vector<string>	_tablecolumns;
};

}
