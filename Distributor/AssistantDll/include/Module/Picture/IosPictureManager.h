#pragma once

#include "PictureManager.h"

namespace ND91Assistant
{
    class DeviceData;
	class PictureData;

    class BaseGroup;
    class PictureFolder;
    class PictureGroup;
	class IosPictureSqlite5;

    // ͼƬ����
	class CLASSINDLL_CLASS_DECL IosPictureManager : public PictureManager
    {
    public:
        IosPictureManager(const DeviceData* pDevice);
        virtual ~IosPictureManager(void);

        // Summary  : ��ʼ������
        // Returns	: �Ƿ�ɹ�
        virtual bool Load();

		// Summary  : �ͷ��ڴ�
		void Release();
		
		// Summary  : ͬ�����ֻ�
		// Returns	: �Ƿ�ɹ�
		bool IosSync(ProgressSubject* pSubject); 
		bool IosSyncThroughATHost();

		// Summary  : �������Ŀ¼
		// Returns	: void
        void ClearCache();

#pragma region ������ز���

        // Summary  : ���ͼ�����
        // Parameter: const wstring displayname������
		virtual std::string AddPhotoGroup(const wstring displayname, wstring path = L"");

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

        // Summary  : ����ͼƬ������(������ͨ����, ���Զ���ӵ�"����ͼƬ"��)
        // Returns	: �Ƿ�ɹ�
        // Parameter: localPath : Ҫ��ӵı���ͼƬ
        // Parameter: groupId   : Ҫ��ӵ��ķ���
        //virtual bool AddPictureToGroup(wstring localPath, PICTURE_TYPE nType);

		// Summary  : ��Ӷ������ͼƬ(��֧��ios4.0���ϰ汾)
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : ����ͼƬ����
		virtual bool AddPictureToGroup(vector<wstring>* localPaths, const string groupId);


		// Summary  : ����������ͼ�����Ѵ��ڵ�ͼƬ, ��ӵ���һ��ͼ�����(��ios��)
		// Returns	: �Ƿ�ɹ�: ��������ȫ�����߷���id�Ҳ����򷵻�false
		// Parameter: const vector<const PictureData*>* vPictures : ͼƬָ�뼯��
		// Parameter: groupId  : ����ID
		bool IosAddExistPicture(const vector<const PictureData*>* vPictures, const string groupId);


		// Summary  : ����ɾ��ͼƬ
		// Returns  : bool
		// Parameter: vector<const PictureData * > vPictures : ��ɾ����ͼƬ����
		// Parameter: PictureFolder* pFolder : ɾ��ͼƬ����������
		virtual bool DeletePicture( const vector<const PictureData*>* vPictures, const string groupId);


		// Summary  : ��ͼ��������Ƴ�ͼƬ(��ios��)
		//            ���˷�������ͨ����, ��"����ͼƬ"����Ȼ�д�ͼƬ
		//            ���˷���������ͼƬ, ��֧�ַ���false
		// Returns	: �Ƿ�ɹ�: ������id��"ȫ��ͼƬ"���߷���id�Ҳ����򷵻�false
		// Parameter: const vector<const PictureData*>* vPictures : ͼƬָ�뼯��
		// Parameter: groupId  : ����ID
		bool IosRemoveFromGroup( const vector<const PictureData*>* vPictures, const string groupId);


		// Summary  : ��ͼƬ�Ƶ���һ������
		//            ֻ������ͨ������ƶ�. ���������Ҫʹ����Ӻ�ɾ������
		// Returns	: bool �Ƿ�ɹ�
		// Parameter: const vector<const PictureData*>* vPictures : ͼƬָ�뼯��
		// Parameter: pSourceGroup : ԭ����
		// Parameter: pDestGroup   : Ҫ�Ƶ��ķ���
		virtual bool MovePicture( const vector<const PictureData*>* vPictures, 
									string sourceGroupId, 
									string destGroupId);

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

		// Summary  : �ж��Ƿ�֧���������ñ�ֽ
		// Returns  : bool
		bool IsSupportHomeScreen();

        // Summary  : ��Ϊ��ֽ
        // Returns	: bool
        // Parameter: wstring localpath
        virtual bool SetWallPaper(PictureData* pPicture, SetWallPaper_Type setType);

		// ��ȡȫ��ͼƬ����
		int GetAllPicturesCount();

		//Summary  : ΪҪ�ϴ���ͼƬ����plist�ļ�
		//Parameter: ͼƬ��ϢpVecPicture��ͼƬ��uuid��ͼ�������groupName
		void GeneratePlistFile(vector<PictureData*>* pVecPicture, vector<char*> uuids, wstring groupName);

		// Summary  : ��ȡ�ֻ��ϴ洢��ͼ���UUID
		// Returns	: char*ͼ���UUID
		char* GetLibraryUUIDFromDevice();

#pragma region ͼƬ��ز���

#pragma region ˽�з���
    private:

		// ͬ���޸ĺ�����ͼƬ���ݵ��ֻ��� (4.0-5.0�汾���ͼƬͬ��ר��)
		bool IosSyncHelper();

        // ���ݣ�·����ʼ��
        bool IosInit();

        // Summary  : �����豸��ϵͳ�汾, ������ͬ��iosͼƬ����ʵ����
        // Returns  : iosͼƬ����ʵ����ָ��
        IosPicturesImpl* CreateIosImplement();

        // IOS�����������
        void LoadIosBucket();

        // IOS����ͼ��ͼƬ
        void LoadIosPicture();

        virtual int GetPhotoCount();
#pragma endregion

	protected:
		IosPicturesImpl*            _pIosImpl;      	// IosͼƬ����ʵ����

    private:
        int							_nDeviceVer;		// ϵͳ�汾
		SetWallPaper_Type			_wallpaperType;     // �Ƿ�֧��˫����ֽ
    };

}
