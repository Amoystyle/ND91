
#include "IosUsbIO.h"
#include "Core/GlobalOption.h"
#include "Common/FileOperation.h"
#include "AFCFileSysemBase.h"
#include "Common/CodeOperation.h"
// #include "IosDaemon.h"

using namespace ND91Assistant;

IosUsbIO::IosUsbIO(void* pAfcHandle, bool bJailBreaked)
{
	_pAfcHandle = pAfcHandle; //文件操作句柄
	_bJailBreaked = bJailBreaked; //是否越狱
}

std::wstring IosUsbIO::SendIosFile(std::wstring filename, bool bReceiveResult)
{
 	//先删除设备上的结果文件
 	AFCFileSysemBase fileStream(_pAfcHandle, _bJailBreaked);
 	if (fileStream.Ios_Exists(_resultFileOnPhone))
 		fileStream.Ios_DeleteFile(_resultFileOnPhone);
 
 	// copy文件到设备	
 	fileStream.Ios_CopyToPhone(filename, _cmdFileOnPhone);
 
 	//删除临时文件
 	::DeleteFile(filename.c_str());
 
 	if (!bReceiveResult)
 		return L"";
 
 	// 结果文件的临时目录
 	std::wstring folder;
 	OPTION->Get(GlobalOption::STR_IOS_TMP_FILE_FOLDER, folder);
 
 	// 等待返回结果文件
 	bool blnCompleted = false;
 	unsigned long timeout = 30000; //30秒等待时间
 	unsigned long lFirstTick = GetTickCount();
 	unsigned long lNextTick = 0;	
 	do 
 	{
 		if (fileStream.Ios_Exists(_resultFileOnPhone))
 		{
 			blnCompleted = true;
 
 			// 把结果文件copy到临时目录			
 			wstring fileTemp = folder + L"\\" + CFileOperation::GetFileName(_resultFileOnPhone);;
 			if (fileStream.Ios_DownFromPhone(_resultFileOnPhone, fileTemp))
 				return fileTemp;
 		}
 
 		lNextTick = GetTickCount();
 	} while (!blnCompleted && (lNextTick-lFirstTick) < timeout);		

	return L"";
}
