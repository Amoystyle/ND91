 #pragma once


namespace ND91Assistant
{
    class DeviceData;

	class CLASSINDLL_CLASS_DECL CGetDeviceImageInfo
	{
	public:
		CGetDeviceImageInfo( wstring strVID, wstring strMobiletype, wstring strIMEI, wstring strNum);
		CGetDeviceImageInfo( DeviceData* pDevice );
		~CGetDeviceImageInfo(void);

		bool GetDeviceImageInfo();
		wstring _Mobiletype;

	public:
		int _startX;		// ͼƬx����
		int _startY;		// ͼƬy����
		int _width;			// ͼƬ��
		int _height;		// ͼƬ��
		wstring _url;		// ͼƬ��url	

	private:
		wstring _VID;		// �豸hardid
		wstring _IMEI;      // �豸IMEI
		wstring _Num;		// �豸���

	protected:
		bool ParseInfo(string imginfo);
	};
}

