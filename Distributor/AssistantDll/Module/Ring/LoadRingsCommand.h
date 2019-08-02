#pragma once

#include "Module/TableQueryCommand.h"
#include "Module/Ring/RingData.h"

namespace ND91Assistant
{

// 这些手机型号取出来的铃声可为music
#define RINGASMUSIC_MODEL "qsd8250_surf"

    enum AudioType
	{
		SYSTEM_RING, //系统铃声
		CUSTOM_RING, //自定义铃声
	};

	class LoadRingsCommand :public TableQueryCommand
	{
	public:
		LoadRingsCommand(const DeviceData* pDevice);
		~LoadRingsCommand(void);

		// Summary  : 获取铃声数目
		// Returns  : int
		int GetRingCount();

		//系统铃声 tablename = "sysaudio"，
		//歌曲自定义铃声tablename = "cusaudio"
		// ringlist装载返回的rings
		void SetAudioType(AudioType type);

		const deque<RingData*>* LoadAlarmRings();

		const deque<RingData*>* GetDataList() { return &_Ringlist; };

		bool  DoneSuccess(){return _success;}

		void Execute();

	protected:
		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data);

	private:
		// Summary  : 
		// Returns	: void
		// Parameter: wstring remotefilePath
		void LoadRingsByPath(wstring remotefilePath, wstring localpath = L"");

		bool GetRingtonesList(wstring filePath);

		bool GetSystemRingList();

		void RingListToRingData(const map<wstring, wstring> *mapList, RingData::MediaFileKind kind);

		AudioType			_audioType;
		
		deque<RingData*>    _Ringlist;

		bool	  _success;                //操作结果

		wstring   _strTempRingList;       //临时文件夹下的RingList文件
		wstring   _strIosRingList;        //手机端上存放RingList文件的目录

		wstring   _strIosUserRing;        //手机端用户自定义铃声
	};
}