#include "stdafx.h"
#include "GetSoftInfoThread.h"
#include "Logic/WebSoftModel.h"
#include "Logic/WebSoftHelp.h"

unsigned long GetSoftInfoThread::Main()
{
	RETURN_FALSE_IF(_url.empty());

	std::string retData = CNetOperation::Get(_url);

	WebSoftHelp help(_platType);
	WebSoftData* pdata = help.PareAppData(retData);
	if (CheckExit())
		return 0;

	wstring temp = pdata->_desc;
	int pos = temp.find_first_of(L"<img src=");
	if ( pos > 0 )
		pdata->_desc = temp.substr(0,pos);

	vector<std::string> verDatas;
	help.GetDescPicture( CCodeOperation::UnicodeToUTF_8(temp), verDatas);

	if ( verDatas.size() > 0 )
	{
		wstring path = WebResourceModuleCONFIG(_strWebLegalSoft) + L"WebAppIcons\\" + CStrOperation::IntToWString(pdata->_id);
		CFileOperation::MakeLocalDir( path );

		for ( int i=0;i<(int)verDatas.size();i++ )
		{
			if (CheckExit())
				break;
			std::string url = verDatas[i];
			int pos = url.rfind("//");
			std::string temp = url.substr(pos);
			wstring picPath = path + L"\\" + CCodeOperation::UTF_8ToUnicode(temp);

			if ( !CFileOperation::IsFileExist( picPath.c_str() ) )
			{
				if ( CNetOperation::DownloadFile( url, picPath ) )
				{
					pdata->_VerPicDatas.push_back(picPath);
				}
				if (CheckExit())
					break;
			}
			else
				pdata->_VerPicDatas.push_back(picPath);
		}
	}
	if (CheckExit())
		return 0;

 	if ( pdata->_VerPicDatas.size() >=0 )
	{
		WebSoftMsg msg;
		msg.message = WM_WebLegalSoft_OneAppInfoHasLoad;
		msg.platType = _platType;
		msg.SendToUI();
	}

	return 0;
}

GetSoftInfoThread::GetSoftInfoThread( const string id )
:BaseThread(id)
{

}

GetSoftInfoThread::~GetSoftInfoThread()
{

}
