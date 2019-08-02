#include "stdafx.h"

#include "AndroidPropertyData.h"

AndroidPropertyData::AndroidPropertyData(const char* pData, size_t nSize)
{
    vector<string> lines = CStrOperation::parseLines(pData, nSize);
	//守护返回的数据有些缺少又括号，需要特殊处理
	for (vector<string>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
	{
		if (iter->length()-1 != iter->find_last_of("]"))
		{
			*iter += "]";
		}
	}
    string line;
    for (vector<string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        line = *it;
        int nIndex = line.find(':');
        if (nIndex > 0)
        {
            string strProp(line,0, nIndex);
            string strValue(line, nIndex+1);
            _PropList[FormatValue(strProp)] = FormatValue(strValue);
        }
    }
}

AndroidPropertyData::~AndroidPropertyData()
{
}

wstring ND91Assistant::AndroidPropertyData::GetPropertyValue( string strProp )
{
    string strValue = "";
    map<string, string>::const_iterator it = _PropList.find(strProp);
    if (it != _PropList.end())
        strValue = it->second;
	return CCodeOperation::UTF_8ToUnicode(strValue);
}

string ND91Assistant::AndroidPropertyData::FormatValue(string str)
{
    if (str.length() < 2)
        return str;
    int n1 = 0, n2 = 0;
    n1 = str.find('[');
    n2 = str.find(']');
    if (n1 >= 0 && n2 > n1)
        return str.substr(n1+1, n2-n1-1);
    return "";
}