// 
// 本文件定义ios设备的photo database文件中的各种数据(简称图库数据)
// PhotoDataBase简称Pdb, 下面的图库数据类都以Pdb开头
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

#pragma region 基类与文件头

    // 图库数据的通用头部定义
    class PdbHead
    {
    public:
        char    _name[5];   // 名字: 4个ASCII字符
        int     _headSize;  // 头部长度
        int     _number;    // 个数

        PdbHead();
        void Read(BinaryBufferReader* pReader, const char* pName);
        void Write(BinaryBufferWriter* pWrite);
    };

    // 图库数据类的共通基类
    class PdbBase
    {
    public:
        PdbBase() { _paddingCount = 0; }
        PdbHead _head;              // 头部
        int     _paddingCount;      // 填充字数

    protected:
        void WritePadding(BinaryBufferWriter* pWriter);
    };

    enum PDB_LIST_TYPE {
        PDB_IMAGE = 1,      // Image List
        PDB_CATEGORY = 2,   // Category List
        PDB_FILE  = 3,      // File List
    };

    // 子块数据: 数据头后面是类型, 类型后面是子块数据
    class PdbListHead : public PdbBase
    {
    public:
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        PDB_LIST_TYPE  _childType;
    };


    // 图库文件头
    class PdbFileHead : public PdbBase
    {
    public:
        void Read(BinaryBufferReader* pReader);

        // 子节点数量
        int GetChildNumber();

        // Summary  : 新增分类与文件时使用的ID, ID加1
        // Returns  : 新ID
        int GetNewId();

        void Write(BinaryBufferWriter* pWriter);

    private:
        int     _data1to5[5];          // 5个 int
        __int64 _data6;             // 1个 int64
        __int64 _data7;             // 1个 int64
        int     _data8to12[5];         // 5个 int
    };

#pragma  endregion

#pragma region 图片详细信息

    enum PDB_INFO_TYPE
    {
        PDB_CATEGORY_NAME   = 1,    // Album name 
        PDB_INFO_THUMBNAIL  = 2,    // Thumbnail image 
        PDB_FILE_NAME       = 3,    // File name 
        PDB_INFO_IMAGE      = 5,    // Full Resolution image 
        PDB_INFO_MHAF       = 6,    // unknown
    };

    // 图片信息
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

    // 字符串
    class PdbString
    {
    public:
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        wstring GetName() { return _value; }
        void SetName(wstring newName) { _value = newName; }

        // Summary  : 初始化字符串
        // Parameter: PDB_INFO_TYPE type 只接受PDB_CATEGORY_NAME/PDB_FILE_NAME二种类型
        // Parameter: wstring name 字符串的值
        void Init(PDB_INFO_TYPE type, wstring name);

    private:
        PdbObjectInfo _info;        // Data Object头

        int     _size;      // length in bytes of the string 
        int     _utf;       // 1 == UTF-8; 2 == UTF-16
        int     _unknown;
        wstring _value;     // string content

        int     _paddingCount;  // 填充字符数(未继承PdbBase故自行定义)
    };

    class PdbImageName : public PdbBase
    {
    public:
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        // Summary  : 初始化为普通缩略图数据
        // Parameter: int correlationId 缩略图类型
        // Parameter: IthmbDB * pThumbFile 缩略图实例对象指针
        // Parameter: Bitmap* pBmp 新增的图片(需要提取其宽高信息)
        void InitThumb(int correlationId, IthmbDB* pThumbFile, Bitmap* pBmp);

        // Summary  : 初始化为原图片文件数据
        // Parameter: int correlationId 缩略图类型
        // Parameter: wstring fileName 文件名(:Thumbs:F**:XXXX.JPG格式)
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

        // Summary  : 根据横竖padding和宽高, 得到图像实际大小
        // Returns  : 图像在缩略图数据中的位置
        Rect GetImageRect();

        // Summary  : 将 ":Thumbs:F**:XXXX.JPG" 转换为 "F**|XXXX.JPG"
        // Parameter:  path 路径
        // Parameter:  c 路径分隔符
        static wstring ConvertPath(wstring path, wchar_t c);

        void SetSizeAndPadding(Rect r);

    private:
        void Init(int correlationId, int width, int height, int imageSize, int occupySize);
    private:
        PdbObjectInfo _info;        // Data Object头

        int     _childNumber;       // 固定为1: 有个PdbStringInfo文件名子块
        int     _correlationId;     // 缩略图文件ID
        int     _ithmbOffset;       // 缩略图数据保存在.ithmb文件中的位置, -1表示未设置
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


    // 未知用途的数据块, 以mhaf开头
    class PdbMhafInfo : public PdbBase
    {
    public:
        PdbMhafInfo();
        void Init();

        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);
    private:
        PdbObjectInfo _info;        // Data Object头
        string _padding;
    };

#pragma  endregion

#pragma region 图片数据部分

    // 图片数据
    class PdbImageItem : public PdbBase
    {
    public:
        PdbImageItem();
        ~PdbImageItem();

        // Summary  : 初始化数据
        // Parameter: int id 图片ID
        void Init(int id);
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter, bool needMHAF);

        // Summary  : 找指定的缩略图类型
        // Returns  : 找到则返回缩略图偏移量, 找不到指定类型则返回-1
        // Parameter: type 缩略图类型
        // Parameter: r 输出参数, 存放缩略图的实际占用区域
        int GetThumbOffset(IOS_THUMB_TYPE type, Rect& r);

        // Summary  : 修改图片(取得传入图片所有权)
        // Parameter: Bitmap * pBitmap 图片, 接管所有权
        void SetImage(Bitmap* pBitmap);

        // 交换所有缩略图子项(将新增图片保存到删除图片位置时使用)
        // 注意: 直接使用被删除图片的offset
        void SwapChild(PdbImageItem* pItem);

        // 按照vector的顺序, 新建所有缩略图子项(将新增图片添加到缩略图文件末尾时使用)
        // 注意: 会在缩略图文件增加offset
        void InitThumbs(THUMB_FILES* pThumbFiles, wstring filename);

        // Summary  : 写缩略图文件
        // Parameter: THUMB_FILES * pThumbFiles 缩略图文件访问对象
        void WriteThumbs(THUMB_FILES* pThumbFiles);

        int  GetId() { return _id; }
        Bitmap* GetBitmap() const { return _pBitmap; }

        // Summary  : 取源图片路径
        // Returns  : 图片路径(:Thumbs:F**:XXXX.jpg), 找不到则返回空字符串
        wstring GetSourceFilePath();

        // Summary  : 将保持在内存中的图片保存到硬盘
        // Return   : 返回文件大小
        // Parameter: 本地硬盘路径
        int SaveImage(const wstring workFolder);

        // Summary  : 清除保持在内存中的图片
        void ClearImage() { SAFE_DELETE(_pBitmap); }

        // 判断是否需要原图
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

        PdbImageName*           _pImageName;        // 图片文件信息
        vector<PdbImageName*>   _thumbImageName;    // 缩略图信息
        Bitmap*                 _pBitmap;           // 图片内容(非空表示被修改或新增)
        bool _hasMHAF;  // 是否有MHAF数据块
    };

    // 图片列表子块
    class PdbImageList : public PdbBase
    {
    public:
        PdbImageList(PdbListHead head) : _listHead(head) {}
        ~PdbImageList();
        void Init();

        void Read(BinaryBufferReader* pReader);

        // 新增图片写入缩略图, 原有文件不写缩略图, 删除图片不修改缩略图
        void Write(BinaryBufferWriter* pWriter, THUMB_FILES* pThumbFiles, wstring tempDir, bool needMHAF = false);

		// Summary  : 返回所有图片id
        // Returns  : vector<图片ID>, 调用者负责delete
		vector<int>* Load();

        // Summary  : 返回所有图片数量
        // Returns  : 
        int size();

        // Summary  : 删除图片
        // Returns  : 成功返回true, 没找到返回false
        // Parameter: int imageId 图片id
        bool Delete(int imageId);

        // Summary  :修改图片内容
        // Returns  : 成功返回true, 没找到返回false
        // Parameter: int imageId 图片id
        // Parameter: Bitmap * pBitmap 图片内容
        bool Update( int imageId, Bitmap* pBitmap, wstring workFolder);

        // Summary  : 新增图片
        // Parameter: int imageId 图片id
        // Parameter: Bitmap * pImage 图片内容
        void Add(int imageId, Bitmap* pImage);

		// Summary  : 得到某图片、某缩略图类型的偏移量
		// Returns  : 缩略图偏移量, 找不到则返回-1
		// Parameter: int imageId 图片id
		// Parameter: IOS_THUMB_TYPE type 缩略图类型
		int GetThumbOffset(int id, IOS_THUMB_TYPE type, Rect& r);

        // Summary  : 取得需要删除的文件
        // Returns  : 文件路径数组 (格式: THUMBS:F**:XXXX.jpg)
        vector<wstring> GetDeleteFilePath();

        // Summary  : 取所有需要上传的文件名
        //            注意: 应在Write()之后调用, 因为新增的文件在Write时才生成文件名
        // Returns  : map<图片id, 图片路径> (图片路径格式: THUMBS:F**:XXXX.jpg)
        list<int> GetUploadFiles();

        // Summary  : 取原图路径(ios 3.2以上版本)
        // Returns  : 图片路径(:THUMBS:F**:XXXX.jpg ), 找不到返回空字符串
        // Parameter: int id 图片ID
        wstring GetImageFilePath( int id );

        // 判断是否需要原图(是否有4005/4006类型)
        bool NeedSourceFile( THUMB_FILES* pThumbFiles );

    private:
        // Summary  : 找图片
        // Returns  : PdbImageItem的指针, 没找到返回NULL
        // Parameter: int id 图片id
        PdbImageItem* GetImageItem(int id);

        // 生成随机文件名
        wstring GetRandomName(wstring workFolder);

        // 保存所有新增图片到ithmb文件(需要的话保存原图到文件)
        void SaveToFile(THUMB_FILES* pThumbFiles, bool needSourceFile, wstring tempDir);

    private:
        PdbListHead  _listHead;     // 头部信息

        vector<PdbImageItem*> _items;           // 原有图片

        vector<PdbImageItem*> _newItems;        // 新增图片
        // 其中thumbOffset的值为0, 保存时再赋值
        // 可能保存到被删除图片的位置

        vector<PdbImageItem*> _removedItems;    // 被删除图片
        // 这里保留被删除图片, 是为了重复利用ithmb文件中占用的位置

        list<int>           _needUpload;        // 需要上传的图片ID
    };

#pragma  endregion

#pragma region 分类部分

    // 分类元素
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

    // 分类
    class PdbCategory : public PdbBase
    {
    public:

        ~PdbCategory();
        void Init(int id, wstring name);

        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        int     GetId()     { return _playlistId; }
        wstring GetName();

        // Summary  : 取本分类下的所有图片
        // Returns  : vector<图片id>的指针, 调用者负责删除(若指定ID不存在返回NULL)
        vector<int>* GetImages();

        // Summary  : 改名
        void Rename(wstring newName);

        // Summary  : 是否存在图片
        bool Exist(int imageId);

        // Summary  : 从分类中移除一个图像
        // Returns  : 成功返回true, 不存在返回false
        // Parameter: int imageId 图像id
        bool DelImage(int imageId);

        // Summary  : 添加图片
        // Returns  : 成功返回true, 已存在返回false
        // Parameter: int imageId 图片id
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

    // 分类列表子块
    class PdbCategoryList : public PdbBase
    {
    public:
        PdbCategoryList(PdbListHead head) : _listHead(head) {}
        ~PdbCategoryList() ;

        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        // Summary  : 取所有分类的ID和名称
        // Returns  : map<分类ID, 分类名>
        map<int, wstring>* GetAllCategory();

        // Summary  : 取某个分类下的所有图片
        // Returns  : vector<图片id>的指针, 调用者负责删除(若指定ID不存在返回NULL)
        // Parameter: 分类ID
        vector<int>* GetCategoryImages(int id);

        // Summary  : 根据分类id找分类
        // Returns  : 分类指针(若指定ID不存在返回NULL)
        // Parameter: int id 分类ID
        PdbCategory* GetCategory(int id);

        // Summary  : 根据分类名找分类
        // Returns  : 分类指针(若指定ID不存在返回NULL)
        // Parameter: wstring name 分类名
        PdbCategory* GetCategory(wstring name);

        // Summary  : 分类改名
        // Parameter: int id 分类id
        // Parameter: wstring newName 新名
        void Rename(int id, wstring newName);

        // Summary  : 增加分类
        // Parameter: int id 分类id
        // Parameter: wstring name 分类名
        void Add(int id, wstring name);

        // Summary  : 删除分类
        // Parameter: int id 分类id
        void Del( int id );

        // Summary  : 将图片从一个分类移动到另一个分类
        // Return   : 0 成功 1 原分类不存在 2 新分类不存在 3 新分类已有此图 4 原分类无此图
        // Parameter: int imageId 图片id
        // Parameter: int originalCategoryId 原分类id
        // Parameter: int newCategoryId 新分类id
        int MoveImage( int imageId, int originalCategoryId, int newCategoryId );

        // Summary  : 将已有的图片添加到分类(需先添加到图库)
        // Return   : 0 成功 1 分类不存在 2 分类中已有此图片
        // Parameter: int imageId 图片id
        // Parameter: int categoryId 分类id
        int AddImage( int imageId, int categoryId );

        // Summary  : 从分类中移除图片
        // Returns  : 成功返回true, 没找到返回false
        // Parameter: int imageId 图片id
        // Parameter: int categoryId 分类id
        bool RemoveImage(int imageId, int categoryId ); 

        // Summary  : 分类中是否存在指定图片
        bool ExistImage(int imageId);

    private:
        PdbListHead  _listHead;         // 头部信息
        vector<PdbCategory*>   _childs; // 保存所有分类
    };

#pragma  endregion

#pragma region 文件部分

    // 文件
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

    // 文件列表子块
    class PdbFileList : public PdbBase
    {
    public:
        PdbFileList(PdbListHead head) : _listHead(head) {}
        ~PdbFileList() ;

        void Init();
        void Read(BinaryBufferReader* pReader);
        void Write(BinaryBufferWriter* pWriter);

        // Summary  : 取得所有缩略图文件的ID和图像大小
        // Returns  : map<文件ID, 图像大小>
        map<int, int> GetIdAndSize();

    private:
        PdbListHead  _listHead;     // 头部信息

        vector<PdbFileItem*>  _childs;
    };

#pragma  endregion

}
