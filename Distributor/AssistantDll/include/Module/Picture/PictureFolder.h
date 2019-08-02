#pragma once

#include "BaseGroup.h"

namespace ND91Assistant
{
	class BaseGroup;
	class AndroidPhotoBucket;
	class PictureData;
    class NodeData;
    class DeviceData;

	// ��Ƭ/��ֽ/��ͼ�������������ʹ�õ�������
    class CLASSINDLL_CLASS_DECL PictureFolder : public BaseGroup
	{
        friend class PictureManager;
		friend class AndroidPictureManager;
        friend class IosPicturesImpl;
        friend class IosPicturesImpl3;
        friend class IosPicturesImpl4;
        friend class IosPicturesImpl421;

	public:
		PictureFolder(const DeviceData* pDevice, std::wstring picPath,
						PICTURE_TYPE type);
		virtual ~PictureFolder(void);

        const vector<PictureData*>* GetPictures();

        // ����ͼƬ������
        virtual void InsertPicture(const PictureData* pic );

		// ɾ��ͼƬ
		bool DeleteIosPicture(const PictureData* pic);

        // Summary  : ���������Ƴ�һ��ͼƬ
        // Parameter: PictureData * picҪ�Ƴ���ͼƬ
        virtual void RemovePictureFromList(const PictureData* pic);

        // ����ͼƬ����
        virtual int Size() const { return _pictures.size(); }

        void Clear();

        // Summary  : ȡ�������ļ������ֺ�����ͼ���
        // Parameter: int & number �ļ�������
        // Parameter: int & index  ����ͼ���(����4.2.1����������)
        void GetMaxNumber(int& number, int& index);

    private:

        void SetBucketData(AndroidPhotoBucket* pContainer);

        // Summary  : ����һ��ͼƬ������
        // Parameter: NodeData * pNode �ڵ�����
        // Parameter: PICTURE_TYPE type ͼƬ����
        void InsertPicture(NodeData* pNode, PICTURE_TYPE type);

        // ����ͼƬ������(android)
        void InsertPictureAndroid(const PictureData* pic );

	private:
        vector<PictureData*> _pictures;  // ������ͼƬ
	};

}
