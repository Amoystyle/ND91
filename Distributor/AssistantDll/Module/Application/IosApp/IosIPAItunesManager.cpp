#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosIPAItunesManager.h"
#include "Device/CFConverter.h"
#include "Monitor/ProgressSubject.h"

IosIPAItunesManager* IosIPAItunesManager::_pInstance = NULL;

// 创建 N 个 回调函数
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
/*  注意: 有6个数字要一样                                        */
/*  5 个数组的大小和 MakePackInstallCallBack<> 的模版参数必须相同   */
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
void OnPackInstallCallBack(void* p1, void* p2)  // 安装APP进度回调
{
    IosIPAItunesManager::GetInstance()->PraseProgress(i, p1);
}

template<int i>
void OnPackDataTransferCallBack(void* p1)   // 使用ITUNES默认上传APP包的进度回调
{
    IosIPAItunesManager::GetInstance()->PraseProgress(i, p1);
}

template<int i>
void OnRemoveIPAArchiveCallback(void* p1, void* p2) // 删除APP备份的进度回调
{
    IosIPAItunesManager::GetInstance()->PraseProgress(i, p1);
}

template<int i>
void OnArchiveIPACallback(void* p1, void* p2)   // 备份APP的进度回调
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
                ret = i;    // 不直接返回，因为要执行LeaveCriticalSection
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

        //判断是否出错了
        find = mapMsg.find("Error");
        if (find!=mapMsg.end())
        {
            //出错处理
            pSubject->SendPrompt(MESSAGE_ERROR);
            return ;
        }
        else
        {
            //const ProgressData* pLast = IosIPAOperationWithItunes::GetInstance()->GetSubject().GetLastProgress();
            __int64 nValue = 0;

            //判断是否完成
            find = mapMsg.find("Status");
            string strStatus = "";
            if (find != mapMsg.end() && CCFConverter::GetIosData(find->second, strStatus) && (int)strStatus.find("Complete") >= 0)
            {				
                nValue = 100 - pSubject->GetLastProgress()->_currentTaskProgress;
            }
            else
            {
                //反馈进度
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