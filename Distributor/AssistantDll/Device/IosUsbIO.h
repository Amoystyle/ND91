#pragma once

#include "IosFileIO.h"

namespace ND91Assistant
{
    // ios�ļ��շ� USB��ʽ
    class IosUsbIO : public IosFileIO
    {
    public:
        IosUsbIO(void* pAfcHandle, bool bJailBreaked);

		// �������ios�豸
		// file: �����ļ�������·����
		// ���أ�����ļ�
        virtual std::wstring SendIosFile(std::wstring data, bool bReceiveResult = true);

	private:
		void* _pAfcHandle; //�ļ��������
		bool _bJailBreaked; //�Ƿ�Խ��
    };
}
