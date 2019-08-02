#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosPictureSqlite4.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureGroup.h"
#include "Common/Path.h"

IosPictureSqlite4::IosPictureSqlite4(const DeviceData* pDevice)
:_pDevice(pDevice)
{
	_nVersion = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion);

	_SQLite		= new CppSQLite3DB;
	_AuxSQLite	= new CppSQLite3DB;
}

IosPictureSqlite4::~IosPictureSqlite4(void)
{
	SAFE_DELETE(_SQLite);
	SAFE_DELETE(_AuxSQLite);
}

bool IosPictureSqlite4::OpenPhotoDB()
{
	// 打开Photos.sqlite，PhotosAux.sqlite数据库
	try
	{
		if ( ! _SQLite->Open(CCodeOperation::UnicodeToUTF_8(_pDevice->GetIosPicturePath()->GetLocalSqlitePath()).c_str()) ||
			 ! _AuxSQLite->Open(CCodeOperation::UnicodeToUTF_8(_pDevice->GetIosPicturePath()->GetLocalAuxSqlitePath()).c_str()) )
		{
			_SQLite->Close();
			_AuxSQLite->Close();
			return false;
		}
	}
	catch (CppSQLite3Exception& e)
	{
		_SQLite->Close();
		_AuxSQLite->Close();
		WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() );
		return false;
	}
	return true;
}

void IosPictureSqlite4::PhotoDBTransaction()
{
	_SQLite->BeginTransaction();
	_AuxSQLite->BeginTransaction();
}

void IosPictureSqlite4::PhotoDBCommit()
{
	_SQLite->Commit();
	_AuxSQLite->Commit();
}

void IosPictureSqlite4::PhotoDBRollback()
{
	_SQLite->Rollback();
	_AuxSQLite->Rollback();
}

bool IosPictureSqlite4::GetPhotoFromSqlite(vector<PictureData*>* pVecPicture)
{
	try
	{	
		if ( ! _SQLite->Open(CCodeOperation::UnicodeToUTF_8(_pDevice->GetIosPicturePath()->GetLocalSqlitePath()).c_str()) ) 
		{
			_SQLite->Close();
			return false;
		}

		CppSQLite3Query query = _SQLite->ExecQuery(
			"SELECT primaryKey, captureTime, width, height, thumbnailIndex, directory, filename FROM Photo");

		while (!query.Eof())
		{
			PictureData* pPicture = new PictureData();

			pPicture->Create(
							 CCodeOperation::GB2312ToUnicode(query.GetStringField(6)),
							 (time_t)atoi(query.GetStringField(1)) + CDealTime::IosStartTime().GetTime(),
							 (time_t)atoi(query.GetStringField(1)) + CDealTime::IosStartTime().GetTime(),
							 query.GetIntField(2),
							 query.GetIntField(3),
							 query.GetIntField(4),
							 query.GetIntField(0)
							 );

			wstring path = _pDevice->GetIosPicturePath()->GetIosMediaPath() + L"/" + CCodeOperation::GB2312ToUnicode(query.GetStringField(5)) + L"/";
			wstring picName = pPicture->_name;

			// 录像有原图和缩略图封面
			if ( SUFFIXMOV == picName.substr(picName.length()-4) )
			{
				picName = picName.substr(0, picName.length()-4);
				pPicture->_remotePath =  _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath() + picName + PICJPG;
				pPicture->_localPath = _pDevice->GetIosPicturePath()->GetLocal100APPLEPath() + picName + PICJPG;

				pPicture->_thumbRemotePath =  _pDevice->GetIosPicturePath()->GetIosAlbumThumbPath() + picName + THMPIC;
				pPicture->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalAlbumPath() + picName + THMPIC;	
			} 
			else
			{
				// 相册只有原图路径，缩略图在.ithmb文件里面
				pPicture->_remotePath = path + pPicture->_name;
				pPicture->_localPath = _pDevice->GetIosPicturePath()->GetLocal100APPLEPath() + pPicture->_name;
			}

			pVecPicture->push_back(pPicture);
			query.NextRow();
		}
		_SQLite->Close();
	}
	catch (CppSQLite3Exception& e)
	{
		_SQLite->Close();
		WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() );
		return false;
	}
	return true;
}

bool IosPictureSqlite4::InsertPhotoToSqlite(vector<PictureData*>* pVecPicture)
{
	CppSQLite3Query query;
	CppSQLite3Buffer strSQL;

	RETURN_FALSE_IF ( !pVecPicture || pVecPicture->empty() );

	try
	{
		RETURN_FALSE_IF ( ! OpenPhotoDB() ) ;
		PhotoDBTransaction();

		for (size_t i=0; i<pVecPicture->size(); ++i)
		{
			PictureData* pdata = pVecPicture->at(i);

			strSQL.Format("SELECT max(primaryKey) as maxKey from PhotoAlbumToPhotoJoin");
			query = _SQLite->ExecQuery(strSQL);

			int primaryKey = -1;
			if (!query.Eof())
				primaryKey = query.GetIntField(0) + 1;
			else
				primaryKey = 1;

			pdata->SetId(primaryKey);

			//--------------------插入数据至Photo表-----------------------------------

			strSQL.Format("INSERT INTO Photo (primaryKey, title, captureTime, width, height, thumbnailIndex,\
						  directory, filename) VALUES \
						  ('%d', '%s', '%f', '%d', '%d', '%d', '%s', '%s');",
						  primaryKey,
						  CCodeOperation::UnicodeToUTF_8(CFileOperation::GetFileNameNoExt(pdata->_remotePath)).c_str(),
						  (float)pdata->_createTime, pdata->_width, pdata->_height, pdata->_thumbnailIndex,
						  DIRALBUM, CCodeOperation::UnicodeToUTF_8(pdata->_name).c_str());

			_SQLite->ExecDML(strSQL);


			//----------------------插入数据至PhotoAlbumToPhotoJoin表------------------

			strSQL.Format("INSERT INTO PhotoAlbumToPhotoJoin (primaryKey, left, right, rightOrder) \
						  VALUES ('%d', '%d', '%d', '%d');",
						  primaryKey, 1, primaryKey, primaryKey*64);

			_SQLite->ExecDML(strSQL);


			//----------------------插入数据至PhotoExtras表------------------

			RETURN_FALSE_IF ( ! InsertToPhotoExtras(primaryKey, pdata->_name, pdata->_size) );

			//----------------------插入数据至AuxPhoto表------------------

			strSQL.Format("INSERT INTO AuxPhoto (primaryKey, latitude, longitude) \
						  VALUES ('%d', null, null);", primaryKey);

			_AuxSQLite->ExecDML(strSQL);
		}
		PhotoDBCommit();
		ClosePhotoDB();
	}
	catch (CppSQLite3Exception& e)
	{
		PhotoDBRollback();
		ClosePhotoDB();
		WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() );
		return false;
	}
	return true;
}

bool IosPictureSqlite4::InsertToPhotoExtras( int id, wstring name, int size )
{
	RETURN_FALSE_IF ( id<0 );
	RETURN_FALSE_IF ( name.empty() );

	//----------------------插入数据至PhotoExtras表------------------

	CppSQLite3Buffer strSQL;

	unsigned char PhotoExtras6[] = 
	{
		0x04,0x0b,0x73,0x74,0x72,0x65,0x61,0x6d,0x74,0x79,
		0x49,0x4d,0x47,0x5f,0x30,0x30,0x39,0x35,0x2e,0x4a,					// 图片类型标记
		0x50,0x47,0x86,0x86,0x86                                                                 
	};

	unsigned char PhotoExtras8[] = 
	{
		0x04,0x0b,0x73,0x74,0x72,0x65,0x61,0x6d,0x74,0x79,
		0x7b,0x43,0x47,0x53,0x69,0x7a,0x65,0x3d,0x66,0x66,					// 图片类型标记
		0x7d,0x96,0x81,0xa0,0x00,0x78,0x86,                                                     
	};

	std::string strTemp;
	map<int, string> strValues;
	map<int, string>::iterator it;

	if (_nVersion<421)
	{
		strValues[7]	= "720";
		strTemp = (char*)PhotoExtras8;
		strValues[8]	= strTemp;
		sprintf_s((char*)strTemp.c_str(), 10, "%d", size);
		strValues[9]	= strTemp;
		strValues[13]	= "0";
	}	
	strValues[1]	= DIRALBUM;
	strValues[2]	= CCodeOperation::UnicodeToUTF_8(name);
	strValues[3]	= "186918";
	strTemp = (char*)PhotoExtras6;
	strValues[6]	= strTemp;
	strValues[10]	= "1";

	// 添加一张图片要往PhotoExtras插入5-9条数据
	try
	{
		for (it=strValues.begin();it!=strValues.end();it++)
		{
			strSQL.Format("INSERT INTO PhotoExtras (foreignKey, identifier, sequence, value) \
						  VALUES ('%d', '%d', '%d', '%s');",
						  id, it->first, -1, it->second.c_str());

			_SQLite->ExecDML(strSQL);
		}
	}
	catch (CppSQLite3Exception& e)
	{
		_SQLite->Close();
		WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() );
		return false;
	}

	return true;
}

bool IosPictureSqlite4::DeletePhotoFromSqlite(vector<int>* pVecPicId)
{
	CppSQLite3Buffer strSQL;
	RETURN_FALSE_IF ( !pVecPicId || pVecPicId->empty() );

	try
	{
		RETURN_FALSE_IF ( ! OpenPhotoDB() ) ;
		PhotoDBTransaction();

		for (size_t i=0; i<pVecPicId->size(); ++i)
		{
			int pictureid = pVecPicId->at(i);
			// ------------- Photos.sqlite的操作 -----------

			strSQL.Format("DELETE FROM Photo WHERE primaryKey=%d", pictureid);
			_SQLite->ExecDML(strSQL);

			// 删除
			strSQL.Format("DELETE FROM PhotoAlbumToPhotoJoin WHERE primaryKey=%d", pictureid);
			_SQLite->ExecDML(strSQL);

			// 删除属性表
			strSQL.Format("DELETE FROM PhotoExtras WHERE foreignKey=%d", pictureid);
			_SQLite->ExecDML(strSQL);


			// ------------- PhotosAux.sqlite的操作 -----------
			strSQL.Format("DELETE FROM AuxPhoto WHERE primaryKey=%d", pictureid);
			_AuxSQLite->ExecDML(strSQL);
		}
		PhotoDBCommit();
		ClosePhotoDB();
	}
	catch (CppSQLite3Exception& e)
	{
		PhotoDBRollback();
		ClosePhotoDB();
		WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() );
		return false;
	}
	return true;
}

bool IosPictureSqlite4::UpdatePhotoToSqlite( PictureData* pdata )
{
	try
	{
		if ( ! _SQLite->Open(CCodeOperation::UnicodeToUTF_8(_pDevice->GetIosPicturePath()->GetLocalSqlitePath()).c_str()) ) 
		{
			_SQLite->Close();
			return false;
		}

		CppSQLite3Buffer strSQL;
		strSQL.Format("UPDATE Photo SET width=%d, height=%d, recordModDate=%f WHERE primaryKey=%d",
			pdata->_width, pdata->_height,
			float(pdata->_modifyTime - CDealTime::IosStartTime().GetTime()), pdata->GetId());

		_SQLite->ExecDML(strSQL);
		_SQLite->Close();
	}
	catch (CppSQLite3Exception& e)
	{
		_SQLite->Close();
		WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() );
		return false;
	}
	return true;
}

bool IosPictureSqlite4::ClosePhotoDB()
{
	_SQLite->Close();
	_AuxSQLite->Close();
	return true;
}

void IosPictureSqlite4::WriteError(wstring strMsg, int nErrorCode, std::string strErrorMsg)
{
	wostringstream woss;
	woss << strMsg + L": " << nErrorCode << L", msg = " << strErrorMsg.c_str();
	LOG_ERROR(woss.str());
}

#endif