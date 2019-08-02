#pragma once

#include "BaseData.h"
#include "Core/DeviceData.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL IManager
    {
    public:
        IManager(const DeviceData* pDevice);
        virtual ~IManager();

        // 删除
        virtual CommandExecuteResult Delete(const vector<BaseData*>* pDatas, const vector<BaseData*>* pDataList);
        virtual CommandExecuteResult Delete(const vector<BaseData*>* pDatas, BaseData* pData);

    protected:
        //获取需要更新数据的容器
        //参数：添加、删除、更新数据容器的指针
        void GetUpdateDataList(vector<BaseData*>* pDatas, 
            vector<const BaseData*>* pAddDataList, 
            vector<const BaseData*>* pDeleteDataList, 
            vector<const BaseData*>* pUpdateDataList);

        //重设_datas的状态
        //参数：添加、删除、更新操作是否成功的bool值
        void ResetDataStatus(vector<BaseData*>* pDatas, bool bAdd, bool bDelete, bool bUpdate);
    protected:
        const DeviceData*	_pDevice;
        bool _bLoad;
    };
}