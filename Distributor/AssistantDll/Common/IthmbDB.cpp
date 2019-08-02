#include "stdafx.h"

IthmbDB::IthmbDB( IOS_THUMB_TYPE t, wstring filePath )
{
    _type = t;
    _dbFile = filePath;

    switch(t)
    {
    case F3001:  // 未知
        Init(256, 256, 256, 256, 131072);
        break;

    case F3002:  // 未知
        Init(128, 128, 128, 128, 32768);
        break;

    case F3003:  // 未知
        Init(64, 64, 64, 64, 8192);
        break;

    case F3004:  // 
        Init(56, 55, 55, 55, 8192);
        break;

    case F3005:  // 专辑大图
        Init(320, 320, 320, 320, 204800);
        break;

    case F3006:  // 专辑小图
        Init(56, 55, 55, 55, 8192);
        break;

    case F3007:  // 视频封面
        Init(88, 88, 88, 88, 15488);
        break;

    case F3008:  // 
        Init(640, 480, 640, 480, 614400);
        break;

    case F3009:  // 
        Init(160, 120, 160, 120, 40960);
        break;

    case F3011:  // 
        Init(80, 79, 75, 75, 12640);
        break;

    case F3034:  // 
        Init(56, 55, 55, 55, 8192);
        break;

    case F3039:  // 视频封面 4.3
        Init(160, 120, 160, 120, 0xa000);
        break;

    case F3041:  // 
        Init(80, 79, 75, 75, 12640);
        break;

    case F3101:  // iPod用
        Init(512, 512, 510, 510, 0x80000);
        break;

    case F3102:  // iPod用
        Init(256, 256, 254, 254, 0x20000);
        break;

    case F3105:  // iPod用
        Init(640, 640, 640, 640, 0xc8000);
        break;

    case F3106:  // iPod用
        Init(112, 110, 110, 110, 0x7000);
        break;

    case F3107:  // iPod用
        Init(176, 176, 176, 176, 0x10000);
        break;

    case F3111:  // 视频封面 iphone4 增加
        Init(160, 158, 150, 150, 0xc580);
        break;

    case F3113:  // 视频封面 iphone4 增加
        Init(112, 110, 110, 110, 0x7000);
        break;

    case F3141:  // 视频封面 4.3
        Init(160, 158, 150, 150, 0xc580);
        break;

    case F3143:  // 视频封面 4.3
        Init(112, 110, 110, 110, 0x7000);
        break;

    case F4001:  // 视频封面
        Init(120, 110, 120, 110, 28800);
        break;

    case F4002:  // 视频封面
        Init(32, 32, 32, 32, 2048);
        break;

    case F4010:  // 视频封面
        Init(168, 166, 166, 166, 55776);
        break;

    case F4031:  // 
        Init(120, 120, 120, 120, 28800);
        break;

    case F4032:  // 
        Init(32, 32, 32, 32, 2048);
        break;

    case F4040:  // 
        Init(168, 166, 166, 166, 55776);
        break;

    case F4005:  // 原图片, 文件大小为0不处理
    case F4006:  // 原图片, 文件大小为0不处理(iOS4增加)
    case F4035:  // 原图片, 文件大小为0不处理(iPod中发现)
        Init(0, 0, 0, 0, 0);
        break;

    case X32 :
        Init(32, 32, 32, 32, 0x081c);
        break;

    case X79 :
        Init(80, 79, 75, 75, 0x317c);
        break;

    case X120:
        Init(120, 120, 120, 120, 0x709c);
        break;

    case X158:
        Init(160, 158, 150, 150, 0xc59c);
        break;

    case X166:
        Init(168, 166, 166, 166, 0xd9fc);
        break;

    default:
        break;
    }
}

void IthmbDB::Init(int width, int height, int realWidth, int realHeight, int occupySpace)
{
    _width  = width;
    _height = height;

    _realWidth  = realWidth;
    _realHeight = realHeight;

    _occupySpace = occupySpace;
}

IthmbDB::~IthmbDB(void)
{

}


Bitmap* IthmbDB::ReadImage( int index) const
{
    // _type < 1000 就是类似 32x32.ithmb 的照片缩略图文件
    char* pBuffer = ReadFile(_dbFile, _occupySpace*index,
        _type < 1000 ? _occupySpace : _width* _height * 2);
    RETURN_NULL_IF( !pBuffer );

    Rect r(0, 0, _realWidth, _realHeight);
    if (_type < 1000)
    {
        // 读取实际宽高
        int imageWidth, imageHeight;
        const char* p = pBuffer + _occupySpace - 12;
        memcpy_s(&imageWidth, 4, p, 4);
        memcpy_s(&imageHeight, 4, p+4, 4);

        // 计算填充的行和列
        int paddingX = 0, paddingY = 0;
        if ( _type != X79 && _type != X158 )
        {
            paddingX = (_realWidth - imageWidth )/2;
            paddingY = (_realHeight - imageHeight + 1)/2;
        }

        r = Rect(paddingX, paddingY, imageWidth, imageHeight);
    }
    else if ( _type == F3101 || _type == F3102 )
    {
        r.X = r.Y = 1;
    }


    Bitmap* pBmp = ReadRect(pBuffer, r);

    delete[] pBuffer;
    return pBmp;
}

void IthmbDB::QuaterToRgb555(const char* pBuffer, Rect r, char* pData) const
{
    // 已经不能再分: 拷贝一个像素
    if ( r.Width == 1 && r.Height == 1 )
    {
        char* p = pData + _width * r.Y * 2 + r.X * 2;
        p[0] = pBuffer[0];
        p[1] = pBuffer[1];
        return;
    }

    int quaterLength = r.Width * r.Height / 2;
    Rect r1(r.X, r.Y, r.Width/2, r.Height/2);
    QuaterToRgb555(pBuffer, r1, pData);

    pBuffer += quaterLength;
    Rect r2(r.X, r.Y + r.Height/2, r.Width/2, r.Height/2);
    QuaterToRgb555(pBuffer, r2, pData);

    pBuffer += quaterLength;
    Rect r3(r.X + r.Height/2, r.Y, r.Width/2, r.Height/2);
    QuaterToRgb555(pBuffer, r3, pData);

    pBuffer += quaterLength;
    Rect r4(r.X + r.Height/2, r.Y + r.Height/2, r.Width/2, r.Height/2);
    QuaterToRgb555(pBuffer, r4, pData);
}

char* IthmbDB::ReadFile(wstring filename, int start, int length) const
{
    char* pBuffer = new char[length];

    ifstream ifs(filename.c_str(), ios_base::in | ios_base::binary);
    ifs.seekg(start, ios_base::beg);
    ifs.read(pBuffer, length);
    ifs.close();

    if ( _type == F3101 || _type == F3102 )
    {
        char* pNewBuffer = new char[length];
        Rect r(0, 0, _width, _height);
        QuaterToRgb555(pBuffer, r, pNewBuffer);

        SAFE_DELETE(pBuffer);
        return pNewBuffer;
    }
    else
    {
        return pBuffer;
    }
}

Rect IthmbDB::WriteImageOffset(Image* pImage, int offset)
{
    return WriteImageIndex(pImage, offset/_occupySpace);
}

Rect IthmbDB::GetSizeAndPadding( int width, int height ) 
{
    return Rect(width,
                height,
                (_realWidth - width)/2,
                (_realHeight - height)/2);
}

Rect IthmbDB::WriteImageIndex(Image* pImage, int index)
{
    Bitmap* pThumb = MakeThumbnail(pImage, NeedFillThumb(), _realWidth, _realHeight);

    char* pBuffer = Image2Buffer(pThumb);

    if ( _type < 1000 ) // 32x32.ithmb: 这类文件格式不一样, 每张图后面有宽高数据
    {
        int width = pThumb->GetWidth();
        int height = pThumb->GetHeight();
        memcpy(pBuffer + _occupySpace - 12, &width, 4);
        memcpy(pBuffer + _occupySpace - 8, &height, 4);
    }
    else if ( _type == F3101 || _type == F3102 )
    {
        // 大小跟pBuffer一样(pBuffer在Image2Buffer中new)
        char* pNewBuffer = new char[_occupySpace];
        ZeroMemory(pNewBuffer, _occupySpace);

        Rect r(0,0,_width, _height);
        char* p = pNewBuffer;
        Rgb555ToQuater(pBuffer, r, p);

        // 用新数据块替换旧数据块
        SAFE_DELETE(pBuffer);
        pBuffer = pNewBuffer;
    }

    WriteToFile(index, pBuffer, _occupySpace);

    delete[] pBuffer;

    Rect r = GetSizeAndPadding(pThumb->GetWidth(), pThumb->GetHeight());
    delete pThumb;
    return r;
}

void IthmbDB::Rgb555ToQuater(const char* pBuffer, Rect r, char*& pData) const
{
    // 已经不能再分: 拷贝一个像素
    if ( r.Width == 1 && r.Height == 1 )
    {
        const char* p = pBuffer + _width * r.Y * 2 + r.X * 2;
        *pData = p[0];
        pData++;
        *pData = p[1];
        pData++;
        return;
    }

    Rect r1(r.X, r.Y, r.Width/2, r.Height/2);
    Rgb555ToQuater(pBuffer, r1, pData);

    Rect r2(r.X, r.Y + r.Height/2, r.Width/2, r.Height/2);
    Rgb555ToQuater(pBuffer, r2, pData);

    Rect r3(r.X + r.Height/2, r.Y, r.Width/2, r.Height/2);
    Rgb555ToQuater(pBuffer, r3, pData);

    Rect r4(r.X + r.Height/2, r.Y + r.Height/2, r.Width/2, r.Height/2);
    Rgb555ToQuater(pBuffer, r4, pData);  
}

char* IthmbDB::Image2Buffer( Bitmap* pThumb )
{
    // 从缩略图提取数据
    BitmapData* pData = new BitmapData;
    Rect rect(0, 0, pThumb->GetWidth(), pThumb->GetHeight());
    pThumb->LockBits(&rect, ImageLockModeRead, PixelFormat16bppRGB555, pData);

    bool needPaddingWithe = NeedPaddingWhite();

    // 按照图片实际占据大小创建数据区
    char* pBuffer = new char[_occupySpace];
    memset(pBuffer, needPaddingWithe ? 0xFF : 0, _occupySpace);

    int imageWidth = pThumb->GetWidth();
    int imageHeight = pThumb->GetHeight();

    bool padding = ! NeedFillThumb();

    int paddingRow = ((padding ? _height : _realHeight) - imageHeight + 1) / 2; // 上下填充行数
    int paddingColumn = ((padding ? _width : _realWidth) - imageWidth) / 2;    // 左右填充列数

    if ( needPaddingWithe )
    {
        for(int row = 0; row < paddingRow; ++row)  // 填充行(上面和下面)
        {
            memset(pBuffer + _width * 2 * row, 0xFF, _realWidth * 2);
            memset(pBuffer + _width * 2 * (_realHeight-row-1), 0xFF, _realWidth * 2);
        }
    }

    char* pPixels = (char*)pData->Scan0;
    for(int row = 0; row < imageHeight; ++row)
    {
        // 一行数据的起始位置
        char* pLine = pBuffer + _width * 2 * (paddingRow + row);

        if ( needPaddingWithe )   // 填充列(左边和右边)
        {
            memset(pLine, 0xFF, paddingColumn*2);
            memset(pLine + (paddingColumn + imageWidth) * 2,
                0xFF, paddingColumn*2);
        }

        // 拷贝每行图像数据
        memcpy(pLine + paddingColumn * 2, pPixels + pData->Stride * row, imageWidth * 2);
    }

    // 释放数据, 返回数据区
    pThumb->UnlockBits(pData);
    delete pData;
    return pBuffer;
}

Bitmap* IthmbDB::MakeThumbnail(Image* pImage, bool bNoEmpty, int destWidth, int destHeight)
{
    // 计算水平和垂直方向上的压缩率
    double xRatio = double(pImage->GetWidth ()) / destWidth;
    double yRatio = double(pImage->GetHeight()) / destHeight;

    // 如果不留白, 就用最小的压缩率, 否则就用最大的压缩率
    double ratio = bNoEmpty ? min(xRatio, yRatio) : max(xRatio, yRatio);

    // 计算原图中用于生成缩略图的区域r1, 和生成的缩略图的区域destRect
    Rect sourceRect, destRect;
    if ( bNoEmpty )
    {
        sourceRect.Width  = int(ratio * destWidth);
        sourceRect.Height = int(ratio * destHeight);
        sourceRect.X = (pImage->GetWidth() - sourceRect.Width ) / 2;
        sourceRect.Y = (pImage->GetHeight()- sourceRect.Height) / 2;

        destRect.Width = destWidth;
        destRect.Height= destHeight;
    }
    else
    {
        sourceRect.X = 0;
        sourceRect.Y = 0;
        sourceRect.Width  = pImage->GetWidth();
        sourceRect.Height = pImage->GetHeight();

        destRect.Width  = int(pImage->GetWidth() / ratio);
        destRect.Height = int(pImage->GetHeight()/ ratio);
    }
    destRect.X = 0;
    destRect.Y = 0;

    return Draw(pImage, sourceRect, destRect);
}

Bitmap* IthmbDB::Draw(Image* pSourceImage, Rect sourceRect, Rect destRect)
{
    Bitmap* pDestImage = new Bitmap(destRect.Width, destRect.Height, PixelFormat16bppRGB555);
    Graphics g(pDestImage);

    // 设置抄自 2.0版助手 iPodUtility::CreateThumbnail()
    g.Clear(Color::Transparent);                            // 以透明背景色填充
    //     g.SetInterpolationMode(InterpolationModeHighQuality);   // 设置插值方式
    //     g.SetSmoothingMode(SmoothingModeHighQuality);           // 设置平滑模式

    // 画缩略图
    g.DrawImage(pSourceImage, destRect,
        sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height,
        UnitPixel);

    return pDestImage;
}

void IthmbDB::WriteToFile( int index, char* pBuffer, int bufferSize )
{
    RETURN_IF(! CFileOperation::CreateEmptyFile(_dbFile.c_str()));

    fstream fs(_dbFile.c_str(), ios_base::in | ios_base::out | ios_base::binary);

    fs.seekp(_occupySpace * index, ios_base::beg);
    fs.write(pBuffer, bufferSize);

    fs.close();
}


Bitmap* IthmbDB::ReadImageByOffset( int offset, Rect r ) const
{
    char* pBuffer = ReadFile(_dbFile, offset, _width* _height * 2);
    RETURN_NULL_IF( !pBuffer );

    Bitmap* pBmp = ReadRect(pBuffer, r);
    delete[] pBuffer;
    return pBmp;
}

int ND91Assistant::IthmbDB::GetFileSize() const
{
    return CFileOperation::GetFileSizeW(_dbFile);
}


int ND91Assistant::IthmbDB::GetOccupySpace() const
{
    return _occupySpace;
}

int ND91Assistant::IthmbDB::GetRealWidth() const
{
    return _realWidth;
}

int ND91Assistant::IthmbDB::GetRealHeight() const
{
    return _realHeight;
}

int ND91Assistant::IthmbDB::GetWidth() const
{
    return _width;
}

int ND91Assistant::IthmbDB::GetHeight() const
{
    return _height;
}

std::wstring ND91Assistant::IthmbDB::GetFilename() const
{
    return _dbFile;
}

ND91Assistant::IOS_THUMB_TYPE ND91Assistant::IthmbDB::GetType() const
{
    return _type;
}

bool IthmbDB::IsSouceFileType( IOS_THUMB_TYPE t )
{
    return F4005 == t
        || F4006 == t
        || F4035 == t;
}

bool IthmbDB::NeedPaddingWhite() 
{
    return F3011 == _type
        || F3111 == _type
        || F3141 == _type
        || X79  == _type
        || X158 == _type;
}

bool ND91Assistant::IthmbDB::NeedFillThumb()
{
    return F3011 == _type
        || F3111 == _type
        || F3141 == _type
        || F3143 == _type
        || F3004 == _type
        || F3006 == _type
        || F3007 == _type
        || X79  == _type
        || X158 == _type;
}

Bitmap* IthmbDB::ReadRect( const char* pBuffer, Rect r) const
{
    // 创建图像, 取得写入用数据区
    Bitmap* pBmp = new Bitmap(r.Width, r.Height, PixelFormat16bppRGB555);
    BitmapData* pData = new BitmapData;
    Rect rect(0, 0, r.Width, r.Height);
    pBmp->LockBits(&rect, ImageLockModeWrite, PixelFormat16bppRGB555, pData);

    // 将每行图像数据拷贝到数据区
    char* pPixels = (char*)pData->Scan0;
    for(int row = 0; row < r.Height; ++ row)
    {
        const char* pLineStart = pBuffer + (r.Y + row) * _width * 2;
        memcpy(pPixels + pData->Stride * row, pLineStart + r.X * 2, r.Width * 2);
    }

    pBmp->UnlockBits(pData);
    delete pData;

    return pBmp;
}

bool IthmbDB::MakeBthFile(wstring thmFile, wstring bthFile, bool ip4)
{
    // 在图像右侧和下方填充4/8个像素(白色), 即缩略图间的白色间隔
    // 估计ios直接将BTH文件内容写到视频缓冲区, 生成缩略图显示
    // 在某些ithmb文件中也有类似的处理(参考NeedPaddingWhite方法)
    int REAL_WIDTH = ip4 ? 150 : 75;
    int REAL_HEIGHT= ip4 ? 150 : 75;
    int BTH_WIDTH  = ip4 ? 158 : 79;
    int BTH_HEIGHT = ip4 ? 158 : 79;

    // 提取数据
    Bitmap* pBmp = Bitmap::FromFile(thmFile.c_str());
    RETURN_FALSE_IF( ! pBmp );

    BitmapData* pData = new BitmapData;
    Rect rect(0, 0, pBmp->GetWidth(), pBmp->GetHeight());
    pBmp->LockBits(&rect, ImageLockModeRead, PixelFormat16bppRGB555, pData);

    // 创建BTH文件内容数据区
    unsigned char fileHeader[16] = {0xBA, 0xCD, 0xDC, 0xAB, ip4 ? 0x48 : 0x47, 4, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    int bodyLength = BTH_WIDTH * BTH_HEIGHT * 2;
    char* fileBody = new char[bodyLength];
    memset(fileBody, 0x7FFF, bodyLength); // 填充为白色

    // 拷贝数据
    char* pPixels = (char*)pData->Scan0;
    for(int row = 0; row < REAL_HEIGHT; ++row)
    {
        memcpy(fileBody + BTH_WIDTH * 2 * row, pPixels + pData->Stride * row, REAL_WIDTH * 2);
    }

    // 释放原图数据
    pBmp->UnlockBits(pData);
    delete pData;
    delete pBmp;

    // 写文件
    fstream fs(bthFile.c_str(), ios_base::binary|ios_base::out);
    fs.write((char*)fileHeader, 16);
    fs.write(fileBody, bodyLength);
    fs.close();

    delete[] fileBody;
    return true;
}
