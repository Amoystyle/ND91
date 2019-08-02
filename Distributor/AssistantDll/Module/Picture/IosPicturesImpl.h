#pragma once

#include "Module/Picture/BaseGroup.h"

namespace ND91Assistant
{
    class PictureData;
    class PictureFolder;
	class PictureGroup;
	class AndroidPhotoBucket;
	class IosWallpaperOper;
    class UploadFileCommand;
	class DownloadFileCommand;
	class DirectoryOperationCommand;
	class IosPictureSqlite4;
	class IosPictureSqlite5;
	class IosPhotoDB;

    // interface: ��Ƭ/��ֽ/��ͼ/ͼ������Ĳ�ͬ�汾ʵ��
	class IosPicturesImpl
	{
	public:
        IosPicturesImpl(const DeviceData* pDevice);
        virtual ~IosPicturesImpl();

#pragma region ��Ƭ/��ֽ/��ͼ ��ز���

		// Summary  : ͼƬ����ģ���ʼ������
		// Returns  : void
		virtual bool Init();

        // Summary  : ����(��Ƭ/��ֽ/��ͼ)ͼƬ
        // Parameter: PictureFolder * pCamera     ���������Ƭ
        // Parameter: PictureFolder * pWallpaper  ������ű�ֽ
        // Parameter: PictureFolder * pScreenshot ������Ž�ͼ
        virtual void LoadSpecialPictures(PictureFolder* pCamera,
            PictureFolder* pWallpaper, PictureFolder* pScreenshot) = 0;

		// Summary  : ��������(��Ƭ/��ֽ/��ͼ)ͼƬ 
		// Returns  : ��ͼƬָ�뼯��
		// Parameter: vector<wstring> sourceFiles  ͼƬ�ļ�·������ 
		// Parameter: int fileNumber		�ļ�������һ������(���������ļ���)
		// Parameter: int thumbIndex		����ͼ���(����4.2.1���ϰ汾����)
		virtual vector<PictureData*>* AddSpecialPictures(vector<wstring>* vPicFiles,
						int fileNumber, int thumbIndex = -1, std::string gruopId = "") = 0;

		// Summary  : ɾ������(��Ƭ/��ֽ/��ͼ)ͼƬ
		// Returns  : �Ƿ�ɹ�
		// Parameter: vector<const PictureData * > vPictures  ��ɾ��ͼƬ����   
		virtual bool DeleteSpecialPictures(const vector<const PictureData*>* vPictures) = 0;

        // Summary  : �޸�(��Ƭ/��ֽ/��ͼ)ͼƬ
        // Returns  : �Ƿ�ɹ�
        // Parameter: PictureData * pic ͼƬָ��
        // Parameter: std::wstring srcPath ͼƬ�ļ�·��
        virtual bool UpdateSpecialPicture( PictureData* pic, wstring srcPath) = 0;

		// Summary  : ���ô���������ֽ
		// Returns  : bool				
		// Parameter: wstring wallpaper   ��ѡ�б�ֽ�ı���·��
		virtual bool SetLockBackground( wstring wallpaper ) = 0;

		// Summary  : ��������Ļ��ֽ
		// Returns  : bool
		virtual bool SetHomeBackground( wstring wallpaper ) = 0;

		// ��ø�ģ���Ƿ���Ҫͬ��
		bool GetPhotosIthmbState()		{ return _bPhotosIthmbChange;		}
		bool GetPhotoDataBaseState()	{ return _bPhotoDataBaseChange;		}
		bool GetSqliteState()			{ return _bSqliteChange;			}
		bool GetPhotoDataIthmbState()	{ return _bPhotoDataIthmbChange;	}

		// �����Ҫͬ����ͼƬ
		map<wstring, wstring>* GetUploadPictureFiles(void) { return _UploadPictureFiles; }
		// �����ҪͼƬ��uuid
		vector<char*> GetPictureUuids() {return _pictUuids;}
		
		// ��ģ��ͬ��״̬�ĳ�ʼ��
		void ResetSyncState()
		{
			_bPhotosIthmbChange		= false;		
			_bPhotoDataBaseChange	= false;
			_bSqliteChange			= false;			
			_bPhotoDataIthmbChange	= false;
			_pictUuids.clear();
		}

#pragma endregion


#pragma region ͼ�ⲿ����ز���

		// ----- ͼ�������� -----

		// IOS��������ͼ�����
		virtual map<int, wstring>* LoadGalleryBucket();

		// ���ͼ�����
		virtual int AddGalleryGroup( const wstring displayname,  wstring path);

		// Summary  : �޸�ͼ�������
		// Returns  : �Ƿ�ɹ�
		// Parameter: const string nGroupid ����ID
		// Parameter: const wstring name	������
		virtual bool RenameGalleryGroup(const string nGroupid, const wstring name);
		
		// Summary  : ɾ��ͼ�����
		// Returns  : �Ƿ�ɹ�
		// Parameter: PictureGroup * pGroup ����ָ��
		virtual bool DeleteGalleryGroup(int groupId, vector<PictureData*>* VerPictDatas);



		// ----- ͼ��ͼƬ���� -----
		
		// ��������ͼ��ͼƬ
		virtual bool LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas) = 0;

		// Summary  : ��Ӷ���ͼ��ͼƬ
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : ����ͼƬ����
		virtual vector<PictureData*>* AddPictureToGroup( vector<wstring>* localPaths, const string groupId, 
														  int nextNumber=0, int index=0, const string firstGroupId="" ) = 0;

		// Summary  : ��һ�������ͼƬ�Ƶ���һ������
		//            ֻ������ͨ������ƶ�. ���������Ҫʹ����Ӻ�ɾ������
		// Returns	: bool �Ƿ�ɹ�
		// Parameter: const vector<const PictureData*>* vPictures : ͼƬָ�뼯��
		// Parameter: pSourceGroup : ԭ����
		// Parameter: pDestGroup   : Ҫ�Ƶ��ķ���
		virtual bool MovePictures( const vector<const PictureData*>* vPictures, string sourceGroupId, string destGroupId);

		// Summary  : ����ɾ��ͼ��ͼƬ
		// Returns  : bool
		// Parameter: vector<const PictureData * > vPictures : ��ɾ����ͼƬ����
		// Parameter: PictureFolder* pFolder : ɾ��ͼƬ����������
		virtual bool DeleteGalleryPictures( const vector<const PictureData*>* vPictures );

		// Summary  : ��ͼ��������Ƴ�ͼƬ
		//            ���˷�������ͨ����, ��"����ͼƬ"����Ȼ�д�ͼƬ
		//            ���˷���������ͼƬ, ��֧�ַ���false
		// Returns	: �Ƿ�ɹ�: ������id��"ȫ��ͼƬ"���߷���id�Ҳ����򷵻�false
		// Parameter: const vector<const PictureData*>* vPictures : ͼƬָ�뼯��
		// Parameter: groupId  : ����ID
		virtual bool RemoveFromGroup( const vector<const PictureData*>* vPictures, const string groupId);

		// Summary  : �������Ѵ��ڵ�ͼƬ, ��ӵ���һ������
		// Returns	: �Ƿ�ɹ�: ��������ȫ�����߷���id�Ҳ����򷵻�false
		// Parameter: const vector<const PictureData*>* vPictures : ͼƬָ�뼯��
		// Parameter: groupId  : ����ID
		virtual bool IosAddExistPictures(const vector<const PictureData*>* vPictures, const string groupId);

		// Summary  : �༭ͼ��ͼƬ
		// Returns  : bool �Ƿ�ɹ�
		// Parameter: const PictureData * pPicture
		virtual bool UpdateGalleryPicture( PictureData* pPicture, wstring srcPath );

		// ��������ͼ�ļ�
		virtual bool DownloadThumbnail(PictureData* pPicture) = 0;
		
		// ����ԭͼ�ļ�
		virtual bool DownloadPicture(const PictureData* pPicture, wstring customPath);


		// ----- ͼƬ������ظ������� -----

		// �õ��ð汾������ithumb�ļ�id
		virtual vector<int> GetThumbType() = 0;

		// Summary  : ����ͼ����������ļ�
		// Returns  : bool
		virtual bool DownloadInitFiles() = 0;

		// ��ȡͼƬ������
		int GetAllPicturesCount();

		// ��ȡ������, ͼ��ͼƬ������
		virtual int GetSpecialPhotos();

#pragma endregion


#pragma region ͬ���������

		// Summary  : ����ͼ��ͼƬ����ָ��(���ṩ��4.x�汾���Ӻ�ɾ��ͼƬ����)
		// Returns  : void
		// Parameter: IosPhotoDB * pIosDB
		void SetIosDB(IosPhotoDB* pIosDB) { _pIosDB = pIosDB; }	

		// Summary  : �ͷ�2������
		// Returns  : void
		void ReleaseCase();

		// Summary  : �ϴ�ͼƬ�����������������ͼ�⡢�������ͼ��
		// Returns  : bool
		virtual bool UploadPictureFiles(ProgressSubject* pSubject=NULL, int percent=100) = 0;

		// Summary  : ɾ��ͼƬ�����������������ͼƬ���������ͼ��
		// Returns  : bool
		virtual bool DeletePictreFiles() = 0;

		// Summary  : �ϴ�PhotoData��sqlite�ļ�
		// Returns  : bool
		virtual bool PhotoSqilteSync() = 0;

		// Summary  : �ϴ�photodatabase�ļ�
		// Returns  : bool
		bool PhotodatabaseSync();

		// Summary  : �ϴ�Photos��ithumb�ļ�(ͼ������ͼ�ļ�)
		// Returns  : bool
		bool PhotosIthmbSync(ProgressSubject* pSubject=NULL, int percent=100);

		// Summary  : �ϴ�PhotoData��ithumb�ļ�(�������ͼ�ļ�)
		// Returns  : bool
		bool PhotoDataIthmbSync(ProgressSubject* pSubject=NULL, int percent=100);

	protected:

		// Summary  : ͨ��IosPhotoDB���421�汾���ϴ���ͼ��ͼƬ����
		// Returns  : void
		bool UploadFiles4X(ProgressSubject* pSubject=NULL, int percent=100);

		// Summary  : ͨ��IosPhotoDB���421�汾��ɾ����ͼ��ͼƬ����
		// Returns  : void
		bool DeleteFiles4X();

		// Summary  : ����sqlite���ϴ�
		// Returns  : bool
		bool SqilteDBSync();

#pragma endregion

    protected:

        // ȡ��Ƭ�ͽ�ͼ
        void LoadPhotoAndScreenShot(PictureFolder* pCamera, PictureFolder* pScreenshot);

		// ��ȡ4.x����ͼ��ͼƬ
		bool LoadPicturesByGroup4X(PictureGroup* pGroup, vector<PictureData*>& pVerDatas);
		
		// ���4.xͼ��ͼƬ
		vector<PictureData*>* AddPictureToGroup4X( vector<wstring>* localPaths, const string groupId, const string firstGroupId );

        // Summary  : ���ɲ�����չ�����ļ���
        wstring MakeFilename(PICTURE_TYPE t, int fileNumber);

		// �������ͼ�ļ���
		vector<int> GetThumbNames();

		// Summary  : ��ȡָ��Ŀ¼�µ�.ithumb�ļ�
		// Returns  : vector<NodeData*>
		// Parameter: DirData * pFolder
		map<wstring, wstring> GetPhotoAllThumbs(bool isPhotoData );

		// ����3.0-4.3.5�汾��ͼ������ļ�
		bool DownloadGalleryFiles();

		// ����421��������������ͼ�ļ�
		bool DownloadPhotoDataThumbs();

		// ����3.0-435�汾ͼ��ͼƬ����ͼ
		bool DownloadGalleryThumbnails(PictureData* pPicture);

		// ����421���������������ͼ
		bool DownloadSpecialThumbnails(PictureData* pPicture);

		// ��ȡ4.x���ͼƬ����
		int GetCameraCount4x();

		// ���3.x-435ͼ��ͼƬ����
		int GetGalleryPicCount();

		// ��ñ�ֽ����
		int GetWallpaperPicCount();

		// ���ݲ�ͬ·���õ���Ŀ¼�µı�ֽ��Ŀ
		int GetPicCountByPath( wstring wallPaperPath );

	public:
		IosWallpaperOper*			_pWallpaper;				// ��ֽ��ز�����
		vector<char*>               _pictUuids;                 // ��Ҫͬ����ͼƬ��uuid
    protected:

        const DeviceData*			_pDevice;
	
		IosPictureSqlite4*			_ios4SQL;					// ios4.x���ݿ�����ӿ�
		IosPictureSqlite5*			_ios5SQL;					// ios5���ݿ�����ӿ�
		IosPhotoDB*					_pIosDB;					// ֻΪ��ȡ4.x�汾�ϴ���ɾ��ͼ��ͼƬ����ʱ����

		map<wstring, wstring>*		_UploadPictureFiles;
		vector<wstring>*			_DeletePictureFiles;

		bool						_bPhotosIthmbChange;		// ͼ������ͼ�ļ����б��޸�
		bool						_bPhotoDataBaseChange;		// ͼ��photodatabase���޸�
		bool						_bSqliteChange;				// ���ݿ�sqlite�ļ����޸�
		bool						_bPhotoDataIthmbChange;		// �������ͼ�ļ��Ƿ��޸�

		DownloadFileCommand*        _pDownCmd;

    private:

		int							_nVersion;					// �̼�ϵͳ�汾��
						


	};
}
