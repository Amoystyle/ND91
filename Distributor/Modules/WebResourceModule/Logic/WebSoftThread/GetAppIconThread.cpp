#include "stdafx.h"
#include "GetAppIconThread.h"
#include "Logic/WebSoftModel.h"
#include "Logic/WebSoftHelp.h"

unsigned long GetAppIconThread::Main()
{

	for ( size_t i=0;i<_urls.size();i++ )
	{
		if (CheckExit())
			break;
		wstring path = WebResourceModuleCONFIG(_strWebLegalSoft) + L"WebAppIcons\\icon_" + CStrOperation::IntToWString(_fids.at(i)) + L".png";
		std::string url = CCodeOperation::UnicodeToUTF_8(_urls.at(i));

		if ( !CFileOperation::IsFileExist(path.c_str()) )
		{
			if ( CNetOperation::DownloadFile( url, path ) )
			{
				if (CheckExit())
					break;
				if ( CFileOperation::GetFileSizeW(path) < 1024 )
					if ( !CNetOperation::DownloadFile( url, path ) ) continue;

				if (CheckExit())
					break;

				WebSoftMsg msg;
				msg.message = WM_WebLegalSoft_IconHasDownload;
				msg.platType = _platType;
				msg.f_id = _fids.at(i);
				msg.path = path;
				msg.SendToUI();
			}
		}
		else
		{
			if ( CFileOperation::GetFileSizeW(path) < 1024 )
			{
				if ( CNetOperation::DownloadFile( url, path ) )
				{
					if (CheckExit())
						break;
					if ( CFileOperation::GetFileSizeW(path) < 1024 )
						if ( !CNetOperation::DownloadFile( url, path ) ) continue;
					if (CheckExit())
						break;
				}
			}

			WebSoftMsg msg;
			msg.message = WM_WebLegalSoft_IconHasDownload;
			msg.platType = _platType;
			msg.f_id = _fids.at(i);
			msg.path = path;
			msg.SendToUI();
		}
	}

	return 0;
}

GetAppIconThread::GetAppIconThread( const string id )
:BaseThread(id)
{

}

GetAppIconThread::~GetAppIconThread()
{

}
