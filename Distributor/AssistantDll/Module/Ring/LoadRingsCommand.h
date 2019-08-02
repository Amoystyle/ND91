#pragma once

#include "Module/TableQueryCommand.h"
#include "Module/Ring/RingData.h"

namespace ND91Assistant
{

// ��Щ�ֻ��ͺ�ȡ������������Ϊmusic
#define RINGASMUSIC_MODEL "qsd8250_surf"

    enum AudioType
	{
		SYSTEM_RING, //ϵͳ����
		CUSTOM_RING, //�Զ�������
	};

	class LoadRingsCommand :public TableQueryCommand
	{
	public:
		LoadRingsCommand(const DeviceData* pDevice);
		~LoadRingsCommand(void);

		// Summary  : ��ȡ������Ŀ
		// Returns  : int
		int GetRingCount();

		//ϵͳ���� tablename = "sysaudio"��
		//�����Զ�������tablename = "cusaudio"
		// ringlistװ�ط��ص�rings
		void SetAudioType(AudioType type);

		const deque<RingData*>* LoadAlarmRings();

		const deque<RingData*>* GetDataList() { return &_Ringlist; };

		bool  DoneSuccess(){return _success;}

		void Execute();

	protected:
		// �����������ݰ�
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

		bool	  _success;                //�������

		wstring   _strTempRingList;       //��ʱ�ļ����µ�RingList�ļ�
		wstring   _strIosRingList;        //�ֻ����ϴ��RingList�ļ���Ŀ¼

		wstring   _strIosUserRing;        //�ֻ����û��Զ�������
	};
}