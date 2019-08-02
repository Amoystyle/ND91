///////////////////////////////////////////////////////////
//  ScreenshotCommand.h
//  Implementation of the Class ScreenshotCommand
//  Created on:      01-六月-2011 9:43:03
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_E5F5FD3B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_)
#define EA_E5F5FD3B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_

namespace ND91Assistant
{
    // 截屏命令
    class ScreenshotCommand : public BaseCommand
    {

    public:

        ScreenshotCommand(const DeviceData* pDevice, wstring strSavePath);
        virtual ~ScreenshotCommand();

        // 执行命令
        virtual void Execute();


		// Summary  : 获得屏幕图片二进制数据
		// Returns	: string
		string  GetScreenImageBinaryData(){return _ScreenImagebinaryData;}

#pragma region 这四个方法是Command的主要内容，处理令格式与结果格式

		// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
        virtual std::string GetAndroidBodyData();

		// 解析返回数据包
		virtual void SetAndroidReturnData(std::string data);

        // 从IOS上获取的命令结果文件名（全路径）
        virtual std::wstring GetIosResultFilePath();

		// 解析ios设备的返回文件，解析后删除文件
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion
	
    private:
        //特殊的命令返回格式
        bool parseAndroidPackageHead( BinaryBufferReader* pReader );
        bool SaveScreenShot(wstring strFilePath);

        void WifiExecute();
        void AndroidUSBExecute();
        void IosUSBExecute();

        wstring _strSavePath;	// 取得的屏幕截图要保存到的PC中的路径	

		string    _ScreenImagebinaryData;

    };
}

#endif // !defined(EA_E5F5FD3B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_)
