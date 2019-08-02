#pragma once
#include <string>
#include <vector>

using namespace std;
class WebResourceCommonFunc
{
public:
    static wchar_t* AllocateBuffer(wstring sn);
	static void Split(const string &s, char delim, vector<string> &elems);
	static wstring GetDataDir();
	static string GetGuid();
	static string GetUrlActionValue(string url, string action);
};