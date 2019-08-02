#pragma once

#include "unzip.h"

namespace ND91Assistant
{
	// IPA包相关的一些操作,比如包里面一些文件信息提取,解压等
	class IPAPackageHelper
	{
	public:
		IPAPackageHelper(const DeviceInfo* pDevPtr, AFCFileSysemBase* fileStream, wstring ipaFolderOnPc);
		~IPAPackageHelper();

        // 获取IPA包解压后的大小
        __int64 GetPackageUncompressedSize();

        // 传送IPA里面的主文件到设备,用于快速安装(边解压边传送)
        bool TransferIPAPayload(wstring& ipaFolder, ProgressSubject* pProgressSubject);

        // 是否是含有存档的IPA
        static bool IsArchivedIPA(wstring ipaFolderOnPc);

        // 检查软件是否是正版软件
        static bool CheckPackageCopyRight(wstring ipaFolderOnPc);

        // 安装完成之后传送IPA里面的备份文件到设备
        void TransferIPAArchive(ProgressSubject* pProgressSubject);

    private:
        // 线程回调函数 
        static DWORD WINAPI TransferFilePayloadRun(LPVOID lpParameter);
        static DWORD WINAPI TransferFileContainerRun(LPVOID lpParameter);

        // 传送文件，若isTransferPayload为true,则传送主程序文件，为false, 则传送存档文件
        bool TransferFile(bool isTransferPayload);
        int do_extract(unzFile uf,bool isTransferPayload);
        int do_extract_currentfile(unzFile uf,bool isTransferPayload);

        // 文件传送类
        AFCFileSysemBase* _fileStream;

        // 设备信息
        const DeviceInfo* _pDevInfoPtr;

        // 设备上IPA文件目录
        wstring _ipaFolderOnIos;

        // pc上IPA文件目录
        wstring _ipaFolderOnPc;

        // 文件解压到本地的临时目录
        wstring _strFolderTemp;

        // 传输文件进度
        ProgressSubject* _pProgressSubject;

        // 传输文件结果
        bool _result;

        //unzFile _uf;
        //CRITICAL_SECTION _cs;//访问临界值
	};
}
