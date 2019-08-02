#pragma once

#include "Module/Picture/BaseGroup.h"

namespace ND91Assistant
{

    class BinaryBufferReader;

    // 图片分类基类(安卓专用)
    class AndroidPhotoBucket
    {
        friend class LoadPhotoBucketsCommand;
    public:
        AndroidPhotoBucket(void);
        virtual ~AndroidPhotoBucket(void);
        AndroidPhotoBucket(const AndroidPhotoBucket* pBucket);

        // Summary  : 根据路径判断分类类型
        // Returns	: ND91Assistant::PICTURE_TYPE 分类类型
        // Parameter: wstring path 分类路径
        static PICTURE_TYPE GetBucketTypeByPath(wstring path);
        bool ParseAndroidData(BinaryBufferReader& reader);

        string    _id;      // 分类Id
        wstring   _name;	// 分类名称
        wstring   _path;    // 路径
        bool      _visible; // 是否可见
    };

}
