#pragma once

#include "Common/BinaryBufferReader.h"
#include <string>
using namespace std;
namespace ND91Assistant
{
    class PandaThemeActionReturn
    {
    friend class AndroidMonitorThread;
    public:
        PandaThemeActionReturn();
        PandaThemeActionReturn(const PandaThemeActionReturn* pData);
        ~PandaThemeActionReturn();

        int _nAction;
        int _nResult;
        wstring _strValue;
    private:
        //从心跳线程通知来的
        static PandaThemeActionReturn* FromDeamonNotifyBuffer(BinaryBufferReader* pReader);
    };
}