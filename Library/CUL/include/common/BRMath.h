#pragma once
#include "../cul.h"
#include <string>

using namespace std;
namespace ND91Assistant
{
	class CUL_CLASS_DECL BRMath
	{
	public:
		BRMath(wstring wstrCode);
		~BRMath();

	public:
		string _strIP;
		int    _nType;  // 0=Android    1=ios

	private:
		string ParseCode(char code1, char code2);
		//新的获取方式
		string ParseCode( string strCode );
	};
}