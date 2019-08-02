#pragma  once

#include <string>
#include <windows.h>
#include <GdiPlus.h>

using namespace std;
using namespace Gdiplus;

namespace ND91Assistant
{

    // ����ͼ����
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
        F3005 = 3005,   // ר����ͼ
        F3006 = 3006,   // ר��Сͼ TrackBrowser
        F3007 = 3007,   // ��Ƶ���� VideoBrowser
        F3008 = 3008,   // FullScreen
        F3009 = 3009,   // Email
        F3011 = 3011,   // Misc
        F3101 = 3101,   // iPod
        F3102 = 3102,   // iPod
        F3105 = 3105,   // iPod
        F3106 = 3106,   // iPod
        F3107 = 3107,   // iPod
        F3111 = 3111,   // ��Ƶ���� iphone4 ����
        F3113 = 3113,   // ��Ƶ���� iphone4 ����
        F3034 = 3034,   // iPad����
        F3039 = 3039,   // ��Ƶ���� ios 4.3
        F3041 = 3041,   // iPad����
        F3141 = 3141,   // ��Ƶ���� ios 4.3
        F3143 = 3143,   // ��Ƶ���� ios 4.3
        F4001 = 4001,   // ��Ƶ����
        F4002 = 4002,   // ��Ƶ����
        F4005 = 4005,   // ԭͼ
        F4006 = 4006,   // ԭͼ
        F4010 = 4010,   // ��Ƶ����
        F4031 = 4031,   // iPad����
        F4032 = 4032,   // iPad����
        F4035 = 4035,   // ԭͼ
        F4040 = 4040,   // iPad����
    };


    // ios�豸�� ithmb �ļ���д
    class IthmbDB
    {
    public:
        // Summary  : ���캯��
        // Parameter: IOS_THUMB_TYPE t �ļ�����
        // Parameter: wstring filePath ithmb    �ļ�·��
        IthmbDB(IOS_THUMB_TYPE t, wstring filePath);
        ~IthmbDB();

        // Summary  : ��ithmb�ļ�����ȡimg 
        // Returns  : ����ͼָ��(�����߸���delete)
        // Parameter: int index ithmb�еĵڼ���ͼƬ(��0��ʼ)
        Bitmap*  ReadImage(int index) const;

        // Summary  : ��ithmb�ļ�����ȡimg 
        // Returns  : ����ͼָ��(�����߸���delete)
        // Parameter: index ithmb�е��ļ�ƫ����
        // Parameter: r ����ͼʵ��ռ��λ��
        Bitmap*  ReadImageByOffset(int offset, Rect r) const;

        // Summary  : ��������ͼ��д�뵽ithmb�ļ���ָ��λ��
        // Parameter: Image * pImage ԭͼ
        // Parameter: int index д��ithmb�ڼ����ļ�(��0��ʼ)
        Rect WriteImageIndex(Image* pImage, int index);

        // Summary  : ��������ͼ��д�뵽ithmb�ļ���ָ��λ��
        // Parameter: Image * pImage ԭͼ
        // Parameter: int offset д��ithmb�ļ��е�ƫ����(��0��ʼ)
        Rect WriteImageOffset(Image* pImage, int offset);

        int     GetFileSize() const;
        int     GetOccupySpace() const;
        int     GetRealWidth() const;
        int     GetRealHeight() const;
        int     GetWidth() const;
        int     GetHeight() const;
        wstring GetFilename() const;
        IOS_THUMB_TYPE GetType() const;

        // Summary  : ��������ͼ
        // Returns  : ����ͼָ��(�����߸���delete)
        // Parameter: Image * pImage ԭͼ
        // Parameter: bool bNoEmpty �Ƿ����հ�
        //                          = true : ����С��ѹ������������ͼ(�����հף������Ĳ��ֱ�����);
        //                          = false: ������ѹ������������ͼ(�������¿հ�)
        static Bitmap* MakeThumbnail(Image* pImage, bool bNoEmpty, int destWidth, int destHeight);

        // �жϴ������Ƿ�"ԭͼƬ"����, ���򷵻�true
        // ԭͼƬ���͵�����ͼ������, ���Բ�����
        static bool IsSouceFileType(IOS_THUMB_TYPE t);

        // Summary  : ����BTH����ͼ�ļ�
        // Returns  : �ɹ�����true
        // Parameter: sourceFile THM�ļ�·��
        // Parameter: bthFile    BTH�ļ�·��
        // Parameter: ip4  true��ʾip4.0���ϰ汾
        static bool MakeBthFile(wstring thmFile, wstring bthFile, bool ip4);

    private:
        // ��Դͼ��������ͼ
        static Bitmap* Draw(Image* pSourceImage, Rect sourceRect, Rect destRect);

        // Summary  : ������д��ڼ���ͼƬ
        // Parameter: int index �ڼ���ͼƬ
        // Parameter: char * pBuffer ����
        // Parameter: int bufferSize ���ݳ���
        void WriteToFile(int index, char* pBuffer, int bufferSize);

        // ��ȡ�ļ�ָ��λ,
        char* ReadFile(wstring filename, int start, int length) const;

        // Summary  : ��ʼ������
        // Parameter: int width                 ����ͼ�Ŀ��
        // Parameter: int height                ����ͼ�ĸ߶�
        // Parameter: int realWidth             ͼƬ���ݵ�ʵ�ʿ��
        // Parameter: int realHeight            ͼƬ���ݵ�ʵ�ʸ߶�
        // Parameter: int occupySpace           ÿ��ͼƬ��ռ�ÿռ�
        void Init(int width, int height, int realWidth, int realHeight, int occupySpace);

        // Summary  : ������Ҫд�������
        // Returns  : ��Ҫд�������buffer, ��СΪ_occupySpace
        // Parameter: Bitmap * pThumb ����ͼָ��
        char* Image2Buffer(Bitmap* pThumb);

        // �Ƿ���Ҫ���ܱ�����ɫ
        bool NeedPaddingWhite();

        // ����true��ʾ����ͼ�����հ�, ����ԭ����, ��ͼ���м䲿�����
        bool NeedFillThumb();

        Bitmap* ReadRect(const char* pBuffer, Rect r) const;

        // Summary  : ���ĵȷָ�ʽͼ������ת��Ϊ����RGB555ͼ������
        //              Ŀǰ��F3101/F3102Ϊ�ĵȷָ�ʽ
        // Parameter: const char * pBuffer �ĵȷָ�ʽͼ������
        // Parameter: Rect r ͼ���С
        // Parameter: char * pData �������RGB555ͼ������
        void QuaterToRgb555( const char* pBuffer, Rect r, char* pData) const;

        // Summary  : ������RGB555ͼ������ת��Ϊ�ĵȷָ�ʽͼ������
        //              Ŀǰ��F3101/F3102Ϊ�ĵȷָ�ʽ
        // Parameter: const char * pBuffer �������RGB555ͼ������
        // Parameter: Rect r ͼ���С
        // Parameter: char*& pData �ĵȷָ�ʽͼ������(ע���ָ�뽫���޸�)
        void Rgb555ToQuater(const char* pBuffer, Rect r, char*& pData) const;

        Rect GetSizeAndPadding( int width, int height ) ;
    private:

        wstring _dbFile;        // ithmb�ļ�·��

        IOS_THUMB_TYPE _type;   // �ļ�����

        int     _width;         // �ļ��е�ͼ��߶�
        int     _height;        // �ļ��е�ͼ����
        int     _realWidth;     // ͼƬ���ݵ�ʵ�ʿ��
        int     _realHeight;    // ͼƬ���ݵ�ʵ�ʸ߶�

        int     _occupySpace;   // ÿ��ͼƬ��ռ�ÿռ�
    };
}
