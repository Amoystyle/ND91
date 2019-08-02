#pragma once

namespace ND91Assistant
{
	class BaseData;
    class BinaryBufferReader;
    // ����ͼ
    class CLASSINDLL_CLASS_DECL ThumbnailData :public BaseData
    {
        friend class LoadThumbnailsCommand;
    public:
        ThumbnailData(void);
        virtual ~ThumbnailData(void);
        ThumbnailData(const ThumbnailData* pnail);

        int      _imageId;		// ͼƬid
        wstring  _remotePath;	// ͼƬ·��
        int      _width;		// ͼƬ��
        int      _height;		// ͼƬ��
        wstring  _localPath;	// �洢�ڱ��ص�·��

    private:
        bool ParseAndroidData(BinaryBufferReader& reader);
    };
}
