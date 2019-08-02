#include "stdafx.h"
#include "IosPhotoDB.h"

IosPhotoDB::IosPhotoDB(wstring path,
                       const vector<int>& thumbTypes,
                       wstring thumbFolder)
:_filePath(path)
{
    _images = NULL;
    _category = NULL;
    _files = NULL;
    _needMHAF = false;

    _workFolder = CFileOperation::GetFileDir(path);
    _thumbFolder = thumbFolder.empty() ? _workFolder + L"Thumbs\\" : thumbFolder;
    if ( '\\' != *_thumbFolder.rbegin() )
        _thumbFolder.push_back('\\');

    Read();
    InitThumbInfo(thumbTypes);
}

IosPhotoDB::~IosPhotoDB(void)
{
    SAFE_DELETE(_images);
    SAFE_DELETE(_category);
    SAFE_DELETE(_files);

    RELEASE_VECTOR(_thumbs);
}

void IosPhotoDB::Read()
{
    string fileData = CFileOperation::ReadFile(_filePath, true);
    BinaryBufferReader reader(fileData.c_str(), fileData.size());

    _head.Read(&reader);

    PdbListHead head;
    for(int i = 0; i < _head.GetChildNumber(); ++i)
    {
        head.Read(&reader);

        switch (head._childType)
        {
        case PDB_IMAGE:
            _images = new PdbImageList(head);
            _images->Read(&reader);
            break;

        case PDB_CATEGORY:
            _category = new PdbCategoryList(head);
            _category->Read(&reader);
            break;

        case PDB_FILE:
            _files = new PdbFileList(head);
            _files->Read(&reader);
            break;

        default:
            break;
        }
    }
}

void IosPhotoDB::Write(wstring destFile)
{
    BinaryBufferWriter bodyWriter;

    if ( _images ) _images->Write(&bodyWriter, &_thumbs, _workFolder, _needMHAF); 
    if ( _category ) _category->Write(&bodyWriter); 
    if ( _files ) _files->Write(&bodyWriter); 

    int bodySize;
    const char* body = bodyWriter.GetBuffer(bodySize);

    // 写入到buffer
    BinaryBufferWriter writer;
    _head._head._number = bodySize + _head._head._headSize;
    _head.Write(&writer);
    writer.WriteBuffer(body, bodySize);

    int fileSize;
    const char* file = writer.GetBuffer(fileSize);

    // 写到文件
    ofstream ofs(destFile.c_str(), ios_base::out|ios_base::binary);
    ofs.write(file, fileSize);
    ofs.close();
}

vector<int>* IosPhotoDB::Load( )
{
    return _images->Load();
}

IthmbDB* IosPhotoDB::GetIthmb( IOS_THUMB_TYPE type )
{
    wostringstream woss;
    woss << _thumbFolder << 'F' << int(type) << L"_1.ithmb";
    
    wstring file = woss.str();

    return new IthmbDB(type, file);
}

map<int, int> IosPhotoDB::GetThumbIdAndSize()
{
    return _files ? _files->GetIdAndSize() : map<int, int>();
}

map<int, wstring>* IosPhotoDB::GetAllCategory()
{
    return _category->GetAllCategory();
}

vector<int>* IosPhotoDB::GetCategoryImages( int id )
{
    return _category->GetCategoryImages(id);
}

void IosPhotoDB::RenameCategory( int id, wstring newName )
{
    _category->Rename(id, newName);
}

int IosPhotoDB::AddCategory( wstring name )
{
    RETURN_VALUE_IF( _category->GetCategory(name), -1);

    int id = _head.GetNewId();
    _category->Add(id, name);

    return id;
}

void IosPhotoDB::DelCategory( int id )
{
    _category->Del(id);
}

int IosPhotoDB::MoveImageBetweenCategory( int imageId, int originalCategoryId, int newCategoryId )
{
    return _category->MoveImage(imageId, originalCategoryId, newCategoryId);
}

int IosPhotoDB::AddImageToCategory( int imageId, int categoryId )
{
    return _category->AddImage(imageId, categoryId);
}

int IosPhotoDB::AddImage( Bitmap* pImage)
{
    int id = _head.GetNewId();
    _images->Add(id, pImage);

    return id;
}

bool IosPhotoDB::DelImage( int imageId, bool deleteAll)
{
    bool bRet = _images->Delete(imageId);

	if (bRet && deleteAll)
	{
		map<int, wstring>* all = _category->GetAllCategory();
		map<int, wstring>::iterator it;
		for( it = all->begin(); it != all->end(); ++it)
		{
			RemoveFromCategory(imageId, it->first);
		}
        SAFE_DELETE(all);
	}

	return bRet;
}

bool IosPhotoDB::UpdateImage( int imageId, Bitmap* pBitmap )
{
    return _images->Update(imageId, pBitmap, _workFolder);
}

bool IosPhotoDB::RemoveFromCategory( int imageId, int categoryId )
{
    return _category->RemoveImage(imageId, categoryId);
}

Bitmap* IosPhotoDB::GetImage( int id, IOS_THUMB_TYPE type )
{
    const IthmbDB* pIthmb = GetIthmb(type);
    RETURN_NULL_IF( ! pIthmb );

    Rect r;
    int offset = _images->GetThumbOffset(id, type, r);
	RETURN_NULL_IF( -1 == offset );

    return pIthmb->ReadImageByOffset(offset, r);
}

list<int> IosPhotoDB::GetUploadFiles()
{
    if ( _images->NeedSourceFile(&_thumbs) )
        return _images->GetUploadFiles();
    else
        return list<int>();
}

vector<wstring> IosPhotoDB::GetDeleteFiles()
{
    if ( ! _images->NeedSourceFile(&_thumbs) )
        return vector<wstring>();

    vector<wstring> pathVector = _images->GetDeleteFilePath();

    for(size_t i = 0; i < pathVector.size(); ++i)
    {
        pathVector[i] = PdbImageName::ConvertPath(pathVector[i], '/');
    }

    return pathVector;
}

std::wstring IosPhotoDB::GetImageFilePath(int id, char delimiter)
{
    wstring str = _images->GetImageFilePath(id);
    RETURN_VALUE_IF(str.empty(), L"");

    return PdbImageName::ConvertPath(str, delimiter);
}

void IosPhotoDB::InitThumbInfo(const vector<int>& thumbTypes) 
{
    for( size_t i = 0; i < thumbTypes.size(); ++i )
    {
        int id = thumbTypes[i];
        if ( id <=0 || id >= 10000)
            continue;

        IthmbDB* pThumb = GetIthmb((IOS_THUMB_TYPE)id);
        if ( pThumb )
        {
            _thumbs.push_back(pThumb);
        }
    }
}

vector<wstring> IosPhotoDB::GetThumbFileName()
{
    vector<wstring> filenames;
    for(THUMB_FILES::iterator it = _thumbs.begin(); it != _thumbs.end(); ++it)
    {
        IthmbDB* pThumb = *it;
        if ( ! pThumb )
            continue;
        filenames.push_back(CFileOperation::GetFileName(pThumb->GetFilename()));
    }
    return filenames;
}

int ND91Assistant::IosPhotoDB::size()
{
    return _images->size();
}
