#pragma once

#include <string>
#include <vector>
#include "../cul.h"
#include "../stdint.h"

using namespace std;

/// 字符串处理
class CUL_CLASS_DECL CStrOperation
{
public:

    //将16进制字符串转成整型: 请使用strtol

    //将整型转成4个16进制字符串(nSize：是否不足4个字符则前面补零)
	static std::string Int2Hex(unsigned short nInt, int nSize=4);
    //将16进制的String 转成整型
    static int Hex2Int(const string strHex);

	//将字符串内容切割成行列表(并去掉行尾换行符）
	static vector<string> parseLines(const char* pData, size_t nSize);
    static vector<string> parseLines(string s);

	//将字符串按chSpe切割成列表, bPassNull:是否忽略空内容
	static vector<string> parseStrings(const char* pData, size_t nSize, char chSpe, bool bPassNull=true);
    static vector<string> parseStrings(string s, char chSpe, bool bPassNull=true);
	static vector<wstring> parseStringsW(const wstring asrc, const wstring aSpe, bool bPassNull=true);

	//替换字符串
	static string Replace(const string str, const string string_to_replace, const string new_string);
    static wstring ReplaceW(const wstring str, const wstring string_to_replace, const wstring new_string);

	//从字符串中读取string: 遇到\0则停止，达到指定字符数则停止
    static string ReadString(const char* szData, const int nLen);

    // 删除开头的指定字符
	static string trimLeft(const string str, const string strCH);
    static wstring trimLeftW(const wstring str, const wstring strCH);

    // 删除结尾的指定字符
    static string trimRight(const string str, const string strCH);
    static wstring trimRightW(const wstring str, const wstring strCH);

    // 是否以指定字符串开头
	static bool startsWith(const string str, const string substr);
	static bool startsWith(const wstring str, const wstring substr);

    // 是否以指定字符串结尾
	static bool endsWith(const string str, const string substr);
    static bool endsWithW(const wstring str, const wstring substr);

    // 转小写
	static string toLower(const string str);
	static wstring toLowerW(const wstring str);

    // 转大写
	static string toUpper(const string str);

	// 转大写
	static wstring toUpperW(const wstring str);

	
    // 左侧添加字符，请使用 s.insert(s.begin(), nLen-s.length(), chSpe);
    // 右侧添加字符，请使用 s.resize(nLen, chSpe);

    // 数字转字符串
	static string Int2string(int nValue);

    static wstring Int64ToWString(int64_t nValue);

	static wstring IntToWString(int nValue);

	// Summary  : 判断分隔符分割的一个长串中，完整包含一个子串
	// Returns	: bool
	// Parameter: const wstring longstr 长串
	// Parameter: const wstring substr  要匹配的字串   
	// Parameter: const wstring sepreate分割符
	static bool ContainsInW(const wstring longstr, const wstring substr, const wstring sepreate);

	// Summary  :
	// Returns	: int
	// Parameter: string path
	static string CreateHashCode(string path);

    // 获取文件大小的文字描述
	static wstring FormatFileSize(size_t nData, int nDouble=1);
	static wstring FormatFileSize(int64_t nSize);

    // 以二进制格式显示字节流
    static string BufferToHex(const char* pBuffer, size_t nSize);

    // 整型格式化转换
    static void MyItows(int i, wchar_t* buffer, int size);

	// url编码格式转换
	static string HtmlEncode(string str);

    //Des加密
    static string EncryptDes(string strData, unsigned char* key);
	static string EncryptDesEBC( string strData, unsigned char* key );

    // Ded解密
    static string DecryptDes(string strData, unsigned char* key);

    //MD5
    static string GetMD5(string strData);
    //字符串比较
    static int CompareUnCase(string str1, string str2);
    static int CompareUnCase(wstring str1, wstring str2);
    
    // 中文字符判定
    static bool IsChineseLetter(wchar_t ch);
    static bool IsChineseString(wstring str);

    // 检查文件非法字符
    static bool CheckFileName(wstring strName);

    // 检查并替换文件名非法字符
    static wstring CheckAndReplaceFileName(const wstring strName, wchar_t char_to_replace);

    // 转Base64字符串
    static string ToBase64String(string src);
    static string FromBase64String(string src);
};
