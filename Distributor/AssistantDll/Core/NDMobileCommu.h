#pragma once
#include "NDMMPubDef.h"
#ifdef WIN32
#else
    #define __stdcall
    #include <unistd.h>
#endif
#define MODULE_360
namespace ND91Assistant
{
	// Srv�ӿ���
	class NDMobileSrvCommu
	{
	private:
		~NDMobileSrvCommu();
	public:
		NDMobileSrvCommu();

		// �����ڴ棬�ڽ����˳���ʱ�����
		virtual void __stdcall ReleaseMemory();

		// ���ٽӿ���
		virtual void __stdcall DestroyInstanceSrvCommu();

		// �жϻ����Ƿ����
		virtual /*BOOL*/bool __stdcall IosEnvironmentSuc(/*LPWSTR*/wchar_t* pUrl, size_t nSize);

#ifdef MODULE_DRIVER
        // �ж��Ƿ�Ios�豸
        virtual /*BOOL*/bool __stdcall IsIosDevice(/*LPCWSTR*/const wchar_t* lpVID, /*LPCWSTR*/const wchar_t* lpPID);

		// Qualifier: ��ȡ����������
		virtual /*BOOL*/bool GetDriverInfo(PMOBILE_DEVICE_INFO lpDeviceInfo, /*LPCWSTR*/const wchar_t* lpVID, /*LPCWSTR*/const wchar_t* lpPID);
#endif
		// Summary  : ��ȡ����
		// Returns  : UINT __stdcall ���ص����ٷֱ�ֵ����80�������Ϊ80%
		// Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
		virtual /*UINT*/unsigned int __stdcall GetBatteryLever(/*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : ��ȡ�豸״̬
		// Returns  : NDASSISTANT_DEVICE_STATE �����豸״̬
		// Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
		virtual NDASSISTANT_DEVICE_STATE __stdcall GetDeviceState(/*LPCWSTR*/const wchar_t* lpSn);

		virtual void __stdcall SetLogPath(/*LPCWSTR*/const wchar_t* lpPath);

	private:
		// �����豸
		// nTime ��ʱʱ��
		/*BOOL*/bool ConnectDevice(int nWaitTime, /*LPCWSTR*/const wchar_t* lpSn);
	};
	// Mgr�ӿ���
    class NDMobileMgrCommu
    {
   
	private:
		~NDMobileMgrCommu();

	public:
		NDMobileMgrCommu();

		// �����ڴ棬�ڽ����˳���ʱ�����
		virtual void __stdcall ReleaseMemory();

		// ���ٽӿ�ʵ��
		virtual void __stdcall DestroyInstanceMgrCommu();
		// ��ʼ��, ����ʼ��
        virtual /*BOOL*/bool __stdcall BeginConnect (/*LPCWSTR*/const wchar_t* lpSn); 
        //virtual void __stdcall Uninit();

#pragma region ��Ϣ
        
        // Summary  : ��ȡ������Ϣ
        // Returns  : BOOL �ɹ�����TRUE
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        // Parameter: PMOBILE_BASE_INFO lpMobileInfo �洢�豸��Ϣ
        virtual /*BOOL*/bool __stdcall GetBaseInfo(PMOBILE_BASE_INFO lpMobileInfo, /*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : ��ȡ�ֻ��ռ���Ϣ
		// Returns  : BOOL __stdcall �ɹ�����TRUE
		// Parameter: PDISK_INFO lpDiskInfo �洢�ռ���Ϣ
		// Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
		virtual /*BOOL*/bool __stdcall GetSpaceInfo(PDISK_INFO lpDiskInfo, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡ����
        // Returns  : UINT __stdcall ���ص����ٷֱ�ֵ����80�������Ϊ80%
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*UINT*/unsigned int __stdcall GetBatteryLever(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡӦ�ø���
        // Returns  : INT  ����Ӧ�ø���
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*INT*/int __stdcall GetAppCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡ���ָ���
        // Returns  : INT  �������ָ���
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*INT*/int __stdcall GetMusicCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡͼƬ����
        // Returns  : INT  ����ͼƬ����(��δԽ���򲻰�����ֽ����)
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*INT*/int __stdcall GetImageCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡ��Ƶ����
        // Returns  : INT  ������Ƶ����
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*INT*/int __stdcall GetVideoCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡ���������
        // Returns  : INT ָ��Ŀ¼�µ��ļ�����(������Ŀ¼)
        // Parameter: LPCWSTR lpAppPath ���豸�ϵ�Ŀ¼
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*INT*/int __stdcall GeteBookCount(/*LPCWSTR*/const wchar_t* lpAppPath, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡ��ϵ�˸���
        // Returns  : INT  ������ϵ�˸���
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*INT*/int __stdcall GetContactCount(/*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡ����Ϣ����
        // Returns  : INT  ���ض���Ϣ����
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*INT*/int __stdcall GetMessageCount(/*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : �����豸����
		// Returns  : BOOL __stdcall �ɹ�����TRUE
		// Parameter: LPCWSTR lpNewName ���豸��������
		// Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
		virtual /*BOOL*/bool __stdcall SetDevName(/*LPCWSTR*/const wchar_t* lpNewName, /*LPCWSTR*/const wchar_t* lpSn);

#pragma endregion

#pragma region ����

        // Summary  : ��װ����
        // Returns  : UINT __stdcall �ɹ�����0�����򷵻ش������
        // Parameter: LPCWSTR lpAppPath ������ı���·��
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*UINT*/unsigned int __stdcall InstallApp(/*LPCWSTR*/const wchar_t* lpAppPath, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ж�س���
        // Returns  : UINT __stdcall �ɹ�����0�����򷵻ش������
        // Parameter: LPCWSTR lpAppId   ��MOBILE_APP_INFO���lpAppId
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*UINT*/unsigned int __stdcall UnInstallApp( /*LPCWSTR*/const wchar_t* lpAppId, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡ���г����б�
        // Returns  : BOOL �ɹ�����TRUE
        // Parameter: MOBILE_APP_INFO * * lpAppInfo ���洢�����б�
        // Parameter: int * lpSize ���б��С
        // Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
        virtual /*BOOL*/bool __stdcall GetAllApps( MOBILE_APP_INFO** lpAppInfo, int* lpSize, /*LPCWSTR*/const wchar_t* lpSn); 

        // Summary  : �ͷų����б���Ϣ
        // Returns  : BOOL �ɹ�����TRUE
        // Parameter: MOBILE_APP_INFO * lpAppInfo
        virtual /*BOOL*/bool __stdcall ReleaseAllApps(MOBILE_APP_INFO* lpAppInfo, /*LPCWSTR*/const wchar_t* lpSn);

        // Summary  : ��ȡ����ͼ��
        // Returns  : BOOL �ɹ�����TRUE
        // Parameter: LPCWSTR lpAppId    ��MOBILE_APP_INFO���szPackageName
        // Parameter: LPCWSTR lpSavePath �����ر��������
        virtual /*BOOL*/bool __stdcall GetPackageIcon( /*LPCWSTR*/const wchar_t* lpAppId, /*LPCWSTR*/const wchar_t* lpSavePath, /*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : PXLתΪIPA
		// Returns  : BOOL __stdcall �ɹ�����TRUE
		// Parameter: LPCWSTR lpSrcPxlFullName ��PXL�ļ�������·����
		// Parameter: LPWSTR lpDesIpaFullName ��ת�����IPA�ļ���
		// Parameter: int nMaxPath ��ת�����IPA�ļ�����󳤶�
		// Parameter: LPCWSTR lpSn ���豸Ψһ��ʶ
		virtual /*BOOL*/bool __stdcall PxlToIpa(/*LPCWSTR*/const wchar_t* lpSrcPxlFullName, /*LPWSTR*/wchar_t* lpDesIpaFullName, int nMaxPath, /*LPCWSTR*/const wchar_t* lpSn);

#pragma endregion

#pragma region ҵ��

        // Summary  : �ļ��ϴ�
        // Returns  : �Ƿ�ɹ�
        // Parameter: LPCWSTR lpLocalPath  �����ļ���(ȫ·��)
        // Parameter: LPCWSTR lpDevicePath �豸�ϵ�Ŀ¼·��
        // Parameter: LPCWSTR lpSn �豸Ψһ��ʶ
        virtual /*BOOL*/bool __stdcall UploadFile(/*LPCWSTR*/const wchar_t* lpLocalPath, /*LPCWSTR*/const wchar_t* lpDevicePath, /*LPCWSTR*/const wchar_t* lpSn);

		// Summary  : �ϴ���ֽ
		// Returns  : �Ƿ�ɹ�
		// Parameter: LPCWSTR lpLocalPath ͼƬ�ļ�
		// Parameter: LPCWSTR lpSn �豸Ψһ��ʶ
		virtual /*BOOL*/bool __stdcall UploadWallpaper(/*LPCWSTR*/const wchar_t* lpLocalPath, /*LPCWSTR*/const wchar_t* lpSn);

#pragma endregion

		virtual void __stdcall SetLogPath(/*LPCWSTR*/const wchar_t* lpPath);
    };
}