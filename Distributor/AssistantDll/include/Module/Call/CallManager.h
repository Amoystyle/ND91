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

        // ��������
        CommandExecuteResult Load(bool bReload = false, bool bOnlySize = false);

        // �ͷ�����
        void Release();

        // �������
        const vector<CallData*>* GetDatas();

        // �ύ�޸�
        CommandExecuteResult Update(ProgressObserver* pObserver = NULL);

        // ɾ����¼
        CommandExecuteResult Delete(int id);
        CommandExecuteResult Delete(const vector<const CallData*>* pDataList);

        // ɾ���ֻ��ϵ�ȫ����¼
        CommandExecuteResult DeleteAll();

        // ����ͨ����¼����
        bool ImportCallData( const vector<const CallData*>* pDataList,ProgressObserver* pObserver = NULL);

        int SuccessCount() { return _nSuccessCount; };

    private:

#pragma warning (disable:4251)

        vector<CallData*> _datas;

#pragma warning (default:4251)

        int _nSuccessCount;
    };
}