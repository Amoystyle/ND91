
#include "IosFileIO.h"

namespace ND91Assistant
{
    // ios文件收发 WIFI方式
    class IosWifiIO : public IosFileIO
    {
    public:
        IosWifiIO(int nHandle);

		// 发送命令到ios设备
		// file: 命令文件（完整路径）
		// 返回：结果文件
		virtual std::wstring SendIosFile(std::wstring data, bool bReceiveResult = true);

	private:
		int _nWiFiHandle;
			
    };
}
