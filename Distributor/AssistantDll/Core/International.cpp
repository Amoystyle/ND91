#include "stdafx.h"

#include "Core/International.h"

wstring IOSRINGGUID[]={
	L"system:Marimba",
	L"system:Boing",
	L"system:Trill",
	L"system:Pinball",
	L"system:Piano Riff",
	L"system:Bark",
	L"system:Robot",
	L"system:Strum",
	L"system:Ascending",
	L"system:Alarm",
	L"system:Sci-Fi",
	L"system:Blues",
	L"system:Old Phone",
	L"system:Old Car Horn",
	L"<default>",
	L"system:Doorbell",
	L"system:Motorcycle",
	L"system:Xylophone",
	L"system:Sonar",
	L"system:Time Passing",
	L"system:Harp",
	L"system:Digital",
	L"system:Timba",
	L"system:Crickets",
	L"system:Duck",
	L"system:Bell Tower"
};

std::map<STRING_ID, std::wstring> International::_strings;

void ND91Assistant::International::SetString( STRING_ID id, wstring value )
{
    _strings[id] = value;
}

std::wstring ND91Assistant::International::GetString( STRING_ID id )
{
    std::map<STRING_ID, wstring>::const_iterator it = _strings.find(id);
    if ( it == _strings.end() )
        return L"";

    return it->second;
}

std::wstring ND91Assistant::International::GetIosRingNameByGuid( wstring guid )
{
	RETURN_VALUE_IF ( guid.empty(), L"");

	for (int i = 0; i < 26; i++)
	{
		if( IOSRINGGUID[i] == guid )
		{
			return International::GetString((STRING_ID)(system_Marimba+i));
		}
	}
	return L"";
}

void ND91Assistant::International::Release()
{
    _strings.clear();
}


