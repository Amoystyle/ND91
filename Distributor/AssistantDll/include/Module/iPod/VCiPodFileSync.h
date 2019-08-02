#pragma once

#ifdef _ASSISTANT_DLL  // ��iPodManagerDll�в���Ҫ����

#include "Core/DeviceData.h"
#include "Device/IIosFileSystem.h"

namespace ND91Assistant
{
	//ipod�����ļ�ͬ��������
	class CLASSINDLL_CLASS_DECL iPodFileSync
	{
	public:
		//��ʼ���ļ�ͬ��
		static void InitFileSync(DeviceData* Device,wstring sqlDbPath);
		//�ϴ�cdb�ļ�
		static void UpLoadCDB();
		//�ϴ������ļ���lib���ݿ��ļ�
		static void UpdateDevItunesDB();
		//�ϴ������б���ص��ļ�
		static void UploadPlayListDB(wstring SrcFile);
		//��������
		static void DownLodMusic(wstring localfile,wstring remotfile);
		//�ϴ��½���������ļ�
		static void UpLoadFile(wstring localfile,wstring remotfile);
		//ɾ������
		static void DeleteMusicFile( wstring filename);
		//���ios����afc���
		static int GetAFCHandle();
		//����豸uuid
		static wstring GetUUID();
		//��ù̼��汾��
		static int GetDBVer();
		//����豸�ļ�Ŀ¼��·��
		static wstring GetDevicePath(wstring FileName);
	private:
		//�豸���
		static DeviceData* _pDevice;	
		//����itunes_controlĿ¼·��
		static wstring _sqlDbPath;
		//�豸��itunes_controlĿ¼��·��
		static wstring DeviceiTunsPath;

		static wstring _UUID;
	};
}

#endif