#pragma once

#include "Module/Picture/BaseGroup.h"
#include "Module/Picture/PhotoBucketData.h"
#include "Module/Picture/PictureData.h"
#include "Module/Picture/PictureFolder.h"
#include "Module/Picture/PictureGroup.h"
#include "Module/Picture/ThumbnailData.h"

namespace ND91Assistant
{
    // ���ñ�ֽ����������
	enum SetWallPaper_Type
	{
        UnKnowType      = 0,	// δ֪
		LockBackground  = 1,	// ������Ļ
		HomeBackground  = 2,	// ����Ļ(4.0 ����)
		LockAndHome     = 3,	// ͬʱ��������Ļ��������Ļ
	};

    class DeviceData;
    class AndroidPhotoBucket;
	class PictureData;

    class PictureFolder;
    class PictureGroup;
	class PictureLibrary;
	class BaseGroup;

    // ͼƬ����
    class CLASSINDLL_CLASS_DECL PictureManager
    {
    public:
        PictureManager(const DeviceData* pDevice);
        virtual ~PictureManager(void);

        // Summary  : ��ʼ������
        // Returns	: �Ƿ�ɹ�
        virtual bool Load() = 0;

		// �ͷ�
		void Release();

#pragma region ������ز���

        // ��ȡͼ�����
        void GetPictureGroups(vector<PictureGroup*>* pGroups){
            pGroups->insert(pGroups->begin(), _PictureGroups.begin(), _PictureGroups.end());
        }

		// ��ȡ��Ƭ
		PictureFolder* GetCamera(){return _pCamera;}

		// ��ȡ��Ļ��ͼ
		PictureFolder* GetScreenShot(){return _pScreenShot;}

		// ��ȡ��ֽ
		PictureFolder* GetWallpaper(){return _pWallpaper;}

        // Summary  : ���ͼ�����
        // Parameter: const wstring displayname������
		virtual std::string AddPhotoGroup(const wstring displayname, wstring path = L"") = 0;

        // Summary  : ɾ��ͼ�����
        // Returns  : �Ƿ�ɹ�
        // Parameter: PictureGroup * pGroup ����ָ��
        virtual bool DeleteGroup(PictureGroup* pGroup) = 0;

		// Summary  : �޸�ͼ�������
		// Returns  : �Ƿ�ɹ�
		// Parameter: const string nGroupid ����ID
		// Parameter: const wstring name	������
		virtual bool RenameGroup(const string nGroupid, const wstring name) = 0;

#pragma endregion

#pragma region ͼƬ��ز���

        // ��ȡȫ��ͼƬ
        void GetAllPictures(vector<PictureData*>* pDeque) const;

		// Summary  : ����ͼƬ������(������ͨ����, ���Զ���ӵ�"����ͼƬ"��)
		// Returns	: �Ƿ�ɹ�
		// Parameter: localPath : Ҫ��ӵı���ͼƬ
		// Parameter: nType		: Ҫ��ӵ��ķ���
		bool AddPictureToGroup(wstring localPath, PICTURE_TYPE nType);
        
		// Summary  : ��Ӷ������ͼƬ(��֧��ios4.0���ϰ汾)
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : ����ͼƬ����
		virtual bool AddPictureToGroup(vector<wstring>* localPaths, const string groupId) = 0;


		// Summary  : ����ɾ��ͼƬ
		// Returns  : bool
		// Parameter: vector<const PictureData * > vPictures : ��ɾ����ͼƬ����
		// Parameter: PictureFolder* pFolder : ɾ��ͼƬ����������
		virtual bool DeletePicture( const vector<const PictureData*>* vPictures, const string groupId) = 0;

        // Summary  : �༭ͼƬ
		// Returns  : bool �Ƿ�ɹ�
		// Parameter: const PictureData * pPicture
		virtual bool ChangeImage( PictureData* pPicture, wstring srcPath) = 0;

		// Summary  : ��ͼƬ�Ƶ���һ������
		//            ֻ������ͨ������ƶ�. ���������Ҫʹ����Ӻ�ɾ������
		// Returns	: bool �Ƿ�ɹ�
		// Parameter: const vector<const PictureData*>* vPictures : ͼƬָ�뼯��
		// Parameter: pSourceGroup : ԭ����
		// Parameter: pDestGroup   : Ҫ�Ƶ��ķ���
		virtual bool MovePicture( const vector<const PictureData*>* vPictures, 
									string sourceGroupId, 
									string destGroupId) = 0;

        // Summary  : ����ͼƬ
        // Returns  : �Ƿ�ɹ�
        // Parameter: pPicture ͼƬָ��
        // Parameter: wstring �Զ���ͼƬ�����·��
        virtual bool DownloadPicture(const PictureData* pPicture, wstring customPath = L"") = 0;

        // Summary  : ����ͼƬ������ͼ
        // Returns  : �Ƿ�ɹ�
        // Parameter: pPicture ͼƬָ��
        virtual bool DownloadThumbnail(PictureData* pPicture) = 0;

		// Summary  : ��Ϊ��ֽ
		// Returns	: bool
		// Parameter: wstring localpath
		virtual bool SetWallPaper(PictureData* pPicture, SetWallPaper_Type setType) = 0;

		// Summary  : ���ݷ���id��÷���ָ��
		// Returns  : BaseGroup*	ͼƬָ��
		// Parameter: string id		����id
        BaseGroup* GetBaseGroup( string id );

        // ��δԽ���򲻰�����ֽ����
        virtual int GetPhotoCount();

		// �����������Ƿ������ͼƬ
		bool ContainsInGroup(int pictureId);

#pragma region ͼƬ��ز���

#pragma region ˽�з���
    protected:

        PictureGroup* FindGroup( string id );

        // ������Ƭ����
        PictureFolder* CreateCamera(wstring path);

        // ������ֽ����
        PictureFolder* CreateWallpaper(wstring path);

        // ������ͼ����(IOS��)
        PictureFolder* CreateScreenshot( wstring path );

		// Summary  : ��������ͼ
		// Returns  : 
		// Parameter: std::wstring picPath Դͼ�ļ�·��
		// Parameter: int & width  Ԥ������ͼ���, ��ԭͼ��߾�С��Ԥ������򱻸�ֵΪԭͼ���
		// Parameter: int & height Ԥ������ͼ�߶�, ��ԭͼ��߾�С��Ԥ������򱻸�ֵΪԭͼ�߶�
		// Parameter: std::wstring thmPath ����ͼ�ļ�·��
		bool MakeThumbnail(std::wstring picPath, int& width, int& height, std::wstring thmPath, bool isIos);

#pragma endregion

    protected:

        // ͼ�����ͨ������PictureGroup���ͼƬID, ͼƬ��������_AllPictures��
        vector<PictureGroup*>		_PictureGroups;		// ͼ�����
        PictureLibrary*	            _pAllPictures;		// ͼ��ͼƬ

        // ��Ƭ/��ֽ/��ͼ�������������, ʹ��PictureFolder���PictureDataָ��
        PictureFolder*              _pCamera;	  		// ��Ƭ
        PictureFolder*              _pWallpaper;  		// ��ֽ
        PictureFolder*              _pScreenShot; 		// ��ͼ

        const DeviceData*			_pDevice;			

    };

}
