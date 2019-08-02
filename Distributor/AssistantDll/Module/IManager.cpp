#include "pch_module.h"

#include "Module/IManager.h"

IManager::IManager(const DeviceData* pDevice)
{
    _pDevice = pDevice;
    _bLoad = false;
}

IManager::~IManager(void)
{
}

CommandExecuteResult ND91Assistant::IManager::Delete(const vector<BaseData*>* pDatas, const vector<BaseData*>* pDataList)
{
    for (vector<BaseData*>::const_iterator it = pDataList->begin(); it != pDataList->end(); it++)
    {
        Delete(pDatas, *it);
    }
    return COMMAND_EXECUTE_SUCCESS;
}

CommandExecuteResult ND91Assistant::IManager::Delete(const vector<BaseData*>* pDatas, BaseData* pData)
{
    if (find(pDatas->begin(), pDatas->end(), pData) == pDatas->end())
        return COMMAND_EXECUTE_NOFOUND;

    pData->SetStatus(BaseData::DEL);
    return COMMAND_EXECUTE_SUCCESS;
}

//获取需要更新数据的容器
//参数：添加、删除、更新数据容器的指针
void ND91Assistant::IManager::GetUpdateDataList(vector<BaseData*>* pDatas, 
                                                vector<const BaseData*>* pAddDataList, 
                                                vector<const BaseData*>* pDeleteDataList, 
                                                vector<const BaseData*>* pUpdateDataList)
{
    vector<BaseData*>::iterator it;
    for( it = pDatas->begin(); it != pDatas->end(); ++it)
    {
        BaseData* p = *it;
        if (p->GetStatus() == BaseData::NEW)
        {
            (*pAddDataList).push_back(p);
        }
        else if (p->GetStatus() == BaseData::DEL)
        {
            (*pDeleteDataList).push_back(p);	
        }
        else if (p->GetStatus() == BaseData::UPDATE)
        {
            (*pUpdateDataList).push_back(p);
        }
    }
}

//重设_datas的状态
//参数：添加、删除、更新操作是否成功的bool值
void ND91Assistant::IManager::ResetDataStatus(vector<BaseData*>* pDatas, bool bAdd, bool bDelete, bool bUpdate)
{
    if (bAdd || bDelete || bUpdate)
    {
        std::vector<BaseData*>::iterator it = pDatas->begin();        
        while (it != pDatas->end())
        {
            BaseData* p = *it;
            if (p->GetStatus() == BaseData::NEW && bAdd)
            {
                p->SetStatus(BaseData::NO_CHANGE);
                ++it;
            }
            else if (p->GetStatus() == BaseData::DEL && bDelete)
            {
                SAFE_DELETE(p);
                it = pDatas->erase(it);
            }
            else if (p->GetStatus() == BaseData::UPDATE && bUpdate)
            {
                p->SetStatus(BaseData::NO_CHANGE);
                ++it;
            }
            else
                ++it;
        }
    }
}