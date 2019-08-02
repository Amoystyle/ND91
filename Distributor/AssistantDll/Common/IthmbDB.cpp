#include "stdafx.h"

IthmbDB::IthmbDB( IOS_THUMB_TYPE t, wstring filePath )
{
    _type = t;
    _dbFile = filePath;

    switch(t)
    {
    case F3001:  // δ֪
        Init(256, 256, 256, 256, 131072);
        break;

    case F3002:  // δ֪
        Init(128, 128, 128, 128, 32768);
        break;

    case F3003:  // δ֪
        Init(64, 64, 64, 64, 8192);
        break;

    case F3004:  // 
        Init(56, 55, 55, 55, 8192);
        break;

    case F3005:  // ר����ͼ
        Init(320, 320, 320, 320, 204800);
        break;

    case F3006:  // ר��Сͼ
        Init(56, 55, 55, 55, 8192);
        break;

    case F3007:  // ��Ƶ����
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

    case F3039:  // ��Ƶ���� 4.3
        Init(160, 120, 160, 120, 0xa000);
        break;

    case F3041:  // 
        Init(80, 79, 75, 75, 12640);
        break;

    case F3101:  // iPod��
        Init(512, 512, 510, 510, 0x80000);
        break;

    case F3102:  // iPod��
        Init(256, 256, 254, 254, 0x20000);
        break;

    case F3105:  // iPod��
        Init(640, 640, 640, 640, 0xc8000);
        break;

    case F3106:  // iPod��
        Init(112, 110, 110, 110, 0x7000);
        break;

    case F3107:  // iPod��
        Init(176, 176, 176, 176, 0x10000);
        break;

    case F3111:  // ��Ƶ���� iphone4 ����
        Init(160, 158, 150, 150, 0xc580);
        break;

    case F3113:  // ��Ƶ���� iphone4 ����
        Init(112, 110, 110, 110, 0x7000);
        break;

    case F3141:  // ��Ƶ���� 4.3
        Init(160, 158, 150, 150, 0xc580);
        break;

    case F3143:  // ��Ƶ���� 4.3
        Init(112, 110, 110, 110, 0x7000);
        break;

    case F4001:  // ��Ƶ����
        Init(120, 110, 120, 110, 28800);
        break;

    case F4002:  // ��Ƶ����
        Init(32, 32, 32, 32, 2048);
        break;

    case F4010:  // ��Ƶ����
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

    case F4005:  // ԭͼƬ, �ļ���СΪ0������
    case F4006:  // ԭͼƬ, �ļ���СΪ0������(iOS4����)
    case F4035:  // ԭͼƬ, �ļ���СΪ0������(iPod�з���)
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
    // _type < 1000 �������� 32x32.ithmb ����Ƭ����ͼ�ļ�
    char* pBuffer = ReadFile(_dbFile, _occupySpace*index,
        _type < 1000 ? _occupySpace : _width* _height * 2);
    RETURN_NULL_IF( !pBuffer );

    Rect r(0, 0, _realWidth, _realHeight);
    if (_type < 1000)
    {
        // ��ȡʵ�ʿ��
        int imageWidth, imageHeight;
        const char* p = pBuffer + _occupySpace - 12;
        memcpy_s(&imageWidth, 4, p, 4);
        memcpy_s(&imageHeight, 4, p+4, 4);

        // ���������к���
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
    // �Ѿ������ٷ�: ����һ������
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

    if ( _type < 1000 ) // 32x32.ithmb: �����ļ���ʽ��һ��, ÿ��ͼ�����п������
    {
        int width = pThumb->GetWidth();
        int height = pThumb->GetHeight();
        memcpy(pBuffer + _occupySpace - 12, &width, 4);
        memcpy(pBuffer + _occupySpace - 8, &height, 4);
    }
    else if ( _type == F3101 || _type == F3102 )
    {
        // ��С��pBufferһ��(pBuffer��Image2Buffer��new)
        char* pNewBuffer = new char[_occupySpace];
        ZeroMemory(pNewBuffer, _occupySpace);

        Rect r(0,0,_width, _height);
        char* p = pNewBuffer;
        Rgb555ToQuater(pBuffer, r, p);

        // �������ݿ��滻�����ݿ�
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
    // �Ѿ������ٷ�: ����һ������
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
    // ������ͼ��ȡ����
    BitmapData* pData = new BitmapData;
    Rect rect(0, 0, pThumb->GetWidth(), pThumb->GetHeight());
    pThumb->LockBits(&rect, ImageLockModeRead, PixelFormat16bppRGB555, pData);

    bool needPaddingWithe = NeedPaddingWhite();

    // ����ͼƬʵ��ռ�ݴ�С����������
    char* pBuffer = new char[_occupySpace];
    memset(pBuffer, needPaddingWithe ? 0xFF : 0, _occupySpace);

    int imageWidth = pThumb->GetWidth();
    int imageHeight = pThumb->GetHeight();

    bool padding = ! NeedFillThumb();

    int paddingRow = ((padding ? _height : _realHeight) - imageHeight + 1) / 2; // �����������
    int paddingColumn = ((padding ? _width : _realWidth) - imageWidth) / 2;    // �����������

    if ( needPaddingWithe )
    {
        for(int row = 0; row < paddingRow; ++row)  // �����(���������)
        {
            memset(pBuffer + _width * 2 * row, 0xFF, _realWidth * 2);
            memset(pBuffer + _width * 2 * (_realHeight-row-1), 0xFF, _realWidth * 2);
        }
    }

    char* pPixels = (char*)pData->Scan0;
    for(int row = 0; row < imageHeight; ++row)
    {
        // һ�����ݵ���ʼλ��
        char* pLine = pBuffer + _width * 2 * (paddingRow + row);

        if ( needPaddingWithe )   // �����(��ߺ��ұ�)
        {
            memset(pLine, 0xFF, paddingColumn*2);
            memset(pLine + (paddingColumn + imageWidth) * 2,
                0xFF, paddingColumn*2);
        }

        // ����ÿ��ͼ������
        memcpy(pLine + paddingColumn * 2, pPixels + pData->Stride * row, imageWidth * 2);
    }

    // �ͷ�����, ����������
    pThumb->UnlockBits(pData);
    delete pData;
    return pBuffer;
}

Bitmap* IthmbDB::MakeThumbnail(Image* pImage, bool bNoEmpty, int destWidth, int destHeight)
{
    // ����ˮƽ�ʹ�ֱ�����ϵ�ѹ����
    double xRatio = double(pImage->GetWidth ()) / destWidth;
    double yRatio = double(pImage->GetHeight()) / destHeight;

    // ���������, ������С��ѹ����, �����������ѹ����
    double ratio = bNoEmpty ? min(xRatio, yRatio) : max(xRatio, yRatio);

    // ����ԭͼ��������������ͼ������r1, �����ɵ�����ͼ������destRect
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

    // ���ó��� 2.0������ iPodUtility::CreateThumbnail()
    g.Clear(Color::Transparent);                            // ��͸������ɫ���
    //     g.SetInterpolationMode(InterpolationModeHighQuality);   // ���ò�ֵ��ʽ
    //     g.SetSmoothingMode(SmoothingModeHighQuality);           // ����ƽ��ģʽ

    // ������ͼ
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
    // ����ͼ��, ȡ��д����������
    Bitmap* pBmp = new Bitmap(r.Width, r.Height, PixelFormat16bppRGB555);
    BitmapData* pData = new BitmapData;
    Rect rect(0, 0, r.Width, r.Height);
    pBmp->LockBits(&rect, ImageLockModeWrite, PixelFormat16bppRGB555, pData);

    // ��ÿ��ͼ�����ݿ�����������
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
    // ��ͼ���Ҳ���·����4/8������(��ɫ), ������ͼ��İ�ɫ���
    // ����iosֱ�ӽ�BTH�ļ�����д����Ƶ������, ��������ͼ��ʾ
    // ��ĳЩithmb�ļ���Ҳ�����ƵĴ���(�ο�NeedPaddingWhite����)
    int REAL_WIDTH = ip4 ? 150 : 75;
    int REAL_HEIGHT= ip4 ? 150 : 75;
    int BTH_WIDTH  = ip4 ? 158 : 79;
    int BTH_HEIGHT = ip4 ? 158 : 79;

    // ��ȡ����
    Bitmap* pBmp = Bitmap::FromFile(thmFile.c_str());
    RETURN_FALSE_IF( ! pBmp );

    BitmapData* pData = new BitmapData;
    Rect rect(0, 0, pBmp->GetWidth(), pBmp->GetHeight());
    pBmp->LockBits(&rect, ImageLockModeRead, PixelFormat16bppRGB555, pData);

    // ����BTH�ļ�����������
    unsigned char fileHeader[16] = {0xBA, 0xCD, 0xDC, 0xAB, ip4 ? 0x48 : 0x47, 4, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    int bodyLength = BTH_WIDTH * BTH_HEIGHT * 2;
    char* fileBody = new char[bodyLength];
    memset(fileBody, 0x7FFF, bodyLength); // ���Ϊ��ɫ

    // ��������
    char* pPixels = (char*)pData->Scan0;
    for(int row = 0; row < REAL_HEIGHT; ++row)
    {
        memcpy(fileBody + BTH_WIDTH * 2 * row, pPixels + pData->Stride * row, REAL_WIDTH * 2);
    }

    // �ͷ�ԭͼ����
    pBmp->UnlockBits(pData);
    delete pData;
    delete pBmp;

    // д�ļ�
    fstream fs(bthFile.c_str(), ios_base::binary|ios_base::out);
    fs.write((char*)fileHeader, 16);
    fs.write(fileBody, bodyLength);
    fs.close();

    delete[] fileBody;
    return true;
}
