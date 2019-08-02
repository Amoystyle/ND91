#pragma once

#include "unzip.h"

namespace ND91Assistant
{
	// IPA����ص�һЩ����,���������һЩ�ļ���Ϣ��ȡ,��ѹ��
	class IPAPackageHelper
	{
	public:
		IPAPackageHelper(const DeviceInfo* pDevPtr, AFCFileSysemBase* fileStream, wstring ipaFolderOnPc);
		~IPAPackageHelper();

        // ��ȡIPA����ѹ��Ĵ�С
        __int64 GetPackageUncompressedSize();

        // ����IPA��������ļ����豸,���ڿ��ٰ�װ(�߽�ѹ�ߴ���)
        bool TransferIPAPayload(wstring& ipaFolder, ProgressSubject* pProgressSubject);

        // �Ƿ��Ǻ��д浵��IPA
        static bool IsArchivedIPA(wstring ipaFolderOnPc);

        // �������Ƿ����������
        static bool CheckPackageCopyRight(wstring ipaFolderOnPc);

        // ��װ���֮����IPA����ı����ļ����豸
        void TransferIPAArchive(ProgressSubject* pProgressSubject);

    private:
        // �̻߳ص����� 
        static DWORD WINAPI TransferFilePayloadRun(LPVOID lpParameter);
        static DWORD WINAPI TransferFileContainerRun(LPVOID lpParameter);

        // �����ļ�����isTransferPayloadΪtrue,�����������ļ���Ϊfalse, ���ʹ浵�ļ�
        bool TransferFile(bool isTransferPayload);
        int do_extract(unzFile uf,bool isTransferPayload);
        int do_extract_currentfile(unzFile uf,bool isTransferPayload);

        // �ļ�������
        AFCFileSysemBase* _fileStream;

        // �豸��Ϣ
        const DeviceInfo* _pDevInfoPtr;

        // �豸��IPA�ļ�Ŀ¼
        wstring _ipaFolderOnIos;

        // pc��IPA�ļ�Ŀ¼
        wstring _ipaFolderOnPc;

        // �ļ���ѹ�����ص���ʱĿ¼
        wstring _strFolderTemp;

        // �����ļ�����
        ProgressSubject* _pProgressSubject;

        // �����ļ����
        bool _result;

        //unzFile _uf;
        //CRITICAL_SECTION _cs;//�����ٽ�ֵ
	};
}
