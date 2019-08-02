#pragma once

#include "PictureManager.h"

namespace ND91Assistant
{
    class DeviceData;
    class AndroidPhotoBucket;
	class PictureData;

    class BaseGroup;
    class PictureFolder;
    class PictureGroup;

    // ͼƬ����
    class CLASSINDLL_CLASS_DECL AndroidPictureManager : public PictureManager
    {
    public:
        AndroidPictureManager(const DeviceData* pDevice);
        virtual ~AndroidPictureManager(void);

        // Summary  : ��ʼ������
        // Returns	: �Ƿ�ɹ�
        virtual bool Load();

#pragma region ������ز���

        // Summary  : ���ͼ�����
        // Parameter: const wstring displayname������
        virtual string AddPhotoGroup(const wstring displayname, wstring path = L"");

        // Summary  : ɾ��ͼ�����
        // Returns  : �Ƿ�ɹ�
        // Parameter: PictureGroup * pGroup ����ָ��
        virtual bool DeleteGroup(PictureGroup* pGroup);

		// Summary  : �޸�ͼ�������
		// Returns  : �Ƿ�ɹ�
		// Parameter: const string nGroupid ����ID
		// Parameter: const wstring name	������
		virtual bool RenameGroup(const string nGroupid, const wstring name);

#pragma endregion

#pragma region ͼƬ��ز���

        
		// Summary  : ��Ӷ������ͼƬ(��֧��ios4.0���ϰ汾)
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : ����ͼƬ����
		virtual bool AddPictureToGroup(vector<wstring>* localPaths, const string groupId);


		// Summary  : ����ɾ��ͼƬ
		// Returns  : bool
		// Parameter: vector<const PictureData * > vPictures : ��ɾ����ͼƬ����
		// Parameter: PictureFolder* pFolder : ɾ��ͼƬ����������
		virtual bool DeletePicture( const vector<const PictureData*>* vPictures, const string groupId);


		// Summary  : ��ͼƬ�Ƶ���һ������
		//            ֻ������ͨ������ƶ�. ���������Ҫʹ����Ӻ�ɾ������
		// Returns	: bool �Ƿ�ɹ�
		// Parameter: const vector<const PictureData*>* vPictures : ͼƬָ�뼯��
		// Parameter: pSourceGroup : ԭ����
		// Parameter: pDestGroup   : Ҫ�Ƶ��ķ���
		virtual bool MovePicture( const vector<const PictureData*>* vPictures, string sourceGroupId, string destGroupId);

        // Summary  : �༭ͼƬ
		// Returns  : bool �Ƿ�ɹ�
		// Parameter: const PictureData * pPicture
		virtual bool ChangeImage( PictureData* pPicture, wstring srcPath);

        // Summary  : ����ͼƬ
        // Returns  : �Ƿ�ɹ�
        // Parameter: pPicture ͼƬָ��
        // Parameter: wstring �Զ���ͼƬ�����·��
        virtual bool DownloadPicture(const PictureData* pPicture, wstring customPath = L"");

        // Summary  : ����ͼƬ������ͼ
        // Returns  : �Ƿ�ɹ�
        // Parameter: pPicture ͼƬָ��
        virtual bool DownloadThumbnail(PictureData* pPicture);

		// Summary  : ��Ϊ��ֽ
		// Returns	: bool
		// Parameter: wstring localpath
		virtual bool SetWallPaper(PictureData* pPicture, SetWallPaper_Type setType);

        virtual int GetPhotoCount();
#pragma region ͼƬ��ز���

#pragma region ˽�з���
    private:

		// Summary  : ���ݷ���id�ҷ����Ƿ����
		bool ExistBucket(string groupId);

		// Summary  : �������� (��׿��)
		// Return   : ����ID, �ձ�ʾʧ��
		// Parameter: wstring path �豸�ϵ�·��
		string CreateAndroidBucket(wstring path);

        // ���豸�ϴ�������
        bool CreateAndroidBucketInDevice( AndroidPhotoBucket* Bucket );

        // ���ڴ��д�������
        void CreateAndroidBucketInMemory(AndroidPhotoBucket* pBucket);

        // Summary  : ���ط��� (��׿��)
        // Returns	: void
        void LoadAndroidBucket();

        // Summary  : ����ͼƬ (��׿��)
        // Returns	: void
        void LoadAndroidPicture();

        // Summary  : ��������ͼ��Ϣ (��׿��)
        void LoadAndroidThumbnails();

		// Summary  : ���ͼƬ (��׿��)
		// Parameter  groupId	����ID, �ձ�ʾʧ��
		// Parameter: filePath	�����ļ���·��
		const PictureData* AndroidAddPicture(wstring filePath, const string groupId);

#pragma endregion
        
    private:

    };

}
