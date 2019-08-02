#pragma once

namespace ND91Assistant
{
	class BaseData;
    class BinaryBufferReader;
    // ËõÂÔÍ¼
    class CLASSINDLL_CLASS_DECL ThumbnailData :public BaseData
    {
        friend class LoadThumbnailsCommand;
    public:
        ThumbnailData(void);
        virtual ~ThumbnailData(void);
        ThumbnailData(const ThumbnailData* pnail);

        int      _imageId;		// Í¼Æ¬id
        wstring  _remotePath;	// Í¼Æ¬Â·¾¶
        int      _width;		// Í¼Æ¬¿í
        int      _height;		// Í¼Æ¬¸ß
        wstring  _localPath;	// ´æ´¢ÔÚ±¾µØµÄÂ·¾¶

    private:
        bool ParseAndroidData(BinaryBufferReader& reader);
    };
}
