#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosIPAItunesManager.h"
#include "Device/CFConverter.h"
#include "Monitor/ProgressSubject.h"

IosIPAItunesManager* IosIPAItunesManager::_pInstance = NULL;

// ���� N �� �ص�����
template<int N>
void MakePackInstallCallBack()
{
    g_InstallCallbacks[N-1]       = &OnPackInstallCallBack<N-1>;
    g_TransferCallbacks[N-1]      = &OnPackDataTransferCallBack<N-1>;
    g_RemoveArchiveCallbacks[N-1] = &OnRemoveIPAArchiveCallback<N-1>;
    g_ArchiveCallbacks[N-1]       = &OnArchiveIPACallback<N-1>;

    MakePackInstallCallBack<N-1>();
}

template<>
void MakePackInstallCallBack<0>()
{
}


/************************************************************************/
/*  ע��: ��6������Ҫһ��                                        */
/*  5 ������Ĵ�С�� MakePackInstallCallBack<> ��ģ�����������ͬ   */
/************************************************************************/
#define ARRAY_SIZE 20
PROGRESS_CALLBACK2  g_InstallCallbacks[ARRAY_SIZE];
PROGRESS_CALLBACK1  g_TransferCallbacks[ARRAY_SIZE];
PROGRESS_CALLBACK2  g_RemoveArchiveCallbacks[ARRAY_SIZE];
PROGRESS_CALLBACK2  g_ArchiveCallbacks[ARRAY_SIZE];
ProgressSubject*    g_Subjects[ARRAY_SIZE];

IosIPAItunesManager* ND91Assistant::IosIPAItunesManager::GetInstance()
{
    if ( _pInstance == NULL )
    {
        _pInstance = new IosIPAItunesManager();
        MakePackInstallCallBack<20>();
    }

    return _pInstance;
}


template<int i>
void OnPackInstallCallBack(void* p1, void* p2)  // ��װAPP���Ȼص�
{
    IosIPAItunesManager::GetInstance()->PraseProgress(i, p1);
}

template<int i>
void OnPackDataTransferCallBack(void* p1)   // ʹ��ITUNESĬ���ϴ�APP���Ľ��Ȼص�
{
    IosIPAItunesManager::GetInstance()->PraseProgress(i, p1);
}

template<int i>
void OnRemoveIPAArchiveCallback(void* p1, void* p2) // ɾ��APP���ݵĽ��Ȼص�
{
    IosIPAItunesManager::GetInstance()->PraseProgress(i, p1);
}

template<int i>
void OnArchiveIPACallback(void* p1, void* p2)   // ����APP�Ľ��Ȼص�
{
    IosIPAItunesManager::GetInstance()->PraseProgress(i, p1);
}

int ND91Assistant::IosIPAItunesManager::SetProgress( ProgressSubject* p )
{
    int ret = -1;

    if ( p )
    {
        EnterCriticalSection(&_lock);

        for(int i=0; i < ARRAY_SIZE; i++)
        {
            if ( NULL == g_Subjects[i] )
            {
                g_Subjects[i] = p;
                ret = i;    // ��ֱ�ӷ��أ���ΪҪִ��LeaveCriticalSection
                break;
            }
        }

        LeaveCriticalSection(&_lock);
    }

    return ret;
}

void IosIPAItunesManager::ClearProgress(int progressId )
{
    EnterCriticalSection(&_lock);
    RETURN_IF ( progressId < 0 || progressId >= ARRAY_SIZE );
    g_Subjects[progressId] = NULL;
    LeaveCriticalSection(&_lock);
}

ND91Assistant::IosIPAItunesManager::IosIPAItunesManager()
{
    InitializeCriticalSection(&_lock);
}

ND91Assistant::IosIPAItunesManager::~IosIPAItunesManager()
{
    DeleteCriticalSection(&_lock);
}


PROGRESS_CALLBACK2 IosIPAItunesManager::GetPackInstallCallBack( int progressId )
{
    RETURN_NULL_IF ( progressId < 0 || progressId >= ARRAY_SIZE );
    return g_InstallCallbacks[progressId];
}

PROGRESS_CALLBACK1 IosIPAItunesManager::GetDataTransferCallBack( int progressId )
{
    RETURN_NULL_IF ( progressId < 0 || progressId >= ARRAY_SIZE );
    return g_TransferCallbacks[progressId];
}

PROGRESS_CALLBACK2 IosIPAItunesManager::GetRemoveIPAArchiveCallBack( int progressId )
{
    RETURN_NULL_IF ( progressId < 0 || progressId >= ARRAY_SIZE );
    return g_RemoveArchiveCallbacks[progressId];
}

PROGRESS_CALLBACK2 IosIPAItunesManager::GetArchiveIPACallBack( int progressId )
{
    RETURN_NULL_IF ( progressId < 0 || progressId >= ARRAY_SIZE );
    return g_ArchiveCallbacks[progressId];
}

void IosIPAItunesManager::PraseProgress(int progressId, void* pCFData)
{
    RETURN_IF( ! pCFData );
    RETURN_IF ( progressId < 0 || progressId >= ARRAY_SIZE );
    ProgressSubject* pSubject =  g_Subjects[progressId];
    RETURN_IF( !pSubject);

    map<string, void*> mapMsg;
    if (CCFConverter::GetIosData(pCFData, mapMsg))
    {
        map<string, void*>::iterator find;

        //�ж��Ƿ������
        find = mapMsg.find("Error");
        if (find!=mapMsg.end())
        {
            //������
            pSubject->SendPrompt(MESSAGE_ERROR);
            return ;
        }
        else
        {
            //const ProgressData* pLast = IosIPAOperationWithItunes::GetInstance()->GetSubject().GetLastProgress();
            __int64 nValue = 0;

            //�ж��Ƿ����
            find = mapMsg.find("Status");
            string strStatus = "";
            if (find != mapMsg.end() && CCFConverter::GetIosData(find->second, strStatus) && (int)strStatus.find("Complete") >= 0)
            {				
                nValue = 100 - pSubject->GetLastProgress()->_currentTaskProgress;
            }
            else
            {
                //��������
                find = mapMsg.find("PercentComplete");

                int32_t nPerssent = 0;
                if (find!=mapMsg.end() && CCFConverter::GetIosData(find->second, nPerssent))
                {
                    nValue = nPerssent - pSubject->GetLastProgress()->_currentTaskProgress;
                }
            }			

            pSubject->AddTaskProgress(nValue>0?nValue:0);
            pSubject->SendProgress();
        }
    }
}

#endif