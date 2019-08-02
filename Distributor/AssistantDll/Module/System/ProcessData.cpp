#include "stdafx.h"

#ifdef MODULE_SYSTEM

#include "ProcessData.h"

ProcessData::ProcessData(void)
{
    _eState = Unknow;
}

ProcessData::~ProcessData(void)
{
}

void ProcessData::Update()
{

}

bool ProcessData::ParseAndroidData(string strLine)
{
    vector<string> strList = CStrOperation::parseStrings(strLine.c_str(), strLine.length(), ' ');
    if (strList.size() == 9)
    {
        _strUser    = strList.at(0);
        _nPID       = atoi(strList.at(1).c_str());
        _nPPID      = atoi(strList.at(2).c_str());
        _nSize      = atol(strList.at(3).c_str());
        _nRss       = atol(strList.at(4).c_str());
        if (strList.at(7).length() == 1)
        {
            switch(strList.at(7)[0])
            {
            case 'D':
                _eState = D;
                break;
            case 'R':
                _eState = R;
                break;
            case 'S':
                _eState = S;
                break;
            case 'T':
                _eState = T;
                break;
            case 'W':
                _eState = W;
                break;
            case 'X':
                _eState = X;
                break;
            case 'Z':
                _eState = Z;
                break;
            default:
                break;
            }
        }
        _strName    = strList.at(8);
    }
	else if (strList.size() == 5)
	{
		_nPID       = atoi(strList.at(0).c_str());
		_nPPID      = atoi(strList.at(1).c_str());
		_nSize      = atol(strList.at(2).c_str());
		if (strList.at(3).length() == 1)
		{
			switch(strList.at(3)[0])
			{
			case 'D':
				_eState = D;
				break;
			case 'R':
				_eState = R;
				break;
			case 'S':
				_eState = S;
				break;
			case 'T':
				_eState = T;
				break;
			case 'W':
				_eState = W;
				break;
			case 'X':
				_eState = X;
				break;
			case 'Z':
				_eState = Z;
				break;
			default:
				break;
			}
		}
		_strName    = strList.at(4);
	}
    else
        return false;
    return true;
}




#endif