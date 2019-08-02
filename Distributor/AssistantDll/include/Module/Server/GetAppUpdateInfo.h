#pragma once


namespace ND91Assistant
{
    class DeviceData;

	// �ӷ���˻�ȡӦ�����ݽṹ��
	class CLASSINDLL_CLASS_DECL AppUpdateInfo
	{
		public:
			AppUpdateInfo()
			{		
				_nfid		= 0;		// ֵΪ0ʱ˵��û�дӷ�����ȡ������
				_itunesid	= 0;		// ֵΪ0ʱ˵��û�дӷ�����ȡ������
				_nsize		= 0;
				_nDownNum	= 0;
				_bLegal		= false;
				_bUpdate	= false;	

				_localPath  = L"";
			}

			~AppUpdateInfo(void){;}
		
		public:

			int			_nfid;			// ���id
			int			_itunesid;		// itunesid ����iosӦ��ר��
			int			_nsize;			// �����С	
			int			_nDownNum;		// �����ش���
			bool		_bLegal;		// �Ƿ�������Ӧ��	false ������ true ����
			bool		_bUpdate;		// �Ƿ���Ҫ����	
			time_t		_addTime;		// �ϼ�ʱ��
			wstring		_name;			// �����
			wstring		_identifier;	// �����ʶ
			wstring     _buildVersion;	// �����汾
			wstring		_oldVersion;	// �ɰ汾��
			wstring		_newVersion;	// �°汾��
			wstring		_verCode;		// �汾ID	
			wstring		_url;			// �°汾URL	(�����URL, ������app��ַΪ������url, ����ͨ��GetAppsRealUpdateUrls��ȡ)

			wstring     _localPath;    //�����������Ӧ�ı��������·��

	};

	class CLASSINDLL_CLASS_DECL CGetAppUpdateInfo
	{
		public:
			CGetAppUpdateInfo( int deviceType );		// ��ȡ����PC�ϵ�Ӧ�õ�������Ϣ
			CGetAppUpdateInfo( DeviceData* pDevice );	// ��ȡ�ֻ�Ӧ�õ�������Ϣ
			~CGetAppUpdateInfo(void);

			// vector<AppUpdateInfo*>* pinfo �����������Ҳ�Ƿ��ؽ����
			// �������AppUpdateInfo��Ҫ _identifier, _oldVersion, _verCode����
			// �������AppUpdateInfo���г�Ա����
			// Summary  : ��ȡ�豸����Ӧ��������Ϣ				(�ɽӿڣ�����xml��ʽ����)
			// Returns  : bool		�Ƿ��ȡ�ɹ�
			// Parameter: vector<AppUpdateInfo*>* pinfo		������Ϣ���� 
			bool GetAppUpdateInfo( vector<AppUpdateInfo*>* pinfo );

			// vector<AppUpdateInfo*>* pinfo �����������Ҳ�Ƿ��ؽ����
			// �������AppUpdateInfo��Ҫ _identifier, _buildVersion, _oldVersion, _verCode����
			// �������AppUpdateInfo���г�Ա����
			// Summary  : ��ȡ�豸����Ӧ��������Ϣ				(act=405, �½ӿڣ�����json��ʽ����,����json�������xml)
			// Returns  : bool		�Ƿ��ȡ�ɹ�
			// Parameter: vector<AppUpdateInfo*>* pinfo		������Ϣ����
			bool GetAppUpdateData( vector<AppUpdateInfo*>* pinfo );


			// vector<AppUpdateInfo*>* pinfo �����������Ҳ�Ƿ��ؽ����
			// ����������	��	act=404  �������AppUpdateInfo��Ҫ�� _identifier, _oldVersion, _verCode	����: com.clickgamer.AngryBirds,	5.1.2
			// ������������	��	act=605  �������AppUpdateInfo��Ҫ�� _nfid, _oldVersion, _verCode		����: 578036166,	5.1.2
			// �������AppUpdateInfo���г�Ա����
			// Summary  : ��ȡ�豸����Ӧ��������Ϣ, ���������������				(���½ӿڣ����ش�json��ʽ����)
			// Returns  : bool		�Ƿ��ȡ�ɹ�
			// Parameter: vector<AppUpdateInfo*>* pinfo		������Ϣ����
			bool GetAppUpdateDataNew( vector<AppUpdateInfo*>* pinfo, bool bLegal );

			// Summary  : ͨ��AppUpdateInfo��_nfid(���id)��ȡ���������������URL;
			// Returns  : bool
			// Parameter: vector<AppUpdateInfo * > * pinfo			
			bool GetAppsRealUpdateUrls( vector<AppUpdateInfo*>* pinfo );

			// Summary  : ͨ��AppUpdateInfo��_identifier(�����ʶ)��ȡ����Ӧ�õ�itunesid;
			// Returns  : bool
			// Parameter: vector<AppUpdateInfo * > * pinfo
			bool GetItunesidByIdentify( vector<AppUpdateInfo*>* pinfo );

		private:
			
			DeviceData*		_pDevice;

			int				_deviceType;

			// �ɽӿ�, �������ص�XML����
			bool ParseInfo(string imginfo, vector<AppUpdateInfo*>* pinfo);

			// �½ӿ�, �������ص�Json��xml������
			bool ParseData(string appInfos, vector<AppUpdateInfo*>* pinfo);

			// �����½ӿ�, �������صĴ�Json������
			bool ParseJsonData(string appInfos, vector<AppUpdateInfo*>* pinfo, bool bLegal );
	};
}
