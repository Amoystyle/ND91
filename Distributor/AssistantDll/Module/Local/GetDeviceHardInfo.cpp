#include "stdafx.h"
#include "Module/Local/GetDeviceHardInfo.h"

CGetDeviceHardInfo::CGetDeviceHardInfo(wstring strID)
{
	_success = false;
	_id		 = CStrOperation::toLowerW(strID);

	GetHardInfo();
}

CGetDeviceHardInfo::~CGetDeviceHardInfo(void)
{

}

void CGetDeviceHardInfo::GetHardInfo()
{
	HKEY  hKey;
	DWORD dwIndex	= 0;
	DWORD dwBufsize	= MAX_PATH;
	bool  bfind		= false;

	wstring subKey = L"SYSTEM\\CurrentControlSet\\Enum\\USB\\";  
	TCHAR szKey[MAX_PATH];

	RETURN_IF ( ERROR_SUCCESS != RegOpenKeyEx( HKEY_LOCAL_MACHINE, subKey.c_str(), NULL,KEY_READ, &hKey) ); 

	while (RegEnumKeyEx(hKey, dwIndex++, szKey, &dwBufsize,NULL,NULL,NULL,NULL) != ERROR_NO_MORE_ITEMS)
	{
		if (_success)
			break;

		wstring strkey = szKey;
		strkey		= CStrOperation::toLowerW(strkey);
		szKey[0]	= '\0';
		dwBufsize	= MAX_PATH;

		if ( (int)strkey.find(L"vid") < 0  )
			continue;
		else
		{
			HKEY  suKey		= NULL;
			DWORD dwNum		= 0;
			DWORD dwBsize	= MAX_PATH;
			TCHAR szBkey[MAX_PATH];

			wstring theKey = subKey + strkey; 
			RETURN_IF ( ERROR_SUCCESS != RegOpenKeyEx( HKEY_LOCAL_MACHINE, theKey.c_str(), NULL,KEY_READ, &suKey) );
			while (RegEnumKeyEx(suKey, dwNum++, szBkey, &dwBsize,0,0,0,0) != ERROR_NO_MORE_ITEMS)
			{
				wstring strid = szBkey;
				szBkey[0] = '\0';
				dwBsize	= MAX_PATH;

				strid = CStrOperation::toLowerW(strid);
				if ( _id == strid )
				{
					int pos  = strkey.find(L"&");
					int rpos = strkey.rfind(L"&");
					_vid	 = strkey.substr(0,pos);

					if ( rpos>0 && rpos>pos )
						_pid = strkey.substr(pos+1,rpos-pos-1);
					else
						_pid = strkey.substr(pos+1);

					_success = true;
					break;
				}
			}
			RegCloseKey(suKey);
		}
	}
	RegCloseKey(hKey);
}