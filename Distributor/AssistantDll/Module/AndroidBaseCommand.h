#pragma once

#include "IBaseCommand.h"
namespace ND91Assistant
{
    class AndroidBaseCommand : public IBaseCommand
    {
    public:
        AndroidBaseCommand(const DeviceData* pDevice);
        ~AndroidBaseCommand();
        
        virtual void Execute();
    
        // ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
        virtual std::string GetAndroidBodyData() = 0;

        // �����������ݰ�
        virtual void SetAndroidReturnData(std::string data) = 0;


    protected:
        // ����Android���ݰ��������������ͷŴ�ָ�룬��
        std::string GetAndroidDatagram();

        // ����Android���ص����ݰ�ͷ -- ǰ20���ֽ� (��׼)
        bool parseAndroidPackageHead(BinaryBufferReader* pReader);

        // ����Android���ص����ݰ�ͷ -- ǰ16���ֽ�
        bool parseAndroidPackageHead_Top16(BinaryBufferReader* pReader);

        // ����Android���ص����ݰ�ͷ -- ǰ12���ֽ�
        bool parseAndroidPackageHead_Top12(BinaryBufferReader* pReader);

    protected:
        // Android���͵İ�ͷ: ����ÿ������Ĺ��캯����ʼ��
        // ֻ��ʹ��AndroidDefine.h�ж�������ְ�ͷ֮һ
        const char*	_pHead;

        // Android���͵ı�־������ÿ������Ĺ��캯����ʼ��
        // ֻ��ʹ��enum ANDROID_COMMAND_FLAG��ֵ�������
        short	_nFlag;

        int     _nAndroidChannelID;         //��׿ҵ����
        int     _nAndroidCommandID;         //��׿������

        char	_pReturnHead[4];			// Android���صİ�ͷ
        short	_nReturnFlag;				// Android���صı�־
        short	_nReturnLongConnectSign;	// Android���صĳ����ӱ�־
        int		_nReturnChannelID;			// Android���ص�ҵ����
        int		_nReturnCommonType;			// Android���ص�ҵ������
        int		_nReturnBodySize;			// Android���ص��������С

    };
}