#pragma  once

#include "CallData.h"
#include "Module/IManager.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL CallManager : IManager
    {
    public:
        CallManager(const DeviceData* pDevice);
        virtual ~CallManager();

        // 加载数据
        CommandExecuteResult Load(bool bReload = false, bool bOnlySize = false);

        // 释放数据
        void Release();

        // 获得数据
        const vector<CallData*>* GetDatas();

        // 提交修改
        CommandExecuteResult Update(ProgressObserver* pObserver = NULL);

        // 删除记录
        CommandExecuteResult Delete(int id);
        CommandExecuteResult Delete(const vector<const CallData*>* pDataList);

        // 删除手机上的全部记录
        CommandExecuteResult DeleteAll();

        // 导入通话记录数据
        bool ImportCallData( const vector<const CallData*>* pDataList,ProgressObserver* pObserver = NULL);

        int SuccessCount() { return _nSuccessCount; };

    private:

#pragma warning (disable:4251)

        vector<CallData*> _datas;

#pragma warning (default:4251)

        int _nSuccessCount;
    };
}