#pragma once

struct FileVersionInfo;

namespace ND91Assistant
{
    class DeviceData;
    class Authorization;
    class iTunesAuthorize;

    enum AuthorizorError
    {
        ArgsError           = -9124,
        ConnectModeError    = -9125,
        AuthorizeException  = -9126,
        InternalError       = -9128,
    };

    // ����IPA��Ȩ������
	class Authorizor
	{
	public:
		Authorizor(const DeviceData* pDevice);
		~Authorizor();

        // ��ʼ����������
        bool Init();

        // ��ǰ�豸�Ƿ��ʺ���Ȩ
        // ���� 0���ʺ���Ȩ
        // -1 �豸�̼��汾����4.0
        // -2 ���ӷ�ʽ����USB
        // -3 iTunes�汾����
        // -4 δ��װiTunes
        // -5 iTunes�汾Ϊ���װ�
        static int IsValideToAuthorize(const DeviceData* pDevice);

        // ���豸��Ȩ
        bool Authorize(wstring data, wstring ipaName);

        // �޸������޸�
        int FixFlashExit();

        // ipa��װ�ɹ���.����޷�֪���Ƿ���������������,
        // ���Զ��豸�������˺Ž���һ���޸�.[��ʱ�������]
        void AttemptFix();

        // ��Ȩ�����ļ�Ŀ¼
        static wstring _CacheFolder;

	private:

		void InitInternal( );
        
        // ������Ȩ��Ϣ�������߸����ͷ�Authorization*)
        Authorization* DecryptAuthorization(wstring data);

        // ��ȡiTunes�汾��
        wstring GetiTunesVersion();

        // ��Ȩ
        bool Authorize(Authorization* au);

        // �жϵ�ǰҪ��Ȩ���˺Ŷ�Ӧ����Ȩ�ļ��Ƿ��ڻ�������,�еĻ�ʹ�û���.
        bool IsAuthorizationInCache(Authorization* au, Authorization* localCache);

        // ����������Ȩ
        int AuthorizeFromExternal(Authorization* au);
        int AuthorizeFromExternalITunesAuthorize(Authorization* au);

        // ������Ȩ�˺���Ϣ
        void CacheAccount(Authorization* au);

        // ����һ����Ȩ�˻����豸
        void SerilizeCacheAccountToDevice();

        // �ӷ�������ȡ��Ȩ�ļ���Ϣ,����ѹ��ָ��Ŀ¼
        bool GetAuthorizationFromServer(Authorization* authorization);

        // ������Ȩ��Ϣ
        void CacheAuthorization(Authorization* au);

        // MD5����
        wstring GetMD5(wstring Data);

        // ��ȡ����ķ�������ַ,����Ӳ����� defaultPath ��ַʧЧ��
        wstring GetNewestUrl();

        // ��������Ȩ�ļ���ʱ�併������
        static bool SortByCreateDate(Authorization* v1, Authorization* v2);

        // ӳ���˺�id ��pathMap ������,�������ɴ����Ȩ��Ϣ�ļ�������,����Ҳûɶ�������.
        wstring MapPath(wstring url);

        // ɱ��/����Helper_16����
        void KillProcessHelper_16();
        int CreateProcessHelper_16(Authorization* au);

        // ��ȡ��ǰ�豸��װ������IPA����Ӧ�˻�DSID
        void GetAppDSIDS(vector<wstring>& dsids);

        // iTunes�汾��Ϣ
        static void GetItunesVersionInfo();

	private:

        const DeviceData*       _pDevice;               // ��ǰ�豸
        vector<wstring>         _authorizedAccounts;    // ��ǰ�豸�Ѿ���Ȩ�˻�
        vector<Authorization*>  _authorizations;        // ������Ȩ�ļ���Ϣ

        wstring                 _softID;                // ��ǰ��Ȩ���id,�����
        wstring                 _ipaName;               // ��ǰ��Ȩipa����,�����.

        wstring                 _ConfigFolderOnDevice;  // �����ļ����豸�ϵ�Ŀ¼
        wstring                 _infoFileOnDevice;      // �����ļ����豸�ϵ�·��
        //static wstring          _RNG;                   // Des����KEY;

        static bool                                _isSimpleiTunes;    // �û������Ƿ��Ǽ��iTunes,Ĭ�ϷǼ��
        static FileVersionInfo*    _pITunesVersionInfo; // iTunes�汾��Ϣ

        //wstring                 _md5RNG;                // md5KEY;
        wstring                 _defaultPath;           // Ĭ����Ȩ�ļ���������ַ
        wstring                 _pathMap;               // ���ɱ���Ŀ¼���ֵ
        wstring                 _helperName;            // helper��������

        bool                    _isInitialized;         // �Ƿ��Ѿ���ʼ��

        iTunesAuthorize*        _externalAuthorize;     // �ⲿ��Ȩ�ӿ�

        vector<wstring>         _attemptFixErrorIDs;    // �޸�ʧ�ܵ�Ӧ��ID
	};
}