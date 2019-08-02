#pragma once

#include "Module/Picture/BaseGroup.h"

namespace ND91Assistant
{

    class BinaryBufferReader;

    // ͼƬ�������(��׿ר��)
    class AndroidPhotoBucket
    {
        friend class LoadPhotoBucketsCommand;
    public:
        AndroidPhotoBucket(void);
        virtual ~AndroidPhotoBucket(void);
        AndroidPhotoBucket(const AndroidPhotoBucket* pBucket);

        // Summary  : ����·���жϷ�������
        // Returns	: ND91Assistant::PICTURE_TYPE ��������
        // Parameter: wstring path ����·��
        static PICTURE_TYPE GetBucketTypeByPath(wstring path);
        bool ParseAndroidData(BinaryBufferReader& reader);

        string    _id;      // ����Id
        wstring   _name;	// ��������
        wstring   _path;    // ·��
        bool      _visible; // �Ƿ�ɼ�
    };

}
