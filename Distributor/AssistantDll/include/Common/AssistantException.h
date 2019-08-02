#pragma once

#include "BaseException.h"

// �궨�壺�����������AssistantException�Ĺ��캯��
#define ASSISTANT_EXCETPION(x) AssistantException(x, __FILE__ , __LINE__)
//#ifdef _DEBUG
#define THROW_ASSISTANT_EXCEPTION(x) throw AssistantException(AssistantException::x, __FILE__ , __LINE__)
// #else
// #define THROW_ASSISTANT_EXCEPTION(x) AssistantException(AssistantException::x, __FILE__ , __LINE__)   // ֻдLog
// #endif

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL AssistantException: public BaseException
    {
    public:
        // ����ҵ��������
        enum ASSISTANT_ERROR_CODE
        {
            INVALID_LOG_FILE_NAME,          // ��Ч��LOG�ļ���
            WSA_STARTUP_FAIL,               // WSAStartup()���÷��ش���

            PARSE_ANDROID_DATAGRAM_FAIL,    // Android�ػ��������ݰ���ʽ��������
            UNKNOWN_DEVICE_OR_CONNECTION,   // δʵ�ֵ��豸���ͻ���������
            ADB_SOCKET_ERROR,               // �������Ӵ���
            INVALID_DEVICE_SERIAL_NUMBER,   // ��Ч���豸��ʶ�ַ���
            ADB_COMMAND_RETURN_ERROR,       // adb��������ݸ�ʽ����
            ADB_EXE_NOT_FOUND,              // �Ҳ���ADB.EXE�����ļ�

			ADB_START_SERVER_ERROR,			//����ADB�������
			ADB_START_DEMON_ERROR,			//�����ػ�����
			ADB_FORWORD_ERROR,				//ӳ��˿ڴ���
			ADB_WAKE_DEMON_ERROE,			//�����ػ�����

            FILE_NOT_FOUND,                 // �ļ��������ļ�δ�ҵ�
            FILE_IS_EMPTY,                  // ���ļ�
            OPEN_FILE_FAIL,                 // ���ļ�ʧ��
			INVALID_FILE_NAME,              // ��Ч���ļ���
			REMOTE_FILE_ALREADY_EXIST,      // Ŀ���ļ��Ѿ�����
			REMOTE_PATH_NOT_EXIST,			// �ֻ���·��������
			REMOTE_PATH_CREATE_FAIL,        // �ֻ���·������ʧ��
            DEVICE_NOT_ACTIVE,              // �豸δ�������ʹ��

			IOS_WIFI_C0NN_ERROR,			//ios�豸WIFI����ʧ��
			IOS_WIFI_PSW_ERROR,				//IOS�豸wifi�����������
			IOS_WIFI_TRANSFER_ERROR,		//IOS�豸wifi����ʧ��

            INVALID_IO_OBJECT,              // �����IO����ָ�롣�ڲ���������ϵ������Ա
            PROGRAM_ERROR,                  // �����ڲ���������ϵ������Ա

            LS_RESULT_PARSE_FAIL,           // ��׿LS����Ľ���޷�ʶ���ʽ���뽫���ͷ���������Ա

			IOS_ITUNES_APP_OPERATION_LOCK, //ͨ��ITUNES����APP�Ѽ���

            STRING_NOT_SET,                 // �ڲ�����δ������Ҫ��ʾ���ַ������ݣ����International��
            DLL_RETURN_ERROR,               // DLL�������ش������

			PRODUCT_VERSION_NOT_AGREE,			        // �̼��汾������Ҫ��
            NotValideToAuthorizeByDeviceVersion,        // �豸�ǲ��ʺ�������Ȩ(�ݽ�֧��IOS4.0�����ϰ汾)
            NotValideToAuthorizeByNotUSB,               // ���ӷ�ʽ����USB
            NotValideToAuthorizeByItunesVersionTooLow,  // iTunes�汾����
            NotValideToAuthorizeByNotFoundItunes,       // δ��װiTunes
            NotValideToAuthorizeByItunesIsSimple,       // ���װ汾iTunes
			IOS_UNINSTALL_APPSYNC,				// δ��װAppSync
            PRODUCT_PROHIBIT_APPINSTATLL,       // �豸��ֹ��װӦ��
            INSTALAPPFILELOST,                  // Ӧ�ó����������
            INSTALAPPUNZIPERROR,                // Ӧ�ó�����޷���ѹ
            DeviceUnSupported,                  // ��Ӧ����ipad���ã����ʺϵ�ǰ�豸iPhone/iPod
            NoJailBreakedUnSupported,           // ��Խ���豸�ݲ��ṩ��װӦ�ù���
            NoJailBreakedNoLegalSoft,           // ��Խ���豸��֧�ְ�װ������Ӧ��
            AuthorizorFaile,                    // ��Ȩʧ��

			ARCHIVE_APP_ARCHIVE_ON_DEVICE_ERROR, //���ݳ���ʱ�����ݵ��ֻ���ʧ��
			ARCHIVE_APP_COPY_TO_PC_ERROR, //���ݳ���ʱ��������PC��ʧ��

			IOS_RESTORE_NOTIFICATIONS_ERROR,  //Register RestoreNotifications Failed
			IOS_REGISTER_SUBSCRIBE_ERROR, //Register Subscribe Failed
			IOS_REGISTER_SUBSCRIBE_EXCEPTION_ERROR, //Register Subscribe Failed With Exception

			DRIVER_INSTALL_ERROR,			// ������װ����

            PHOTO_DATABASE_FORMAT_ERROR,    // ����photo database�ļ�ʱ������ʽ����

			NOT_SUPPORT, //��֧��

			CDB_FILE_FORMAT_ERROR,    // ����CDB�ļ�ʱ������ʽ����

			ADB_COMMAND_NO_NDDEVICE,       // �ɵ�adbû��nddevice����
        };

        AssistantException(ASSISTANT_ERROR_CODE code, string filename, int lineNo) :
          BaseException((int)code, filename, lineNo)
          {

          }

    };
}