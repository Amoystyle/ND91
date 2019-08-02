#pragma once

#include <string>
#include <vector>
using namespace std;

//NPK XML Define
#define NPK_XML_ROOT		L"npk"
#define NPK_XML_ITEM		L"item"
#define NPK_XML_ITEMNAME	L"name"
#define NPK_XML_APK			L"apkFileName"
#define NPK_XML_PKGNAME		L"packageName"
#define NPK_XML_VERSIONNAME	L"versionName"
#define NPK_XML_DATA		L"data"
#define NPK_XML_DATA_LOCAL  L"localName"
#define NPK_XML_DATA_REMOTE L"remotePath"
#define NPK_XML_DATA_FOLDER	L"folder"

namespace ND91Assistant
{
	//APK����Ҫ����������
	struct NPK_CopyData{
		wstring localName;			//����·��
		wstring remotePath;			//Զ��·��
		bool bFolder;				//�Ƿ��ļ���
	};
	//NPK������Item
	struct NPK_ITEM{
		wstring packageName;			//Apk����
		wstring Name;				//Apk���� Ŀǰ��֪��ɶ����
		wstring apkFileName;			//�ļ���
		wstring versionName;			//�汾��
		vector<NPK_CopyData*> datas;	//��Ҫ�����������б�
	};

	class NpkData
	{
	// ����NPK�����ļ��õ���������
	public:
		NpkData(string strData);
		NpkData(wstring strData);
		~NpkData();

	public:
		vector<NPK_ITEM*> _items;	//��Ҫ��װ��APK���б�

	private:
		void parseData(wstring strData);
	};
}