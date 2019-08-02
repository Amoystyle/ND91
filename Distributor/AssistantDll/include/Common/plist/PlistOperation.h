#pragma once
#include <AssistantDll.h>
#include <map>
#include <vector>
#include <string>
using namespace std;

namespace ND91Assistant
{
    class AppData;
}

class CLASSINDLL_CLASS_DECL PlistOperation
{
public:
	// Summary  : 初始化：加载动态库（MAC下无需调用）
	// Returns  : 是否成功
	// Parameter: string dllPath 动态库路径
	static bool Init(wchar_t* dllPath);

	// Summary  : 释放动态库
	static void Release();

	// Summary  : plist数据
	// Returns  : 输出的plist
	// Parameter: string inputPlistData 二进制或XML格式plist数据（xml格式若包含中文需为UTF8)
	// Parameter: bool outputBinary 指定输出格式，true为二进制，false为XML
	static string ConvertPlist(string inputPlistData, bool outputBinary);

	// Summary  : 向plist文件中增加一堆key-value
	//            若key已经存在则替换其value(文件内容必须是Dict)
	// Returns  : 是否成功()
	// Parameter: wstring filename   文件名
	// Parameter: string key        添加的key
	// Parameter: string value      添加的value
	static bool AddDictPlist(wstring filename, string key, string value);

	// Summary  : 向plist文件中增加一堆key-value
	//			  若key已经存在则替换其value(文件内容必须是Dict)
	// Returns  : 是否成功()
	// Parameter: wstring filename   文件名
	// Parameter: string key        添加的key
	// Parameter: bool value		添加的value
	static bool AddDictPlist(wstring filename, string key, bool value);

	// Summary  : 向plist文件中增加一个key-map<string, string>
	//			  若key已经存在则替换其value(文件内容必须是Dict)
	// Returns  : 是否成功()
	// Parameter: wstring filename   文件名
	// Parameter: string key        添加的key
	// Parameter: bool value		添加的value
	static bool AddDictPlist(wstring filename, string key, map<string,string> mapInfos );

	// Summary  : 获取plist文件Key键对应的值
	// Returns  : 是否成功
	// Parameter: wstring filename	文件名
	// Parameter: string key		输入的key
	// Parameter: string & value	获取的value
	static bool GetDictPlist(wstring filename, string key, string& value);


	// Summary  : 获取plist文件Key键对应的bool值
	// Returns  : 是否成功
	// Parameter: wstring filename	文件名
	// Parameter: string key		输入的key
	// Parameter: bool & value		获取的value
	static bool GetDictPlist(wstring filename, string key, bool& value);

	// Summary  : 获取plist文件Key键对应的Dict包含的subKey对应的值
	// Returns  : 是否成功
	// Parameter: wstring filename	文件名
	// Parameter: string key		输入的key
	// Parameter: string subKey		输入的子键subKey
	// Parameter: string & value	获取的value
	static bool GetDictPlist(wstring filename, string key, string subKey, string& value);



	// 判断plist是否包含这个Key
	static bool PlistContainsKey(wstring filename, string key);
	static bool NDCFDictionaryContainsKey( void* pl, string key );

	// 判断plist这个Key对应的Value是否包含subkey
	static bool PlistContainsKey(wstring filename, string key, string subKey);

	// 获取plist文件所有的数据，存放在三个容器里面
	static bool GetPlistAllValuesKeys(wstring filename, vector<string> verDatas, map<string, string> mapInfos, map<string,string> mapDatas );

    // Summary  : 从IPA文件中的info.plist读取应用数据
    // Returns  : 数据，为NULL表示失败
    // Parameter: wstring tempDir 临时目录，文件解压在此，完成后删除文件
    // Parameter: wstring ipaFile ipa文件
    static ND91Assistant::AppData* GetIpaAppInfo(wstring tempDir, wstring ipaFile);

    // Summary  : 解析info.plist文件
    // Returns  : ND91Assistant::AppData* 数据，为NULL表示失败
    // Parameter: wstring strPath info.plist文件路径
    static ND91Assistant::AppData* ParseIpaAppInfoPlist(wstring strPath, wstring ipaFile = L"");

	//从XML文件中读取key的值
	static bool XmlPlistGetKey(wstring filename, string key, string& value);

	//产生临时的plist文件
	static bool GenerateTempPlist(void* pCFDict);
};
