#pragma once

#include "sqlite/CppSqlite3.h"

namespace ND91Assistant
{
    // ��Ƭ/��ֽ/��ͼsqlite���� IOS 5.0����ʵ��
	class PictureGroup;
	class PictureData;

	class IosPictureSqlite5
	{
	public:
		IosPictureSqlite5( const DeviceData* pDevice );
        virtual ~IosPictureSqlite5();

		// Summary  : ��ʼ��IOS5���ݿ�
		// Returns  : bool
		bool OpenIos5DB();

		// Summary  : ����ios5����ͼ�����
		// Returns  : map<int, wstring>
		map<int, wstring>* LoadIos5Category();

		// Summary  : ���ios5ͼ�����
		// Returns  : ���ط���ID
		int AddIos5Category(const wstring name);

		// Summary  : ������ios5ͼ�����
		// Returns  : bool
		bool RenameIos5Category(const int id, const wstring name);

		// Summary  : ɾ��ios5ͼ�����
		// Parameter: const int id		����ID	
		// Returns  : bool
		bool DeleteIos5Category(const int id, vector<int>* vDelPicId);

		// Summary  : �������,����ͼƬ
		// Returns  : bool
		// Parameter: vector<PictureData * > * pVecPicture   ͼƬ����
		// Parameter: bool isGallery						 �Ƿ��Ǽ���ͼ��ͼƬ	
		bool LoadIos5Pictures( vector<PictureData*>* pVecPicture, bool isGallery );

		// Summary  : �������ͼƬ����
		// Returns  : bool
		// Parameter: PictureData * pdata			ͼƬָ��
		bool InsertIos5PicturesToDB(vector<PictureData*>* pVecPicture, std::string gruopId);

		// Summary  : ɾ��һ�����,����ͼƬ����
		// Returns  : bool
		// Parameter: int id            ͼƬid
		bool DeleteIos5Pictures(vector<int>* pVecPicId);

		// Summary  : �޸�һ�����,����ͼƬ����
		// Returns  : bool
		// Parameter: PictureData * pdata        ͼƬָ��
		bool UpdateIos5Pictures(const PictureData* pdata);

		// Summary  : ����Ѵ��ڵ�ͼ��ͼƬ����
		// Returns  : bool
		// Parameter: vector<PictureData*>* pVecPicture			ͼƬָ�뼯��
		// Parameter: std::string gruopId						����id
		bool Ios5AddExistPictureToGroup(vector<int>* pVecPictureid, std::string gruopId);

		// Summary  : ��ͼ��������Ƴ�ͼƬ����
		// Returns  : bool
		// Parameter: vector<PictureData*>* pVecPicture			ͼƬָ�뼯��
		// Parameter: std::string gruopId						����id
		bool Ios5RemoveFromGroup(vector<int>* pVecPictureid, std::string gruopId);

		// Summary  : ��ͼ��������ƶ�ͼƬ����
		// Returns  : bool
		// Parameter: vector<PictureData*>* pVecPicture			ͼƬָ�뼯��
		// Parameter: std::string sourceGroupId					ԭ����id
		// Parameter: std::string DestGroupId					Ŀ�����id
		bool Ios5MovePictures(vector<int>* pVecPictureid, std::string sourceGroupId, std::string destGroupId);

		// Summary  : ���ݷ����ø÷����µ�����ͼƬ
		// Returns  : vector<PictureData*>*
		// Parameter: PictureGroup* pGroup
		bool GetIosCategoryImages(PictureGroup* pGroup, vector<PictureData*>* pVerPictures, vector<PictureData*>& pVerDatas);

		// Summary : ���ͼƬ��UUID
		// Returns : ����ͼƬ��UUID
		vector<char*> GetPictureUuids();

		// Summary : ��ͼƬGUID���͵�UUIDת����char*
		char* ConvertUuidToPChar(GUID uuidIn);

		// Summary �������ʱ��ŵ�ͼƬ��UUID
		void ClearTempPictureUuids();		

	public:
		vector<char*> _picUuids;  // ͼƬ��UUID

	private:

		// Summary  : ios5��Sqlite���ݿⴥ��������
		// Returns  : bool
		bool InitDBTrigger();

        // ��ȡ���ݿ��е�һЩ�ֶ�
        bool InitParamsByDB();
		
		// Summary  : ��ӡ�����ݿ�����쳣��־
		// Returns  : void
		// Parameter: wstring strMsg
		// Parameter: int nErrorCode
		// Parameter: std::string strErrorMsg
		void WriteError(wstring strMsg, int nErrorCode, std::string strErrorMsg);

		bool DropDBTrigger();
	private:
		
		const DeviceData*		_pDevice;

		CppSQLite3DB*			_SQLdb;				// �ӿھ��	
     
        // ��ȡ���ݿ��е�һЩ�ֶΣ��Ա����Ӧ�����ݿ����
        int _iAlbumENT;
        int _iGenericAssetENT;
        int _iGenericAlbumENT;
        int _iAssetENT;
        int _iWallpaperAssetENT;
        int _iAdditionalAssetAttributesEnt;
        int _iAlbumListENT;
        bool _bInitParams; 

	};
}