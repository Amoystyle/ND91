#include "pch_module.h"

#ifdef MODULE_RECORD

#include "Module/Record/RecordingManager.h"
#include "Module/System/SystemManager.h"
#include "Common/Path.h"

RecordingManager::RecordingManager(const DeviceData* pDevice)
{
	_pDevice = pDevice;

	_remoteDBPath	= _pDevice->GetIosRecordingPath()->GetIosVoiceMemosDBPath();	
	_localDBPath	= _pDevice->GetIosRecordingPath()->GetLocalVoiceMemosDBPath();	

}


RecordingManager::~RecordingManager(void)
{
	RELEASE_VECTOR(_RecordsData);
}

bool RecordingManager::IosInit()
{
	DownloadFileCommand downcmd(_pDevice);

	if (!CFileOperation::IsDirExist(_pDevice->GetIosRecordingPath()->GetLocalRecordingPath().c_str()))
	{
		RETURN_FALSE_IF( ! CFileOperation::MakeLocalDir(_pDevice->GetIosRecordingPath()->GetLocalRecordingPath()) );
	}

	if (CFileOperation::IsFileExist(_localDBPath.c_str()))
	{
        CFileOperation::DeleteFile(_localDBPath);
	}

	RETURN_FALSE_IF( ! downcmd.Download(_localDBPath, _remoteDBPath) );
	
	return true;
}

vector<RecordingData*> RecordingManager::LoadRecording()
{
	wstring RecordPath;
	DirectoryOperationCommand dircmd(_pDevice);

	RELEASE_VECTOR(_RecordsData);

	IosInit();

	bool isCracked = _pDevice->GetDevInfo()->_ios_bJailBreaked;

	CppSQLite3Query query;
	try
	{
		_SQLdb.Open(CCodeOperation::UnicodeToUTF_8(_localDBPath).c_str());

		query = _SQLdb.ExecQuery("Select Z_PK, ZLABELPRESET, ZDURATION, ZDATE, ZPATH, ZCUSTOMLABEL FROM ZRECORDING");

		while (!query.Eof())
		{
			RecordingData *pdata = new RecordingData;

			pdata->_pk			= query.GetIntField(0);
			pdata->_labelPreset = query.GetIntField(1);
			pdata->_duration	= query.GetFloatField(2);
			pdata->_createDate	= (time_t)query.GetIntField(3) + CDealTime::IosStartTime().GetTime();

			RecordPath			= CCodeOperation::GB2312ToUnicode(query.GetStringField(4));
			pdata->_path		= isCracked ? RecordPath : RecordPath.substr(17);

			pdata->_customLabel = CCodeOperation::GB2312ToUnicode(query.GetStringField(5));
			pdata->_size		= dircmd.IosGetFileSize(pdata->_path);
			pdata->_name		= CFileOperation::GetFileName(pdata->_path);

			_RecordsData.push_back(pdata);

			query.NextRow();
		}
		query.Finalize();
		_SQLdb.Close();
	}
	catch (CppSQLite3Exception& e)
	{
		query.Finalize();
		_SQLdb.Close();
		wostringstream woss;
		woss << L"Sqlite DB read error: code = " << e.ErrorCode() << L", msg = " << e.ErrorMessage();
		LOG_ERROR(woss.str());
	}

	return _RecordsData;

}

bool RecordingManager::SaveRecordOnPC( int RecordId, wstring path )
{
	RETURN_FALSE_IF( RecordId<0 );
	RETURN_FALSE_IF( path.empty() );

	RecordingData* pData = FindTheData(RecordId);

	RETURN_FALSE_IF( NULL == pData);

	DownloadFileCommand downcmd(_pDevice);
	return downcmd.Download(path + pData->_name, pData->_path);
	 
}

bool RecordingManager::DeleteRecording( int RecordId )
{
	bool success = true;
	RETURN_FALSE_IF( RecordId<0 );
	DirectoryOperationCommand dircmd(_pDevice);

	RecordingData* pData = FindTheData(RecordId);

	RETURN_FALSE_IF( NULL == pData);

	(*_it) = pData;
	_RecordsData.erase(_it);

	CppSQLite3Query query;
	try
	{
		_SQLdb.Open(CCodeOperation::UnicodeToUTF_8(_localDBPath).c_str());

		_strSQL.Format("DELETE FROM ZRECORDING WHERE Z_PK=%d", pData->_pk);
		query = _SQLdb.ExecQuery((const char*)_strSQL);

		query.Finalize();
		_SQLdb.Close();
	}
	catch (CppSQLite3Exception& e)
	{
		query.Finalize();
		_SQLdb.Close();
		wostringstream woss;
		woss << L"Sqlite DB read error: code = " << e.ErrorCode() << L", msg = " << e.ErrorMessage();
		LOG_ERROR(woss.str());

		return false;
	}

	if (dircmd.Exist(pData->_path.c_str()))
	{
		success = dircmd.DeleteFileLink(pData->_path);
	}

	success = Sync();

	SAFE_DELETE(pData);
	return success;

}

bool RecordingManager::Sync()
{

	DirectoryOperationCommand dircmd(_pDevice);
	UploadFileCommand upcmd(_pDevice);

	if (dircmd.Exist(_remoteDBPath.c_str()))
		RETURN_FALSE_IF( ! dircmd.DeleteFileLink(_remoteDBPath) );

	RETURN_FALSE_IF( ! upcmd.Upload(_localDBPath, _remoteDBPath) );

	DeviceData* pdata = (DeviceData*)_pDevice;
	RETURN_FALSE_IF(!pdata->GetSystemManager()->LogoffDev());

	return true;
}

RecordingData* RecordingManager::FindTheData( int RecordId )
{
	RETURN_NULL_IF( _RecordsData.size()<0 );

	RecordingData* pData = NULL;
	for ( _it = _RecordsData.begin(); _it != _RecordsData.end(); _it++)
	{
		if ( (*_it)->_pk == RecordId )
		{
			pData = (*_it);
			break;
		}
	}
	return pData; 
}



#endif