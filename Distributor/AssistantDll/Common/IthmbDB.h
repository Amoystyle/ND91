#pragma  once

#include <string>
#include <windows.h>
#include <GdiPlus.h>

using namespace std;
using namespace Gdiplus;

namespace ND91Assistant
{

    // 缩略图类型
    enum IOS_THUMB_TYPE 
    {
        X120 = 120,
        X158 = 158,
        X166 = 166,
        X32  = 32,
        X79  = 79,
        F3001 = 3001,   // CoverFlow
        F3002 = 3002,   // 
        F3003 = 3003,   // 
        F3004 = 3004,   // PhotoBrowser
        F3005 = 3005,   // 专辑大图
        F3006 = 3006,   // 专辑小图 TrackBrowser
        F3007 = 3007,   // 视频封面 VideoBrowser
        F3008 = 3008,   // FullScreen
        F3009 = 3009,   // Email
        F3011 = 3011,   // Misc
        F3101 = 3101,   // iPod
        F3102 = 3102,   // iPod
        F3105 = 3105,   // iPod
        F3106 = 3106,   // iPod
        F3107 = 3107,   // iPod
        F3111 = 3111,   // 视频封面 iphone4 增加
        F3113 = 3113,   // 视频封面 iphone4 增加
        F3034 = 3034,   // iPad增加
        F3039 = 3039,   // 视频封面 ios 4.3
        F3041 = 3041,   // iPad增加
        F3141 = 3141,   // 视频封面 ios 4.3
        F3143 = 3143,   // 视频封面 ios 4.3
        F4001 = 4001,   // 视频封面
        F4002 = 4002,   // 视频封面
        F4005 = 4005,   // 原图
        F4006 = 4006,   // 原图
        F4010 = 4010,   // 视频封面
        F4031 = 4031,   // iPad增加
        F4032 = 4032,   // iPad增加
        F4035 = 4035,   // 原图
        F4040 = 4040,   // iPad增加
    };


    // ios设备的 ithmb 文件读写
    class IthmbDB
    {
    public:
        // Summary  : 构造函数
        // Parameter: IOS_THUMB_TYPE t 文件类型
        // Parameter: wstring filePath ithmb    文件路径
        IthmbDB(IOS_THUMB_TYPE t, wstring filePath);
        ~IthmbDB();

        // Summary  : 从ithmb文件中提取img 
        // Returns  : 缩略图指针(调用者负责delete)
        // Parameter: int index ithmb中的第几个图片(从0开始)
        Bitmap*  ReadImage(int index) const;

        // Summary  : 从ithmb文件中提取img 
        // Returns  : 缩略图指针(调用者负责delete)
        // Parameter: index ithmb中的文件偏移量
        // Parameter: r 缩略图实际占用位置
        Bitmap*  ReadImageByOffset(int offset, Rect r) const;

        // Summary  : 生成缩略图并写入到ithmb文件中指定位置
        // Parameter: Image * pImage 原图
        // Parameter: int index 写入ithmb第几个文件(从0开始)
        Rect WriteImageIndex(Image* pImage, int index);

        // Summary  : 生成缩略图并写入到ithmb文件中指定位置
        // Parameter: Image * pImage 原图
        // Parameter: int offset 写入ithmb文件中的偏移量(从0开始)
        Rect WriteImageOffset(Image* pImage, int offset);

        int     GetFileSize() const;
        int     GetOccupySpace() const;
        int     GetRealWidth() const;
        int     GetRealHeight() const;
        int     GetWidth() const;
        int     GetHeight() const;
        wstring GetFilename() const;
        IOS_THUMB_TYPE GetType() const;

        // Summary  : 生成缩略图
        // Returns  : 缩略图指针(调用者负责delete)
        // Parameter: Image * pImage 原图
        // Parameter: bool bNoEmpty 是否不留空白
        //                          = true : 以最小的压缩率生成缩略图(不留空白，超出的部分被剪切);
        //                          = false: 以最大的压缩率生成缩略图(可能留下空白)
        static Bitmap* MakeThumbnail(Image* pImage, bool bNoEmpty, int destWidth, int destHeight);

        // 判断此类型是否"原图片"类型, 是则返回true
        // 原图片类型的缩略图无数据, 可以不存在
        static bool IsSouceFileType(IOS_THUMB_TYPE t);

        // Summary  : 生成BTH缩略图文件
        // Returns  : 成功返回true
        // Parameter: sourceFile THM文件路径
        // Parameter: bthFile    BTH文件路径
        // Parameter: ip4  true表示ip4.0以上版本
        static bool MakeBthFile(wstring thmFile, wstring bthFile, bool ip4);

    private:
        // 从源图生成缩略图
        static Bitmap* Draw(Image* pSourceImage, Rect sourceRect, Rect destRect);

        // Summary  : 将数据写入第几个图片
        // Parameter: int index 第几个图片
        // Parameter: char * pBuffer 数据
        // Parameter: int bufferSize 数据长度
        void WriteToFile(int index, char* pBuffer, int bufferSize);

        // 读取文件指定位,
        char* ReadFile(wstring filename, int start, int length) const;

        // Summary  : 初始化函数
        // Parameter: int width                 缩略图的宽度
        // Parameter: int height                缩略图的高度
        // Parameter: int realWidth             图片内容的实际宽度
        // Parameter: int realHeight            图片内容的实际高度
        // Parameter: int occupySpace           每张图片的占用空间
        void Init(int width, int height, int realWidth, int realHeight, int occupySpace);

        // Summary  : 生成需要写入的数据
        // Returns  : 需要写入的数据buffer, 大小为_occupySpace
        // Parameter: Bitmap * pThumb 缩略图指针
        char* Image2Buffer(Bitmap* pThumb);

        // 是否需要在周边添充白色
        bool NeedPaddingWhite();

        // 返回true表示缩略图不留空白, 保持原比例, 用图像中间部分填充
        bool NeedFillThumb();

        Bitmap* ReadRect(const char* pBuffer, Rect r) const;

        // Summary  : 将四等分格式图像数据转换为正常RGB555图像数据
        //              目前仅F3101/F3102为四等分格式
        // Parameter: const char * pBuffer 四等分格式图像数据
        // Parameter: Rect r 图像大小
        // Parameter: char * pData 存放正常RGB555图像数据
        void QuaterToRgb555( const char* pBuffer, Rect r, char* pData) const;

        // Summary  : 将正常RGB555图像数据转换为四等分格式图像数据
        //              目前仅F3101/F3102为四等分格式
        // Parameter: const char * pBuffer 存放正常RGB555图像数据
        // Parameter: Rect r 图像大小
        // Parameter: char*& pData 四等分格式图像数据(注意此指针将被修改)
        void Rgb555ToQuater(const char* pBuffer, Rect r, char*& pData) const;

        Rect GetSizeAndPadding( int width, int height ) ;
    private:

        wstring _dbFile;        // ithmb文件路径

        IOS_THUMB_TYPE _type;   // 文件类型

        int     _width;         // 文件中的图像高度
        int     _height;        // 文件中的图像宽度
        int     _realWidth;     // 图片内容的实际宽度
        int     _realHeight;    // 图片内容的实际高度

        int     _occupySpace;   // 每张图片的占用空间
    };
}
