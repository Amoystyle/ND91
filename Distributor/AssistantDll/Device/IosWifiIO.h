
#include "IosFileIO.h"

namespace ND91Assistant
{
    // ios�ļ��շ� WIFI��ʽ
    class IosWifiIO : public IosFileIO
    {
    public:
        IosWifiIO(int nHandle);

		// �������ios�豸
		// file: �����ļ�������·����
		// ���أ�����ļ�
		virtual std::wstring SendIosFile(std::wstring data, bool bReceiveResult = true);

	private:
		int _nWiFiHandle;
			
    };
}
