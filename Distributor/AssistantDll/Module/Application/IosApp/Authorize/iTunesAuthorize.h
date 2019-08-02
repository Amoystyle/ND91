#ifndef iTunesAuthorize_h__
#define iTunesAuthorize_h__
#pragma pack(push) //�������״̬
#pragma pack(1)//�趨Ϊ1�ֽڶ�
#include "Cer_export.h"
#pragma pack(pop)//�ָ�����״̬
#define HELPER_ENCRYPT_BYTES_LEN			104 //����Ϊ8�ı���
#define HELPER_SECURITY_LENGTH				(sizeof(DES_cblock) + (HELPER_ENCRYPT_BYTES_LEN * 2))

namespace ND91Assistant
{
    class DeviceInfo;
    class iTunesAuthorize
    {
    public:
	    iTunesAuthorize();

	    //1 ����������xxx.serInfo
	    //2	��������xxx.serInfo�ļ����12�����ݻ��������ļ����͸��ͻ���
	    //3 ��������C:\ProgramData\Apple Computer\iTunes\SC Info\Ŀ¼�µ�3���ļ����͸��ͻ���
	    //4 �ͻ��˴ӷ�����ȡ��3����Ȩ�ļ��ŵ��û���ȨĿ¼��,��C:\ProgramData\Apple Computer\iTunes\SC Info\
	    //5	�ͻ��˴ӷ�����ȡ��xxx.serInfo
	    //	��ȡǰ6����Ϊus.m_serverNumber1,�����ֵ��6����Ϊus.m_serverNumber2

	    //����itunes��Ȩ�ӿڣ�
	    //������iPodDev���ӵ��豸
	    //authorizationFromServer �Ƿ�ӷ�����ȡУ����Ϣ
	    //icInfoPath ��Ȩ���ļ�·��
	    //serverNumbers xxx.serInfo���ļ�����ÿ��һ��itemһ��12������m_serverNumber1+m_serverNumber2��
	    //ids 91����������Ȩ��iTunes�˻�������ipa�����ApplicationDSID
        //int count ids����
	    //szuuid �豸ID
        //iOSVersion �̼��汾
        //afcConn afc1���Ӿ��
	    //����-1������: 1.ȷ��iTunes�汾�Ƿ�֧�� 2.����itunes��Ȩ 
	    //3.ѡ���豸����,�̼�ҪiOS5�����ϰ汾,iOS4��֧�� 4.�豸���Ѱ�װһ����Ȩ�˺Ŵ�appstore���ص���� 5.�����豸����Ȩ
	    int Authorize(/*struct am_device *iPodDev,*/ bool authorizationFromServer, string icInfoPath, 
            vector<ND_UInt32> serverNumbers, ND_UInt64* ids, int count, /*char* szuuid, ND_UInt32 iOSVersion, ND_UInt32 afcConn*/const DeviceInfo* pDevInfoPtr);

		//itunes�Ƿ�֧����Ȩ
		static long IsAssistance();

    private:
	    int setSecurityBytes(unsigned char *data); //���غ�data��10����ʹ����Ч
		static int set_env();
    };
}
#endif // iTunesAuthorize_h__