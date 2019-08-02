#include "StdAfx.h"
#include "CpaDataHelper.h"
#include "CpaThread.h"
#include "SafeMap.h"
#include "Manager/ThreadManager.h"

CCpaDataHelper::CCpaDataHelper(void)
{
	_pAndroidCpaData = new CSafeMap();
	_pIOSCpaData = new CSafeMap();
	_bAndroidInitFinished = false;
	_bIOSInitFinished = false;
}

CCpaDataHelper::~CCpaDataHelper(void)
{
	if(NULL != CCpaDataHelper::_instance)
	{
		delete CCpaDataHelper::_instance;
		_instance = NULL;
	}

	if (NULL != _pIOSCpaData)
	{
		_pIOSCpaData->Clear();
		delete _pIOSCpaData;
		_pIOSCpaData = NULL;
	}

	if (NULL != _pAndroidCpaData)
	{
		_pAndroidCpaData->Clear();
		delete _pAndroidCpaData;
		_pAndroidCpaData =NULL;
	}
}

bool CCpaDataHelper::getAndroidInitFinished()
{
	return _bAndroidInitFinished;
}

bool CCpaDataHelper::getIOSInitFinished()
{
	return _bIOSInitFinished;
}

CCpaDataHelper* CCpaDataHelper::GetInstance()
{
	if (NULL == _instance)
	{
		_instance = new CCpaDataHelper();
	}
	return _instance;
}

void CCpaDataHelper::InitAndroidCpaHashValue()
{
	wstring path = CONFIG(_strAndroid_CpaDownPath);
	if (CFileOperation::IsDirExist(path.c_str()))
	{
		vector<wstring> apkPathes;

		CFileOperation::FindAllFileInDir(path,apkPathes,L"apk");

		vector<wstring> npkPathes;
		CFileOperation::FindAllFileInDir(path,npkPathes,L"npk");
		for (vector<wstring>::iterator iter = apkPathes.begin(); iter != apkPathes.end(); ++iter)
		{
			wstring filePath = *iter;
			if (CFileOperation::IsFileExist(filePath.c_str()))
			{
				string strMd5 = CFileOperation::GetFileMD5(filePath);//CFileOperationEx::GetMD5(filePath);//
				_pAndroidCpaData->Insert(strMd5, filePath);
			}
		}
		
		for (vector<wstring>::iterator iter = npkPathes.begin(); iter != npkPathes.end(); ++iter)
		{
			wstring filePath = *iter;
			if (CFileOperation::IsFileExist(filePath.c_str()))
			{
				string strMd5 = CFileOperation::GetFileMD5(filePath);//CFileOperationEx::GetMD5(filePath);//
				_pAndroidCpaData->Insert(strMd5, filePath);
			}
		}
	}
	_bAndroidInitFinished = true;
	LocalCpaInfoInitFinishedMsg finishMsg;
	finishMsg.bIsAndroidPlatform = true;
	finishMsg.bIsAllFinished = false;
	finishMsg.SendToUI();
}


void CCpaDataHelper::InitIOSCpaHashValue()
{
	wstring path = CONFIG(_strIos_CpaDownPath);
	if (CFileOperation::IsDirExist(path.c_str()))
	{
		vector<wstring> ipaPathes;

		CFileOperation::FindAllFileInDir(path,ipaPathes,L"ipa");

		vector<wstring> pxlPathes;
		CFileOperation::FindAllFileInDir(path,pxlPathes,L"pxl");

		for (vector<wstring>::iterator iter = ipaPathes.begin(); iter != ipaPathes.end(); ++iter)
		{
			wstring filePath = *iter;
			if (CFileOperation::IsFileExist(filePath.c_str()))
			{
				//char* buf = NULL;
				//size_t nFileSize = readBinFile(filePath,&buf);
				//MD5 md5(buf,nFileSize);
				//string strMd5 = CFileOperationEx::GetMD5(filePath);
				string strMd5 = CFileOperation::GetFileMD5(filePath);
				_pIOSCpaData->Insert(strMd5, filePath);
			}
		}

		for (vector<wstring>::iterator iter = pxlPathes.begin(); iter != pxlPathes.end(); ++iter)
		{
			wstring filePath = *iter;
			if (CFileOperation::IsFileExist(filePath.c_str()))
			{
				string strMd5 = CFileOperation::GetFileMD5(filePath);//CFileOperationEx::GetMD5(filePath);//
				_pIOSCpaData->Insert(strMd5,filePath);
			}
		}
	}

	_bIOSInitFinished = true;
	LocalCpaInfoInitFinishedMsg finishMsg;
	finishMsg.bIsAndroidPlatform = false;
	finishMsg.bIsAllFinished = true;
	finishMsg.SendToUI();
}

//size_t CCpaDataHelper::readBinFile(wstring strFilePath, char** ppData)
//{
//	string path = CCodeOperation::ws2s(strFilePath);
//	FILE *fp;
//	if((fp=fopen(path.c_str(),"rb")) == NULL)
//	{
//		return 0;
//	}
//	fseek( fp, 0, SEEK_END );
//	size_t nFileByteLen = ftell( fp );
//	fseek( fp, 0, SEEK_SET );
//
//	*ppData = (char*)malloc(nFileByteLen + 1);
//	memset(*ppData, '\0', nFileByteLen + 1);
//	fread( *ppData, nFileByteLen, sizeof(char), fp );
//	fclose(fp);
//	return nFileByteLen;
//}


void CCpaDataHelper::InitInternal()
{
	InitAndroidCpaHashValue();
	InitIOSCpaHashValue();
}

void CCpaDataHelper::Init()
{
	string threadId = "CCpaDataHelper_InitInternal_" +  CStrOperation::Int2string(Singleton::GetThreadManager()->GetGUID());
	CCpaThread *_pInternalThread = new CCpaThread(threadId);
	_pInternalThread->Run();
}

bool CCpaDataHelper::Handle( const LocalCpaAddOrRemoveMsg* pMsg )
{
	if (pMsg->bIsAndroidPlatform)
	{
		for (vector<wstring>::const_iterator iter = pMsg->vecAddoveCpa.begin(); iter != pMsg->vecAddoveCpa.end(); iter++)
		{
			wstring filePath = *iter;
			if (CFileOperation::IsFileExist(filePath.c_str()))
			{
				string strMd5 = CFileOperation::GetFileMD5(filePath);//CFileOperationEx::GetMD5(filePath);//
				if (!strMd5.empty())
				{
					_pAndroidCpaData->Insert(strMd5,filePath);
					CpaWebButtonStatusChangedMsg msg;
					msg.bIsExist = true;
					msg.strParam = L"";
					msg.nParam = 0;
					msg.strHashValue = CCodeOperation::s2ws(strMd5);
					msg.platform = PLATFORM_Android;
					msg.PostToUI();
				}
				
			}
		}
		for (map<string,wstring>::const_iterator iter = pMsg->mapRemoveCpa.begin(); iter != pMsg->mapRemoveCpa.end(); ++iter)
		{
			this->_pAndroidCpaData->Earse(iter->first);
			CpaWebButtonStatusChangedMsg msg;
			msg.bIsExist = false;
			msg.strParam = L"";
			msg.nParam = 0;
			msg.platform = PLATFORM_Android;
			msg.strHashValue = CCodeOperation::s2ws(iter->first);
			msg.PostToUI();
		}
	}
	else
	{
		for (vector<wstring>::const_iterator iter = pMsg->vecAddoveCpa.begin(); iter != pMsg->vecAddoveCpa.end(); iter++)
		{
			wstring filePath = *iter;
			if (CFileOperation::IsFileExist(filePath.c_str()))
			{
				string strMd5 = CFileOperation::GetFileMD5(filePath);//CFileOperationEx::GetMD5(filePath);//
				if (!strMd5.empty())
				{
					this->_pIOSCpaData->Insert(strMd5,filePath);
					CpaWebButtonStatusChangedMsg msg;
					msg.platform = PLATFORM_Iphone;
					msg.bIsExist = true;
					msg.strParam = L"";
					msg.nParam = 0;
					msg.strHashValue = CCodeOperation::s2ws(strMd5);
					msg.PostToUI();
				}
				
			}
		}

		for (map<string,wstring>::const_iterator iter = pMsg->mapRemoveCpa.begin(); iter != pMsg->mapRemoveCpa.end(); ++iter)
		{
			this->_pIOSCpaData->Earse(iter->first);
			CpaWebButtonStatusChangedMsg msg;
			msg.platform = PLATFORM_Iphone;
			msg.bIsExist = false;
			msg.strParam = L"";
			msg.nParam = 0;
			msg.strHashValue = CCodeOperation::s2ws(iter->first);
			msg.PostToUI();
		}
	}
	return true;
}

bool CCpaDataHelper::Handle( CpaWebIsCancelMsg* pMsg )
{
	if (pMsg->bIsAndroidPlatform)
	{
		if (!_bAndroidInitFinished)
		{
			pMsg->bIsCancel = true;
		}
	}
	else
	{
		if (!_bIOSInitFinished)
		{
			pMsg->bIsCancel = true;
		}
	}
	return true;
}

std::wstring CCpaDataHelper::GetAndroidItemPath( string hashValue )
{
	return _pAndroidCpaData->GetValue(hashValue);
}

std::wstring CCpaDataHelper::GetIOSItemPath( string hashValue )
{
	return _pIOSCpaData->GetValue(hashValue);
}

std::wstring CCpaDataHelper::GetItemPath(string hashValue)
{
	wstring wsTemp = _pAndroidCpaData->GetValue(hashValue);
	if (wsTemp.empty())
	{
		wsTemp = _pIOSCpaData->GetValue(hashValue);
	}

	return wsTemp;
}

vector<wstring> CCpaDataHelper::GetMultiItemPath(const vector<wstring>& vec_Hash)
{
	vector<wstring> vec_files;
	for (vector<wstring>::const_iterator sniter = vec_Hash.begin(); sniter != vec_Hash.end(); ++sniter)
	{
		wstring wsPath = this->GetItemPath(CCodeOperation::UnicodeToGB2312(*sniter));
		if (!wsPath.empty())
		{
			vec_files.push_back(wsPath);
		}
	}

	return vec_files;
}

map<string,wstring>* CCpaDataHelper::GetAllDataCopy()
{
	map<string,wstring>* result = new map<string,wstring>();
	map<string,wstring>*  androidMap = _pAndroidCpaData->GetMapCopy();
	map<string,wstring>*  iosMap = _pIOSCpaData->GetMapCopy();

	result->insert(androidMap->begin(),androidMap->end());
	result->insert(iosMap->begin(),iosMap->end());
	androidMap->clear();
	iosMap->clear();
	delete androidMap;
	androidMap = NULL;

	delete iosMap;
	iosMap = NULL;
	return result;

}

CCpaDataHelper* CCpaDataHelper::_instance = NULL;
