#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/IosPictureSqlite5.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureGroup.h"
#include "Common/Path.h"

#define OPEN_DB(x)	try \
					{ \
						if ( ! _SQLdb->Open(CCodeOperation::UnicodeToUTF_8(_pDevice->GetIosPicturePath()->GetLocalSqlitePath()).c_str()) || \
							 ! InitParamsByDB() || ! InitDBTrigger()) \
						{ \
							_SQLdb->Close(); \
							ret = x; \
						} 


#define CLOSE_DB(x)	 DropDBTrigger(); \
					_SQLdb->Close(); \
					} \
					catch (CppSQLite3Exception& e) \
					{ \
						_SQLdb->Close(); \
						WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() ); \
						ret = x; \
					}


#define OPEN_TRANSACTION_DB(x)	try \
								{ \
								if ( ! _SQLdb->Open(CCodeOperation::UnicodeToUTF_8(_pDevice->GetIosPicturePath()->GetLocalSqlitePath()).c_str()) || \
								! InitParamsByDB() || ! InitDBTrigger()) \
									{ \
									_SQLdb->Close(); \
									ret = x; \
									} \
								_SQLdb->BeginTransaction();

#define CLOSE_TRANSACTION_DB(x)	 DropDBTrigger(); \
								_SQLdb->Commit(); \
								_SQLdb->Close(); \
								} \
								catch (CppSQLite3Exception& e) \
								{ \
									_SQLdb->Rollback(); \
									_SQLdb->Close(); \
									WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() ); \
									ret = x; \
								} 
								

IosPictureSqlite5::IosPictureSqlite5( const DeviceData* pDevice )
: _pDevice(pDevice)
{
	_SQLdb                          = new CppSQLite3DB;

    //_iAlbumENT =  IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion) >= 600 ? 8:6;
    //_iGenericAssetENT = IosPublicFunc::GetVerNum(_pDevice->GetDevInfo()->_deviceProductVersion) >= 600 ? 14 : 11;

    _iAlbumENT                      = 6;
    _iGenericAssetENT               = 11;
    _iGenericAlbumENT               = 5;
    _iAssetENT                      = 12;
    _iWallpaperAssetENT             = 14;
    _iAdditionalAssetAttributesEnt  = 1;
    _iAlbumListENT                  = 3;
    _bInitParams                    = false;
	_picUuids.clear();

//     _iAlbumENT                      = 8;
//     _iGenericAssetENT               = 14;
//     _iGenericAlbumENT               = 7;
//     _iAssetENT                      = 15;
//     _iWallpaperAssetENT             = 16;
//     _iAdditionalAssetAttributesEnt  = 1;
//     _iAlbumListENT                  = 3;
//     _bInitParams                    = false;
}

IosPictureSqlite5::~IosPictureSqlite5()
{
	SAFE_DELETE(_SQLdb);
}

bool IosPictureSqlite5::OpenIos5DB()
{
	bool ret = true;
	OPEN_DB(false)
	CLOSE_DB(false);
	return ret;
}

map<int, wstring>* IosPictureSqlite5::LoadIos5Category()
{
	CppSQLite3Query  query;
	map<int, wstring>* pMap = new map<int, wstring>;
	
	// ZKIND<3000 OR ZKIND>4000为所有图片分类id
	// ZKIND为1000,1504是相册分类
	map<int, wstring>* ret = NULL;
	OPEN_DB(NULL)

// 	query = _SQLdb->ExecQuery("SELECT Z_PK,ZKIND,ZTITLE FROM ZGENERICALBUM WHERE \
// 							   (ZKIND=1552 OR (Z_ENT=6 AND ZKIND<>1000 AND ZKIND<>1504 \
// 							   AND (ZKIND<3000 OR ZKIND>4000)));");				
    CppSQLite3Buffer strSQL;
    strSQL.Format("SELECT Z_PK,ZKIND,ZTITLE FROM ZGENERICALBUM WHERE \
 							   (ZKIND=1552 OR (Z_ENT=%d AND ZKIND<>1000 AND ZKIND<>1504 \
 							   AND (ZKIND<3000 OR ZKIND>4000)));", _iAlbumENT);
    query = _SQLdb->ExecQuery(strSQL);

	while (!query.Eof())
	{
		wstring title;
		int id = query.GetIntField(0);
		int kind = query.GetIntField(1);
		if ( 1552 == kind)			
			title = ALL_IMAGE_GROUP_NAME;		// 所有图库图片分类ID
		else
			title = CCodeOperation::UTF_8ToUnicode(query.GetStringField(2));
		(*pMap)[id] = title;

		query.NextRow();
	}
	query.Finalize();

	CLOSE_DB(NULL);

	return pMap;
}

int IosPictureSqlite5::AddIos5Category(const wstring name)
{
	int nCategoryID = -1;
	int ret			= -1;

	GUID guid; 
	if (::CoCreateGuid(&guid) != S_OK)
		return false;

	OPEN_DB(-1)

	CppSQLite3Buffer strSQL;
	strSQL.Format("Insert Into ZGENERICALBUM (Z_ENT,Z_OPT,ZCACHEDCOUNT,ZCACHEDPHOTOSCOUNT,ZCACHEDVIDEOSCOUNT,\
					ZKIND,ZPENDINGITEMSCOUNT,ZPENDINGITEMSTYPE,ZTITLE,ZIMPORTSESSIONID,ZUUID) \
					values (%d,1,0,0,0,2,0,1,'%s','',?);", _iAlbumENT, CCodeOperation::UnicodeToUTF_8(name).c_str() );
	_SQLdb->SaveBlob(strSQL, &guid, sizeof(GUID));

	// 找到刚插入的数据的key
	strSQL.Format("select last_insert_rowid()");
	nCategoryID = _SQLdb->ExecScalar(strSQL);

	CLOSE_DB(-1);
	return nCategoryID;
}

bool IosPictureSqlite5::RenameIos5Category(const int id, const wstring name)
{
	bool ret = true;

	OPEN_DB(false)
	CppSQLite3Buffer strSQL;
	strSQL.Format("update ZGENERICALBUM set ZTITLE='%s' where Z_PK='%d'", CCodeOperation::UnicodeToUTF_8(name).c_str(), id);
	_SQLdb->ExecDML(strSQL);
	CLOSE_DB(false);

	return ret;
}

bool IosPictureSqlite5::DeleteIos5Category(const int id, vector<int>* vDelPicId)
{
	RETURN_FALSE_IF ( id < 0 );	
	bool ret = true;
	OPEN_TRANSACTION_DB(false)

	CppSQLite3Buffer strSQL;
	for(size_t i=0; i<vDelPicId->size(); ++i )
	{
		strSQL.Format( "delete from ZGENERICASSET where Z_PK = '%d'", vDelPicId->at(i));
		_SQLdb->ExecDML(strSQL);
	}

	strSQL.Format("delete from ZGENERICALBUM where Z_PK='%d'", id);
	_SQLdb->ExecDML(strSQL);

	CLOSE_TRANSACTION_DB(false);
	return ret;
}


bool IosPictureSqlite5::LoadIos5Pictures( vector<PictureData*>* pVecPicture, bool isGallery )
{
	int nSaveType=0;
	wstring localPah, localthumbPath;
	
	if (isGallery)
	{
		nSaveType		= 256;							// 图库图片类型
		localPah		= _pDevice->GetIosPicturePath()->GetLocalPicPath();
		localthumbPath	= _pDevice->GetIosPicturePath()->GetLocalThumbnailPath();
	}
	else
	{
		nSaveType		= 3;							// 相册截屏图片类型
		localPah		= _pDevice->GetIosPicturePath()->GetLocal100APPLEPath();
		localthumbPath	= _pDevice->GetIosPicturePath()->GetLocalAlbumPath();
	}

	bool ret = true;
	OPEN_DB(false)
	CppSQLite3Query query;
	CppSQLite3Buffer strSQL;
	strSQL.Format("SELECT a.[Z_PK], a.[ZHEIGHT], a.[ZWIDTH], a.[ZTHUMBNAILINDEX], \
				  a.[ZDATECREATED], a.[ZSORTTOKEN], a.[ZUUID], a.[ZDIRECTORY], a.[ZORIENTATION],  \
				  a.[ZFILENAME], a.[ZTITLE], a.[ZIMAGEURLDATA], a.[ZTHUMBNAILURLDATA] \
				  From ZGENERICASSET a,ZADDITIONALASSETATTRIBUTES b WHERE a.[Z_PK]=b.[ZASSET] AND ZSAVEDASSETTYPE = '%d';", nSaveType);
	query = _SQLdb->ExecQuery(strSQL);

	while (!query.Eof())
	{
		PictureData* pPicture = new PictureData();

		pPicture->Create(
						CCodeOperation::GB2312ToUnicode(query.GetStringField(9)),
						(time_t)atoi(query.GetStringField(4)) + CDealTime::IosStartTime().GetTime(),
						(time_t)atoi(query.GetStringField(4)) + CDealTime::IosStartTime().GetTime(),
						query.GetIntField(2),
						query.GetIntField(1),
						query.GetIntField(3),
						query.GetIntField(0)
						);
		if ( pPicture->_name.empty() )
		{
			query.NextRow();
			continue;
		}

		wstring remotepath = _pDevice->GetIosPicturePath()->GetIosMediaPath() + L"/" + CCodeOperation::GB2312ToUnicode(query.GetStringField(7)) + L"/";	
		wstring picName = pPicture->_name.substr(0, pPicture->_name.length()-4);

		// 录像有原图和缩略图封面
		if ( SUFFIXMOV == pPicture->_name.substr(pPicture->_name.length()-4) )
		{
			pPicture->_remotePath =  _pDevice->GetIosPicturePath()->GetIos5VideoThumbPath() + picName + PICJPG;
			pPicture->_localPath = _pDevice->GetIosPicturePath()->GetLocal100APPLEPath() + picName + PICJPG;

			pPicture->_thumbRemotePath =  _pDevice->GetIosPicturePath()->GetIos5VideoThumbPath() + picName + THMPIC;
			pPicture->_thumbLocalPath = _pDevice->GetIosPicturePath()->GetLocalAlbumPath() + picName + THMPIC;	
		} 
		else
		{	
			// 相册只有原图路径，缩略图在.ithmb文件里面
			pPicture->_remotePath = remotepath + pPicture->_name;
			pPicture->_localPath = localPah + pPicture->_name;
			pPicture->_thumbLocalPath = localthumbPath + picName + THMPIC;
		}
		PICTURE_TYPE type = isGallery ? PICTURE_TYPE_NORMAL : PICTURE_TYPE_UNSET; 
		pPicture->SetType(type);

		pVecPicture->push_back(pPicture);
		query.NextRow();
	}
	query.Finalize();
	CLOSE_DB(false);	
	return ret;
}

char* IosPictureSqlite5::ConvertUuidToPChar(GUID uuidIn)
{
	char* destChar = new char[37];
	sprintf(destChar,  "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", 
		uuidIn.Data1, uuidIn.Data2, uuidIn.Data3, 
		uuidIn.Data4[0],uuidIn.Data4[1], uuidIn.Data4[2], uuidIn.Data4[3], 
		uuidIn.Data4[4], uuidIn.Data4[5], uuidIn.Data4[6], uuidIn.Data4[7]);
	destChar[36] = 0;
	return destChar;
}

vector<char*> IosPictureSqlite5::GetPictureUuids()
{
	return _picUuids;
}

void IosPictureSqlite5::ClearTempPictureUuids()
{
	_picUuids.clear();
}

bool IosPictureSqlite5::InsertIos5PicturesToDB( vector<PictureData*>* pVecPicture, std::string gruopId )
{	
	int nGid = atoi(gruopId.c_str());
	std::string strFileDir	= nGid == 0 ? DIRALBUM : DIRGALLERY;		// 相册与图库图片手机存放路径不一样
	int nSaveType			= nGid == 0 ? CAMERATYPE : GALLERYTYPE;	// 保存类型标记不一样

	bool ret = true;
	OPEN_TRANSACTION_DB(false)
	
	CppSQLite3Buffer strSQL;
	CppSQLite3Query  query;

	vector<char*> uuids; // Added to save temp uuid for new added pictures

	for (size_t i=0; i<pVecPicture->size(); ++i)
	{
		PictureData* pdata = pVecPicture->at(i);
		std::string imgType = CStrOperation::endsWithW(pdata->_name, PICJPG) ? JPGTYPE : PNGTYPE; 
		std::string picTitle = CCodeOperation::UnicodeToUTF_8(CFileOperation::GetFileNameNoExt(pdata->_remotePath)).c_str();

		GUID guid; 
		if (::CoCreateGuid(&guid) != S_OK)
			return false;

		uuids.push_back(ConvertUuidToPChar(guid));

		strSQL.Format( "Insert into ZGENERICASSET ( \
                        Z_ENT,Z_OPT,ZCOMPLETE,ZFLAGGED,\
						ZHEIGHT,ZWIDTH,ZKIND,ZORIENTATION,\
                        ZSAVEDASSETTYPE,ZTHUMBNAILINDEX,\
						ZDATECREATED,ZMODIFICATIONDATE,ZSORTTOKEN,\
                        ZDIRECTORY,ZFILENAME,ZTITLE,ZUNIFORMTYPEIDENTIFIER,ZFACERECTANGLES,\
                        ZUUID,ZLOCATIONDATA,ZIMAGEURLDATA,ZTHUMBNAILURLDATA) values ( \
                        '%d', 1, 1, 0, \
                        '%d', '%d', 0, 1, \
                        '%d', '%d',\
						'%d','%d', '%d', \
                        '%s', '%s', '%s', '%s', null, \
                        ? , null, null, null);", _iAssetENT, pdata->_height, pdata->_width, nSaveType,
						pdata->_thumbnailIndex, (int)pdata->_createTime, (int)pdata->_createTime,
						(int)pdata->_createTime, strFileDir.c_str(), CCodeOperation::UnicodeToUTF_8(pdata->_name).c_str(), 
						picTitle.c_str(), imgType.c_str() );
		_SQLdb->SaveBlob(strSQL, &guid, sizeof(GUID) );
		
		// 找到刚插入的数据的key
		strSQL.Format("select last_insert_rowid()");
		int primaryKey = _SQLdb->ExecScalar(strSQL);
		pdata->SetId(primaryKey);

		if ( primaryKey >= 0 )
		{
			strSQL.Format( "update ZADDITIONALASSETATTRIBUTES set ZORIGINALFILESIZE='%d', ZDURATION='%f', \
							ZEMBEDDEDTHUMBNAILHEIGHT=null, ZEMBEDDEDTHUMBNAILLENGTH=null, ZEMBEDDEDTHUMBNAILOFFSET=null, \
							ZEMBEDDEDTHUMBNAILWIDTH=null where ZASSET=%d;", pdata->_size, 0.0f, primaryKey);
			_SQLdb->ExecDML(strSQL);
			
			// 查询相册，截屏分类的id(ZKIND=1000 或者 ZKIND=1504的是相册截图分类)
			// 往所有图库图片分类添加照片时，不执行以下操作
			if (nGid == 0)
				strSQL.Format("select Z_PK from ZGENERICALBUM where (Z_ENT=%d AND (ZKIND=1000 OR ZKIND=1504))", _iAlbumENT);
			else
				strSQL.Format("select Z_PK from ZGENERICALBUM where Z_ENT=%d AND Z_PK='%s';", _iAlbumENT, gruopId.c_str() );

			query = _SQLdb->ExecQuery(strSQL);
			while (!query.Eof())
			{	
				// 插入表数据，将再次触发数据库触发器
				int Folderid = query.GetIntField(0);
				strSQL.Format( "Insert Into Z_%dASSETS (Z_%dALBUMS,Z_%dASSETS,Z_FOK_%dASSETS) values('%d','%d',0)", _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iGenericAssetENT, Folderid, primaryKey);
				_SQLdb->ExecDML(strSQL);	
				query.NextRow();
			}
			query.Finalize();
		}
	}

	_picUuids = uuids;

	CLOSE_TRANSACTION_DB(false);
	return ret;
}


bool IosPictureSqlite5::DropDBTrigger()
{
		CppSQLite3Buffer strSQL;
        strSQL.Format(" drop trigger  trggr_after_ins_AAssetAttri");
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format(" drop trigger  trggr_after_del_AAssetAttri");
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format(" drop trigger  trggr_after_ins_GAsset");
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format(" drop trigger  trggr_after_del_GAsset");
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format(" drop trigger  trggr_after_ins_GAlbum");
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format(" drop trigger  trggr_after_del_GAlbum");
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format(" drop trigger  trggr_after_ins_5AlbumList");
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format(" drop trigger  trggr_after_ins_6Assets");
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format(" drop trigger  trggr_after_del_6Assets");
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		return true;
}
bool IosPictureSqlite5::InitDBTrigger()
{
	try
	{
		//  对ZADDITIONALASSETATTRIBUTES的插入/删除操作的触发器
// 		std::string sql = "Create Temp Trigger trggr_after_ins_AAssetAttri AFTER INSERT On ZADDITIONALASSETATTRIBUTES FOR EACH ROW \
// 			BEGIN \
// 			update Z_PRIMARYKEY SET Z_MAX=new.Z_PK where Z_ENT=1; \
// 			update ZGENERICASSET SET ZADDITIONALATTRIBUTES=new.Z_PK where Z_PK=new.ZASSET; \
// 			END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        //  对ZADDITIONALASSETATTRIBUTES的插入/删除操作的触发器
        CppSQLite3Buffer strSQL;
        strSQL.Format("Create Temp Trigger trggr_after_ins_AAssetAttri AFTER INSERT On ZADDITIONALASSETATTRIBUTES FOR EACH ROW \
 			            BEGIN \
 			            update Z_PRIMARYKEY SET Z_MAX=new.Z_PK where Z_ENT=%d; \
 			            update ZGENERICASSET SET ZADDITIONALATTRIBUTES=new.Z_PK where Z_PK=new.ZASSET; \
 			            END;", _iAdditionalAssetAttributesEnt);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

// 		sql = "Create Temp Trigger trggr_after_del_AAssetAttri AFTER DELETE On ZADDITIONALASSETATTRIBUTES FOR EACH ROW \
// 			BEGIN \
// 			update [Z_PRIMARYKEY] SET Z_MAX=( select case \
// 			when (select count(*) from [ZADDITIONALASSETATTRIBUTES])=0 then 0 else max(Z_PK) \
// 			end as x from [ZADDITIONALASSETATTRIBUTES]) where Z_ENT=1;  \
// 			END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        strSQL.Format("Create Temp Trigger trggr_after_del_AAssetAttri AFTER DELETE On ZADDITIONALASSETATTRIBUTES FOR EACH ROW \
 			            BEGIN \
 			            update [Z_PRIMARYKEY] SET Z_MAX=( select case \
 			            when (select count(*) from [ZADDITIONALASSETATTRIBUTES])=0 then 0 else max(Z_PK) \
 			            end as x from [ZADDITIONALASSETATTRIBUTES]) where Z_ENT=%d;  \
 			            END;", _iAdditionalAssetAttributesEnt);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		//  对ZGENERICASSET的插入/删除操作的触发器
// 		sql = "Create Temp Trigger trggr_after_ins_GAsset AFTER INSERT On ZGENERICASSET FOR EACH ROW \
// 				BEGIN \
// 				Insert Into ZADDITIONALASSETATTRIBUTES (Z_ENT,Z_OPT,ZHIGHDYNAMICRANGETYPE,ZORIGINALORIENTATION,ZORIGINALHEIGHT,ZORIGINALWIDTH,ZASSET,Z11_ASSET,ZORIGINALFILENAME,ZORIGINALPATH,ZIMPORTSESSIONID,ZORIGINALASSETSUUID) \
// 				values (1,1,1,1, new.ZHEIGHT,new.ZWIDTH,new.Z_PK,new.Z_ENT,new.ZFILENAME,new.ZDIRECTORY,null,null); \
// 				update Z_PRIMARYKEY SET Z_MAX=new.Z_PK where Z_ENT=11; \
// 				update ZGENERICALBUM set \
// 				ZCACHEDVIDEOSCOUNT=(select count(*) From MAIN.ZGENERICASSET WHERE ZKIND=1  AND Z_ENT=12 AND ZSAVEDASSETTYPE=3), \
// 				ZCACHEDPHOTOSCOUNT=(select count(*) From MAIN.ZGENERICASSET WHERE ZKIND=0  AND Z_ENT=12 AND ZSAVEDASSETTYPE=3), \
// 				ZCACHEDCOUNT= (select count(*) From MAIN.ZGENERICASSET WHERE (Z_ENT=12 AND ZSAVEDASSETTYPE=3)) \
// 				WHERE (ZKIND=1000 OR ZKIND=1504); \
// 				END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        strSQL.Format("Create Temp Trigger trggr_after_ins_GAsset AFTER INSERT On ZGENERICASSET FOR EACH ROW \
 				        BEGIN \
 				        Insert Into ZADDITIONALASSETATTRIBUTES (Z_ENT,Z_OPT,ZHIGHDYNAMICRANGETYPE,ZORIGINALORIENTATION,ZORIGINALHEIGHT,ZORIGINALWIDTH,ZASSET,Z%d_ASSET,ZORIGINALFILENAME,ZORIGINALPATH,ZIMPORTSESSIONID,ZORIGINALASSETSUUID) \
 				        values (%d,1,1,1, new.ZHEIGHT,new.ZWIDTH,new.Z_PK,new.Z_ENT,new.ZFILENAME,new.ZDIRECTORY,null,null); \
 				        update Z_PRIMARYKEY SET Z_MAX=new.Z_PK where Z_ENT=11; \
 				        update ZGENERICALBUM set \
 				        ZCACHEDVIDEOSCOUNT=(select count(*) From MAIN.ZGENERICASSET WHERE ZKIND=1  AND Z_ENT=%d AND ZSAVEDASSETTYPE=3), \
 				        ZCACHEDPHOTOSCOUNT=(select count(*) From MAIN.ZGENERICASSET WHERE ZKIND=0  AND Z_ENT=%d AND ZSAVEDASSETTYPE=3), \
 				        ZCACHEDCOUNT= (select count(*) From MAIN.ZGENERICASSET WHERE (Z_ENT=%d AND ZSAVEDASSETTYPE=3)) \
 				        WHERE (ZKIND=1000 OR ZKIND=1504); \
 				        END;", _iGenericAssetENT, _iAdditionalAssetAttributesEnt, _iAssetENT, _iAssetENT, _iAssetENT);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

// 		sql = "Create Temp Trigger trggr_after_del_GAsset AFTER DELETE On ZGENERICASSET FOR EACH ROW \
// 				BEGIN\
// 				update [Z_PRIMARYKEY] SET Z_MAX=(select case when (select count(*) from [ZGENERICASSET])=0 then 0 \
// 				else max(Z_PK) end as x from [ZGENERICASSET]) where Z_ENT=11;  \
// 				delete from ZADDITIONALASSETATTRIBUTES where Z_PK=old.ZADDITIONALATTRIBUTES; \
// 				delete from Z_6ASSETS where Z_11ASSETS=old.Z_PK; \
// 				update ZGENERICALBUM set \
// 				ZCACHEDVIDEOSCOUNT=(select count(*) From MAIN.ZGENERICASSET WHERE ZKIND=1  AND Z_ENT=12 AND ZSAVEDASSETTYPE=3),\
// 				ZCACHEDPHOTOSCOUNT=(select count(*) From MAIN.ZGENERICASSET WHERE ZKIND=0  AND Z_ENT=12 AND ZSAVEDASSETTYPE=3),\
// 				ZCACHEDCOUNT= (select count(*) From MAIN.ZGENERICASSET WHERE (Z_ENT=12 AND ZSAVEDASSETTYPE=3))\
// 				WHERE (ZKIND=1000 OR ZKIND=1504);\
// 				END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        strSQL.Format("Create Temp Trigger trggr_after_del_GAsset AFTER DELETE On ZGENERICASSET FOR EACH ROW \
 				        BEGIN\
 				        update [Z_PRIMARYKEY] SET Z_MAX=(select case when (select count(*) from [ZGENERICASSET])=0 then 0 \
 				        else max(Z_PK) end as x from [ZGENERICASSET]) where Z_ENT=%d;  \
 				        delete from ZADDITIONALASSETATTRIBUTES where Z_PK=old.ZADDITIONALATTRIBUTES; \
 				        delete from Z_%dASSETS where Z_%dASSETS=old.Z_PK; \
 				        update ZGENERICALBUM set \
 				        ZCACHEDVIDEOSCOUNT=(select count(*) From MAIN.ZGENERICASSET WHERE ZKIND=1  AND Z_ENT=%d AND ZSAVEDASSETTYPE=3),\
 				        ZCACHEDPHOTOSCOUNT=(select count(*) From MAIN.ZGENERICASSET WHERE ZKIND=0  AND Z_ENT=%d AND ZSAVEDASSETTYPE=3),\
 				        ZCACHEDCOUNT= (select count(*) From MAIN.ZGENERICASSET WHERE (Z_ENT=%d AND ZSAVEDASSETTYPE=3))\
 				        WHERE (ZKIND=1000 OR ZKIND=1504);\
 				        END;", _iGenericAssetENT, _iAlbumENT, _iGenericAssetENT, _iAssetENT, _iAssetENT, _iAssetENT);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		//  对ZGENERICALBUM的插入/删除操作的触发器
// 		sql = "Create Temp Trigger trggr_after_ins_GAlbum AFTER INSERT On ZGENERICALBUM FOR EACH ROW \
// 				BEGIN \
// 				insert into Z_5ALBUMLISTS (Z_5ALBUMS,Z_3ALBUMLISTS,Z_FOK_5ALBUMS) values(new.Z_PK,-1,0); \
// 				update Z_PRIMARYKEY SET Z_MAX=new.Z_PK where Z_ENT=5; \
// 				END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        strSQL.Format("Create Temp Trigger trggr_after_ins_GAlbum AFTER INSERT On ZGENERICALBUM FOR EACH ROW \
                          BEGIN \
                          insert into Z_%dALBUMLISTS (Z_%dALBUMS,Z_%dALBUMLISTS,Z_FOK_%dALBUMS) values(new.Z_PK,-1,0); \
                          update Z_PRIMARYKEY SET Z_MAX=new.Z_PK where Z_ENT=%d; \
                          END;", _iGenericAlbumENT, _iGenericAlbumENT, _iAlbumListENT, _iGenericAlbumENT, _iGenericAlbumENT);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );


// 		sql = "Create Temp Trigger trggr_after_del_GAlbum AFTER DELETE On ZGENERICALBUM FOR EACH ROW\
// 				BEGIN \
// 				delete from Z_5ALBUMLISTS where Z_5ALBUMS=old.Z_PK; \
// 				delete from Z_6ASSETS where Z_6ALBUMS=old.Z_PK; \
// 				update Z_PRIMARYKEY SET Z_MAX=(select max(Z_PK) from ZGENERICALBUM) where Z_ENT=5; \
// 				END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        strSQL.Format("Create Temp Trigger trggr_after_del_GAlbum AFTER DELETE On ZGENERICALBUM FOR EACH ROW\
                          BEGIN \
                          delete from Z_%dALBUMLISTS where Z_%dALBUMS=old.Z_PK; \
                          delete from Z_%dASSETS where Z_%dALBUMS=old.Z_PK; \
                          update Z_PRIMARYKEY SET Z_MAX=(select max(Z_PK) from ZGENERICALBUM) where Z_ENT=%d; \
                          END;", _iGenericAlbumENT, _iGenericAlbumENT, _iAlbumENT, _iAlbumENT, _iGenericAlbumENT);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

// 		sql = "Create Temp Trigger trggr_after_ins_5AlbumList AFTER INSERT On Z_5ALBUMLISTS FOR EACH ROW\
// 				BEGIN \
// 				update Z_5ALBUMLISTS SET \
// 				Z_3ALBUMLISTS=(select Z_PK from ZALBUMLIST where ZIDENTIFIER=0), \
// 				Z_FOK_5ALBUMS=(select MAX(Z_FOK_5ALBUMS)+1024 from Z_5ALBUMLISTS where Z_3ALBUMLISTS=(select Z_PK from ZALBUMLIST where ZIDENTIFIER=0))\
// 				where Z_3ALBUMLISTS=new.Z_3ALBUMLISTS; \
// 				END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        strSQL.Format("Create Temp Trigger trggr_after_ins_5AlbumList AFTER INSERT On Z_%dALBUMLISTS FOR EACH ROW\
                      BEGIN \
                      update Z_%dALBUMLISTS SET \
                      Z_%dALBUMLISTS=(select Z_PK from ZALBUMLIST where ZIDENTIFIER=0), \
                      Z_FOK_%dALBUMS=(select MAX(Z_FOK_%dALBUMS)+1024 from Z_%dALBUMLISTS where Z_%dALBUMLISTS=(select Z_PK from ZALBUMLIST where ZIDENTIFIER=0))\
                      where Z_%dALBUMLISTS=new.Z_%dALBUMLISTS; \
                      END;", _iGenericAlbumENT, _iGenericAlbumENT, _iAlbumListENT, _iGenericAlbumENT, _iGenericAlbumENT, _iGenericAlbumENT, _iAlbumListENT, _iAlbumListENT, _iAlbumListENT);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		//  对Z_6ASSETS/插入/删除操作的触发器
// 		sql = "Create Temp Trigger trggr_after_ins_6Assets AFTER INSERT On Z_6ASSETS FOR EACH ROW \
// 				BEGIN \
// 				update ZGENERICALBUM SET \
// 				ZCACHEDCOUNT=(select count(*) from Z_6ASSETS where Z_6ALBUMS=new.Z_6ALBUMS),\
// 				ZCACHEDPHOTOSCOUNT=(select count() from ZGENERICASSET join Z_6ASSETS on Z_PK=Z_11ASSETS where (Z_6ASSETS.Z_6ALBUMS=new.Z_6ALBUMS AND ZGENERICASSET.ZKIND=0)),\
// 				ZCACHEDVIDEOSCOUNT=(select count() from ZGENERICASSET join Z_6ASSETS on Z_PK=Z_11ASSETS where (Z_6ASSETS.Z_6ALBUMS=new.Z_6ALBUMS AND ZGENERICASSET.ZKIND=1)),\
// 				ZKEYASSET = (new.Z_11ASSETS),\
// 				Z11_KEYASSET = 12\
// 				WHERE Z_PK=new.Z_6ALBUMS;\
// 				update Z_6ASSETS SET Z_FOK_11ASSETS=(SELECT MAX(Z_FOK_11ASSETS)+1024 from Z_6ASSETS WHERE Z_6ALBUMS=new.Z_6ALBUMS)\
// 				WHERE (Z_6ALBUMS=new.Z_6ALBUMS AND Z_11ASSETS=new.Z_11ASSETS);\
// 				END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        strSQL.Format("Create Temp Trigger trggr_after_ins_6Assets AFTER INSERT On Z_%dASSETS FOR EACH ROW \
                      BEGIN \
                      update ZGENERICALBUM SET \
                      ZCACHEDCOUNT=(select count(*) from Z_%dASSETS where Z_%dALBUMS=new.Z_%dALBUMS),\
                      ZCACHEDPHOTOSCOUNT=(select count() from ZGENERICASSET join Z_%dASSETS on Z_PK=Z_%dASSETS where (Z_%dASSETS.Z_%dALBUMS=new.Z_%dALBUMS AND ZGENERICASSET.ZKIND=0)),\
                      ZCACHEDVIDEOSCOUNT=(select count() from ZGENERICASSET join Z_%dASSETS on Z_PK=Z_%dASSETS where (Z_%dASSETS.Z_%dALBUMS=new.Z_%dALBUMS AND ZGENERICASSET.ZKIND=1)),\
                      ZKEYASSET = (new.Z_%dASSETS),\
                      Z%d_KEYASSET = %d\
                      WHERE Z_PK=new.Z_%dALBUMS;\
                      update Z_%dASSETS SET Z_FOK_%dASSETS=(SELECT MAX(Z_FOK_%dASSETS)+1024 from Z_%dASSETS WHERE Z_%dALBUMS=new.Z_%dALBUMS)\
                      WHERE (Z_%dALBUMS=new.Z_%dALBUMS AND Z_%dASSETS=new.Z_%dASSETS);\
                      END;", _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iGenericAssetENT, _iAssetENT, _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iGenericAssetENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iGenericAssetENT);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

// 		sql = "Create Temp Trigger trggr_after_del_6Assets AFTER DELETE On Z_6ASSETS FOR EACH ROW\
// 				BEGIN\
// 				update ZGENERICALBUM SET\
// 				ZCACHEDCOUNT=(select count(*) from Z_6ASSETS where Z_6ALBUMS=old.Z_6ALBUMS),\
// 				ZCACHEDPHOTOSCOUNT=(select count() from ZGENERICASSET join Z_6ASSETS on Z_PK=Z_11ASSETS where (Z_6ASSETS.Z_6ALBUMS=old.Z_6ALBUMS AND ZGENERICASSET.ZKIND=0)),\
// 				ZCACHEDVIDEOSCOUNT=(select count() from ZGENERICASSET join Z_6ASSETS on Z_PK=Z_11ASSETS where (Z_6ASSETS.Z_6ALBUMS=old.Z_6ALBUMS AND ZGENERICASSET.ZKIND=1)),\
// 				ZKEYASSET=(select Max(Z_11ASSETS) from Z_6ASSETS where Z_6ALBUMS=old.Z_6ALBUMS)\
// 				WHERE Z_PK=old.Z_6ALBUMS;\
// 				END;";
// 		RETURN_FALSE_IF ( _SQLdb->ExecDML(sql.c_str()) < 0 );
        strSQL.Format("Create Temp Trigger trggr_after_del_6Assets AFTER DELETE On Z_%dASSETS FOR EACH ROW\
                      BEGIN\
                      update ZGENERICALBUM SET\
                      ZCACHEDCOUNT=(select count(*) from Z_%dASSETS where Z_%dALBUMS=old.Z_%dALBUMS),\
                      ZCACHEDPHOTOSCOUNT=(select count() from ZGENERICASSET join Z_%dASSETS on Z_PK=Z_%dASSETS where (Z_%dASSETS.Z_%dALBUMS=old.Z_%dALBUMS AND ZGENERICASSET.ZKIND=0)),\
                      ZCACHEDVIDEOSCOUNT=(select count() from ZGENERICASSET join Z_%dASSETS on Z_PK=Z_%dASSETS where (Z_%dASSETS.Z_%dALBUMS=old.Z_%dALBUMS AND ZGENERICASSET.ZKIND=1)),\
                      ZKEYASSET=(select Max(Z_%dASSETS) from Z_%dASSETS where Z_%dALBUMS=old.Z_%dALBUMS)\
                      WHERE Z_PK=old.Z_%dALBUMS;\
                      END;", _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iGenericAssetENT, _iAlbumENT, _iAlbumENT, _iAlbumENT, _iAlbumENT);
        RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );
	}
	catch (CppSQLite3Exception& e)
	{
		WriteError( __FUNCTIONW__, e.ErrorCode(), e.ErrorMessage() );
		return false;
	}

	return true;
}

bool IosPictureSqlite5::DeleteIos5Pictures( vector<int>* pVecPicId )
{
	RETURN_FALSE_IF ( !pVecPicId || pVecPicId->empty() );

	bool ret = true;
	OPEN_TRANSACTION_DB(false)

	CppSQLite3Buffer strSQL;
	for (size_t i=0; i<pVecPicId->size(); ++i)
	{
		strSQL.Format( "delete from ZGENERICASSET where Z_PK = '%d'", pVecPicId->at(i));

		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );
	}

	CLOSE_TRANSACTION_DB(false);
	return ret;
}

bool IosPictureSqlite5::UpdateIos5Pictures(const PictureData* pdata )
{
	bool ret = true;	
	OPEN_TRANSACTION_DB(false)	
		
	CppSQLite3Buffer strSQL;
	strSQL.Format( "update ZGENERICASSET set ZHEIGHT='%d',ZWIDTH='%d',ZMODIFICATIONDATE='%d' where Z_PK='%d';", 
					 pdata->_height, pdata->_width, pdata->_modifyTime, pdata->GetId() );
	RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

	strSQL.Format( "update ZADDITIONALASSETATTRIBUTES set ZORIGINALHEIGHT='%d',ZORIGINALWIDTH='%d',ZORIGINALFILESIZE='%d' where ZASSET='%d';", 
					 pdata->_height, pdata->_width, pdata->_size, pdata->GetId() );
	RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

	CLOSE_TRANSACTION_DB(false);
	return ret;
}



bool IosPictureSqlite5::GetIosCategoryImages( PictureGroup* pGroup, vector<PictureData*>* pVerPictures, vector<PictureData*>& pVerDatas )
{
	RETURN_FALSE_IF ( !pGroup );
	RETURN_FALSE_IF ( !pVerPictures || pVerPictures->empty() );

	if ( ALL_IMAGE_GROUP_NAME == pGroup->GetName())
	{
		pVerDatas = *pVerPictures;
		return true;
	}

	vector<PictureData*>::iterator it;

	bool ret = true;
	OPEN_DB(false)

	CppSQLite3Buffer strSQL;
	CppSQLite3Query  query;
	strSQL.Format("select Z_%dASSETS from Z_%dASSETS where Z_%dALBUMS='%s';", _iGenericAssetENT, _iAlbumENT, _iAlbumENT, pGroup->GetId().c_str());
	query = _SQLdb->ExecQuery(strSQL);
	while (!query.Eof())
	{	
		// 找出属于该分类的所有图片
		int picId = query.GetIntField(0);
		for (it=pVerPictures->begin();it!=pVerPictures->end();++it)
		{
			PictureData* pdata = *it;
			if ( picId == pdata->GetId())
			{
				pVerDatas.push_back(*it);
			}		
		}
		query.NextRow();
	}
	query.Finalize();
	CLOSE_DB(false);

	return true;
}


bool IosPictureSqlite5::Ios5AddExistPictureToGroup( vector<int>* pVecPictureid, std::string gruopId )
{
	RETURN_FALSE_IF ( !pVecPictureid || pVecPictureid->empty() );

	bool ret = true;
	OPEN_TRANSACTION_DB(false)

	CppSQLite3Buffer strSQL;
	for (size_t i=0; i<pVecPictureid->size(); ++i)
	{
		//strSQL.Format( "Insert Into Z_6ASSETS (Z_11ASSETS, Z_6ALBUMS, Z_FOK_11ASSETS) values('%d','%s',0);", pVecPictureid->at(i), gruopId.c_str());
        strSQL.Format( "Insert Into Z_%dASSETS (Z_%dASSETS, Z_%dALBUMS, Z_FOK_%dASSETS) values('%d','%s',0);", _iAlbumENT, _iGenericAssetENT, _iAlbumENT, _iGenericAssetENT, pVecPictureid->at(i), gruopId.c_str());

		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );
	}

	CLOSE_TRANSACTION_DB(false);
	return ret;
}

bool IosPictureSqlite5::Ios5RemoveFromGroup( vector<int>* pVecPictureid, std::string gruopId )
{
	RETURN_FALSE_IF ( !pVecPictureid || pVecPictureid->empty() );

	bool ret = true;
	OPEN_TRANSACTION_DB(false)

	CppSQLite3Buffer strSQL;
	for (size_t i=0; i<pVecPictureid->size(); ++i)
	{
		//strSQL.Format( "delete from Z_6ASSETS where Z_11ASSETS='%d' AND Z_6ALBUMS='%s';", pVecPictureid->at(i), gruopId.c_str());
        strSQL.Format( "delete from Z_%dASSETS where Z_%dASSETS='%d' AND Z_%dALBUMS='%s';", _iAlbumENT, _iGenericAssetENT, pVecPictureid->at(i), _iAlbumENT, gruopId.c_str());

		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );
	}

	CLOSE_TRANSACTION_DB(false);
	return ret;
}

bool IosPictureSqlite5::Ios5MovePictures( vector<int>* pVecPictureid, std::string sourceGroupId, std::string destGroupId )
{
	RETURN_FALSE_IF ( !pVecPictureid || pVecPictureid->empty() );

	bool ret = true;
	OPEN_TRANSACTION_DB(false)

	CppSQLite3Buffer strSQL;
	for (size_t i=0; i<pVecPictureid->size(); ++i)
	{
		strSQL.Format( "delete from Z_%dASSETS where Z_%dASSETS='%d' AND Z_%dALBUMS<>'%s';", _iAlbumENT, _iGenericAssetENT, pVecPictureid->at(i), _iAlbumENT, sourceGroupId.c_str());
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );

		strSQL.Format( "update Z_%dASSETS set Z_%dALBUMS='%s' where Z_%dASSETS='%d' and Z_%dALBUMS='%s';", _iAlbumENT, _iAlbumENT, 
						destGroupId.c_str(), _iGenericAssetENT, pVecPictureid->at(i), _iAlbumENT, sourceGroupId.c_str() );
		RETURN_FALSE_IF ( _SQLdb->ExecDML(strSQL) < 0 );
	}

	CLOSE_TRANSACTION_DB(false);
	return ret;
}

void IosPictureSqlite5::WriteError(wstring strMsg, int nErrorCode, std::string strErrorMsg)
{
	wostringstream woss;
	woss << strMsg + L": " << nErrorCode << L", msg = " << strErrorMsg.c_str();
	LOG_ERROR(woss.str());
}

bool IosPictureSqlite5::InitParamsByDB()
{
    RETURN_TRUE_IF(_bInitParams);
    try
    {
        vector<string> strNames;
        strNames.push_back("Album");
        strNames.push_back("GenericAsset");
        strNames.push_back("GenericAlbum");
        strNames.push_back("Asset");
        strNames.push_back("WallpaperAsset");
        strNames.push_back("AdditionalAssetAttributes");
        strNames.push_back("AlbumList");

        string sqlCondition;
        for (size_t i = 0; i < strNames.size(); i++)
        {
            sqlCondition += "[Z_NAME]='"+strNames.at(i)+"'";
            if (i+1 < strNames.size())
                sqlCondition += " or ";
        }

        CppSQLite3Buffer strSQL;
        strSQL.Format( "select [Z_ENT],[Z_NAME] from [Z_PRIMARYKEY] where (%s);" , sqlCondition.c_str());
        CppSQLite3Query query = _SQLdb->ExecQuery(strSQL);				
        while (!query.Eof())
        {
            int z_ent = query.GetIntField(0);
            string z_name = query.GetStringField(1);
            if (z_name == strNames.at(0))
                _iAlbumENT = z_ent;
            else if (z_name == strNames.at(1))
                _iGenericAssetENT = z_ent;
            else if (z_name == strNames.at(2))
                _iGenericAlbumENT = z_ent;
            else if (z_name == strNames.at(3))
                _iAssetENT = z_ent;
            else if (z_name == strNames.at(4))
                _iWallpaperAssetENT = z_ent;
            else if (z_name == strNames.at(5))
                _iAdditionalAssetAttributesEnt = z_ent;
            else if (z_name == strNames.at(6))
                _iAlbumListENT = z_ent;

            query.NextRow();
        }
        query.Finalize();
        _bInitParams = true;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

#endif