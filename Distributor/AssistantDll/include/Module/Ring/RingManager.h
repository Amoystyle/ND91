#pragma  once

#include "Module/IManager.h"
#include "Module/Ring/RingData.h"
#include "Module/Ring/RingDirData.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL RingManager : IManager
    {
    public:
        RingManager(const DeviceData* pDevice);
        virtual ~RingManager();

        // ��������
        CommandExecuteResult Load();

        int GetRingCount();

        void Release();

        // ȡ������
        const vector<RingDirData*>* GetDatas();

        CommandExecuteResult SetRing(int nID, RingData::MediaFileKind mediaType = RingData::Ring);

    private:

#pragma warning (disable:4251)

        vector<RingDirData*> _datas;

#pragma warning (default:4251)

    };
}