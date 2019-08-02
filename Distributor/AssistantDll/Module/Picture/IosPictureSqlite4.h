#pragma once

#include "sqlite/CppSqlite3.h"

namespace ND91Assistant
{
	// ��Ƭ/��ֽ/��ͼsqlite���� (IOS 4.xʵ��)
	class PictureData;
	class IosPictureSqlite4
	{
	public:
		IosPictureSqlite4(const DeviceData* pDevice);
		virtual ~IosPictureSqlite4(void);

		// Summary  : ��photo.sqlite���ݿ��ж�ȡ��Ƭ��ͼ����
		// Returns  : �Ƿ�ɹ�
		// Parameter: vector<PictureData * > * pVecPicture ���ڴ�����ݵ��������
		bool GetPhotoFromSqlite(vector<PictureData*>* pVecPicture);

		// Summary  : ����һ����Ƭ��photo.sqlite���ݿ�
		// Returns  : �Ƿ�ɹ�
		// Parameter: PictureData * pdata
		bool InsertPhotoToSqlite(vector<PictureData*>* pVecPicture);

		// Summary  : ��photo.sqlite���ݿ�ɾ��һ����Ƭ
		// Returns  : �Ƿ�ɹ�
		// Parameter: int pictureid
		bool DeletePhotoFromSqlite(vector<int>* pVecPicId);

		// Summary  : ��photo.sqlite���ݿ��޸�һ����Ƭ����
		// Returns  : �Ƿ�ɹ�
		// Parameter: PictureData * pdata
		bool UpdatePhotoToSqlite(PictureData* pdata);

		// Summary  : �ر����ݿ�
		// Returns  : bool
		bool ClosePhotoDB();


	private:

		// Summary  : �����ݿ�
		// Returns  : bool
		bool OpenPhotoDB();

		// Summary  : �������ݿ�����
		// Returns  : bool
		void PhotoDBTransaction();

		// Summary  : �����ύ
		// Returns  : bool
		void PhotoDBCommit();

		// Summary  : �쳣�ع�
		// Returns  : bool
		void PhotoDBRollback();

		// Summary  : �����ݿ�photo.sqlite��PhotoExtras����ͼƬ����
		// Returns  : bool
		// Parameter: int id		ͼƬid
		// Parameter: wstring name	ͼƬ����
		// Parameter: int size		ͼƬ��С(421������Ҫ)	
		bool InsertToPhotoExtras(int id, wstring name, int size);

		// Summary  : ��ӡ�����ݿ�����쳣��־
		// Returns  : void
		// Parameter: wstring strMsg
		// Parameter: int nErrorCode
		// Parameter: std::string strErrorMsg
		void WriteError(wstring strMsg, int nErrorCode, std::string strErrorMsg);

		const DeviceData*		_pDevice;
		int						_nVersion;			// �̼�ϵͳ�汾��

		CppSQLite3DB*			_SQLite;			// ����photo.sqlite���	
		CppSQLite3DB*			_AuxSQLite;			// ����photoAux.sqlite���

		

	};

}
