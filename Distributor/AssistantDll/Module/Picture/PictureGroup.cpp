#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/PictureGroup.h"
#include "Module/Picture/AndroidPhotoBucket.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureLibrary.h"

PictureGroup::PictureGroup(const DeviceData* pDevice, PictureLibrary* pLibrary)
: BaseGroup(pDevice)
{
	_pLibrary = pLibrary;
    _type = PICTURE_TYPE_NORMAL;
}

void PictureGroup::SetBucketData(const AndroidPhotoBucket* pContainer )
{
    _id = pContainer->_id;
    _name = pContainer->_name;
    _path = pContainer->_path;
    _visible = pContainer->_visible;
}

PictureGroup::~PictureGroup(void)
{
}


void ND91Assistant::PictureGroup::RemovePictureFromList(const PictureData* pic)
{
    _pictures.remove(pic->GetId());
}

void ND91Assistant::PictureGroup::GetPictures( vector<PictureData*>* pVector ) const 
{
    for(list<int>::iterator it = _pictures.begin(); it != _pictures.end(); ++it)
    {
        PictureData* pData = _pLibrary->Get(*it);
        if ( pData )
        {
            pVector->push_back(pData);
        }
    }
}

void ND91Assistant::PictureGroup::InsertPicture(const PictureData* pData ) 
{
    InsertPicture(pData->GetId());
}

void ND91Assistant::PictureGroup::InsertPicture( int id ) const 
{
    _pictures.push_back(id);
}

bool ND91Assistant::PictureGroup::Contains( int id ) const 
{
    return _pictures.end() != find(_pictures.begin(), _pictures.end(), id);
}


#endif