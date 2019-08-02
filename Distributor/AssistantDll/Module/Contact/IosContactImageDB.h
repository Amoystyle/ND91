#pragma once

 namespace ND91Assistant
 {
    class IosContactImageDB : public IosDBBase
    {
    public:
        IosContactImageDB(IosDBPath iosDBPath);
        ~IosContactImageDB(void);

        // Summary  : ��ȡ������ͷ��
        // Returns  : bool �Ƿ�ɹ� 
        // Parameter: ContactData* person  ������
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool GetPhoto(ContactData* person, PhotoFormats type);

        // Summary  : �������޸�������ͷ��
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: int record_id ������ID
        // Parameter: wstring photoPath ͷ��ͼƬ��PC��·��
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool UpdatePhoto(int record_id, wstring photoPath, PhotoFormats type);

        // Summary  : ɾ��������ͷ��
        // Returns  : bool
        // Parameter: int record_id ������ID
        bool DeletePhoto(int record_id);
        
    private:

        // Summary  : ��ȡ������ͷ��(1X,2X,3X�汾)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactData* person ������
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool GetPhoto_For_1X_2X_3X(ContactData* person, PhotoFormats type);

        // Summary  : ��ȡ������ͷ��(4X�汾)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactData* person ������
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool GetPhoto_For_4X(ContactData* person, PhotoFormats type);

        // Summary  : ��ȡ������ͷ��(4X�汾Դͼ)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactData* person ������
        bool GetPhoto_For_4X_Original(ContactData* person);

        // Summary  : ��ȡ������ͷ��(4X�汾����ͼ)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: ContactData* person ������
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool GetPhoto_For_4X_Thumbnail(ContactData* person, PhotoFormats type);

        // Summary  : �������޸�������ͷ��(1X,2X,3X�汾)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: int record_id ������ID
        // Parameter: wstring photoPath ͷ��ͼƬ��PC��·��
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool UpdatePhoto_For_1X_2X_3X(int record_id, wstring photoPath, PhotoFormats type);

        // Summary  : �������޸�������ͷ��(4X�汾)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: int record_id ������ID
        // Parameter: wstring photoPath ͷ��ͼƬ��PC��·��
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool UpdatePhoto_For_4X(int record_id, wstring photoPath, PhotoFormats type);

        // Summary  : �������޸�������ͷ��(4X�汾Դͼ)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: int record_id ������ID
        // Parameter: wstring photoPath ͷ��ͼƬ��PC��·��
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool UpdatePhoto_For_4X_Original(int record_id, wstring photoPath, PhotoFormats type);

        // Summary  : �������޸�������ͷ��(4X�汾����ͼ)
        // Returns  : bool �Ƿ�ɹ�
        // Parameter: int record_id ������ID
        // Parameter: wstring photoPath ͷ��ͼƬ��PC��·��
        // Parameter: PhotoFormats type ͼƬ��ʽ����
        bool UpdatePhoto_For_4X_Thumbnail(int record_id, wstring photoPath, PhotoFormats type);

        // ���ڴ��ȡͼƬ
        Image* ImageFromMemory(const void *buf, size_t size);

        // ����ͼƬ���ڴ�
        void* ImageToMemory(Image *image, void **outbuf, size_t *size);

        // ��ȡ������
        int GetEncoderClsid(const WCHAR *format, CLSID *pClsid );

        // �����ڴ浽HGLOBAL
        bool MemoryToGlobal(const void *buf, size_t size, HGLOBAL global);

        // ��IStream���ڴ�
        bool StreamToMemory(IStream *stream, void **outbuf, size_t *size);
        
    private:

        GdiplusStartupInput _gdiplusStartupInput;
        ULONG_PTR _gdiplusToken;
    };
}