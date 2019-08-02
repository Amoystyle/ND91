#pragma once

#include "BaseGroup.h"

namespace ND91Assistant
{
	class BaseGroup;
	class AndroidPhotoBucket;
	class PictureData;
    class NodeData;
    class DeviceData;

	// 照片/壁纸/截图这三种特殊分类使用的容器类
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

        // 插入图片到容器
        virtual void InsertPicture(const PictureData* pic );

		// 删除图片
		bool DeleteIosPicture(const PictureData* pic);

        // Summary  : 从容器中移除一张图片
        // Parameter: PictureData * pic要移除的图片
        virtual void RemovePictureFromList(const PictureData* pic);

        // 包含图片数量
        virtual int Size() const { return _pictures.size(); }

        void Clear();

        // Summary  : 取得最大的文件名数字和缩略图序号
        // Parameter: int & number 文件名数字
        // Parameter: int & index  缩略图编号(仅对4.2.1以上有意义)
        void GetMaxNumber(int& number, int& index);

    private:

        void SetBucketData(AndroidPhotoBucket* pContainer);

        // Summary  : 插入一张图片到队列
        // Parameter: NodeData * pNode 节点数据
        // Parameter: PICTURE_TYPE type 图片类型
        void InsertPicture(NodeData* pNode, PICTURE_TYPE type);

        // 插入图片到容器(android)
        void InsertPictureAndroid(const PictureData* pic );

	private:
        vector<PictureData*> _pictures;  // 包含的图片
	};

}
