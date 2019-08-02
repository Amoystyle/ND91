
#include "IosWifiIO.h"
#include "Core/GlobalOption.h"
#include "Common/FileOperation.h"
#include "IosWiFiFileSystem.h"
#include "Common/CodeOperation.h"

using namespace ND91Assistant;

IosWifiIO::IosWifiIO(int nHandle)
{
	_nWiFiHandle = nHandle;
}

std::wstring IosWifiIO::SendIosFile(std::wstring filename, bool bReceiveResult)
{
	//��ɾ���豸�ϵĽ���ļ�
	IosWiFiFileSystem fileStream(_nWiFiHandle);
	if (fileStream.Ios_Exists(_resultFileOnPhone))
		fileStream.Ios_DeleteFile(_resultFileOnPhone);

	// copy�ļ����豸	
	fileStream.Ios_CopyToPhone(filename, _cmdFileOnPhone);

	//ɾ����ʱ�ļ�
	::DeleteFile(filename.c_str());

	if (!bReceiveResult)
		return L"";

	// ����ļ�����ʱĿ¼
	std::wstring folder;
	OPTION->Get(GlobalOption::STR_IOS_TMP_FILE_FOLDER, folder);

	// �ȴ����ؽ���ļ�
	bool blnCompleted = false;
	unsigned long timeout = 30000; //30��ȴ�ʱ��
	unsigned long lFirstTick = GetTickCount();
	unsigned long lNextTick = 0;	
	do 
	{
		if (fileStream.Ios_Exists(_resultFileOnPhone))
		{
			blnCompleted = true;

			// �ѽ���ļ�copy����ʱĿ¼			
			wstring fileTemp = folder + L"\\" + CFileOperation::GetFileName(_resultFileOnPhone);;
			if (fileStream.Ios_DownFromPhone(_resultFileOnPhone, fileTemp))
				return fileTemp;
		}

		lNextTick = GetTickCount();
	} while (!blnCompleted && (lNextTick-lFirstTick) < timeout);		

	return L"";
}
