#pragma once

#include "Module/Record/RecordingData.h"
#include "sqlite/CppSQLite3.h"

namespace ND91Assistant
{
    class DeviceData;

	// IOS¼������
	class CLASSINDLL_CLASS_DECL RecordingManager
	{
	public:
		RecordingManager(const DeviceData* pDevice);
		virtual ~RecordingManager(void);

		// Summary  : ��������¼��
		// Returns  : vector<RecordingData>
		vector<RecordingData*> LoadRecording();

		// Summary  : �����豸¼���ļ�������
		// Returns  : bool
		// Parameter: int RecordId
		// Parameter: wstring path
		bool  SaveRecordOnPC(int RecordId, wstring path);

		// Summary  : ɾ���豸¼���ļ�
		// Returns	: bool
		// Parameter: int RecordId
		bool  DeleteRecording(int RecordId);

		wstring GetRecordDbPath() { return _localDBPath ; }

	private:

		// Summary  : ��ʼ������
		// Returns  : bool
		bool IosInit();

		RecordingData* FindTheData(int RecordId);

		// Summary  : ͬ�����ݵ��豸
		// Returns  : bool
		bool Sync();
		
		vector<RecordingData*>				_RecordsData;
		vector<RecordingData*>::iterator	_it;

		const DeviceData*					_pDevice;

		wstring								_remoteDBPath;		// �豸¼�����ݿ��ļ�
		wstring								_localDBPath;		// ���汾�ص����ݿ��ļ�
		
		CppSQLite3Buffer					_strSQL;			// ִ�����ݿ������SQL���
		CppSQLite3DB						_SQLdb;				// �ӿھ��	

	};

}
