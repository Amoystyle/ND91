#pragma once

#include "BaseGroup.h"
#include <list>
#include <vector>
using namespace std;

namespace ND91Assistant
{
    // 图库分类
    class DeviceData;
    class PictureLibrary;
    class AndroidPhotoBucket;

    class CLASSINDLL_CLASS_DECL PictureGroup : public BaseGroup
    {
		friend class IosPictureManager;
		friend class AndroidPictureManager;
        friend class PictureManager;

    public:
        PictureGroup(const DeviceData*	pDevice, PictureLibrary* pLibrary);
        virtual ~PictureGroup(void);

        void GetPictures(vector<PictureData*>* pVector) const ;

        virtual int  Size() const  { return _pictures.size(); }
        virtual void InsertPicture(const PictureData* pData);
        virtual void RemovePictureFromList(const PictureData* pic);

    private:

        const list<int>* GetPictures() const { return &_pictures; }

        // Summary  : 将已存在图库中的图片添加到本分类
        void InsertPicture(int id) const;

        // 设置分类数据
        void SetBucketData(const AndroidPhotoBucket* pContainer);

		// 是否包含指定图片
		bool Contains(int id) const;

    private:
        PictureLibrary*     _pLibrary;     // 真正的图片数据
        mutable list<int>   _pictures;     // 本分类包含的图片id
    };

}
