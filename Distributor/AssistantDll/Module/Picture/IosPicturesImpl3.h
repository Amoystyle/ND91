#pragma once

#include "IosPicturesImpl.h"

namespace ND91Assistant
{
    class PictureFolder;
    class PictureData;
    class NodeData;
	class DeviceData;
	class AndroidPhotoBucket;
	class IosPicturesImpl;

    // ��Ƭ/��ֽ/��ͼ������IOS 3 �汾ʵ����
	class IosPicturesImpl3 : public IosPicturesImpl
	{
	public:
		IosPicturesImpl3(const DeviceData* pDevice);
        virtual ~IosPicturesImpl3(void) {}

        virtual void LoadSpecialPictures(PictureFolder* pCamera,
            PictureFolder* pWallpaper, PictureFolder* pScreenshot);

		// Summary  : ��������ͼƬ 
		// Returns  : ��ͼƬָ�뼯��
		// Parameter: vector<wstring> sourceFiles  ͼƬ�ļ�·������ 
		// Parameter: int fileNumber		�ļ�������һ������(���������ļ���)
		// Parameter: int thumbIndex		����ͼ���(����4.2.1���ϰ汾����)
		virtual vector<PictureData*>* AddSpecialPictures(vector<wstring>* vPicFiles,
			int fileNumber, int thumbIndex = -1, std::string gruopId = "");

		// Summary  : ɾ������ͼƬ
		// Returns  : �Ƿ�ɹ�
		// Parameter: const vector<const PictureData * > vPictures   ��ɾ��ͼƬ����  
		virtual bool DeleteSpecialPictures(const vector<const PictureData*>* pVecPicture);

        // Summary  : �޸�ͼƬ
        // Returns  : �Ƿ�ɹ�
        // Parameter: PictureData * pic ͼƬָ��
        // Parameter: std::wstring srcPath ͼƬ�ļ�·��
        virtual bool UpdateSpecialPicture( PictureData* pic, wstring srcPath);

		// Summary  : ���ô���������Ļ��ֽ
		// Returns  : bool 
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

		// ���ظð汾�����ʼ���ļ�
		virtual bool DownloadInitFiles();


		virtual bool LoadPicturesByGroup(PictureGroup* pGroup, vector<PictureData*>& pVerDatas);

		// Summary  : ��Ӷ���ͼ��ͼƬ
		// Returns  : bool
		// Parameter: vector<wstring> localPaths : ����ͼƬ����
		virtual vector<PictureData*>* AddPictureToGroup( vector<wstring>* localPaths, const string groupId,
														 int nextNumber=0, int index=0, const string firstGroupId="" );

		// ��������ͼ�ļ�
		virtual bool DownloadThumbnail(PictureData* pPicture);

		// ����ԭͼ�ļ�
		virtual bool DownloadPicture(const PictureData* pPicture, wstring customPath);


		// �õ��ð汾������ithumb�ļ�id
		virtual vector<int> GetThumbType();

		// ��ȡ������, ͼ��ͼƬ������
		int GetSpecialPhotos();


#pragma endregion

	private:

        // Summary  : ����Ŀ¼����, ����ͼƬ, ��ŵ�PictureFolder��
        // Parameter: deque<NodeData * > * nodes Ŀ¼�ڵ�
        // Parameter: PictureFolder * pFolder    ����
        void Load( deque<NodeData*>* nodes, PictureFolder* pFolder);

	};

}
