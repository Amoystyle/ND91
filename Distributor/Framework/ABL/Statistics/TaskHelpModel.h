#pragma once

#include "UbdgLib.h"
#include "Thread/StaticsticsInnerMsg.h"

struct PostDownloadInfoMsg;
class CTaskStatisticsThread;

// ��Դ���ͳ���
enum FeedbackResType
{				
	None = 0,				// ��������Դ(0)						
	Soft = 1,				// �����Դ(1)		[Description("���")]						
	Theme = 2,				// ������Դ(2)		[Description("����")]	
	Ring = 3,				// ������Դ(3)		[Description("����")]
	Picture = 4,			// ��ֽ��Դ(4)		[Description("��ֽ")]	
	NovelBook = 5,			// С˵(5)			[Description("С˵")]	
	EZine = 6,				// ������־(6)		[Description("������־")]
	Sms = 7,				// ����(7)			[Description("����")]
	EBook = 8,				// ������(8)		[Description("������")]
	MobileGame = 9,			// �ֻ���Ϸ(9)		[Description("�ֻ���Ϸ")]	
	Hardware = 10,			// Ӳ��վ��Դ(10)	[Description("Ӳ��վ��Դ")]
	Cartoon = 11,			// ����(11)			[Description("����")]	
	RSS = 12,				// ����(12)			[Description("Rss����")]		
	Music = 13,				// ����(13)			[Description("����")]
	Video = 14,				// ��Ƶ(14)			[Description("��Ƶ")]
	College91 = 15,			// 91��ѧԺ��Դ(15)	[Description("91��ѧԺ��Դ")]
	KT91 = 16,				// ����ר��(16)		[Description("����ר��")]		
	ItuneFreeApp,			// ��������(17)		[Description("��������")]
};


// ��������
enum FeedbackType
{
	Download			= 1,
	Browse				= 2,
	Ratting				= 3,
	DownloadSuccess		= 4,
	SetupSuccess		= 5,
	SetupFail			= 6,
	DownloadFromSub		= 7,						
	DownloadFail		= 8,		// ����ʧ��ͳ��
	RepeatSetupSuccess	= 9,		// �ظ���װ�ɹ�
};


class TaskHelpModel :
	//public BaseHandler<StaticsticsInnerMsg>,
	public BaseHandler<PostDownloadInfoMsg>,
	public BaseHandler<PostDownloadFinishMsg>,
	public BaseHandler<PostInstallInfoMsg>,
	public BaseHandler<PostInstallResultInfoMsg>,
	public BaseHandler<PostInstallFailInfoMsg>
{
	friend class CTaskStatisticsThread;
public:
    TaskHelpModel();//BaseUI* pUI
    ~TaskHelpModel();

	// ����
	static TaskHelpModel* GetInstance();

	// Summary  : ��װ���̺�̨����app���ؼ�¼
	// Returns  : void
	// Parameter: InstallsAppInfo* appInfo
	void PostDownLoadInfo( wstring m_appid, wstring m_platform,wstring m_csFileDownUrl,wstring m_strName,wstring m_appVersion,wstring m_csFileSavePath);

	// Summary  : ��װ���̺�̨����app��װ��¼
	// Returns  : void
	// Parameter: CTaskItem * item
	// Parameter: wstring deviceId
	void PostInstallInfo( wstring m_appid, wstring m_csFileSavePath,wstring m_strName,wstring m_appVersion ,wstring deviceId );

	// Summary  : �����˷�������(��ʼδ��¼������/��װ�������¼)
	// Returns  : void
	void SendDataToService( wstring strID );

	// Summary  : �����˷�������(֮ǰδ���ͳɹ��������������)
	// Returns  : void
	void PostDataToService();


//	---------------------------------------------------------------------------------
//	��91��̨����˷���Ӧ�����أ���װ��Ϣ
//	---------------------------------------------------------------------------------

	// Summary  : 91��̨�������ͳ��
	// Returns  : void
	// Parameter: CTaskItem * item
	void PostDownloadFinish( unsigned long m_nSpeed,wstring m_platform,int  m_fid, int m_uStatus);

	// Summary  : 91��̨��װ���ͳ��
	// Returns  : void
	// Parameter: CTaskItem * item
	// Parameter: wstring deviceId
	void PostInstallResultInfo( wstring m_platform,int  m_fid,wstring m_appid, bool bSuccess);

	// Summary  : 91��̨���Ͱ�װʧ��ͳ��
	// Returns  : void
	// Parameter: CTaskItem * item
	// Parameter: wstring deviceId
	void PostInstallFailInfo( wstring filePath, int m_fid, wstring m_appid, wstring m_appVersion,
		unsigned long TotalSize,wstring m_strName,wstring m_installerror,
		wstring deviceId );

private:

	// Summary  : ��������appδ�ϱ��ɹ�����
	// Returns  : void
	void SaveDownloadData(APPDOWNLOAD_RECORD* pdata);

	// Summary  : ���氲װappδ�ϱ��ɹ�����
	// Returns  : void
	void SaveInstallData(APPINSTALL_RECORD* pdata);

	// Summary  : �������ļ���ȡ����appδ�ϱ��ɹ�����
	// Returns  : APPDOWNLOAD_RECORD * pdata
	APPDOWNLOAD_RECORD* ReadDownloadData();

	// Summary  : �������ļ���ȡ��װappδ�ϱ��ɹ�����
	// Returns  : APPINSTALL_RECORD * pdata
	APPINSTALL_RECORD* ReadInstallData();

	// Summary  : �ͷ�����
	// Returns  : void
	void ReleaseData();

	virtual bool Action(int nAction, StaticsticsInnerMsg* pParam);  
	bool SendDownLoadInfo(StaticsticsInnerMsg* pParam);
	bool SendInstallInfo(StaticsticsInnerMsg* pParam);
	bool SendInfo(StaticsticsInnerMsg* pParam);
	bool SendData(StaticsticsInnerMsg* pParam);
	bool SendDownloadSms(StaticsticsInnerMsg* pParam);
	bool SendSuccessInfo(StaticsticsInnerMsg* pParam);
	bool SendFailInfo(StaticsticsInnerMsg* pParam);


private:
	
	void SetValues(char* szBuf, std::string str);

	// ��ȡini�ļ���char*��ֵ
	std::string GetIniCharValue( std::string strIniFile, std::string strGroup, std::string strKey );
	// ��ȡini�ļ���int��ֵ
	long GetIniIntValue( std::string strIniFile, std::string strGroup, std::string strKey );

	//virtual bool Handle( const StaticsticsInnerMsg* pMsg );

	virtual bool Handle( const PostDownloadInfoMsg* pMsg );

	virtual bool Handle( const PostDownloadFinishMsg* pMsg );

	virtual bool Handle( const PostInstallInfoMsg* pMsg );

	virtual bool Handle( const PostInstallResultInfoMsg* pMsg );

	virtual bool Handle( const PostInstallFailInfoMsg* pMsg );

	vector<APPDOWNLOAD_RECORD*> _verDownloads;
	vector<APPINSTALL_RECORD*>  _verInstalls;
	int _nDowntCount;
	int _nInstallCount;

	std::string					_nowVersion;

};