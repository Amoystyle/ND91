#pragma once

namespace ND91Assistant
{
    typedef void (*PROGRESS_CALLBACK1)(void* pEvent);
    typedef void (*PROGRESS_CALLBACK2)(void* pSender, void* pEvent);

    class ProgressSubject;

	// IPA类型的APP操作的进度回调管理，提供二个线程并行安装的能力
	class IosIPAItunesManager
	{
	public:
        static IosIPAItunesManager* GetInstance();

        int  SetProgress(ProgressSubject* p);
        void ClearProgress(int progressId );

        PROGRESS_CALLBACK2 GetPackInstallCallBack(int progressId);
        PROGRESS_CALLBACK1 GetDataTransferCallBack(int progressId);
        PROGRESS_CALLBACK2 GetRemoveIPAArchiveCallBack(int progressId);
        PROGRESS_CALLBACK2 GetArchiveIPACallBack(int progressId);

        void PraseProgress(int progressId, void* pCFData);
	private:
        static IosIPAItunesManager* _pInstance;
        IosIPAItunesManager();
        ~IosIPAItunesManager();

        CRITICAL_SECTION _lock;
	};
}
