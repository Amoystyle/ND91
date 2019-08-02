#pragma once

namespace ND91Assistant
{
	class NoDeamonInterface
	{
	public:
        static void Uninitialize();
        static bool Initialize();

        //DWORD g_ipAddress;
        //CIPAddressCtrl m_ctl_ipaddress.SetAddress(10, 0, 2, 9);
        //m_ctl_ipaddress.GetAddress(g_ipAddress);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: BYTE** pngBytesPtr ��ͼ�ļ���ͷָ��,ִ������Ҫ����iphoneinterface_delete�ͷŷǿյ�*pngBytesPtr
        // Parameter: UINT* pngLength ��ͼ�ļ��Ĵ�С,�ֽ�
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : �������
        static int Screenshot(void* devicePointer, BYTE** pngBytesPtr, UINT* pngLength, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* buffer
        // Note     : ��delete[]�ͷ�DLL�ڲ�new[]���ڴ�
        static void Delete(void* buffer);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ע��
        static int Reload(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ����
        static int Reboot(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : �ػ�
        static int Shutdown(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ͼ���޸�
        static int IconsRepair(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR* imsi ȡ�õ�imsi,ִ������Ҫ����iphoneinterface_delete�ͷŷǿյ�imsi
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        static int GetImsi(void* devicePointer, PTCHAR* imsi, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR* type ȡ�õ�imsi,ִ������Ҫ����iphoneinterface_delete�ͷŷǿյ�imsi
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        static int GetType(void* devicePointer, PTCHAR* type, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: PTCHAR processName ����������SpringBoard
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ��������
        static int Kill(void* devicePointer, PTCHAR processName, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR remotePxlFile Զ���ļ�ȫ·�� ����unicode����
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
		//backupfile: �����ļ���λ�� deondir���ػ�λ��
        // Note     : pxl��װ,����Ѵ��ڻ���ж���ٰ�װ,��app�µ�Info.plist��CFBundleIdentifier��PxlPkg.plist��ߵĲ�һ��,���޸ĳ�PxlPkg.plist��ߵ�CFBundleIdentifier(�ٱ������������Ҫ)
        static int PxlInstall(void* devicePointer, PTCHAR remotePxlFile, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR CFBundleIdentifier PxlPkg.plist���CFBundleIdentifier
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : pxlж��
        static int PxlUninstall(void* devicePointer, PTCHAR CFBundleIdentifier, DWORD iosIp = 0);
       
        // Returns  : int ����ERR_IPHONERUN_LAUNCHD_FILE_LOAD_ERROR��Ҫ��ʾ�û������ֻ�
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ��װipa�ֻ����ֻ����ֲ��(wifi�ػ������ipa�ֻ����ֻ������ػ�����),�������ػ��ļ��и���,��Ҫ�������ͬʱ�����ֻ��˳���,����Ϊusb����
        static int Install91HelperService(void* devicePointer, DWORD iosIp = 0);

        // Method   : iphoneinterface_runProgram iphoneinterface_special_runProgram
        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: PTCHAR program ��system()ִ�еĲ���
        // Parameter: long waitSeconds ִ�еȴ�ʱ��,��
        // Parameter: int * status ִ�з���ֵ,program���򷵻�ֵ�ĵ�8λ(0x00000000~0x000000ff),������Ҫ��NULL
        // Note     : ���г����ű�
        static int RunProgram(void* devicePointer, const PTCHAR program, long waitSeconds, int* status = NULL, DWORD iosIp = 0);
        static int SpecialRunProgram(void* devicePointer, const PTCHAR program, long waitSeconds, int* status = NULL, DWORD iosIp = 0);
        
        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR remoteFile Զ���ļ�·�� ����unicode����
        // Parameter: PTCHAR mode Ȩ��rwxr-xr-xΪ0755 rw-r--r--Ϊ0644 ���Ȩ��Ϊrwsrwsrwt 07777
        // Parameter: bool recursive �Ƿ�Ҳ�޸���Ŀ¼�ļ�
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        static int Chmod(void* devicePointer, PTCHAR remoteFile, PTCHAR mode = _T("rwxr-xr-x"), bool recursive = false, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR remoteFile Զ���ļ�·�� ����unicode����
        // Parameter: PTCHAR uid 0��501
        // Parameter: PTCHAR gid 0��501
        // Parameter: bool recursive �Ƿ�Ҳ�޸���Ŀ¼�ļ�
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        static int Chown(void* devicePointer, PTCHAR remoteFile, PTCHAR uid = _T("501"), PTCHAR gid = _T("501"), bool recursive = false, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR aTelephoneNumber ���ŵ�һ�����պ���
        // Parameter: PTCHAR content ��������
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ����һ�����ŵ�һ�����պ���
        static int SendSmsSingle(void* devicePointer, PTCHAR aTelephoneNumber, PTCHAR content, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : �����޸�
        static int SmsRepair(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ���������޸���Ϣ
        static int ChangeCallRingtone(void* devicePointer, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR smsid ����id
        // Parameter: PTCHAR flag ���ű�ʾλ
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ���ö���״̬
        static int SetSmsFlag(void* devicePointer, PTCHAR smsid, PTCHAR flag, DWORD iosIp = 0);

        // Returns  : int ����0�ɹ�,С��0�����ʧ��ֵ
        // Parameter: void* devicePointer �豸ָ��
        // Parameter: PTCHAR remoteFile Զ���ļ�ȫ·��
        // Parameter: INT64 * size ��С�ֽ�
        // Parameter: UINT16 * mode Ȩ��struct _stat64���st_mode
        // Parameter: UINT32 * uid ����0����501��������
        // Parameter: UINT32 * gid ����0����501��������
        // Parameter: LONG * mtime �ļ��޸�ʱ��struct _stat64���st_mtime
        // Parameter: LONG * birthtime �ļ�����ʱ��
        // Parameter: DWORD iosIp �豸ip��ַ,�����ֽ���127.0.0.1-->iosIp=0x7F000001  Ϊ0��ΪUSB����
        // Note     : ȡ�ļ�״̬
        static int GetFileInfo(void* devicePointer, PTCHAR remoteFile, INT64 *size, UINT16 *mode, UINT32 *uid, UINT32 *gid, LONG *mtime, LONG *birthtime, DWORD iosIp = 0);

		static void SetPath(wstring lbackupfile,wstring ldeamonDir);
	private:
        static wstring backDir;
        static wstring deamonDir;
	};
}