#pragma once

#include "AFCFileSysemBase.h"

namespace ND91Assistant
{
	// ʵ����com.apple.mobile.house_arrest����ͨѶ �̼�3.x��ʼ����������ܣ�
	// �Ͱ汾��Ӧ�ó�����_bShare��ΪFALSE�������ſɸ���_FileSharingEnabled��ֵ�ж��Ƿ����˷���(ΪTUREʱ���б�Ҫ����)
	// ��ֻ��USBģʽ��ʵ��
	class AFCAppFileSystem : public AFCFileSysemBase
	{
	public:

		// ����ĳ��Ӧ�ó���Ĺ����ĵ�Ŀ¼����
		// strIdentifier	: ��Ӧ�ó����ʶ��
		static AFCAppFileSystem* StartAPPHouseArrestService(wstring strIdentifier, DeviceInfo* devInfo);

		/// ��ǰ�ļ�ϵͳ�ܷ��ʵ���Ŀ¼������Ĺ����ļ����ڴ�Ŀ¼��
		//eType	: ������ֵ����
		//����	���������ļ���Ŀ¼�����ϴ����صȲ�����ʱ�����Ҫ�ڴ˸�Ŀ¼��
		virtual string Ios_GetRootDirectory(eFileSystemType eType);		

	private:
		AFCAppFileSystem(void* pAfcHandle, const DeviceInfo* pDevInfo);
		~AFCAppFileSystem();
	};
}
