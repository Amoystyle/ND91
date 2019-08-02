///////////////////////////////////////////////////////////
//  ChannelFacade.h
//  Implementation of the Class ChannelFacade
//  Created on:      01-六月-2011 9:42:51
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_3A044235_2A43_4f37_BABA_04CEC15FE1B2__INCLUDED_)
#define EA_3A044235_2A43_4f37_BABA_04CEC15FE1B2__INCLUDED_

#include "IosFileIO.h"
#include "AndroidTcpIO.h"

namespace ND91Assistant
{
    // IO类调用入口
    class IOFacade
    {

    public:
        IOFacade();
        virtual ~IOFacade();

        // 命令分发：对每个命令建立一个IO类来处理Command
        // 不在DeviceData中建立一个IO类指针，是因为命令可能多发
        // 在这里对每个命令都新建一个IO类，每个IO类处理一个连接
        void Execute(BaseCommand* pCommand);

    };
}

#endif // !defined(EA_3A044235_2A43_4f37_BABA_04CEC15FE1B2__INCLUDED_)
