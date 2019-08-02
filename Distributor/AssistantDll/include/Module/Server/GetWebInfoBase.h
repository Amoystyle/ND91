#pragma once

namespace ND91Assistant
{
    class DeviceData;

	// ͨ��WEB������ȡ���ְ���Ϣ����
	class CLASSINDLL_CLASS_DECL CGetWebInfoBase
	{
	public:
		CGetWebInfoBase();
		CGetWebInfoBase(DeviceData* pDevice);
		~CGetWebInfoBase(void);

		// ��ȡ�豸Ӧ��������Ϣʱ����
		string GetPhoneInfo();

		// Summary  : ��ȡ����Ӧ��������Ϣʱ���� 
		// Returns  : string
		// Parameter: DEVICE_TYPE deviceType   �������(ios����android)
		string GetSendInfo( int deviceType );

		wstring _strUrl;		// ��װ��url
		wstring _strVersion;	// ��ǰ���°汾��
		time_t  _updateTime;	// �汾����ʱ��
		int		_nFileSize;		// ��װ����С
		int		_nResult;		// ִ�н����0Ϊ��ȷ����0Ϊ����
		int		_nNeedForce;    // �Ƿ�ǿ��������1Ϊǿ��	
		wstring _content;		// ��������

	protected:

		wstring GetDeviceMT();  // ��ȡ�豸ƽ̨����
		wstring GetVersion();	// ��ȡ�ֻ��ػ��汾
		wstring GetFwVersion(); // ��ȡ�豸�̼��汾

		bool ParseXml(string xmlData);	// ����XML

		// WEBͨ�ż����ֻ��豸��Ϣ
		string GetPhoneData( string phoneVersion, string phoneSerial, string platForm );

		// ����i=str&ks=index&key=key��  ksΪ����key��������iΪ�ֻ�����Ϣ��keyΪ���ܵ�key
		string EncodeInfo( string phoneData );
	protected:

		DeviceData* _pDevice;


	};

}

