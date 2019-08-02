#pragma once

#include "Module/BaseCommand.h"
#include <vector>

#include "Monitor/AppProgressObserver.h"

using namespace std;
namespace ND91Assistant
{
    class DeviceData;
    class AppData;
	class App2ExeInfo;

    //��װ�����ֻ���
    class InstallCommand : public BaseCommand
    {
    public:
		//��װ����
		enum APP_INSTALL_STEP
		{
			NONE,			//δ��ʼ
			PREPARE,		//׼�����ݣ� ��NPK��Ҫ�Ƚ�ѹ
			UPLOAD,			//�ɹ�
			INSTALL,		//ʧ��
			CONFIRM,		//��װ������Ҫ�û����ֻ���ȷ��
			FINISH,			//���
			FAIL,			//ʧ��
		};
        // Summary  : 
        // Returns  : 
        // Parameter: const DeviceData * pDevice
        // Parameter: const AppData * appData Ҫ��װ�ĳ���
        InstallCommand(const DeviceData* pDevice, const AppData* appData);
        virtual ~InstallCommand();

		void SetParam(App2ExeInfo* exeInfo)
		{
			_exeInfo = exeInfo;
		}

        // ִ������
        virtual void Execute();

        // Summary  : ��װһ��App �ļ򵥵��÷�������̬�����ڼ򵥵ĵ��ð�װ����
        // Returns  : bool �Ƿ�װ�ɹ�
        // Parameter: const DeviceData * pDevice �豸
        // Parameter: wstring strAppPath ��װ������PC�е�λ��
        // Parameter: wstring strPackageName ��װ�������ֻ��еİ���(Ψһ��ʶ)
        // Parameter: APP_TYPE appType ��װ��������
        static bool SimpleInstallApp(const DeviceData* pDevice, wstring strAppPath, wstring strPackageName, APP_TYPE appType);

		// ����Android�����壬
        virtual std::string GetAndroidBodyData();

		// �����������ݰ�
        virtual void SetAndroidReturnData(std::string data);

		// ��������ķ��ذ�ͷ
		bool parseAndroidPackageHead( BinaryBufferReader* pReader );

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·����
        virtual std::wstring GetIosCommandFilePath() { return L""; }

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath() { return L""; }

        // ����ios�ļ��������ļ����������߸���ɾ���ļ�
        virtual std::wstring GetIosCommandFileContent() { return L""; }

        // ���������ļ���������ɾ���ļ�
        virtual  void SetIosReturnFile(std::wstring filename) {}

 private:

	 //��װexe���͵İ�װ��
	 bool InstallApkInExe(wstring strPCPath,unsigned __int64 offsetAppInExe,unsigned __int64 len);
		// Summary  : ��װPC�е�Apk
		// Returns  : bool �Ƿ�װ�ɹ�
		// Parameter: wstring strPCPath ��װ������PC�е�λ��
		// Parameter: wstring strPackageName ��װ�������ֻ��еİ���(Ψһ��ʶ)
		bool InstallApk(wstring strPCPath, wstring strPackageName);

        // Summary  : ��װPC�е�Npk
        // Returns  : bool �Ƿ�װ�ɹ�
        // Parameter: wstring strPCPath ��װ������PC�е�λ��
        bool InstallNpk(wstring strPCPath);

	private:
		// Summary  : ��װ�ֻ��ϵ�Apk
		// Returns  : bool �Ƿ�װ�ɹ�
		// Parameter: wstring strPhonePath ��װ�������ֻ��е�λ��
		// Parameter: wstring strPkgName ��װ�������ֻ��еİ���(Ψһ��ʶ)
		bool InstallRemoteApk(wstring strPhonePath, wstring strPkgName);

	private:
		const AppData* _appData;
		APP_INSTALL_STEP _step;	// ��װ���
		wstring _appRemotePath;	// ��װ�����ϴ����ֻ��е�λ��

	private:
		bool _bLock;		// ��װʱ�Ƿ�����
		bool _bRecover;		// ��װ�Ƿ񸲸�
		int  _bOnSDCard;	// �Ƿ�װ��SDCard�� 2.2��Ч 1=SDCard 2:��SDCard 3δ���ã�Ĭ�ϣ�
        bool _isMove;
		//exe���Ͱ�װ���İ�װ��������
		App2ExeInfo* _exeInfo;

        wstring _errorMsg; // ��װʧ�ܷ��ش�����Ϣ
	public:
		void SetLock(bool b) {_bLock = b;};
		void SetRecover(bool b) {_bRecover = b;};
        void SetOnSDCard(bool b) {_bOnSDCard = b ? 1 : 2;};
        void SetIsMove(bool b) { _isMove = b; }

        wstring GetErrorMsg() { return _errorMsg; }
    };
}
