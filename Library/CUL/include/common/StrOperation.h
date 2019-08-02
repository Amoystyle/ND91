#pragma once

#include <string>
#include <vector>
#include "../cul.h"
#include "../stdint.h"

using namespace std;

/// �ַ�������
class CUL_CLASS_DECL CStrOperation
{
public:

    //��16�����ַ���ת������: ��ʹ��strtol

    //������ת��4��16�����ַ���(nSize���Ƿ���4���ַ���ǰ�油��)
	static std::string Int2Hex(unsigned short nInt, int nSize=4);
    //��16���Ƶ�String ת������
    static int Hex2Int(const string strHex);

	//���ַ��������и�����б�(��ȥ����β���з���
	static vector<string> parseLines(const char* pData, size_t nSize);
    static vector<string> parseLines(string s);

	//���ַ�����chSpe�и���б�, bPassNull:�Ƿ���Կ�����
	static vector<string> parseStrings(const char* pData, size_t nSize, char chSpe, bool bPassNull=true);
    static vector<string> parseStrings(string s, char chSpe, bool bPassNull=true);
	static vector<wstring> parseStringsW(const wstring asrc, const wstring aSpe, bool bPassNull=true);

	//�滻�ַ���
	static string Replace(const string str, const string string_to_replace, const string new_string);
    static wstring ReplaceW(const wstring str, const wstring string_to_replace, const wstring new_string);

	//���ַ����ж�ȡstring: ����\0��ֹͣ���ﵽָ���ַ�����ֹͣ
    static string ReadString(const char* szData, const int nLen);

    // ɾ����ͷ��ָ���ַ�
	static string trimLeft(const string str, const string strCH);
    static wstring trimLeftW(const wstring str, const wstring strCH);

    // ɾ����β��ָ���ַ�
    static string trimRight(const string str, const string strCH);
    static wstring trimRightW(const wstring str, const wstring strCH);

    // �Ƿ���ָ���ַ�����ͷ
	static bool startsWith(const string str, const string substr);
	static bool startsWith(const wstring str, const wstring substr);

    // �Ƿ���ָ���ַ�����β
	static bool endsWith(const string str, const string substr);
    static bool endsWithW(const wstring str, const wstring substr);

    // תСд
	static string toLower(const string str);
	static wstring toLowerW(const wstring str);

    // ת��д
	static string toUpper(const string str);

	// ת��д
	static wstring toUpperW(const wstring str);

	
    // �������ַ�����ʹ�� s.insert(s.begin(), nLen-s.length(), chSpe);
    // �Ҳ�����ַ�����ʹ�� s.resize(nLen, chSpe);

    // ����ת�ַ���
	static string Int2string(int nValue);

    static wstring Int64ToWString(int64_t nValue);

	static wstring IntToWString(int nValue);

	// Summary  : �жϷָ����ָ��һ�������У���������һ���Ӵ�
	// Returns	: bool
	// Parameter: const wstring longstr ����
	// Parameter: const wstring substr  Ҫƥ����ִ�   
	// Parameter: const wstring sepreate�ָ��
	static bool ContainsInW(const wstring longstr, const wstring substr, const wstring sepreate);

	// Summary  :
	// Returns	: int
	// Parameter: string path
	static string CreateHashCode(string path);

    // ��ȡ�ļ���С����������
	static wstring FormatFileSize(size_t nData, int nDouble=1);
	static wstring FormatFileSize(int64_t nSize);

    // �Զ����Ƹ�ʽ��ʾ�ֽ���
    static string BufferToHex(const char* pBuffer, size_t nSize);

    // ���͸�ʽ��ת��
    static void MyItows(int i, wchar_t* buffer, int size);

	// url�����ʽת��
	static string HtmlEncode(string str);

    //Des����
    static string EncryptDes(string strData, unsigned char* key);
	static string EncryptDesEBC( string strData, unsigned char* key );

    // Ded����
    static string DecryptDes(string strData, unsigned char* key);

    //MD5
    static string GetMD5(string strData);
    //�ַ����Ƚ�
    static int CompareUnCase(string str1, string str2);
    static int CompareUnCase(wstring str1, wstring str2);
    
    // �����ַ��ж�
    static bool IsChineseLetter(wchar_t ch);
    static bool IsChineseString(wstring str);

    // ����ļ��Ƿ��ַ�
    static bool CheckFileName(wstring strName);

    // ��鲢�滻�ļ����Ƿ��ַ�
    static wstring CheckAndReplaceFileName(const wstring strName, wchar_t char_to_replace);

    // תBase64�ַ���
    static string ToBase64String(string src);
    static string FromBase64String(string src);
};
