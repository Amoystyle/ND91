#pragma once

 namespace ND91Assistant
 {
    class IosContactImageDB : public IosDBBase
    {
    public:
        IosContactImageDB(IosDBPath iosDBPath);
        ~IosContactImageDB(void);

        // Summary  : 获取联络人头像
        // Returns  : bool 是否成功 
        // Parameter: ContactData* person  联络人
        // Parameter: PhotoFormats type 图片格式类型
        bool GetPhoto(ContactData* person, PhotoFormats type);

        // Summary  : 新增或修改联络人头像
        // Returns  : bool 是否成功
        // Parameter: int record_id 联络人ID
        // Parameter: wstring photoPath 头像图片在PC的路径
        // Parameter: PhotoFormats type 图片格式类型
        bool UpdatePhoto(int record_id, wstring photoPath, PhotoFormats type);

        // Summary  : 删除联络人头像
        // Returns  : bool
        // Parameter: int record_id 联络人ID
        bool DeletePhoto(int record_id);
        
    private:

        // Summary  : 获取联络人头像(1X,2X,3X版本)
        // Returns  : bool 是否成功
        // Parameter: ContactData* person 联络人
        // Parameter: PhotoFormats type 图片格式类型
        bool GetPhoto_For_1X_2X_3X(ContactData* person, PhotoFormats type);

        // Summary  : 获取联络人头像(4X版本)
        // Returns  : bool 是否成功
        // Parameter: ContactData* person 联络人
        // Parameter: PhotoFormats type 图片格式类型
        bool GetPhoto_For_4X(ContactData* person, PhotoFormats type);

        // Summary  : 获取联络人头像(4X版本源图)
        // Returns  : bool 是否成功
        // Parameter: ContactData* person 联络人
        bool GetPhoto_For_4X_Original(ContactData* person);

        // Summary  : 获取联络人头像(4X版本缩略图)
        // Returns  : bool 是否成功
        // Parameter: ContactData* person 联络人
        // Parameter: PhotoFormats type 图片格式类型
        bool GetPhoto_For_4X_Thumbnail(ContactData* person, PhotoFormats type);

        // Summary  : 新增或修改联络人头像(1X,2X,3X版本)
        // Returns  : bool 是否成功
        // Parameter: int record_id 联络人ID
        // Parameter: wstring photoPath 头像图片在PC的路径
        // Parameter: PhotoFormats type 图片格式类型
        bool UpdatePhoto_For_1X_2X_3X(int record_id, wstring photoPath, PhotoFormats type);

        // Summary  : 新增或修改联络人头像(4X版本)
        // Returns  : bool 是否成功
        // Parameter: int record_id 联络人ID
        // Parameter: wstring photoPath 头像图片在PC的路径
        // Parameter: PhotoFormats type 图片格式类型
        bool UpdatePhoto_For_4X(int record_id, wstring photoPath, PhotoFormats type);

        // Summary  : 新增或修改联络人头像(4X版本源图)
        // Returns  : bool 是否成功
        // Parameter: int record_id 联络人ID
        // Parameter: wstring photoPath 头像图片在PC的路径
        // Parameter: PhotoFormats type 图片格式类型
        bool UpdatePhoto_For_4X_Original(int record_id, wstring photoPath, PhotoFormats type);

        // Summary  : 新增或修改联络人头像(4X版本缩略图)
        // Returns  : bool 是否成功
        // Parameter: int record_id 联络人ID
        // Parameter: wstring photoPath 头像图片在PC的路径
        // Parameter: PhotoFormats type 图片格式类型
        bool UpdatePhoto_For_4X_Thumbnail(int record_id, wstring photoPath, PhotoFormats type);

        // 从内存读取图片
        Image* ImageFromMemory(const void *buf, size_t size);

        // 保存图片到内存
        void* ImageToMemory(Image *image, void **outbuf, size_t *size);

        // 获取编码器
        int GetEncoderClsid(const WCHAR *format, CLSID *pClsid );

        // 拷贝内存到HGLOBAL
        bool MemoryToGlobal(const void *buf, size_t size, HGLOBAL global);

        // 读IStream到内存
        bool StreamToMemory(IStream *stream, void **outbuf, size_t *size);
        
    private:

        GdiplusStartupInput _gdiplusStartupInput;
        ULONG_PTR _gdiplusToken;
    };
}