#pragma once

#include "BaseIO.h"
#include "AFCFileSysemBase.h"
#include "IosWiFiFileSystem.h"

namespace ND91Assistant
{
    // ios �ļ��շ���
    class IosFileIO : public BaseIO
    {
    public:
        IosFileIO(const DeviceInfo* pDevInfo);
		IosFileIO(int nHandle);

		virtual ~IosFileIO();

		// �������ios�豸
		// file: �����ļ�������·����
		// ���أ�����ļ�������·����
        virtual std::string SendAndroidDatagram(std::string filename, bool bReceiveResult = true);

        // summary  : ����ios������ؽ��
        // return   : std::wstring						: ����ļ���(unicode)
        // parameter: std::wstring strCmdFileOnPhone	: �ϴ����豸�ϵ������ļ���(ȫ·����unicode)
		// parameter: std::wstring strResultFileOnPhone	: ������PC�ϵĽ���ļ���(ȫ·����unicode)
		// parameter: std::wstring strCmdFileContent	: �����ļ�����(unicode)
		// parameter: bool bReceiveResult				: true=���շ���ֵ false=�����շ���ֵ
        virtual std::wstring SendIosFile(std::wstring strCmdFileOnPhone, std::wstring strResultFileOnPhone, 
			std::wstring strCmdFileContent, bool bReceiveResult = true);

		IIosFileSystem* GetIosFileStream(){return _pFileStream;}

		//ɾ��IOS����������ļ�(��.done�ļ�)
		void DeleteAllCmdDoneFile();

		void SetWaitTime(unsigned long nWaitTime){_nWaitTime = nWaitTime;}
		int GetWaitTime(){return _nWaitTime;}

	private:
		// Summary  : ����IOS�����ļ���PC�ˣ�
		// Returns  : std::wstring						�������ļ�������·����
		// Parameter: std::wstring strCmdFileName		�������ļ���
		// Parameter: std::wstring strCmdFileContent	�������ļ�����
		std::wstring CreateIosCommandFile(std::wstring strCmdFileName, std::wstring strCmdFileContent);

        // ����Ŀ¼
        void CreateDirectory( wstring path );

    protected:
		// �豸�ϵ������ļ���ÿ��������ܲ�һ��
		std::wstring _cmdFileOnPhone;

		//�豸�ϵĽ���ļ���ÿ��������ܲ�һ��
		std::wstring _resultFileOnPhone;

	private:
		IIosFileSystem* _pFileStream;

		unsigned long _nWaitTime;	//�ȴ����ص�ʱ��
    };
}
