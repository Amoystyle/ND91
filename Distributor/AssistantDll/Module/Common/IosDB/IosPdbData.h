// 
// ���ļ�����ios�豸��photo database�ļ��еĸ�������(���ͼ������)
// PhotoDataBase���Pdb, �����ͼ�������඼��Pdb��ͷ
//
#pragma  once

#include <string>
#include <vector>
#include <map>
#include <list>

#include "AssistantDll.h"
#include "Module/Common/IosDB/IthmbDB.h"
#include "Common/BinaryBufferReader.h"
#include "Common/BinaryBufferWriter.h"

using namespace std;

#define AssignName(x)   strncpy_s(_head._name, 5, x, 4);

namespace Gdiplus
{
    class Bitmap;
}

namespace ND91Assistant
{

    typedef vector<IthmbDB*> THUMB_FILES;

#pragma region �������ļ�ͷ

    // ͼ�����ݵ�ͨ��ͷ������
    class PdbHead
    {
    public:
        char    _name[5];   // ����: 4��ASCII�ַ�
        int     _headSize;  // ͷ������
        int     _number;    // ����

        PdbHead();
        void Read(BinaryBufferReader* pReader, const char* pName);
        void Write(BinaryBufferWriter* pWrite);
    };

    // ͼ��������Ĺ�ͨ����
    class PdbBase
    {
    public:
        PdbBase() { _paddingCount = 0; }
        PdbHead _head;              // ͷ��
        int     _paddingCount;      // �������

    protected:
        void WritePadding(BinaryBufferWriter* pWriter);
    };

    enum PDB_LIST_TYPE {
        PDB_IMAGE = 1,      // Image List
        PDB_CATEGORY = 2,   // Category List
        PDB_FILE  = 3,      // File List
    };

    // �ӿ�����: ����ͷ����������, ���ͺ������ӿ�����
    class PdbListHead : public PdbBase
    {
    public:
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        PDB_LIST_TYPE  _childType;
    };


    // ͼ���ļ�ͷ
    class PdbFileHead : public PdbBase
    {
    public:
        void Read(BinaryBufferReader* pReader);

        // �ӽڵ�����
        int GetChildNumber();

        // Summary  : �����������ļ�ʱʹ�õ�ID, ID��1
        // Returns  : ��ID
        int GetNewId();

        void Write(BinaryBufferWriter* pWriter);

    private:
        int     _data1to5[5];          // 5�� int
        __int64 _data6;             // 1�� int64
        __int64 _data7;             // 1�� int64
        int     _data8to12[5];         // 5�� int
    };

#pragma  endregion

#pragma region ͼƬ��ϸ��Ϣ

    enum PDB_INFO_TYPE
    {
        PDB_CATEGORY_NAME   = 1,    // Album name 
        PDB_INFO_THUMBNAIL  = 2,    // Thumbnail image 
        PDB_FILE_NAME       = 3,    // File name 
        PDB_INFO_IMAGE      = 5,    // Full Resolution image 
        PDB_INFO_MHAF       = 6,    // unknown
    };

    // ͼƬ��Ϣ
    class PdbObjectInfo : public PdbBase
    {
    public:
        PdbObjectInfo() {  }

        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);
        void Init(PDB_INFO_TYPE type);

        PDB_INFO_TYPE   _type;

        // this field contains the number of padding bytes added (e.g. 0, 1, 2 or 3).
        // WARNING! This field was always set to 2 for a while.
        // To avoid parser crash, the best way is to ignore it when parsing. 
        char _unknown2;

    private:
        char _unknown1;     // unknown, always 0 so far
    };

    // �ַ���
    class PdbString
    {
    public:
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        wstring GetName() { return _value; }
        void SetName(wstring newName) { _value = newName; }

        // Summary  : ��ʼ���ַ���
        // Parameter: PDB_INFO_TYPE type ֻ����PDB_CATEGORY_NAME/PDB_FILE_NAME��������
        // Parameter: wstring name �ַ�����ֵ
        void Init(PDB_INFO_TYPE type, wstring name);

    private:
        PdbObjectInfo _info;        // Data Objectͷ

        int     _size;      // length in bytes of the string 
        int     _utf;       // 1 == UTF-8; 2 == UTF-16
        int     _unknown;
        wstring _value;     // string content

        int     _paddingCount;  // ����ַ���(δ�̳�PdbBase�����ж���)
    };

    class PdbImageName : public PdbBase
    {
    public:
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        // Summary  : ��ʼ��Ϊ��ͨ����ͼ����
        // Parameter: int correlationId ����ͼ����
        // Parameter: IthmbDB * pThumbFile ����ͼʵ������ָ��
        // Parameter: Bitmap* pBmp ������ͼƬ(��Ҫ��ȡ������Ϣ)
        void InitThumb(int correlationId, IthmbDB* pThumbFile, Bitmap* pBmp);

        // Summary  : ��ʼ��ΪԭͼƬ�ļ�����
        // Parameter: int correlationId ����ͼ����
        // Parameter: wstring fileName �ļ���(:Thumbs:F**:XXXX.JPG��ʽ)
        void InitFile(int correlationId, wstring fileName);

        PDB_INFO_TYPE GetType() { return _info._type; }
        bool IsSourceFileName() { return _isSourceFileName; }
        wstring GetFileName()   { return _filename.GetName(); }
        int GetThumbnailOffset(){ return _ithmbOffset; }
        int GetCorrelationId()  { return _correlationId; }

        void SetThumbnailOffset(int offset) { _ithmbOffset = offset; }
        void SetFileName(wstring value);
        void SetImageSize(int size) { _imageSize = size; }
        void SetOccupySize(int size) { _occupySpace = size; }

        // Summary  : ���ݺ���padding�Ϳ��, �õ�ͼ��ʵ�ʴ�С
        // Returns  : ͼ��������ͼ�����е�λ��
        Rect GetImageRect();

        // Summary  : �� ":Thumbs:F**:XXXX.JPG" ת��Ϊ "F**|XXXX.JPG"
        // Parameter:  path ·��
        // Parameter:  c ·���ָ���
        static wstring ConvertPath(wstring path, wchar_t c);

        void SetSizeAndPadding(Rect r);

    private:
        void Init(int correlationId, int width, int height, int imageSize, int occupySize);
    private:
        PdbObjectInfo _info;        // Data Objectͷ

        int     _childNumber;       // �̶�Ϊ1: �и�PdbStringInfo�ļ����ӿ�
        int     _correlationId;     // ����ͼ�ļ�ID
        int     _ithmbOffset;       // ����ͼ���ݱ�����.ithmb�ļ��е�λ��, -1��ʾδ����
        int     _imageSize;         // size of image in bytes

        short   _verticalPadding;   // approximate difference between scaled image height and pixmap height (signed)
        short   _horizontalPadding; // approximate difference between scaled image width and pixmap width (signed) 
        short   _imageHeight;       // image height
        short   _imageWidth;        // image width

        int     _unknown;           // unknown
        int     _occupySpace;       // 
        bool    _isSourceFileName;        // 

        PdbString _filename;       // File name
    };


    // δ֪��;�����ݿ�, ��mhaf��ͷ
    class PdbMhafInfo : public PdbBase
    {
    public:
        PdbMhafInfo();
        void Init();

        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);
    private:
        PdbObjectInfo _info;        // Data Objectͷ
        string _padding;
    };

#pragma  endregion

#pragma region ͼƬ���ݲ���

    // ͼƬ����
    class PdbImageItem : public PdbBase
    {
    public:
        PdbImageItem();
        ~PdbImageItem();

        // Summary  : ��ʼ������
        // Parameter: int id ͼƬID
        void Init(int id);
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter, bool needMHAF);

        // Summary  : ��ָ��������ͼ����
        // Returns  : �ҵ��򷵻�����ͼƫ����, �Ҳ���ָ�������򷵻�-1
        // Parameter: type ����ͼ����
        // Parameter: r �������, �������ͼ��ʵ��ռ������
        int GetThumbOffset(IOS_THUMB_TYPE type, Rect& r);

        // Summary  : �޸�ͼƬ(ȡ�ô���ͼƬ����Ȩ)
        // Parameter: Bitmap * pBitmap ͼƬ, �ӹ�����Ȩ
        void SetImage(Bitmap* pBitmap);

        // ������������ͼ����(������ͼƬ���浽ɾ��ͼƬλ��ʱʹ��)
        // ע��: ֱ��ʹ�ñ�ɾ��ͼƬ��offset
        void SwapChild(PdbImageItem* pItem);

        // ����vector��˳��, �½���������ͼ����(������ͼƬ��ӵ�����ͼ�ļ�ĩβʱʹ��)
        // ע��: ��������ͼ�ļ�����offset
        void InitThumbs(THUMB_FILES* pThumbFiles, wstring filename);

        // Summary  : д����ͼ�ļ�
        // Parameter: THUMB_FILES * pThumbFiles ����ͼ�ļ����ʶ���
        void WriteThumbs(THUMB_FILES* pThumbFiles);

        int  GetId() { return _id; }
        Bitmap* GetBitmap() const { return _pBitmap; }

        // Summary  : ȡԴͼƬ·��
        // Returns  : ͼƬ·��(:Thumbs:F**:XXXX.jpg), �Ҳ����򷵻ؿ��ַ���
        wstring GetSourceFilePath();

        // Summary  : ���������ڴ��е�ͼƬ���浽Ӳ��
        // Return   : �����ļ���С
        // Parameter: ����Ӳ��·��
        int SaveImage(const wstring workFolder);

        // Summary  : ����������ڴ��е�ͼƬ
        void ClearImage() { SAFE_DELETE(_pBitmap); }

        // �ж��Ƿ���Ҫԭͼ
        bool NeedSourceFile();
        void SetSourceFileSize(int size);

        PdbImageName* GetImageNameById(const int id);

    private:
        int         _childNumber;   // number of children 
        int         _id;            // First mhii is 0x40, second is 0x41, ... (on mobile phones the first mhii appears to be 0x64, second 0x65, ...) 
        __int64     _songId;        // Unique ID corresponding to the 'dbid' field in the iTunesDB mhit record, this is used to map ArtworkDB items to iTunesDB items. 
        int         _unknown1;      // Seems to always be 0 
        int         _rating;        // Rating from iPhoto * 20 
        int         _unknown2;      // Seems to always be 0 
        __time32_t  _originalDate;  // (creation date of file) 
        __time32_t  _digitalDate;   // date when the picture was taken, probably from EXIF information
        int         _souceImageSize;// Size in bytes of the original source image     

        PdbImageName*           _pImageName;        // ͼƬ�ļ���Ϣ
        vector<PdbImageName*>   _thumbImageName;    // ����ͼ��Ϣ
        Bitmap*                 _pBitmap;           // ͼƬ����(�ǿձ�ʾ���޸Ļ�����)
        bool _hasMHAF;  // �Ƿ���MHAF���ݿ�
    };

    // ͼƬ�б��ӿ�
    class PdbImageList : public PdbBase
    {
    public:
        PdbImageList(PdbListHead head) : _listHead(head) {}
        ~PdbImageList();
        void Init();

        void Read(BinaryBufferReader* pReader);

        // ����ͼƬд������ͼ, ԭ���ļ���д����ͼ, ɾ��ͼƬ���޸�����ͼ
        void Write(BinaryBufferWriter* pWriter, THUMB_FILES* pThumbFiles, wstring tempDir, bool needMHAF = false);

		// Summary  : ��������ͼƬid
        // Returns  : vector<ͼƬID>, �����߸���delete
		vector<int>* Load();

        // Summary  : ��������ͼƬ����
        // Returns  : 
        int size();

        // Summary  : ɾ��ͼƬ
        // Returns  : �ɹ�����true, û�ҵ�����false
        // Parameter: int imageId ͼƬid
        bool Delete(int imageId);

        // Summary  :�޸�ͼƬ����
        // Returns  : �ɹ�����true, û�ҵ�����false
        // Parameter: int imageId ͼƬid
        // Parameter: Bitmap * pBitmap ͼƬ����
        bool Update( int imageId, Bitmap* pBitmap, wstring workFolder);

        // Summary  : ����ͼƬ
        // Parameter: int imageId ͼƬid
        // Parameter: Bitmap * pImage ͼƬ����
        void Add(int imageId, Bitmap* pImage);

		// Summary  : �õ�ĳͼƬ��ĳ����ͼ���͵�ƫ����
		// Returns  : ����ͼƫ����, �Ҳ����򷵻�-1
		// Parameter: int imageId ͼƬid
		// Parameter: IOS_THUMB_TYPE type ����ͼ����
		int GetThumbOffset(int id, IOS_THUMB_TYPE type, Rect& r);

        // Summary  : ȡ����Ҫɾ�����ļ�
        // Returns  : �ļ�·������ (��ʽ: THUMBS:F**:XXXX.jpg)
        vector<wstring> GetDeleteFilePath();

        // Summary  : ȡ������Ҫ�ϴ����ļ���
        //            ע��: Ӧ��Write()֮�����, ��Ϊ�������ļ���Writeʱ�������ļ���
        // Returns  : map<ͼƬid, ͼƬ·��> (ͼƬ·����ʽ: THUMBS:F**:XXXX.jpg)
        list<int> GetUploadFiles();

        // Summary  : ȡԭͼ·��(ios 3.2���ϰ汾)
        // Returns  : ͼƬ·��(:THUMBS:F**:XXXX.jpg ), �Ҳ������ؿ��ַ���
        // Parameter: int id ͼƬID
        wstring GetImageFilePath( int id );

        // �ж��Ƿ���Ҫԭͼ(�Ƿ���4005/4006����)
        bool NeedSourceFile( THUMB_FILES* pThumbFiles );

    private:
        // Summary  : ��ͼƬ
        // Returns  : PdbImageItem��ָ��, û�ҵ�����NULL
        // Parameter: int id ͼƬid
        PdbImageItem* GetImageItem(int id);

        // ��������ļ���
        wstring GetRandomName(wstring workFolder);

        // ������������ͼƬ��ithmb�ļ�(��Ҫ�Ļ�����ԭͼ���ļ�)
        void SaveToFile(THUMB_FILES* pThumbFiles, bool needSourceFile, wstring tempDir);

    private:
        PdbListHead  _listHead;     // ͷ����Ϣ

        vector<PdbImageItem*> _items;           // ԭ��ͼƬ

        vector<PdbImageItem*> _newItems;        // ����ͼƬ
        // ����thumbOffset��ֵΪ0, ����ʱ�ٸ�ֵ
        // ���ܱ��浽��ɾ��ͼƬ��λ��

        vector<PdbImageItem*> _removedItems;    // ��ɾ��ͼƬ
        // ���ﱣ����ɾ��ͼƬ, ��Ϊ���ظ�����ithmb�ļ���ռ�õ�λ��

        list<int>           _needUpload;        // ��Ҫ�ϴ���ͼƬID
    };

#pragma  endregion

#pragma region ���ಿ��

    // ����Ԫ��
    class PdbCategoryItem : public PdbBase
    {
    public:
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        void Init(int id);
        int  GetId() { return _id; }
    private:
        int         _unknown;
        int         _id;        // id
    };

    // ����
    class PdbCategory : public PdbBase
    {
    public:

        ~PdbCategory();
        void Init(int id, wstring name);

        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        int     GetId()     { return _playlistId; }
        wstring GetName();

        // Summary  : ȡ�������µ�����ͼƬ
        // Returns  : vector<ͼƬid>��ָ��, �����߸���ɾ��(��ָ��ID�����ڷ���NULL)
        vector<int>* GetImages();

        // Summary  : ����
        void Rename(wstring newName);

        // Summary  : �Ƿ����ͼƬ
        bool Exist(int imageId);

        // Summary  : �ӷ������Ƴ�һ��ͼ��
        // Returns  : �ɹ�����true, �����ڷ���false
        // Parameter: int imageId ͼ��id
        bool DelImage(int imageId);

        // Summary  : ���ͼƬ
        // Returns  : �ɹ�����true, �Ѵ��ڷ���false
        // Parameter: int imageId ͼƬid
        bool AddImage(int imageId);

    private:
        PdbString _name;                    // album name
        vector<PdbCategoryItem*> _items;     // pictures in album

        // number of Data Objects in the List, probably always 1.
        // Sometimes seems to be 2 in the new video iPods' Photo Database:
        // On the new video iPods and nanos of recent vintage, the MHBA
        // has a second MHOD as a child which contains a string of which
        // specifies the transition effect configured in iPhoto for the 
        // slideshow associated with this album.
        int     _stringNumber;
        int     _itemNumber;    // number of pictures in the album
        int     _playlistId;    // a unique integer for each playlist 
        int     _unknown1;
        short   _unknown2;
        
        char    _type;          // 1 = master photo list ("Photo Library"),
                                // 2 = normal album, sometimes 4 and 5
                                // in my test file it's value is 6
        bool    _playMusic;     // 1 = play music during slideshow
        bool    _repeat;        // 1 = repeat the slideshow
        bool    _random;        // 1 = show the slides in random order
        bool    _showTitle;     // 1 = show slide captions

        char    _transitionDirection;   // 0=none, 1=left-to-right, 2=right-to-left,
                                        // 3=top-to-bottom, 4=bottom-to-top
        int     _slideDuration;         // in seconds (from iPhoto setting)
        int     _transitionDuration;    // in milliseconds

        int     _unknown3;
        int     _unknown4;
        __int64 _trackid;               // id of track in iTunesDB to play during slideshow
        int     _previousPlaylistId;    // the id of the previous playlist

    };

    // �����б��ӿ�
    class PdbCategoryList : public PdbBase
    {
    public:
        PdbCategoryList(PdbListHead head) : _listHead(head) {}
        ~PdbCategoryList() ;

        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        // Summary  : ȡ���з����ID������
        // Returns  : map<����ID, ������>
        map<int, wstring>* GetAllCategory();

        // Summary  : ȡĳ�������µ�����ͼƬ
        // Returns  : vector<ͼƬid>��ָ��, �����߸���ɾ��(��ָ��ID�����ڷ���NULL)
        // Parameter: ����ID
        vector<int>* GetCategoryImages(int id);

        // Summary  : ���ݷ���id�ҷ���
        // Returns  : ����ָ��(��ָ��ID�����ڷ���NULL)
        // Parameter: int id ����ID
        PdbCategory* GetCategory(int id);

        // Summary  : ���ݷ������ҷ���
        // Returns  : ����ָ��(��ָ��ID�����ڷ���NULL)
        // Parameter: wstring name ������
        PdbCategory* GetCategory(wstring name);

        // Summary  : �������
        // Parameter: int id ����id
        // Parameter: wstring newName ����
        void Rename(int id, wstring newName);

        // Summary  : ���ӷ���
        // Parameter: int id ����id
        // Parameter: wstring name ������
        void Add(int id, wstring name);

        // Summary  : ɾ������
        // Parameter: int id ����id
        void Del( int id );

        // Summary  : ��ͼƬ��һ�������ƶ�����һ������
        // Return   : 0 �ɹ� 1 ԭ���಻���� 2 �·��಻���� 3 �·������д�ͼ 4 ԭ�����޴�ͼ
        // Parameter: int imageId ͼƬid
        // Parameter: int originalCategoryId ԭ����id
        // Parameter: int newCategoryId �·���id
        int MoveImage( int imageId, int originalCategoryId, int newCategoryId );

        // Summary  : �����е�ͼƬ��ӵ�����(������ӵ�ͼ��)
        // Return   : 0 �ɹ� 1 ���಻���� 2 ���������д�ͼƬ
        // Parameter: int imageId ͼƬid
        // Parameter: int categoryId ����id
        int AddImage( int imageId, int categoryId );

        // Summary  : �ӷ������Ƴ�ͼƬ
        // Returns  : �ɹ�����true, û�ҵ�����false
        // Parameter: int imageId ͼƬid
        // Parameter: int categoryId ����id
        bool RemoveImage(int imageId, int categoryId ); 

        // Summary  : �������Ƿ����ָ��ͼƬ
        bool ExistImage(int imageId);

    private:
        PdbListHead  _listHead;         // ͷ����Ϣ
        vector<PdbCategory*>   _childs; // �������з���
    };

#pragma  endregion

#pragma region �ļ�����

    // �ļ�
    class PdbFileItem : public PdbBase
    {
    public:
        void Init();
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        int GetCorrelationId() { return _correlationId; }
        int GetImageSize() { return _imageSize; }

    private:
        int _unknown;
        int _correlationId;     // used to link this entry with a file and an Image Name
        int _imageSize;         // size of the image in bytes. 
    };

    // �ļ��б��ӿ�
    class PdbFileList : public PdbBase
    {
    public:
        PdbFileList(PdbListHead head) : _listHead(head) {}
        ~PdbFileList() ;

        void Init();
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        // Summary  : ȡ����������ͼ�ļ���ID��ͼ���С
        // Returns  : map<�ļ�ID, ͼ���С>
        map<int, int> GetIdAndSize();

    private:
        PdbListHead  _listHead;     // ͷ����Ϣ

        vector<PdbFileItem*>  _childs;
    };

#pragma  endregion

}
