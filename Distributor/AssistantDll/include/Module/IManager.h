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

        // ɾ��
        virtual CommandExecuteResult Delete(const vector<BaseData*>* pDatas, const vector<BaseData*>* pDataList);
        virtual CommandExecuteResult Delete(const vector<BaseData*>* pDatas, BaseData* pData);

    protected:
        //��ȡ��Ҫ�������ݵ�����
        //��������ӡ�ɾ������������������ָ��
        void GetUpdateDataList(vector<BaseData*>* pDatas, 
            vector<const BaseData*>* pAddDataList, 
            vector<const BaseData*>* pDeleteDataList, 
            vector<const BaseData*>* pUpdateDataList);

        //����_datas��״̬
        //��������ӡ�ɾ�������²����Ƿ�ɹ���boolֵ
        void ResetDataStatus(vector<BaseData*>* pDatas, bool bAdd, bool bDelete, bool bUpdate);
    protected:
        const DeviceData*	_pDevice;
        bool _bLoad;
    };
}