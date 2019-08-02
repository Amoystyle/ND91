#include "stdafx.h"
#ifdef MODULE_CONTACT

#define PRODUCTVERSION       400               // IOS设备版本号  

IosContactImageDB::IosContactImageDB(IosDBPath iosDBPath)
: IosDBBase(iosDBPath)
{
    GdiplusStartup(&_gdiplusToken, &_gdiplusStartupInput, NULL);
}

IosContactImageDB::~IosContactImageDB(void)
{
    GdiplusShutdown(_gdiplusToken);
}

// 获取联络人头像
bool IosContactImageDB::GetPhoto(ContactData* person, PhotoFormats type)
{
    // 是否是4.0图像数据库
    if (_iosDBPath.GetProductVersion() < PRODUCTVERSION)
        return GetPhoto_For_1X_2X_3X(person, type);
    else
        return GetPhoto_For_4X(person, type);
}

bool IosContactImageDB::GetPhoto_For_1X_2X_3X(ContactData* person, PhotoFormats type)
{
    //获取一个联络人头像
    int len = 0;
    int &nLen = len;
    CppSQLite3Buffer sql;
    sql.Format("Select [format],[crop_x],[crop_y],[crop_width],[crop_height],[data] \
               From [ABImage] Where record_id= %d and format = %d;",person->_contactId, (int)type);

    CppSQLite3Query q = ExecQuery((const char*)sql);
    //Photo p;
    while (!q.Eof())
    {
        //p._format = (PhotoFormats)q.GetIntField(0);
        //p._inner_x = q.GetIntField(1);
        //p._inner_y = q.GetIntField(2);
        //p._inner_width = q.GetIntField(3);
        //p._inner_height = q.GetIntField(4);

        // 从数据库读取数据到内存
        const unsigned char * pbin = q.GetBlobField("data",nLen);
        void *temp = malloc(len);
        ::memcpy(temp,pbin,len);
        
        // 从内存生成图片
        //p._data = ImageFromMemory((const void*)temp, len);
        WCHAR szId[20] = {0};
        wstring strId = L"";
        wsprintf(szId,L"%d",person->_contactId);
        strId = szId;
        
        wstring photoPath = _iosDBPath.GetDeviceData()->GetPath()->GetLocalPimPhotoPath();
        photoPath += L"\\";
        photoPath += strId;

        if (type == ORIGINAL_PHOTO)
        {
            photoPath += L"SrcPhoto.png";
            person->_headPhoto._strSavePath = photoPath;
        }
        else if (type == MEDIUM_PHOTO)
        {
            photoPath += L"photo.png";
            person->_headPhoto._mediumPath = photoPath;
        }
        else if (type == THUMBNAIL_PHOTO)
        {
            photoPath += L"ThumbnailPhoto.png";
            person->_headPhoto._thumbnailPath = photoPath;
        }

        Image* image = ImageFromMemory((const void*)temp, len);
        if (image != NULL)
            DUMP_PNG(image,photoPath.c_str());
        free(temp);

        q.NextRow();
    }

    //return p;
    return true;
}

bool IosContactImageDB::GetPhoto_For_4X(ContactData* person, PhotoFormats type)
{
    //4.0 固件获取联络人头像
    switch (type)
    {
        case ORIGINAL_PHOTO:
            // 获取4.0 联络人头像原图
            return GetPhoto_For_4X_Original(person);
            break;
        case MEDIUM_PHOTO:
        case THUMBNAIL_PHOTO:
            // 获取4.0 联络人头像缩略图 兼容iPhone4和其他ios4 固件联络人头像缩略图方法
            return GetPhoto_For_4X_Thumbnail(person, type);
            break;
        default:
            //return Photo();
            return false;
            break;
    }
}

bool IosContactImageDB::GetPhoto_For_4X_Original(ContactData* person)
{
    int len = 0;
    int &nLen = len;
    CppSQLite3Buffer sql;
    sql.Format("SELECT [crop_x] , [crop_y] , [crop_width] , [data] \
               FROM [ABFullSizeImage] WHERE record_id = %d;",person->_contactId);

    CppSQLite3Query q = ExecQuery((const char*)sql);
    //Photo p;
    while (!q.Eof())
    {
        //p._format = ORIGINAL_PHOTO;
        //p._inner_x = q.GetIntField(0);
        //p._inner_y = q.GetIntField(1);
        //p._inner_width = q.GetIntField(2);

        // 从数据库读取数据到内存
        const unsigned char * pbin = q.GetBlobField("data",nLen);
        void *temp = malloc(len);
        ::memcpy(temp,pbin,len);

        // 从内存生成图片
        //p._data = ImageFromMemory((const void*)temp, len);
        WCHAR szId[20] = {0};
        wstring strId = L"";
        wsprintf(szId,L"%d",person->_contactId);
        strId = szId;
        
        wstring photoPath = _iosDBPath.GetDeviceData()->GetPath()->GetLocalPimPhotoPath();
        photoPath += L"\\";
        photoPath += strId;
        photoPath += L"SrcPhoto.png";
        person->_headPhoto._strSavePath = photoPath;

        Image* image = ImageFromMemory((const void*)temp, len);
        if (image != NULL)
            DUMP_PNG(image,photoPath.c_str());

        free(temp);

        q.NextRow();
    }

    //return p;
    return true;
}

bool IosContactImageDB::GetPhoto_For_4X_Thumbnail(ContactData* person, PhotoFormats type)
{
    int len = 0;
    int &nLen = len;
    CppSQLite3Buffer sql;
    sql.Format("SELECT data FROM abthumbnailimage WHERE record_id = %d AND format = CASE \
                WHEN EXISTS ( SELECT 1 FROM abthumbnailimage WHERE format = %d AND data NOTNULL AND record_id = %d ) \
                THEN %d ELSE CASE WHEN %d = 1 THEN 4 ELSE %d END END ;",person->_contactId,(int)type,person->_contactId,(int)type,(int)type,(int)type);

    CppSQLite3Query q = ExecQuery((const char*)sql);
    //Photo p;
    while (!q.Eof())
    {
        //p._format = type;

        // 从数据库读取数据到内存
        const unsigned char * pbin = q.GetBlobField("data",nLen);
        void *temp = malloc(len);
        ::memcpy(temp,pbin,len);

        // 从内存生成图片
        //p._data = ImageFromMemory((const void*)temp, len);
        WCHAR szId[20] = {0};
        wstring strId = L"";
        wsprintf(szId,L"%d",person->_contactId);
        strId = szId;

        wstring photoPath = _iosDBPath.GetDeviceData()->GetPath()->GetLocalPimPhotoPath();
        photoPath += L"\\";
        photoPath += strId;

        if (type == ORIGINAL_PHOTO)
        {
            photoPath += L"SrcPhoto.png";
            person->_headPhoto._strSavePath = photoPath;
        }
        else if (type == MEDIUM_PHOTO)
        {
            photoPath += L"photo.png";
            person->_headPhoto._mediumPath = photoPath;
        }
        else if (type == THUMBNAIL_PHOTO)
        {
            photoPath += L"ThumbnailPhoto.png";
            person->_headPhoto._thumbnailPath = photoPath;
        }

        Image* image = ImageFromMemory((const void*)temp, len);
        if (image != NULL)
            DUMP_PNG(image,photoPath.c_str());

        free(temp);

        q.NextRow();
    }

    //return p;
    return true;
}

// 新增或修改联络人头像
bool IosContactImageDB::UpdatePhoto(int record_id, wstring photoPath, PhotoFormats type)
{
    if (photoPath == L"")
        return false;

    // 是否是4.0图像数据库
    if (_iosDBPath.GetProductVersion() < PRODUCTVERSION)
        return UpdatePhoto_For_1X_2X_3X(record_id, photoPath, type);
    else
        return UpdatePhoto_For_4X(record_id, photoPath, type);
}

bool IosContactImageDB::UpdatePhoto_For_1X_2X_3X(int record_id, wstring photoPath, PhotoFormats type)
{
    // 从图片读取数据到内存
    void *outbuf = NULL;
    size_t size = 0;
    //ImageToMemory(srcImg._data, &outbuf, &size);
    Image* image = Image::FromFile(photoPath.c_str());
    ImageToMemory(image, &outbuf, &size);

    // 保存内存数据到数据库
    CppSQLite3Buffer sql;
    sql.Format("Delete From ABImage Where record_id = %d and format = %d;",record_id,(int)type);
    ExecDML((const char*)sql);
    sql.Format("Insert Into ABImage([record_id],[format],[crop_x],[crop_y],[crop_width],[crop_height],[data]) \
                Values( %d,%d,%d,%d,%d,%d,?);",
                record_id,(int)type,0,0,0,0);

    SaveBlob((const char*)sql,outbuf,size);

    return true;
}

bool IosContactImageDB::UpdatePhoto_For_4X(int record_id, wstring photoPath, PhotoFormats type)
{
    //4.0 固件联络人头像新增,修改
    switch (type)
    {
        case ORIGINAL_PHOTO:
            return UpdatePhoto_For_4X_Original(record_id, photoPath, type);
            break;
        case MEDIUM_PHOTO:
        case THUMBNAIL_PHOTO:
            return UpdatePhoto_For_4X_Thumbnail(record_id, photoPath, type);
            break;
        default:
            return false;
            break;
    }
}

bool IosContactImageDB::UpdatePhoto_For_4X_Original(int record_id, wstring photoPath, PhotoFormats type)
{
    // 从图片读取数据到内存
    void *outbuf = NULL;
    size_t size = 0;
    //ImageToMemory(srcImg._data, &outbuf, &size);
    Image* image = Image::FromFile(photoPath.c_str());
    ImageToMemory(image, &outbuf, &size);

    // 保存内存数据到数据库
    CppSQLite3Buffer sql;
    sql.Format("DELETE FROM [ABFullSizeImage] WHERE record_id = %d;",record_id);
    ExecDML((const char*)sql);
    sql.Format("INSERT INTO [ABFullSizeImage] ([record_id],[crop_x],[crop_y],[crop_width],[data]) \
               VALUES (%d,%d,%d,%d,?);",record_id,0,0,0);
    SaveBlob((const char*)sql,outbuf,size);

    return true;
}

bool IosContactImageDB::UpdatePhoto_For_4X_Thumbnail(int record_id, wstring photoPath, PhotoFormats type)
{
    int format = 0;
    if (type == THUMBNAIL_PHOTO)
        format = 0;//0 对应4.0固件缩略图
    else if (type == MEDIUM_PHOTO)
        format = 1;//1 对应4.0固件中等缩略图

    if (_iosDBPath.GetDeviceProductType() == L"iPhone3,1")
    {
        if (type == MEDIUM_PHOTO)
            format = 4;
        else if (type == THUMBNAIL_PHOTO)
            format = 0;
    }

    // 从图片读取数据到内存
    void *outbuf = NULL;
    size_t size = 0;
    //ImageToMemory(srcImg._data, &outbuf, &size);
    Image* image = Image::FromFile(photoPath.c_str());
    ImageToMemory(image, &outbuf, &size);

    // 保存内存数据到数据库
    CppSQLite3Buffer sql;
    sql.Format("DELETE FROM ABThumbnailImage WHERE record_id = %d AND format = %d;",record_id,format);
    ExecDML((const char*)sql);

    sql.Format("INSERT INTO ABThumbnailImage ([record_id],[format],[data]) \
                VALUES (%d,%d,?);",record_id,format);
    SaveBlob((const char*)sql,outbuf,size);

    return true;
}

// 删除联络人头像
bool IosContactImageDB::DeletePhoto(int record_id)
{
    CppSQLite3Buffer sql;

    // 是否是4.0图像数据库
    if (_iosDBPath.GetProductVersion() < PRODUCTVERSION)
        sql.Format("Delete From ABImage Where record_id = %d;",record_id);
    else
        sql.Format("Delete From  ABFullSizeImage Where record_id = %d;\
                    Delete From  ABThumbnailImage Where record_id = %d;",record_id, record_id);
    return ExecDML((const char*)sql) > 0 ? true : false;
}

Image* IosContactImageDB::ImageFromMemory(const void *buf, size_t size)
{
    IStream *stream = NULL;
    HGLOBAL global = ::GlobalAlloc(GMEM_MOVEABLE, size);
    if (global == NULL)
        return NULL;

    // 拷贝内存到HGLOBAL
    if (!MemoryToGlobal(buf, size, global))
    {
        ::GlobalFree( global );
        return NULL;
    }

    // 从global对象生成IStream
    if (::CreateStreamOnHGlobal(global, TRUE, &stream) != S_OK)
    {
        ::GlobalFree(global);
        return NULL;
    }

    // 从IStream创建Iamge
    Image *image = Image::FromStream(stream);
    stream->Release();

    return image;
}

void* IosContactImageDB::ImageToMemory(Image *image, void **outbuf, size_t *size)
{
    IStream *stream = NULL;
    if (::CreateStreamOnHGlobal(NULL, TRUE, &stream) != S_OK)
        return NULL;

    // 获取编码器
    ::CLSID jpgClsid;
    GetEncoderClsid(L"image/jpeg", &jpgClsid);

    // 保存image到IStream
    Status save_s = image->Save(stream, &jpgClsid);
    if (save_s != Ok)
    {
        stream->Release();
        return NULL;
    }

    // 读IStream到内存
    if (!StreamToMemory(stream, outbuf, size))
    {
        stream->Release();
        return NULL;
    }

    return *outbuf;
}

int IosContactImageDB::GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
    UINT num = 0; 
    UINT size = 0;
    GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;

    ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*)malloc(size);
    GetImageEncoders(num, size, pImageCodecInfo);
    for (UINT i = 0; i < num; ++ i)
    {
        if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[i].Clsid;
            free(pImageCodecInfo);
            return i;
        }
    }

    free(pImageCodecInfo);
    return -1;
}

bool IosContactImageDB::MemoryToGlobal(const void *buf, size_t size, HGLOBAL global)
{
    void *dest = ::GlobalLock(global);
    if( dest == NULL )
        return false;

    memcpy(dest, buf, size);
    ::GlobalUnlock(global);
    return true;
}

bool IosContactImageDB::StreamToMemory(IStream *stream, void **outbuf, size_t *size)
{
    ULARGE_INTEGER ulnSize;
    LARGE_INTEGER lnOffset;
    lnOffset.QuadPart = 0;

    // 获取IStream大小
    if (stream->Seek(lnOffset, STREAM_SEEK_END, &ulnSize) != S_OK)
        return false;

    if (stream->Seek(lnOffset, STREAM_SEEK_SET, NULL) != S_OK)
        return false;

    // 读IStream到内存
    *outbuf = malloc((size_t)ulnSize.QuadPart);
    *size = (size_t)ulnSize.QuadPart;
    ULONG bytesRead;
    if(stream->Read(*outbuf, (ULONG)ulnSize.QuadPart, &bytesRead) != S_OK)
    {
        free(*outbuf);
        return false;
    }

    return true;
}

#endif