// 
// 本文件定义ios设备的photo database文件中的各种数据(简称图库数据)
// PhotoDataBase简称Pdb, 下面的图库数据类都以Pdb开头
//
#include "stdafx.h"
#include "IosPdbData.h"

template<class T>
void DeleteVector(vector<T*>* p)
{
    for(size_t i = 0; i < p->size(); ++i)
	{
        SAFE_DELETE((*p)[i]);
	}

    p->clear();
}

PdbHead::PdbHead()
{
    memset(_name, 0, 5);
    _headSize = 0;
    _number = 0;
}

void PdbHead::Read(BinaryBufferReader* pReader, const char* pName)
{
    pReader->ReadBuffer(_name, 4);
    pReader->ReadInt32(_headSize);
    pReader->ReadInt32(_number);

    if( pName && strncmp(_name, pName, 4))
        THROW_ASSISTANT_EXCEPTION(PHOTO_DATABASE_FORMAT_ERROR);
}

void PdbHead::Write(BinaryBufferWriter* pWrite)
{
    pWrite->WriteBuffer(_name, 4);
    pWrite->WriteInt(_headSize);
    pWrite->WriteInt(_number);
}

void PdbFileHead::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhfd");

    // 12是头的长度, 56是前面12个数的总长度
    _paddingCount = _head._headSize - 12 - 56;

    for(int i=0; i < 5; ++i)
        pReader->ReadInt32(_data1to5[i]);

    pReader->ReadInt64(_data6);
    pReader->ReadInt64(_data7);

    for(int i=0; i < 5; ++i)
        pReader->ReadInt32(_data8to12[i]);

    // 将读取位置设定到文件头结束处
    pReader->AddOffset(_paddingCount);
}
void PdbFileHead::Write(BinaryBufferWriter* pWriter)
{
    _head.Write(pWriter);

    for(int i=0; i < 5; ++i)
        pWriter->WriteInt(_data1to5[i]);

    pWriter->WriteInt64(_data6);
    pWriter->WriteInt64(_data7);

    for(int i=0; i < 5; ++i)
        pWriter->WriteInt(_data8to12[i]);

    WritePadding(pWriter);
}

PdbMhafInfo::PdbMhafInfo()
{
    _paddingCount = 0;
}

void PdbMhafInfo::Read(BinaryBufferReader* pReader)
{
    _info.Read(pReader);
    if ( _info._type != PDB_INFO_MHAF )
        THROW_ASSISTANT_EXCEPTION(PHOTO_DATABASE_FORMAT_ERROR);

    _head.Read(pReader, "mhaf");
    pReader->ReadString(_padding, _head._headSize - 12);
}
void PdbMhafInfo::Write(BinaryBufferWriter* pWriter)
{
    _info.Write(pWriter);
    _head.Write(pWriter);

    // 这里不使用 WritePadding方法, 是为了将读到的内容原封不动写回去
    pWriter->WriteBuffer(_padding.c_str(), _padding.size());
}
void PdbMhafInfo::Init()
{
    _info.Init(PDB_INFO_MHAF);
    _info._head._number = _info._head._headSize + 0x60; // 总长度0x60字节

    AssignName("mhaf");
    _head._headSize = 0x60;
    _head._number = 0x3c;   // 0x60与0x3c的值是照抄2.0版程序

    _padding.clear();
    _padding.resize(0x60 - 12);   // 减去头部12字节
}

PdbImageList::~PdbImageList()
{
    DeleteVector<PdbImageItem>(&_items);
    DeleteVector<PdbImageItem>(&_newItems);
    DeleteVector<PdbImageItem>(&_removedItems);
}

void PdbImageList::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhli");
    _paddingCount = _head._headSize - 12;
    pReader->AddOffset(_paddingCount);

    for( int i = 0; i < _head._number; ++i )
    {
        PdbImageItem* pImageItem = new PdbImageItem();
        pImageItem->Read(pReader);
        _items.push_back(pImageItem);
    }
}

wstring PdbImageList::GetRandomName(wstring workFolder) 
{
    wstring dirName;    // 目录F**: 2位数字为从0~51的随机数
    wstring filename;   // 文件名XXXX: 4个大写字母
    wstring filePath;   // 文件完整路径
    do 
    {
        // 目录F**: 数字从0到51
        int number = MyStrOperation::Random(0, 51);

        wostringstream woss;
        if ( number < 10 )
            woss << L"F0" << number;
        else
            woss << L"F"  << number;
        dirName = woss.str();

        // 文件名是4个大写字母
        filename.clear();
        for(int i=0; i < 4; i++)
        {
            char c = MyStrOperation::Random('A', 'Z');
            filename.push_back(c);
        }

        filePath = workFolder + L"\\Thumbs\\" + dirName + L"\\" + filename + L".jpg";

    } while (CFileOperation::IsFileExist(filePath.c_str()));

    return L":Thumbs:" + dirName + L":" + filename + L".jpg";
}

bool PdbImageList::NeedSourceFile( THUMB_FILES* pThumbFiles ) 
{
    for(THUMB_FILES::iterator it = pThumbFiles->begin(); it != pThumbFiles->end(); ++it)
    {
        IthmbDB* pThumb = *it;
        if ( ! pThumb )
            continue;

        RETURN_TRUE_IF(pThumb->GetType() == F4005 ||
            pThumb->GetType() == F4006 || pThumb->GetType() == F4035
            );
    }
    return false;
}

void PdbImageList::Write(BinaryBufferWriter* pWriter,
                         THUMB_FILES* pThumbFiles,
                         wstring tempDir,
                         bool needMHAF)
{
    // 判断是否有原图存在
    bool needSourceFile = NeedSourceFile(pThumbFiles);

    // MHAF这个不知道有什么用，也不知道哪里需要
    // 目前认为有原图情况下都有MHAF块，假如出现反例再修改判断逻辑
    if ( ! needMHAF ) needMHAF = needSourceFile;

    BinaryBufferWriter BodyWriter;
    _head._number = _items.size() + _newItems.size();
    _head.Write(&BodyWriter);
    WritePadding(&BodyWriter);

    for( size_t i = 0; i < _items.size(); ++i )
    {
        _items[i]->Write(&BodyWriter, needMHAF);  // 写原有图片数据(不必写缩略图)
    }

    // 保存新增图片到ithmb及源文件, 并释放Image指针
    SaveToFile(pThumbFiles, needSourceFile, tempDir);

    // 写图片数据, 并移动到【原有图片】集合
    vector<PdbImageItem*>::iterator it;
    for( it = _newItems.begin(); it != _newItems.end();)
    {
        PdbImageItem* pNewItem = *it;
        pNewItem->Write(&BodyWriter, needMHAF);

        it = _newItems.erase(it);
        _items.push_back(pNewItem);
    }

    // 计算大小, 实际写入
    int bodySize;
    const char* pBody = BodyWriter.GetBuffer(bodySize);

    _listHead._head._number = bodySize + _listHead._head._headSize;
    _listHead.Write(pWriter);
    pWriter->WriteBuffer(pBody, bodySize);
}

void PdbImageList::SaveToFile(THUMB_FILES* pThumbFiles,
                              bool needSourceFile,
                              wstring workFolder) 
{
    vector<PdbImageItem*>::iterator it;
    for( it = _newItems.begin(); it != _newItems.end(); ++it)
    {
        PdbImageItem* pNewItem = *it;

        if( ! _removedItems.empty() )
        {
            PdbImageItem* pRemovedItem = *_removedItems.rbegin();
            _removedItems.pop_back();

            pNewItem->SwapChild(pRemovedItem);  // 直接使用被删除图片的保存位置信息

            SAFE_DELETE(pRemovedItem);
        }
        else
        {
            wstring newName = GetRandomName(workFolder);
            newName.push_back('\0');
            pNewItem->InitThumbs(pThumbFiles, needSourceFile ? newName : L"");
        }

        // 保存到ithmb文件
        pNewItem->WriteThumbs(pThumbFiles);

        // 需要源图的情况, 就保存源图到硬盘
        if ( needSourceFile )
        {
            int fileSize = pNewItem->SaveImage(workFolder);
            pNewItem->SetSourceFileSize(fileSize);
        }

        // 已经写到ithmb或jpg, 内存中不需要保留了
        pNewItem->ClearImage();
    }

    // 修改的情况, 写THUMB文件(源图在修改时即已生成)
    for( it = _items.begin(); it != _items.end(); ++it)
    {
        PdbImageItem* pItem = *it;
        if ( pItem->GetBitmap() )
        {
            pItem->WriteThumbs(pThumbFiles);
            pItem->ClearImage();
        }
    }
}

vector<int>* PdbImageList::Load()
{
    vector<int>* pVec = new vector<int>();

    for(size_t i = 0 ; i < _items.size(); ++i)
    {
		pVec->push_back(_items[i]->GetId());
    }

    for(size_t i = 0 ; i < _newItems.size(); ++i)
    {
		pVec->push_back(_items[i]->GetId());
    }

    return pVec;
}

void PdbImageList::Init()
{
    AssignName("mhli");
    _paddingCount = 80;
}

bool PdbImageList::Delete( int imageId )
{
    vector<PdbImageItem*>::iterator it;

    for( it = _items.begin(); it != _items.end(); ++it)
    {
        if ( (*it)->GetId() == imageId )
        {
            _removedItems.push_back(*it);
            _items.erase(it);

            // 假如此图片修改过, 它会在上传文件列表中, 去掉
            _needUpload.remove(imageId);
            return true;
        }
    }

    for( it = _newItems.begin(); it != _newItems.end(); ++it)
    {
        if ( (*it)->GetId() == imageId )
        {
            SAFE_DELETE(*it);
            _newItems.erase(it);

            // 新增的图片会在上传文件列表中, 去掉
            _needUpload.remove(imageId);
            return true;
        }
    }

    return false;
}

bool PdbImageList::Update( int imageId, Bitmap* pBitmap, wstring workFolder)
{
    PdbImageItem* pItem = GetImageItem(imageId);
    RETURN_FALSE_IF(! pItem );

    pItem->SetImage(pBitmap);

    // 记住这个文件需要上传
    _needUpload.push_back(imageId);

    // 需要源图的情况, 就保存源图到硬盘
    if ( pItem->NeedSourceFile() )
    {
        pItem->SaveImage(workFolder);
    }

    return true;
}

void PdbImageList::Add(int imageId, Bitmap* pImage)
{
    // 注: 为了重用被删除图片的缩略图位置, 此处不设置缩略
    //     图数据(即_thumbImageName为空), 待写入时再设置
    PdbImageItem* pItem = new PdbImageItem();
    pItem->Init(imageId);
    pItem->SetImage(pImage);

    _newItems.push_back(pItem);

    // 记住这个文件需要上传
    _needUpload.push_back(imageId);
}

int PdbImageList::GetThumbOffset(int id, IOS_THUMB_TYPE type, Rect& r)
{
    PdbImageItem* pItem = GetImageItem(id);
    RETURN_VALUE_IF( !pItem, -1);

    return pItem->GetThumbOffset(type, r);
}

vector<wstring> PdbImageList::GetDeleteFilePath()
{
    vector<wstring> path;
    for(size_t i = 0; i < _removedItems.size(); ++i)
    {
        path.push_back(_removedItems[i]->GetSourceFilePath());
    }
    return path;
}

list<int> PdbImageList::GetUploadFiles()
{
    list<int> imageIds;
    imageIds.insert(imageIds.begin(), _needUpload.begin(), _needUpload.end());
    return imageIds;
}

std::wstring PdbImageList::GetImageFilePath( int id )
{
    PdbImageItem* pItem = GetImageItem(id);
    RETURN_VALUE_IF( ! pItem, L"");

    return pItem->GetSourceFilePath();
}

PdbImageItem* PdbImageList::GetImageItem(int id)
{
    for(size_t i = 0; i < _items.size(); ++i )
        if ( _items[i]->GetId() == id )
            return _items[i];

    for(size_t i = 0; i < _newItems.size(); ++i )
        if ( _newItems[i]->GetId() == id )
            return _newItems[i];

    return NULL;
}

void PdbListHead::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhsd");

    int t;
    pReader->ReadInt32(t);
    _childType = (PDB_LIST_TYPE)t;

    _paddingCount = _head._headSize - 12 - 4;
    pReader->AddOffset(_paddingCount);

}

void PdbListHead::Write(BinaryBufferWriter* pWriter)
{
    _head.Write(pWriter);
    pWriter->WriteInt((int)_childType);
    WritePadding(pWriter);
}

PdbImageItem::PdbImageItem()
{
    _pImageName = NULL;
    _pBitmap = NULL;
    _hasMHAF = false;
}

PdbImageItem::~PdbImageItem()
{
    SAFE_DELETE(_pImageName);
    SAFE_DELETE(_pBitmap);

    DeleteVector<PdbImageName>(&_thumbImageName);
}

void PdbImageItem::Init(int id)
{
     AssignName("mhii");
    _head._headSize = 152;
    _paddingCount = 100;

    _childNumber = 0; // 在写入时再赋值
    _id = id;
    _songId = 0;
    _unknown1 = 0;
    _rating = 0;
    _unknown2 = 0;
    __time32_t now = (__time32_t)time(NULL);
    _originalDate = 0; //now - (__time32_t)CDealTime(1904, 1, 1).GetTime();
    _digitalDate  = 0; //now - (__time32_t)CDealTime(1904, 1, 1).GetTime();
    _souceImageSize = 0;
}

void PdbImageItem::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhii");
    _paddingCount = _head._headSize - 12 - 40;

    pReader->ReadInt32(_childNumber);
    pReader->ReadInt32(_id);
    pReader->ReadInt64(_songId);
    pReader->ReadInt32(_unknown1);
    pReader->ReadInt32(_rating);
    pReader->ReadInt32(_unknown2);

    int time; CDealTime macBegin(1904, 1, 1);
    pReader->ReadInt32(time);
    _originalDate = time + (__time32_t)macBegin.GetTime();
    pReader->ReadInt32(time);
    _digitalDate = time + (__time32_t)macBegin.GetTime();
    pReader->ReadInt32(_souceImageSize);

    pReader->AddOffset(_paddingCount);

    for( int i = 0; i < _childNumber; ++i)
    {
        PdbImageName* pImageName = new PdbImageName();//内存泄漏
        pImageName->Read(pReader);

        if ( pImageName->GetType() == PDB_INFO_THUMBNAIL )
        {
            _thumbImageName.push_back(pImageName);
        }
        else if ( pImageName->GetType() == PDB_INFO_MHAF )
        {
            SAFE_DELETE(_pImageName);
            _hasMHAF = true;
        }
        else
        {
            SAFE_DELETE(_pImageName);
            _pImageName = pImageName;
        }
    }
}

void PdbImageItem::Write(BinaryBufferWriter* pWriter, bool needMHAF)
{

    int adjustedChildNumber = _thumbImageName.size();
    if ( needMHAF || _hasMHAF)
        adjustedChildNumber ++;

    if ( _pImageName && ! _pImageName->GetFileName().empty() )
        adjustedChildNumber ++;

    BinaryBufferWriter bodyWriter;
    for( size_t i = 0; i < _thumbImageName.size(); ++i)
    {
        _thumbImageName[i]->Write(&bodyWriter);
    }

    if ( needMHAF || _hasMHAF)
    {
        PdbMhafInfo mhaf;
        mhaf.Init();
        mhaf.Write(&bodyWriter);
    }

    if ( _pImageName && ! _pImageName->GetFileName().empty() )
    {
        _pImageName->Write(&bodyWriter);
    }

    int bodySize;
    const char* pBody = bodyWriter.GetBuffer(bodySize);

    // 写头部
    _head._number = _head._headSize + bodySize;
    _head.Write(pWriter);
    pWriter->WriteInt(adjustedChildNumber);
    pWriter->WriteInt(_id);
    pWriter->WriteInt64(_songId);
    pWriter->WriteInt(_unknown1);
    pWriter->WriteInt(_rating);
    pWriter->WriteInt(_unknown2);
    pWriter->WriteInt(_originalDate);
    pWriter->WriteInt(_digitalDate);
    pWriter->WriteInt(_souceImageSize);
    WritePadding(pWriter);

    pWriter->WriteBuffer(pBody, bodySize);
}

void PdbImageItem::SetImage(Bitmap* pBitmap)
{
    SAFE_DELETE(_pBitmap);
    _pBitmap = pBitmap;
}

int PdbImageItem::GetThumbOffset( IOS_THUMB_TYPE type, Rect& r )
{
    for(size_t i = 0; i < _thumbImageName.size(); ++i )
    {
        if ( type == _thumbImageName[i]->GetCorrelationId() )
        {
            r = _thumbImageName[i]->GetImageRect();
            return _thumbImageName[i]->GetThumbnailOffset();
        }
    }
    return -1;
}

void PdbImageItem::SwapChild(PdbImageItem* pItem)
{
    _thumbImageName.swap(pItem->_thumbImageName);
    _childNumber = _thumbImageName.size();
}

void PdbImageItem::InitThumbs(THUMB_FILES* pThumbFiles,
                              wstring filename)
{
    RETURN_IF(! pThumbFiles);

    for(size_t i = 0; i < pThumbFiles->size(); ++i )
    {
        IthmbDB* pThumb = pThumbFiles->at(i);
        if ( ! pThumb )
            continue;

        IOS_THUMB_TYPE t = pThumb->GetType();
        PdbImageName* pName = new PdbImageName();

        if (t == F4005 || t == F4006 || F4035 == t)
        {
            pName->InitFile(t, filename);
            pName->SetThumbnailOffset(0);
        }
        else
        {
            pName->InitThumb(t, pThumb, _pBitmap);
            pName->SetThumbnailOffset(pThumb->GetFileSize());
        }

        _thumbImageName.push_back(pName);
    }

    _childNumber = _thumbImageName.size();
}

void PdbImageItem::WriteThumbs(THUMB_FILES* pThumbFiles)
{
    RETURN_IF(! pThumbFiles);

    THUMB_FILES::const_iterator it = pThumbFiles->begin();
    for( ; it != pThumbFiles->end(); ++it )
    {
        IthmbDB* pThumb = *it;
        if ( ! pThumb )
            continue;

        IOS_THUMB_TYPE t = pThumb->GetType();
        if ( t != F4005 && t != F4006 && t!= F4035)  // 不是保存原图
        {
            Rect unused;
            int offset = GetThumbOffset(t, unused);

            Rect r = pThumb->WriteImageOffset(_pBitmap, offset);

            PdbImageName* pName = GetImageNameById(t);
            if ( pName )
            {
                pName->SetSizeAndPadding(r);
            }
        }
    }
}


bool PdbImageItem::NeedSourceFile()
{
    for(size_t i = 0; i < _thumbImageName.size(); ++i)
    {
        if ( _thumbImageName[i]->IsSourceFileName() )
        {
            return true;
        }
    }
    return false;
}

int PdbImageItem::SaveImage(const wstring workFolder)
{
    // :Thumbs:F**:XXXX.jpg中的冒号改为反斜杠
    wstring filename = GetSourceFilePath();
    filename = workFolder + L"Thumbs\\" + PdbImageName::ConvertPath(filename, '\\');

    // 若目录不存在则创建
    wstring dirName = CFileOperation::GetFileDir(filename);
    CFileOperation::MakeLocalDir( dirName.c_str() );

    DUMP_IMAGE(_pBitmap, filename.c_str());
    return CFileOperation::GetFileSizeW(filename);
}

wstring PdbImageItem::GetSourceFilePath()
{
    for(size_t i=0; i < _thumbImageName.size(); ++i)
    {
        if ( _thumbImageName[i]->IsSourceFileName() )
            return _thumbImageName[i]->GetFileName();
    }
    return L"";
}

void PdbString::Read(BinaryBufferReader* pReader)
{
    _info.Read(pReader);
    if ( _info._type == PDB_INFO_MHAF )
    {
        pReader->AddOffset(_info._head._number - _info._head._headSize);
        return;
    }

    if ( _info._type != PDB_CATEGORY_NAME && _info._type != PDB_FILE_NAME )
        THROW_ASSISTANT_EXCEPTION(PHOTO_DATABASE_FORMAT_ERROR);

    pReader->ReadInt32(_size);
    pReader->ReadInt32(_utf);
    pReader->ReadInt32(_unknown);

    string  str;
    pReader->ReadString(str, _size);

    if ( _info._type == PDB_CATEGORY_NAME )
        _value = CCodeOperation::UTF_8ToUnicode(str);
    else if ( _info._type == PDB_FILE_NAME )
        _value.assign((wchar_t*) str.c_str(), _size/2);

    _paddingCount = _info._head._number - _info._head._headSize - 12 - _size;
    pReader->AddOffset(_paddingCount);
}

void PdbString::Write(BinaryBufferWriter* pWriter)
{
//     if ( *_value.rbegin() == '\x0' )
//     {
//         _value.erase(_value.size() - 1);
//     }

    // 生成字符串
    string s;
    if ( _info._type == PDB_CATEGORY_NAME )   // UTF-8 
    {
        _utf = 1;
        s = CCodeOperation::UnicodeToUTF_8(_value);
    }
    else if ( _info._type == PDB_FILE_NAME )   // UTF-16
    {
        _utf = 2;
        s.assign((char*) _value.c_str(), _value.size() * 2);
    }

    // 保证4字节对齐
    _paddingCount = (4 - ( s.size() % 4 )) % 4;

    // 先写到临时buffer
    BinaryBufferWriter writer;
    writer.WriteInt(s.size()/* + _paddingCount*/);
    writer.WriteInt(_utf);
    writer.WriteInt(_unknown);

    writer.WriteBuffer(s.c_str(), s.size());    // 写字符串

    writer.WriteRepeatChar(0, _paddingCount);

    // 真正开始写
    string thisData = writer.GetBuffer();
    _info._head._number = _info._head._headSize + thisData.size();
    _info._unknown2 = _paddingCount;
    _info.Write(pWriter);
    pWriter->WriteBuffer(thisData.c_str(), thisData.size());
}

void PdbString::Init(PDB_INFO_TYPE type, wstring name)
{
    _info.Init(type);
    _value = name;
	_unknown = 0;
}

void PdbObjectInfo::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhod");
    short t;
    pReader->ReadShort(t);
    _type = (PDB_INFO_TYPE)t;
    pReader->ReadChar(_unknown1);
    pReader->ReadChar(_unknown2);

    _paddingCount = _head._headSize - 12 - 4;
    pReader->AddOffset(_paddingCount);
}

void PdbObjectInfo::Write(BinaryBufferWriter* pWriter)
{
    _head.Write(pWriter);

    short t = _type;
    pWriter->WriteShort(t);
    pWriter->WriteChar(_unknown1);
    pWriter->WriteChar(_unknown2);

    WritePadding(pWriter);
}

void PdbObjectInfo::Init(PDB_INFO_TYPE type)
{
    AssignName("mhod");
    _paddingCount = 8;
    _head._headSize = 24;
    // _head._number 包含子节点长度, 写时再计算

    _type = type;
    _unknown1 = 0;
    _unknown2 = 0;
}

void PdbImageName::Init(int correlationId, int width, int height, int imageSize, int occupySize )
{
    _info.Init(PDB_INFO_THUMBNAIL);

    AssignName("mhni");
    _head._headSize = 76;
    _paddingCount = 28;

    _childNumber = 1;
    _correlationId = correlationId;
    _ithmbOffset = 0;  // 此处不赋值, 在实际写入前才赋值, 见Write方法
    _imageSize = (correlationId == 4001) ? _occupySpace : imageSize;

    _verticalPadding = 0;
    _horizontalPadding = 0;
    _imageHeight = width;
    _imageWidth = height;

    _unknown = 0;
    _occupySpace = occupySize;

}

void PdbImageName::InitThumb(int correlationId, IthmbDB* pThumbFile, Bitmap* pBmp)
{
    Init(correlationId,
        pThumbFile->GetRealWidth(),
        pThumbFile->GetRealHeight(),
        pThumbFile->GetWidth() * pThumbFile->GetHeight() * 2,
        pThumbFile->GetOccupySpace());

    _isSourceFileName = false;

    // 计算 _horizontalPadding 的值
    if ( correlationId == 3008 )
    {
        int width = pBmp->GetWidth();
        int height = pBmp->GetHeight();

        // 计算压缩率
        double xRatio = double(pBmp->GetWidth ()) / pThumbFile->GetRealWidth();
        double yRatio = double(pBmp->GetHeight()) / pThumbFile->GetRealHeight();

        if ( xRatio < yRatio ) // 横向将有留白
        {
            _imageWidth = int(width / yRatio);
            _imageHeight = int(height / yRatio);

            _horizontalPadding = (pThumbFile->GetRealWidth() - _imageWidth) / 2;
            _imageWidth += _horizontalPadding;
        }
    }

    wstring ithmbFolder = CFileOperation::GetFileDir(pThumbFile->GetFilename());
    bool hasThumbFolder = CStrOperation::endsWithW(ithmbFolder, L"Thumbs\\");
    wostringstream woss;
    if ( hasThumbFolder )
        woss << L":Thumbs";
    
    woss << L":F" << correlationId << L"_1.ithmb"; // << '\0';
    _filename.Init(PDB_FILE_NAME, woss.str());
}

wstring PdbImageName::ConvertPath(wstring path, wchar_t c)
{
    path.erase(0, 8);
    path[3] = c;
    return path;
}

void PdbImageName::InitFile(int correlationId, wstring fileName)
{
    Init(correlationId, 0, 0, 0, 0);
    _isSourceFileName = true;

    _filename.Init(PDB_FILE_NAME, fileName);
}

void PdbImageName::Read(BinaryBufferReader* pReader)
{
    _info.Read(pReader);
    if ( _info._type == PDB_INFO_MHAF )
    {
        pReader->AddOffset(_info._head._number - _info._head._headSize);
        return;
    }

    if ( _info._type != PDB_INFO_IMAGE && _info._type != PDB_INFO_THUMBNAIL )
        THROW_ASSISTANT_EXCEPTION(PHOTO_DATABASE_FORMAT_ERROR);

    _head.Read(pReader, "mhni");

    pReader->ReadInt32(_childNumber);
    pReader->ReadInt32(_correlationId);
    pReader->ReadInt32(_ithmbOffset);
    pReader->ReadInt32(_imageSize);

    pReader->ReadShort(_verticalPadding);
    pReader->ReadShort(_horizontalPadding);
    pReader->ReadShort(_imageHeight);
    pReader->ReadShort(_imageWidth);

    pReader->ReadInt32(_unknown);
    pReader->ReadInt32(_occupySpace);

    int isFullName;
    pReader->ReadInt32(isFullName);

    _isSourceFileName = isFullName == 1 || _correlationId == 4035;

    _paddingCount = _head._headSize - 12 - 36;
    pReader->AddOffset(_paddingCount);

    // 按照格式, _childNumber一定等于1, 这里是谨慎的写法
    if ( _childNumber > 0 )
    {
        _filename.Read(pReader);

        for (int i = 1; i < _childNumber; i++)
        {
            PdbString().Read(pReader);    // 丢弃其他数据
        }
    }
}


void PdbImageName::Write(BinaryBufferWriter* pWriter)
{

    // 先生成文件名数据块
    BinaryBufferWriter writer;
    _filename.Write(&writer);
    string childData = writer.GetBuffer();
    writer.Clear();

    // 再生成ImageName数据块
    _head._number = _head._headSize + childData.length();
    _head.Write(&writer);

    _childNumber = 1;   // 这个固定为1, 表示有个文件名子块
    writer.WriteInt(_childNumber);
    writer.WriteInt(_correlationId);
    writer.WriteInt(_ithmbOffset);
    writer.WriteInt(_imageSize);

    writer.WriteShort(_verticalPadding);
    writer.WriteShort(_horizontalPadding);
    writer.WriteShort(_imageHeight);
    writer.WriteShort(_imageWidth);

    writer.WriteInt(_unknown);
    writer.WriteInt(_occupySpace);
    writer.WriteInt(_isSourceFileName);

    WritePadding(&writer);
    writer.WriteBuffer(childData.c_str(), childData.size());

    string thisData = writer.GetBuffer();

    // 真正开始写
    _info._head._number = _info._head._headSize + thisData.size();
    _info.Write(pWriter);
    pWriter->WriteBuffer(thisData.c_str(), thisData.size());
}

PdbCategory::~PdbCategory()
{
    DeleteVector<PdbCategoryItem>(&_items);
}

void PdbCategory::Init( int id, wstring name )
{
    AssignName("mhba");
	_head._headSize = 0x94;
    _paddingCount = 84;

    _playlistId = id;
    _name.Init(PDB_CATEGORY_NAME, name);

    _stringNumber = 1;
    _itemNumber = 0;
    _unknown1 = 0;
    _unknown2 = 0;

    _type = 6;
    _playMusic = false;
    _repeat = false;
    _random = false;
    _showTitle = false;

    _transitionDirection = 0;
    _slideDuration = 0;
    _transitionDuration = 0;

    _unknown3 = 0;
    _unknown4 = 0;
    _trackid = 0;
    _previousPlaylistId = 0;
}

void PdbCategory::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhba");

    pReader->ReadInt32(_stringNumber);
    pReader->ReadInt32(_itemNumber);
    pReader->ReadInt32(_playlistId);
    pReader->ReadInt32(_unknown1);
    pReader->ReadShort(_unknown2);

    pReader->ReadChar(_type);
    pReader->ReadBool(_playMusic);
    pReader->ReadBool(_repeat);
    pReader->ReadBool(_random);
    pReader->ReadBool(_showTitle);

    pReader->ReadChar(_transitionDirection);
    pReader->ReadInt32(_slideDuration);
    pReader->ReadInt32(_transitionDuration);
    pReader->ReadInt32(_unknown3);
    pReader->ReadInt32(_unknown4);
    pReader->ReadInt64(_trackid);
    pReader->ReadInt32(_previousPlaylistId);

    _paddingCount = _head._headSize - 12 - 52;
    pReader->AddOffset(_paddingCount);

    for(int i = 0; i < _stringNumber; ++i)
    {
        if ( 0 == i )
            _name.Read(pReader);
        else
            PdbString().Read(pReader);  // 直接丢弃
    }

    for(int i = 0; i < _itemNumber; ++i)
    {
        PdbCategoryItem* pItem = new PdbCategoryItem();
        pItem->Read(pReader);
        _items.push_back(pItem);
    }
}

void PdbCategory::Write(BinaryBufferWriter* pWriter)
{
    // 先生成名字数据块
    BinaryBufferWriter writer;
    _name.Write(&writer);
    string nameData = writer.GetBuffer();

    // 再生成所有子节点
    writer.Clear();
    for(vector<PdbCategoryItem*>::iterator it = _items.begin();
        it != _items.end(); ++it)
    {
        (*it)->Write(&writer);
    }
    string childData = writer.GetBuffer();

    // 开始写入
    _head._number = _head._headSize + nameData.size() + childData.size();
    _head.Write(pWriter);

    pWriter->WriteInt(_stringNumber);
	_itemNumber = _items.size();
    pWriter->WriteInt(_itemNumber);
    pWriter->WriteInt(_playlistId);
    pWriter->WriteInt(_unknown1);
    pWriter->WriteShort(_unknown2);

    pWriter->WriteChar(_type);
    pWriter->WriteBool(_playMusic);
    pWriter->WriteBool(_repeat);
    pWriter->WriteBool(_random);
    pWriter->WriteBool(_showTitle);

    pWriter->WriteChar(_transitionDirection);
    pWriter->WriteInt(_slideDuration);
    pWriter->WriteInt(_transitionDuration);
    pWriter->WriteInt(_unknown3);
    pWriter->WriteInt(_unknown4);
    pWriter->WriteInt64(_trackid);
    pWriter->WriteInt(_previousPlaylistId);

    WritePadding(pWriter);

    pWriter->WriteBuffer(nameData.c_str(), nameData.size());
    pWriter->WriteBuffer(childData.c_str(), childData.size());
}

vector<int>* PdbCategory::GetImages()
{
    vector<int>* pVector = new vector<int>;

    for (size_t i = 0; i < _items.size(); ++i)
    {
        pVector->push_back(_items[i]->GetId());
    }

    return pVector;
}

void PdbCategory::Rename(wstring newName)
{
    RETURN_IF ( _name.GetName() == newName );

    _name.SetName(newName);
}

bool PdbCategory::Exist(int imageId)
{
    for (size_t i = 0; i < _items.size(); ++i)
        RETURN_TRUE_IF( _items[i]->GetId() == imageId);
    return false;
}

bool PdbCategory::DelImage(int imageId)
{
    for (vector<PdbCategoryItem*>::iterator it = _items.begin();
        it != _items.end(); ++it)
    {
        if( (*it)->GetId() == imageId)
        {
            SAFE_DELETE(*it);
            _items.erase(it);
            return true;
        }
    }

    return false;
}

bool PdbCategory::AddImage(int imageId)
{
    RETURN_FALSE_IF( Exist(imageId) );

    PdbCategoryItem* pItem = new PdbCategoryItem();
    pItem->Init(imageId);

    _items.push_back(pItem);
    return true;
}

void PdbCategoryItem::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhia");

    pReader->ReadInt32(_unknown);
    pReader->ReadInt32(_id);

    _paddingCount = _head._headSize - 12 - 8;
    pReader->AddOffset(_paddingCount);
}
void PdbCategoryItem::Write(BinaryBufferWriter* pWriter)
{
    _head._headSize = 20 + _paddingCount;
    _head._number = 20 + _paddingCount;
    _head.Write(pWriter);

    pWriter->WriteInt(_unknown);
    pWriter->WriteInt(_id);

    WritePadding(pWriter);
}

void PdbCategoryItem::Init(int id)
{
    AssignName("mhia");
    _paddingCount = 20;

    _unknown = 0;
    _id = id;
}

void PdbFileItem::Init()
{
    AssignName("mhif");
    _paddingCount = 100;

    _head._headSize = 24 + _paddingCount;
    _head._number = 24 + _paddingCount;
}

void PdbFileItem::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhif");

    pReader->ReadInt32(_unknown);
    pReader->ReadInt32(_correlationId);
    pReader->ReadInt32(_imageSize);

    _paddingCount = _head._headSize - 12 - 12;
    pReader->AddOffset(_paddingCount);
}

void PdbFileItem::Write(BinaryBufferWriter* pWriter)
{
    _head.Write(pWriter);

    pWriter->WriteInt(_unknown);
    pWriter->WriteInt(_correlationId);
    pWriter->WriteInt(_imageSize);

    WritePadding(pWriter);
}

PdbCategoryList::~PdbCategoryList() 
{
    DeleteVector<PdbCategory>(&_childs);
}

void PdbCategoryList::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhla");
    _paddingCount = _head._headSize - 12;
    pReader->AddOffset(_paddingCount);

    for(int i=0; i < _head._number; ++i)
    {
        PdbCategory* pAlbum = new PdbCategory();
        pAlbum->Read(pReader);
        _childs.push_back(pAlbum);
    }
}

void PdbCategoryList::Write(BinaryBufferWriter* pWriter)
{
    // 先将内容写到本地Buffer
    BinaryBufferWriter localWriter;
    _head._number = _childs.size();
    _head.Write(&localWriter);
    WritePadding(&localWriter);
    for(size_t i = 0; i < _childs.size(); ++i)
    {
        _childs[i]->Write(&localWriter);
    }

    // 计算长度
    int bodySize;
    const char* pBody = localWriter.GetBuffer(bodySize);
    _listHead._head._number = _listHead._head._headSize + bodySize;

    // 真正写入
    _listHead.Write(pWriter);
    pWriter->WriteBuffer(pBody, bodySize);
}

PdbCategory* PdbCategoryList::GetCategory(int id)
{
    vector<PdbCategory*>::iterator it;
    for( it = _childs.begin(); it != _childs.end(); ++it)
    {
        if ( (*it)->GetId() == id )
        {
            return *it;
        }
    }
    return NULL;
}

PdbCategory* PdbCategoryList::GetCategory(wstring name)
{
    vector<PdbCategory*>::iterator it;
    for( it = _childs.begin(); it != _childs.end(); ++it)
    {
        if ( (*it)->GetName() == name )
        {
            return *it;
        }
    }
    return NULL;
}

vector<int>* PdbCategoryList::GetCategoryImages(int id)
{
    PdbCategory* pCategory = GetCategory(id);
    RETURN_NULL_IF(! pCategory);

    return pCategory->GetImages();
}

map<int, wstring>* PdbCategoryList::GetAllCategory()
{
    map<int, wstring>* pMap = new map<int, wstring>;

    for(size_t i = 0 ; i < _childs.size(); ++i)
    {
        (*pMap)[ _childs[i]->GetId() ] = _childs[i]->GetName();
    }

    return pMap;
}

void PdbCategoryList::Rename(int id, wstring newName)
{
    PdbCategory* pCategory = GetCategory(id);
    RETURN_IF(! pCategory);

    pCategory->Rename(newName);
}

void PdbCategoryList::Add(int id, wstring name)
{
    PdbCategory* pNewChild = new PdbCategory();
    pNewChild->Init(id, name);
    _childs.push_back(pNewChild);
}

void PdbCategoryList::Del( int id )
{
    vector<PdbCategory*>::iterator it;
    for( it = _childs.begin(); it != _childs.end(); ++it)
    {
        if ( (*it)->GetId() == id )
        {
            SAFE_DELETE(*it);
            _childs.erase(it);
            return;
        }
    }
}

bool PdbCategoryList::ExistImage( int imageId )
{
    for(size_t i = 0 ; i < _childs.size(); ++i)
    {
        RETURN_TRUE_IF( _childs[i]->Exist(imageId) );
    }
    return false;
}

bool PdbCategoryList::RemoveImage( int imageId, int categoryId )
{
    PdbCategory* category = GetCategory(categoryId);
    RETURN_FALSE_IF( ! category);

    RETURN_FALSE_IF( ! category->DelImage(imageId));

    return true;
}

int PdbCategoryList::AddImage( int imageId, int categoryId )
{
    PdbCategory* category = GetCategory(categoryId);

    RETURN_VALUE_IF( ! category , 1);

    RETURN_VALUE_IF( ! category->AddImage(imageId), 2);

    return 0;
}

int PdbCategoryList::MoveImage( int imageId, int originalCategoryId, int newCategoryId )
{
    PdbCategory* originalCategory = GetCategory(originalCategoryId);
    PdbCategory* newCategory = GetCategory(newCategoryId);

    RETURN_VALUE_IF( ! originalCategory , 1);
    RETURN_VALUE_IF( ! newCategory , 2);
    RETURN_VALUE_IF( newCategory->Exist(imageId) , 3);
    RETURN_VALUE_IF( ! originalCategory->Exist(imageId), 4);

    newCategory->AddImage(imageId);
    originalCategory->DelImage(imageId);

    return 0;
}

PdbFileList::~PdbFileList() 
{
    DeleteVector<PdbFileItem>(&_childs);
}

void PdbFileList::Init()
{
    AssignName("mhlf");
    _paddingCount = 80;
    _head._headSize = 12 + _paddingCount;
}

void PdbFileList::Read(BinaryBufferReader* pReader)
{
    _head.Read(pReader, "mhlf");
    _paddingCount = _head._headSize - 12;
    pReader->AddOffset(_paddingCount);

    for( int i = 0; i < _head._number; ++i )
    {
        PdbFileItem* pFile = new PdbFileItem();
        pFile->Read(pReader);
        _childs.push_back(pFile);
    }
}

void PdbFileList::Write(BinaryBufferWriter* pWriter)
{
    _listHead.Write(pWriter);

    _head._number = _childs.size();
    _head.Write(pWriter);
    WritePadding(pWriter);

    for(vector<PdbFileItem*>::iterator it = _childs.begin();
        it != _childs.end(); ++it)
    {
        (*it)->Write(pWriter);
    }
}

map<int, int> PdbFileList::GetIdAndSize()
{
    map<int, int> thumbs;

    for(size_t i = 0; i < _childs.size(); ++i)
        thumbs[ _childs[i]->GetCorrelationId() ] = _childs[i]->GetImageSize();

    return thumbs;
}

void PdbBase::WritePadding( BinaryBufferWriter* pWriter )
{
    pWriter->WriteRepeatChar(0, _paddingCount);
}


void ND91Assistant::PdbImageItem::SetSourceFileSize(int size)
{
    for(size_t i=0; i < _thumbImageName.size(); ++i)
    {
        if ( _thumbImageName[i]->IsSourceFileName() )
        {
            _thumbImageName[i]->SetImageSize(size);
            _thumbImageName[i]->SetOccupySize(size);
            return;
        }
    }
}

PdbImageName* ND91Assistant::PdbImageItem::GetImageNameById( const int id )
{
    for(size_t i = 0; i < _thumbImageName.size(); ++i )
    {
        if ( id == _thumbImageName[i]->GetCorrelationId() )
        {
            return _thumbImageName[i];
        }
    }
    return NULL;
}

Rect ND91Assistant::PdbImageName::GetImageRect()
{
    return Rect(
        _horizontalPadding,
        _verticalPadding,
        _imageWidth - _horizontalPadding,
        _imageHeight - _verticalPadding);
}

void ND91Assistant::PdbImageName::SetSizeAndPadding( Rect r )
{
    _imageWidth         = r.X;
    _imageHeight        = r.Y;
    _horizontalPadding  = r.Width;
    _verticalPadding    = r.Height;
}

void ND91Assistant::PdbImageName::SetFileName( wstring value )
{
    _filename.SetName(value);
}

int ND91Assistant::PdbFileHead::GetNewId()
{
    return _data1to5[4]++;
}

int ND91Assistant::PdbFileHead::GetChildNumber()
{
    return _data1to5[2];
}

std::wstring ND91Assistant::PdbCategory::GetName()
{
    return _name.GetName();
}

int ND91Assistant::PdbImageList::size()
{
    return _items.size() + _newItems.size();
}
