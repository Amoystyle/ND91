///////////////////////////////////////////////////////////
//  AndroidDefin.h
//  ��׿��ʽ��صĳ�������
///////////////////////////////////////////////////////////

#pragma once

// ��׿���ݰ��İ�ͷ���ࣺ������
#define ANDROID_PACKAGE_NDPC "NDPC";
#define ANDROID_PACKAGE_SYNC "SYNC";
#define ANDROID_PACKAGE_END  "SEND";

#define ANDROID_PACKAGE_OKAY    "OKAY"

#define ANDROID_DEAMON_REQUIRE_VERSION  "2.6.261"   //�ػ���Ҫ����С�汾��

//#include "Core/Define.h"

namespace ND91Assistant
{
	// �ػ�PackageName
	#define ADB_DEMON_PACKAGE_NAME  "com.nd.assistance.ServerService"

	#define ANDROID_APP_INSTALL_DIR     L"/data/local/tmp/"         //�ػ��ļ�Ҫ�ϴ����ֻ��е�Ŀ¼
	#define ANDROID_APP_INSTALL_PATH    L"/data/local/tmp/pkg.apk"  //�ػ��ϴ��������·��
	#define ANDROID_APP_INSTALL_DIR_DEMON    L"/sdcard/nd/installers/"	//Wifi��װ�İ�װ·��

	#define Default_BookMark_CategoryId  "99999999"

    // ��׿�����ݰ���ʶ�� 2���ֽ�
    enum ANDROID_COMMAND_FLAG
    {
        Unknow					= 0,		// δ֪�ķ����ʶ
        Client					= 1 << 1,	// �ͻ��˵ķ��
        Server					= 1 << 2,	// ����˵ķ��
        NeedPostBack			= 1 << 3,	// ��Ҫ�ط��ķ��
        CreateLongConnection	= 1 << 4,	// ����������
        CloseCurrentConnection	= 1 << 5,	// �رճ�����
        CancelSend				= 1 << 6,	// ȡ������
    };

    // ��׿��ҵ�����ͣ� 4���ֽ�
    enum ANDROID_COMMAND_CHANNEL
    {
        SYSTEM = 0,			//ϵͳҵ���ʶ(������)
        CONTACT = 1,		//��ʾ��ϵ����Ӧ��ҵ��
        SMS = 2,			//��ʾ���ŷ����ҵ��
        PHOTOS = 3,			//ͼƬ����
        TODOLIST = 4,		//��������
        BOOKMARK = 5,		//��ǩ
        CALENDAR = 6,		//������
        MEDIA = 7,			//��ý�塣
        CALLS = 8,			//ͨ����¼
        EMAIL = 9,			//�ʼ�
        NOTE = 10,			//��ǩ
        SETTING = 11,		//����
        APPLICATION = 12,	//�������
        ALARM = 13,			//����
        SYSINFO = 14,		//ϵͳ��Ϣ
        THEME = 15,			//�������
        DEAMONMANAGER = 16,	//�ػ�������ȡ�ػ��汾�ŵȣ�
        SHELL = 17,			//shell ����
        FILETRANSFER = 18,	//�ļ�����
        PUSHFILE = 19,		//�ϴ��ļ�
        FILEMANAGER = 20,	//�ļ�����ӿڡ�
		SMARTTHEME=21,		// ��׿����
        PIM = 199,			//PIM
        CHECKCONFIRM=200,   //����Ƿ���Ҫ����ȷ��
    };

    // ��׿�����ݰ�����ÿ��������һ�����Ͷ��壬ͳһд�ڴ˴�
    enum ANDROID_COMMAND_TYPE
    {
        WAKE_DEMON = 0,			    //�����ػ�(֪ͨ�ػ���PC�˽���)
        GET_DEMON_INFO = 1,         //��ȡ�ػ���Ϣ
        GET_DEMON_VERSION = 3,      //��ȡ�ػ��汾

        GET_CONTACT = 1,            //��ȡ��ϵ��
        ADD_CONTACT = 2,            //������ϵ��
        UPDATE_CONTACT = 3,         //������ϵ��
		GET_CONTACT_PHOTO = 4,      //��ȡ������ͷ��
        DEL_CONTACT = 5,            //ɾ����ϵ��
        ADD_CONTACTGROUP = 6,       //�����ϵ�˷���
        UPDATE_CONTACTGROUP = 7,    //������ϵ�˷���
        DEL_CONTACTGROUP = 8,       //ɾ����ϵ�˷���
        SET_FAVORITE = 9,           //�����ղ���ϵ������
        ADD_TOGROUP = 10,           //�����ϵ�˵���ϵ�˷���
        REMOVE_FROMGROUP = 11,      //����ϵ�˷����Ƴ���ϵ��

        GET_SMS = 21,               //��ȡ����Ϣ
        ADD_SMS = 22,               //��������Ϣ
        UPDATE_SMS = 23,            //�޸Ķ���Ϣ
        DEL_SMS = 24,		        //ɾ������Ϣ
        SEND_SMS = 26,              //���Ͷ���Ϣ
        SETREAD_SMS = 27,           //���ö���Ϣ�Ƿ��Ѷ�
        RESEND_SMS = 28,            //�ط�����Ϣ

        GET_CALL = 41,				//��ȡͨ����¼
        ADD_CALL = 42,              //����ͨ����¼
        DEL_CALL = 43,              //ɾ��ͨ����¼

        GET_IMEI = 1,               //��ȡIMEIֵ
		GET_BATTERY = 14,			//��ȡ�����Ϣ

		GET_APPINFO = 0,			//��ȡApp��Ϣ
        GET_APPLIST = 31,           //��ȡAPP�б�
        GET_ALLAPPLIST = 30,         //��ȡ����APP�б�
        GET_APPICON = 1,            //��ȡAppͼ��
		GET_APPPERMISSIONS = 6,		//��ȡ����Ȩ��
		INSTALL_APP = 3,			//��װAPP
		UNINSTALL_APP = 4,			//ж��APP

        GET_SCREENSHOT = 4,         //��ȡ��Ļ��ͼ

		TABLE_QUERY = 99,			//��ȡ�б�
		ADD_RING = 0,				//�������

		Query_BaseSetting = 1,      // ��ȡ��������
		Query_Profiles = 3,         // ��ȡ�龰ģʽ
		
        GET_MACADDRESS = 26,        // ��ȡ�豸MAC��ַ
		GET_ISROOT = 29,			// ��ȡandroid�豸�Ƿ���root	
		GET_ISWRITE = 30,			// ��ȡsystem/app�Ƿ��д
		GET_ISREMOUNT = 31,			// �޸�system/appΪ��д
		UNINSTALL_SYSTEMAPP = 32,	// ж��android�豸��ϵͳ����

        GET_DIRSPACEINFO = 23,      // ��ȡĿ¼�ؼ���Ϣ

		GET_RESOLUTION = 15,		// ��ȡandroid�豸�ֱ���
        GET_FILEINFO = 16,          // ��ȡ�ļ���Ϣ

        ExecShell = 2,

        QUERY_DEVICENAME = 11,      //��ȡָ���ֻ�������
        ADD_DEVICENAME = 12,        //����һ�����ֻ����ֻ�����
        UPDATE_DEVICENAME = 13,     //�޸�һ���ֻ�������

		UNINSTALL_SYSTEMAPP_BYNDSH = 21, //ʹ��NDSH��ʽж��ϵͳ���
		GET_NDSH_STATE = 20,//��ȡndsh״̬
		GET_SU_AUTH = 21,//��ó����û�Ȩ��
    };

	enum DataRowState
	{
		// ժҪ:
		//     �����ѱ����������������κ� System.Data.DataRowCollection��System.Data.DataRow ������������������ڴ�״̬������֮����ӵ�������֮ǰ����Ӽ������Ƴ�֮��
		DataRowState_Detached = 1,
		//
		// ժҪ:
		//     �������ϴε��� System.Data.DataRow.AcceptChanges() ������δ���ġ�
		DataRowState_Unchanged = 2,
		//
		// ժҪ:
		//     ��������ӵ� System.Data.DataRowCollection �У�System.Data.DataRow.AcceptChanges()
		//     ��δ���á�
		DataRowState_Added = 4,
		//
		// ժҪ:
		//     ������ͨ�� System.Data.DataRow �� System.Data.DataRow.Delete() ������ɾ����
		DataRowState_Deleted = 8,
		//
		// ժҪ:
		//     �����ѱ��޸ģ�System.Data.DataRow.AcceptChanges() ��δ���á�
		DataRowState_Modified = 16,
	};

}
