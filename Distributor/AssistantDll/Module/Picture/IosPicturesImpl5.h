#pragma once

#include "IosPicturesImpl.h"

namespace ND91Assistant
{
    // ���/��ֽ/��ͼ/ͼ�� IOS 5.0����ͼƬ������ʵ��
	class IosPicturesImpl;
	class PictureData;
	class IosPicturesImpl5 : public IosPicturesImpl
	{
	public:
		IosPicturesImpl5(const DeviceData* pDevice);
        virtual ~IosPicturesImpl5();

		bool Init();	

        virtual void LoadSpecialPictures(PictureFolder* pCamera,
            PictureFolder* pWallpaper, PictureFolder* pScreenshot);
	
		// Summary  : ��������ͼƬ
		// Returns  : vector<PictureData*>*			��ͼƬָ�뼯��
		// Parameter: vector<wstring> vPicFiles     ͼƬ�ļ�·������
		// Parameter: int fileNumber
		// Parameter: int thumbIndex
		virtual vector<PictureData*>* AddSpecialPictures(vector<wstring>* vPicFiles,
			int fileNumber, int thumbIndex = -1, std::string gruopId = "");

		// Summary  : ɾ������ͼƬ(��֧��ios4.0.0���ϰ汾)
		// Returns  : �Ƿ�ɹ�
		// Parameter: const vector<const PictureData * > pVecPicture   ��ɾ��ͼƬ����  
		virtual bool DeleteSpecialPictures(const vector<const PictureData*>* pVecPicture);

        // Summary  : �޸�ͼƬ
        // Returns  : �Ƿ�ɹ�
        // Parameter: PictureData * pic ͼƬָ��
        // Parameter: std::wstring srcPath ͼƬ�ļ�·��
        virtual bool UpdateSpecialPicture( PictureData* pic, wstring srcPath);

		// Summary  : ���ô���������Ļ��ֽ
		// Returns  : bool				
		// Parameter: wstring wallpaper   ��ѡ�б�ֽ�ı���·��
		// Parameter: bool isHome			�Ƿ�������������ֽ
		virtual bool SetLockBackground( wstring wallpaper );

		// Summary  : ��������Ļ��ֽ
		// Returns  : bool
		virtual bool SetHomeBackground( wstring wallpaper );

		// Summary  : �ϴ�ͼƬ�����������������ͼ�⡢�������ͼ��
		// Returns  : bool
		virtual bool UploadPictureFiles(ProgressSubject* pSubject=NULL, int percent=100);

		// Summary  : ɾ��ͼƬ�����������������ͼƬ���������ͼ��
		// Returns  : bool
		virtual bool DeletePictreFiles();

		// Summary  : �ϴ�PhotoData��sqlite�ļ�
		// Returns  : bool
		virtual bool PhotoSqilteSync();

#pragma region ͼ�ⲿ����ز���

		// ----- ͼ�������� -----

		// ���ظð汾�����ʼ���ļ�
		virtual bool DownloadInitFiles();

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
		virtual bool DeleteGalleryGroup( int groupId, vector<PictureData*>* VerPictDatas);


		// ----- ͼ��ͼƬ���� -----

		// ��������ͼ��ͼƬ
		virtual bool LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas);

		// Summary  : ��Ӷ���ͼ��ͼƬ
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : ����ͼƬ����
		virtual vector<PictureData*>* AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														 int nextNumber=0, int index=0, const string firstGroupId="" );

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
		virtual bool DeleteGalleryPictures( const vector<const PictureData*>* vPictures);

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
		virtual bool DownloadThumbnail(PictureData* pPicture);


		// ----- ͼƬ������ظ������� -----

		// �õ��ð汾������ithumb�ļ���
		virtual vector<int> GetThumbType();

		// ��ȡ������, ͼ��ͼƬ������
		int GetSpecialPhotos();


#pragma endregion

	private:
		
		// Summary  : �������ͽ���ͼƬ
		// Returns  : void
		// Parameter: PictureFolder * pCamera		������ָ��
		// Parameter: PictureFolder * pScreenshot	��������ָ��
		void LoadPhotoAndScreenShot(PictureFolder* pCamera, PictureFolder* pScreenshot);

		// Summary  : ��.ithmb�ļ��������ͼ
		// Returns  : void
		// Parameter: Image * img			ԭͼָ��
		// Parameter: int idnex				����ͼλ��
		void WriteThumbDB(Image* img, int idnex);

		// Summary  : �ϴ�����ͼ������Ϣ�ļ�
		// Returns  : void
		void UpLoadthumbConfig();

		// Summary  : �ж��Ƿ��ǽ���
		// Returns  : bool
		bool IsScreenShoot(int width, int heigth);

		// Summary  : ios5��ʼ������
		// Returns  : bool
		bool Initialize();

	private:

		bool					_ios5HasInit;		// ios5�Ƿ��Ѿ����س�ʼ�����ݿ��ļ�
		vector<PictureData*>    _vPictures;			// ����ͼ��ͼƬ����	 
	};
}