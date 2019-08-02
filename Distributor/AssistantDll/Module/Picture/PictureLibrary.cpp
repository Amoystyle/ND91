#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/PictureLibrary.h"
#include "Module/Picture/PictureData.h"


void ND91Assistant::PictureLibrary::Clear()
{
    map<int, PictureData*>::iterator it = _library.begin();
    while( it != _library.end() )
    {
        SAFE_DELETE(it->second);
        it ++;
    }
    _library.clear();
}

PictureLibrary::~PictureLibrary()
{
    Clear();
}

void PictureLibrary::Add( const PictureData* pData )
{
    RETURN_IF( ! pData );

    PictureData* pNewData = new PictureData(*pData);

    _library[pData->GetId()] = pNewData;
}

void PictureLibrary::CreatePictureData( int pictureId, string groupId )
{
    RETURN_IF( groupId.empty() );

    // 若id已经存在, 则不创建对象
    RETURN_IF ( _library.find(pictureId) != _library.end() );

    PictureData* pData = new PictureData;
    pData->SetId(pictureId);
    pData->SetType(PICTURE_TYPE_NORMAL);
    pData->SetCategoryId(groupId);

    _library[pData->GetId()] = pData;
}

void PictureLibrary::Del(const PictureData* pData )
{
    RETURN_IF( ! pData );
    Del(pData->GetId());
}

PictureData* PictureLibrary::Get( int id )
{
    map<int, PictureData*>::iterator it = _library.find(id);
    RETURN_NULL_IF( it == _library.end() );

    return it->second;
}

void PictureLibrary::GetAllPictures(vector<PictureData*>* pDeque ) const
{
    map<int, PictureData*>::const_iterator it = _library.begin();
    while( it != _library.end() )
    {
        pDeque->push_back(it->second);
        it ++;
    }
}

void PictureLibrary::Del( int id )
{
    map<int, PictureData*>::iterator it = _library.find(id);
    RETURN_IF( it == _library.end() );

    SAFE_DELETE(it->second);
    _library.erase(it);
}


void PictureLibrary::GetMaxFileData(int& number, int& thumbIndex) 
{
	const PictureData* pPicture = NULL;

	number = 0;
	thumbIndex = 0;
	map<int, PictureData*>::const_iterator it;
	for (it=_library.begin();it!=_library.end();++it)
	{
		pPicture = it->second;
		if ( ! pPicture ) continue;

		wstring name = pPicture->_name;
		
		if ( name.empty() || (int)name.find(IMGTAG)<0  )
			break;
		
		name = name.substr(4);			// 去掉照片前面的"img_"

		int n = _wtoi(name.c_str());
		if ( n > number )
			number = n;

		if ( pPicture->_thumbnailIndex > thumbIndex)
			thumbIndex = pPicture->_thumbnailIndex;
	}

}

#endif