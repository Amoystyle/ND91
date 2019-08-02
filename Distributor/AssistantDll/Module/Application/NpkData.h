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
	//APK所需要拷贝的数据
	struct NPK_CopyData{
		wstring localName;			//本地路径
		wstring remotePath;			//远程路径
		bool bFolder;				//是否文件夹
	};
	//NPK包含的Item
	struct NPK_ITEM{
		wstring packageName;			//Apk名称
		wstring Name;				//Apk名称 目前不知有啥区别
		wstring apkFileName;			//文件名
		wstring versionName;			//版本名
		vector<NPK_CopyData*> datas;	//需要拷贝的数据列表
	};

	class NpkData
	{
	// 解析NPK配置文件得到的数据类
	public:
		NpkData(string strData);
		NpkData(wstring strData);
		~NpkData();

	public:
		vector<NPK_ITEM*> _items;	//需要安装的APK项列表

	private:
		void parseData(wstring strData);
	};
}