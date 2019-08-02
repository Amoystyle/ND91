#pragma once

#include "Module/Record/RecordingData.h"
#include "sqlite/CppSQLite3.h"

namespace ND91Assistant
{
    class DeviceData;

	// IOS录音管理
	class CLASSINDLL_CLASS_DECL RecordingManager
	{
	public:
		RecordingManager(const DeviceData* pDevice);
		virtual ~RecordingManager(void);

		// Summary  : 加载所有录音
		// Returns  : vector<RecordingData>
		vector<RecordingData*> LoadRecording();

		// Summary  : 保存设备录音文件到本地
		// Returns  : bool
		// Parameter: int RecordId
		// Parameter: wstring path
		bool  SaveRecordOnPC(int RecordId, wstring path);

		// Summary  : 删除设备录音文件
		// Returns	: bool
		// Parameter: int RecordId
		bool  DeleteRecording(int RecordId);

		wstring GetRecordDbPath() { return _localDBPath ; }

	private:

		// Summary  : 初始化数据
		// Returns  : bool
		bool IosInit();

		RecordingData* FindTheData(int RecordId);

		// Summary  : 同步数据到设备
		// Returns  : bool
		bool Sync();
		
		vector<RecordingData*>				_RecordsData;
		vector<RecordingData*>::iterator	_it;

		const DeviceData*					_pDevice;

		wstring								_remoteDBPath;		// 设备录音数据库文件
		wstring								_localDBPath;		// 缓存本地的数据库文件
		
		CppSQLite3Buffer					_strSQL;			// 执行数据库操作的SQL语句
		CppSQLite3DB						_SQLdb;				// 接口句柄	

	};

}
