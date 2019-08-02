#pragma once

#include "IosPicturesImpl.h"

namespace ND91Assistant
{
    // ��Ƭ/��ֽ/��ͼ������IOS 4.2.1����ʵ��
	class IosPicturesImpl;
	class PictureData;
	class IosPicturesImpl421 : public IosPicturesImpl
	{
	public:
		IosPicturesImpl421(const DeviceData* pDevice);
        virtual ~IosPicturesImpl421(void) {}


        virtual void LoadSpecialPictures(PictureFolder* pCamera,
            PictureFolder* pWallpaper, PictureFolder* pScreenshot);

		// Summary  : ��������ͼƬ
		// Returns  : vector<PictureData*>*			��ͼƬָ�뼯��
		// Parameter: vector<wstring> vPicFiles     ͼƬ�ļ�·������
		// Parameter: int fileNumber
		// Parameter: int thumbIndex
		virtual vector<PictureData*>* AddSpecialPictures(vector<wstring>* vPicFiles,
			int fileNumber, int thumbIndex = -1, std::string gruopId = "");

		// Summary  : ɾ������ͼƬ
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

		// ----- ͼ��ͼƬ���� -----

		// ���ظð汾�����ʼ���ļ�
		virtual bool DownloadInitFiles();

		// ��������ͼ��ͼƬ
		virtual bool LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas);

		// Summary  : ��Ӷ���ͼ��ͼƬ
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : ����ͼƬ����
		virtual vector<PictureData*>* AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														 int nextNumber=0, int index=0, const string firstGroupId="" );


		// ��������ͼ�ļ�
		virtual bool DownloadThumbnail(PictureData* pPicture);


		// ----- ͼƬ������ظ������� -----

		// �õ��ð汾������ithumb�ļ���
		virtual vector<int> GetThumbType();


#pragma endregion

	private:
		
		// Summary  : ��32x32.ithmb���ļ�д������
		// Returns  : 
		// Parameter: Image * img
		// Parameter: int idnex
		void WriteThumbDB(Image* img, int idnex);

		// �ж��豸�Ƿ���IPad
		bool		_isIPad;							

	};
}