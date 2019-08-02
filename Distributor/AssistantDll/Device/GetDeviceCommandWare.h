#pragma once
#ifndef GetDeviceCommandWare_h__
#define GetDeviceCommandWare_h__

#ifdef _ASSISTANT_DLL 
#define CLASSINDLL_CLASS_DECL     __declspec(dllexport) 
#else 
#define CLASSINDLL_CLASS_DECL     __declspec(dllimport) 
#endif 

//�ļ�˵�����ṩ������ɨ��dllʹ�õĻ���豸�ӿڣ���Ҫ����Ա���̬����ɳ�ʼ���������ȷ���ء�
#ifdef __cplusplus
#define  C_EXTERN  extern "C"
#else
#define  C_EXTERN 
#endif
	
// ���ios�豸�����б�
C_EXTERN CLASSINDLL_CLASS_DECL const char* GetIosDevices();

// ���android�豸�����б�
C_EXTERN CLASSINDLL_CLASS_DECL const char* GetAdbDevices();

// �ͷ�ǰ������ص�dll�ڲ�������ڴ��
C_EXTERN CLASSINDLL_CLASS_DECL void ReleaseCharPtr(char* p);

// �ر�ADB
C_EXTERN CLASSINDLL_CLASS_DECL void KillAdb();

// ��ʼ��ios���ؿ� 0-ʧ�� 1-�ɹ�
C_EXTERN CLASSINDLL_CLASS_DECL int IosInit();

// Summary  : ��adb_usb.ini���޲�������vidʱ���޸�adb_usb.ini
// Returns  : true���޸ĳɹ���false���޸�ʧ�ܡ��������޸ģ���������ini�з���true��
// Parameter: int vid ����ID��ʮ������
C_EXTERN CLASSINDLL_CLASS_DECL bool ModADBIni(int vid);


#endif // GetDeviceCommandWare_h__