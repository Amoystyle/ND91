#pragma once

#include "AssistantDll.h"
#include "IObserver.h"
#include "ProgressData.h"
#include "ICancel.h"
#include <string>
namespace ND91Assistant
{
    enum MESSAGE_CODE {
        MESSAGE_PREPARE,        // ׼���׶Σ��Ѽ�����
		MESSAGE_ERROR,
		MESSAGE_NEED_SU_AUTH_IN_DEVICE,//��Ҫ�ͻ����豸��ȷ�����ֿͻ���Ȩ������
		//����װ���						
		MESSAGE_INSTALL_PREPARE,			// ׼������	
		MESSAGE_INSTALL_UPLOAD,				// �ϴ�APP�ļ�
		MESSAGE_INSTALL_INSTALL,			// ��װ����
        MESSAGE_INSTALL_AUTHORIZOR,			// ��Ȩ
        MESSAGE_INSTALL_UNINSTALL,          // ж�س���
		MESSAGE_INSTALL_COPYDATA,			// ������������
		MESSAGE_INSTALL_NOTIFYSB,			// ����IOS���ͼ��
        MESSAGE_INSTALL_PXLTOIPA,           // ת��
		//���ݳ������
		MESSAGE_ARCHIVE_DOWNLOAD,			//����APP����
		MESSAGE_ARCHIVE_ARRANGE,			//��������
		MESSAGE_ARCHIVE_PACKAGE,			//���APP����		
		MESSAGE_ARCHIVE_REMOVE,				//ɾ���ֻ��ϵı��ݳ���
		MESSAGE_ARCHIVE_APP_ONPHONE,	//�����򱸷ݵ��ֻ���ĳ��Ŀ¼��	
        //�ļ����
        MESSAGE_UPLOAD,                     // �ϴ��ļ�
        MESSAGE_DOWNLOAD,                   // �����ļ�
        MESSAGE_BACKUP,                     // ��������
        MESSAGE_RESTORE,                    // �ָ�����
        MESSAGE_EXPORT,                     // ��������
        MESSAGE_IMPORT,                     // ��������
        MESSAGE_PARSE,                      // ������������

		MESSAGE_PROCESS,					//�ر�����
		MESSAGE_COMPLETE,					//����

		//���ݻ�ԭ
		MESSAGE_NoSupportSynchronizer,		//��֧��ͬ��,��ϵ�˱��������ʾ�����Ϣ���޷�������ϵ��
		MESSAGE_EnableiCloud,				//��ͬ����������ϵ�˱�������յ�����Ϣ��ʹ����ͬ������ϵ���޷�����(Ҫ������ȡ����ͬ��������ϵ�˲�����ͬ��������)	
		MESSAGE_EnableSyncFail,				//ͬ�����ܱ���ֹ�����ҳ��Դ�ʧ�ܣ�
		MESSAGE_DeamonDisConnect,			//���ػ������ӶϿ��������ػ�������.
    };

    // �Ѵ���ͬ���ļ�/�ļ���ʱ�Ĳ�������Ҫ�û����룩
    enum EXIST_ACTION
    {
        OVERWRITE,              // ����
        OVERWRITE_ALL,          // ��������
        RENAME,                 // ������
        RENAME_ALL,             // ����������
        RENAME_COPY,            // ������(�ļ���ǰ��������)
        RENAME_COPY_ALL,        // ����������(�ļ���ǰ��������)
        CANCEL,                 // ȡ��
        NONE,                   // ��ֵ���޺���
    };

    // ������ʾ������
    class CLASSINDLL_CLASS_DECL ProgressObserver : public IObserver, public ICancel
    {
    public:
        virtual ~ProgressObserver() {}

        // ���ս�����ʾ��������ʾ������ݣ�����������֧�֣��ɽ���ʵ��
        virtual void GetPrompt(MESSAGE_CODE msgCode) = 0;

        // ���ս�������
        virtual void GetProgress(ProgressData pData) = 0;

        // ���ս��������Ϣ
        virtual void Complete() = 0;

        // Summary  : ѯ���û��ļ�/Ŀ¼�Ѵ���ʱ��δ���
        // Returns  : ND91Assistant::EXIST_ACTION �û���ѡ��
        // Parameter: std::wstring name �ļ�/Ŀ¼��
        virtual EXIST_ACTION GetExistAction(std::wstring name) = 0;

        virtual bool IsCancel() { return ICancel::IsCancel(); }
    };

	class CLASSINDLL_CLASS_DECL NullProgressObserver : public ProgressObserver
	{
		virtual ~NullProgressObserver() {}

		// ���ս�����ʾ��������ʾ������ݣ�����������֧�֣��ɽ���ʵ��
		virtual void GetPrompt(MESSAGE_CODE msgCode) {}

		// ���ս�������
		virtual void GetProgress(ProgressData pData) {}

		// ���ս��������Ϣ
		virtual void Complete() {}
	};

}
