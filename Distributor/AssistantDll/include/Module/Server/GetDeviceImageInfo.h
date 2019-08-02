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
		int _startX;		// 图片x坐标
		int _startY;		// 图片y坐标
		int _width;			// 图片宽
		int _height;		// 图片高
		wstring _url;		// 图片的url	

	private:
		wstring _VID;		// 设备hardid
		wstring _IMEI;      // 设备IMEI
		wstring _Num;		// 设备编号

	protected:
		bool ParseInfo(string imginfo);
	};
}

