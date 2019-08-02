///////////////////////////////////////////////////////////
//  ChannelFacade.h
//  Implementation of the Class ChannelFacade
//  Created on:      01-����-2011 9:42:51
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_3A044235_2A43_4f37_BABA_04CEC15FE1B2__INCLUDED_)
#define EA_3A044235_2A43_4f37_BABA_04CEC15FE1B2__INCLUDED_

#include "IosFileIO.h"
#include "AndroidTcpIO.h"

namespace ND91Assistant
{
    // IO��������
    class IOFacade
    {

    public:
        IOFacade();
        virtual ~IOFacade();

        // ����ַ�����ÿ�������һ��IO��������Command
        // ����DeviceData�н���һ��IO��ָ�룬����Ϊ������ܶ෢
        // �������ÿ������½�һ��IO�࣬ÿ��IO�ദ��һ������
        void Execute(BaseCommand* pCommand);

    };
}

#endif // !defined(EA_3A044235_2A43_4f37_BABA_04CEC15FE1B2__INCLUDED_)
