#pragma once

#include "AFCFileSysemBase.h"

namespace ND91Assistant
{
	class DeviceData;

	class IosDaemonHelper
	{
	public:
		IosDaemonHelper(const DeviceData* pDevice);
		~IosDaemonHelper(void);

		//strDaemonZipFile:�ػ���zipѹ����
		bool InstallDaemon(wstring strDaemonZipFile, wstring daemonVersionOnService);
		
		//����֮ǰ����ִ��ֹͣ�ػ�����"TQServer"����
        bool UnInstallDaemon();

    private:
		//strDeamonFolder�ػ�zip����ѹ���·��
        bool InstallDaemonByFolder(wstring strDeamonFolder, bool isNew);

        void ClearOldDeamonFile();
		void InitRun(int nVerNum);
		void BackFile(wstring strDeamonFolder, bool isNew);
		void UpdateFile(wstring strDeamonFolder);		
        // ���� �ػ���PList�ļ�
        bool UpdateInfoPList(wstring strDeamonFolder);

		bool StartIosServer(wstring strDeamonFolder);



        // �ϴ��ļ��б�
        void UploadFile(bool isNew);

        // 4.x�Ժ�̼�ʹ��
        bool InstallDaemo_New();// ���ػ���װ��ʽ
        // 3.x����ǰ�̼�ʹ��
        bool InstallDaemo_Old();// ���ػ���װ��ʽ

        // ���ػ��汾��д�뵽�ֻ�
        bool WriteTQServerVersion();

        // �����ػ�
        bool StartDaemon();

	private:
		AFCFileSysemBase* _pFileSteam;
		wstring _strRunFile;
		wstring _strRunPlist;
		wstring _strSerialNum;
        wstring _strDaemonFolderTemp;// �ػ���ѹ�ڱ��ص���ʱĿ¼
        wstring _daemonVersionOnService;// �ػ��ڷ������ϵİ汾��

		const DeviceData* _pDevice;
	};
}
