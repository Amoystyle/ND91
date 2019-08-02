#include "stdafx.h"
#include "BatchDownloadThread.h"
#include "../Logic/DownProtocolAnalysis.h"
#include "rapidxml/rapidxml.hpp"

vector<wstring> BatchDownloadThread::BatchDownload(wstring url)
{
	string soururl=CCodeOperation::WstringToString(url);
	vector<wstring> lstUrls;
	wstring tmp=WebResourceModuleCONFIG(_strbatchdownloadPath);
	wstring strXml=L"";

	bool isExist=CFileOperation::IsDirExist(tmp.c_str());
	if(!isExist)
		CFileOperation::MakeLocalDir(tmp);
	strXml=tmp+L"\\batchdownload.xml";
	isExist=CFileOperation::IsFileExist(strXml.c_str());
	if(isExist)
		CFileOperation::DeleteFileW(strXml);

	bool isSuccess=CNetOperation::DownloadFile(soururl,strXml);
	if(isSuccess)
	{
		isExist=CFileOperation::IsFileExist(strXml.c_str());
		if(isExist)
		{
			rapidxml::xml_document<> doc;
			string xml = CFileOperation::ReadFile(strXml);
			doc.parse<0>((char*)xml.c_str());
			rapidxml::xml_node<>* root = doc.first_node("content");
			if(root)
			{
				rapidxml::xml_node<>* soft = root->first_node();
				while (soft)
				{
					string url = soft->first_node("url")->value();
					if(url.empty())
						url=soft->first_node("url")->first_node()->value();

					lstUrls.push_back(CCodeOperation::StringToWstring(url));		

					soft = soft->next_sibling();
				}
			}
		}
	}

	return lstUrls;
}

unsigned long BatchDownloadThread::Main()
{
	if (url.length() == 0)
	{
		LOG->WriteDebug(_T("批量下载URL地址为空"));
		return 0 ;
	}
	vector<wstring> batchUrl = BatchDownload(url);
	for (size_t i = 0;i<batchUrl.size();i++)
	{
		WebResourceInnserMsg msg;
		msg.type = WebResourceMsg_ClickGotoDes;
		msg.Send();

		AnalysisData *data = new AnalysisData;
		bool flag = DownProtocolAnalysis::AnalysisUrl(*data,batchUrl.at(i),_T("download"));
		if (flag && data->url.length() != 0)
		{
			data->islegal = false;
			WebResourceInnserMsg msg;
			msg.type = WebResourceMsg_DownloadResource;
			msg.wParam = (WPARAM)data;
			msg.nParam = _nPlatForm;
			if (_bIsCpa)
			{
				msg.strParam=L"CPA";
			}
			msg.pCpaInfo = _pCpaInfo;
			msg.Send();
			LOG->WriteDebug(_T("查找到Download页面,发送下载消息"));
			LOG->WriteDebug(batchUrl.at(i));
		}
	}


	return 0;
}

BatchDownloadThread::BatchDownloadThread( const string id )
:BaseThread(id)
{
	_bIsCpa = false;
	_nPlatForm=0;
	_pCpaInfo = NULL;
}

BatchDownloadThread::~BatchDownloadThread()
{

}

