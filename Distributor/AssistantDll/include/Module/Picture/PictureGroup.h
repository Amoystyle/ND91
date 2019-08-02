#pragma once

#include "BaseGroup.h"
#include <list>
#include <vector>
using namespace std;

namespace ND91Assistant
{
    // ͼ�����
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

        // Summary  : ���Ѵ���ͼ���е�ͼƬ��ӵ�������
        void InsertPicture(int id) const;

        // ���÷�������
        void SetBucketData(const AndroidPhotoBucket* pContainer);

		// �Ƿ����ָ��ͼƬ
		bool Contains(int id) const;

    private:
        PictureLibrary*     _pLibrary;     // ������ͼƬ����
        mutable list<int>   _pictures;     // �����������ͼƬid
    };

}
