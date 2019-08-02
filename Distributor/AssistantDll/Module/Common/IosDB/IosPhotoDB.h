#pragma  once

#include "IosPdbData.h"
#include "IthmbDB.h"

#include <string>
#include <map>
#include <list>

using namespace std;

namespace Gdiplus
{
    class Bitmap;
}

namespace ND91Assistant
{
    // ios�豸��photo database�ļ���д��
    class IosPhotoDB
    {
    public:
        // Summary  : ���캯��
        // Parameter: wstring path ����Ӳ�̵�photo database�ļ�·��
        // Parameter: const vector<int> & thumbTypes ��Ҫ֧�ֵ�THUMB�ļ�����
        // Parameter: wstring thumbFileFolder ithmb�ļ�����Ŀ¼
        //              Ϊ��ʱ�趨Ϊphoto database�ļ�����Ŀ¼��THUMBS��Ŀ¼
        IosPhotoDB(wstring path, const vector<int>& thumbTypes, wstring thumbFolder = L"");
        ~IosPhotoDB(void);

        // Summary  : ȡ����ͼ�ļ���Ϣ
        // Returns  : map<�ļ�ID, ͼ���С>
        map<int, int> GetThumbIdAndSize();

        // Summary  : ��������ͼƬid
        // Returns  : vector<ͼƬID>, �����߸���delete
        vector<int>* Load();

        // Summary  : ��������ͼƬ����
        int size();

        // Summary  : ����ǰDB����д���ļ�
        // Parameter: wstring destFile д���Ŀ���ļ���
        void Write(wstring destFile);

        // Summary  : ��ͼƬ��ӵ�ͼ��(�����ԭͼ�ļ�, writeʱ�����)
        // Returns  : ͼƬid: >0��ʾ�ɹ�, -1��ʾʧ��
        // Parameter: Image * pImage ͼƬָ��
        int AddImage(Bitmap* pImage);

        // Summary  : ��ͼ����ɾ��ͼƬ(��ɾ��ԭͼ�ļ�)
        // Returns  : �ɹ�����true, û�ҵ�����false
        // Parameter: int imageId ͼƬid
		// Parameter: �Ƿ�����з�����ɾ��
        bool DelImage(int imageId, bool deleteAll = true);

        // Summary  : �޸�ͼƬ����(ͬʱ�޸�ԭͼ�ļ�)
        // Returns  : �ɹ�����true, û�ҵ�����false
        // Parameter: int imageId ͼƬid
        // Parameter: Bitmap * pBitmap ͼƬ����
        bool UpdateImage(int imageId, Bitmap* pBitmap);

		// Summary  : ȡͼƬ����
		// Returns  : Bitmap* ͼƬ���ݣ������߸���ɾ��(�Ҳ�������NULL)
		// Parameter: int id ͼƬid
		// Parameter: IOS_THUMB_TYPE type ����ͼ����
		Bitmap* GetImage(int id, IOS_THUMB_TYPE type);

        // Summary  : ȡԭͼ·��(ios 3.2���ϰ汾)
        // Returns  : ͼƬ·��, �Ҳ������ؿ��ַ���
        // Parameter: id ͼƬID
        // Parameter: delimiter Ŀ¼�ָ���, ֻ����\��/
        wstring GetImageFilePath(int id, char delimiter);

        // Summary  : ȡ������Ҫ�ϴ����ļ���
        // Returns  : map<ͼƬid, ͼƬ·��>
        list<int> GetUploadFiles();

        // Summary  : ȡ������Ҫɾ�����ļ���
        // Returns  : vector<ͼƬ·��>
        vector<wstring> GetDeleteFiles();

#pragma region ͼ��������

        // Summary  : ����ͼ�����з���
        // Returns  : map<����ID, ��������>��ָ��, �����߸���ɾ��
        map<int, wstring>* GetAllCategory();

        // Summary  : ȡĳ�������µ�����ͼƬ
        // Returns  : vector<ͼƬid>��ָ��, �����߸���ɾ��
        // Parameter: int id ����ID
        vector<int>* GetCategoryImages(int id);

        // Summary  : �������
        // Parameter: int id ����id
        // Parameter: wstring newName ����
        void RenameCategory(int id, wstring newName);

        // Summary  : ���ӷ���
        // Returns  : ����ID, -1��ʾ����ʧ��
        // Parameter: wstring name ������
        int AddCategory(wstring name);

        // Summary  : ɾ������
        // Parameter: int id ����id
        void DelCategory(int id);

        // Summary  : ��ͼƬ��һ�������ƶ�����һ������
        // Return   : 0 �ɹ� 1 ԭ���಻���� 2 �·��಻���� 3 �·������д�ͼ 4 ԭ�����޴�ͼ
        // Parameter: int imageId ͼƬid
        // Parameter: int originalCategoryId ԭ����id
        // Parameter: int newCategoryId �·���id
        int MoveImageBetweenCategory(int imageId, int originalCategoryId, int newCategoryId);

        // Summary  : �����е�ͼƬ��ӵ�����(������ӵ�ͼ��)
        // Return   : 0 �ɹ� 1 ���಻���� 2 ���������д�ͼƬ
        // Parameter: int imageId ͼƬid
        // Parameter: int categoryId ����id
        int AddImageToCategory(int imageId, int categoryId);

        // Summary  : �ӷ������Ƴ�ͼƬ(�Ƴ��������з����о���, ��ͼ��Ȼ������"ȫ��"��)
        // Returns  : �ɹ�����true, û�ҵ�����false
        // Parameter: int imageId ͼƬid
        // Parameter: int categoryId ����id
        bool RemoveFromCategory(int imageId, int categoryId);

#pragma endregion

        vector<wstring> GetThumbFileName();

        void SetNeedMHAF(bool val) { _needMHAF = val; }

    private:

        void Read();

        void InitThumbInfo(const vector<int>& thumbTypes);

        IthmbDB* GetIthmb( IOS_THUMB_TYPE type );

    private:

        wstring         _filePath;      // �ļ�·��
        wstring         _workFolder;    // �����ʱ�ļ��Ĺ���Ŀ¼
        wstring         _thumbFolder;   // ithmb�ļ����ڵ�Ŀ¼

        PdbFileHead         _head;      // �ļ�ͷ
        PdbImageList*       _images;    // ͼƬ����
        PdbCategoryList*    _category;  // ��������
        PdbFileList*        _files;     // �ļ�����

        THUMB_FILES         _thumbs;    // ÿ������ͼ�Ķ�д��ʵ��

        bool            _needMHAF;      // �㲻����Ƿ���Ҫ��ֻ���õ�����������
    };
}
