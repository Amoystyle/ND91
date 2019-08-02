
#include "AdbInstallCommand.h"
#include "Common/CodeOperation.h"
#include "Common/FileOperation.h"
#include "Common/unzip.h"

#include "Device/AdbGetPropertyCommand.h"
#include "Device/AdbUploadFileCommand.h"
#include "Device/AdbInstallAppCommand.h"
#include "Device/AdbUninstallAppCommand.h"
#include "Device/AdbPSCommand.h"
#include "Device/AdbSetchmod.h"
#include "Device/AdbCreateDir.h"
#include "Device/AdbDeleteFile.h"
#include "Device/AdbUploadDirCommand.h"

#include "Core/GlobalOption.h"

#include <iostream>
#include "common/rapidxml/rapidxml.hpp"

#include "data/NpkData.h"


using namespace ND91Assistant;
using namespace rapidxml;

AdbInstallCommand::AdbInstallCommand(string sn)
:_sn(sn)
{
}

AdbInstallCommand::~AdbInstallCommand()
{
}

bool ND91Assistant::AdbInstallCommand::InstallApk( wstring strPCPath, wstring strPackageName)
{
    wstring strInstallDir    = ANDROID_APP_INSTALL_DIR;   //守护文件要上传到手机中的目录
    wstring strInstallPath   = ANDROID_APP_INSTALL_PATH;  //守护上传后的完整路径
    string sn = _sn;

//1上传文件
    //提权
    AdbSetchmod setchmodCmd(sn, strInstallDir);
    setchmodCmd.Execute();
    if (!setchmodCmd.Success())
        return false;

    //创建文件夹
    AdbCreateDir createDirCmd(sn, strInstallDir);
    createDirCmd.Execute();   
    if (!createDirCmd.Success())
        return false;

    //检查文件夹是否存在
    AdbCheckDir checkDirCmd(sn, strInstallDir);
    checkDirCmd.Execute();   
    if (!checkDirCmd.Success())
        return false;

    //上传文件
    AdbUploadFileCommand uploadCmd(sn, strPCPath, strInstallPath);
    uploadCmd.Execute();
    if (!uploadCmd.Success())
        return false;

//2安装
    //提权
    AdbSetchmod setchmodCmd2(sn, strInstallPath);
    setchmodCmd2.Execute();
    if (!setchmodCmd2.Success())
        return false;

    if (strPackageName.length() > 0)
    {
        //卸载原程序
        AdbUninstallAppCommand uninstallCmd(sn, strPackageName, false);
        uninstallCmd.Execute();
    }

    //安装程序
    AdbInstallAppCommand installCmd(sn, strInstallPath, false, true, false);
    installCmd.Execute();
    if (!installCmd.Success())
        return false;
//3删除
    //删除文件
    AdbDeleteFile deleteCmd(sn, strInstallPath);
    deleteCmd.Execute();

    return true;
}

bool ND91Assistant::AdbInstallCommand::InstallNpk( wstring strPCPath )
{
    wstring strXMLPath; //NPK的XML路径
	wstring strNPKPath;	//NPK的数据目录

    wstring strTempPath;
    OPTION->Get(GlobalOption::STR_ANDROID_NPK_TEMP_PATH, strTempPath);
    //解压NPK
    strTempPath += CFileOperation::GetFileNameNoExt(strPCPath);    
    strTempPath += L"\\";
    strXMLPath = strTempPath + L"npk.xml";
	CFileOperation::UnZipFile(strPCPath, strTempPath);

    //载入xml
    string str = CFileOperation::ReadFile(strXMLPath, false);
	NpkData npkData(str);
	for (vector<NPK_ITEM*>::iterator it = npkData._items.begin(); it != npkData._items.end(); it++)
	{
		NPK_ITEM* pItem = (NPK_ITEM*)(*it);
		//安装APK
		strNPKPath = strTempPath + CCodeOperation::GB2312ToUnicode(pItem->Name) + L"\\";
		if (!InstallApk(strNPKPath + CCodeOperation::GB2312ToUnicode(pItem->apkFileName)))
			break;
		for (vector<NPK_CopyData*>::iterator it2 = pItem->datas.begin(); it2 != pItem->datas.end(); it2++)
		{
			NPK_CopyData* pData = (NPK_CopyData*)(*it2);
			//拷贝Data    

			AdbUploadDirCommand uploadDir(_sn, 
				strNPKPath + CCodeOperation::GB2312ToUnicode(pData->localName) + L"\\", 
				CCodeOperation::GB2312ToUnicode(pData->remotePath) + L"/");
			uploadDir.Execute();
			if (!uploadDir.Success())
				break;
		}
	}
    return true;
}

bool ND91Assistant::AdbInstallCommand::UnInstallApp( wstring strPackageName, bool bKeepData)
{
    AdbUninstallAppCommand uninstallCommand(_sn, strPackageName, bKeepData);
    uninstallCommand.Execute();
    return uninstallCommand.Success();
}