
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
    wstring strInstallDir    = ANDROID_APP_INSTALL_DIR;   //�ػ��ļ�Ҫ�ϴ����ֻ��е�Ŀ¼
    wstring strInstallPath   = ANDROID_APP_INSTALL_PATH;  //�ػ��ϴ��������·��
    string sn = _sn;

//1�ϴ��ļ�
    //��Ȩ
    AdbSetchmod setchmodCmd(sn, strInstallDir);
    setchmodCmd.Execute();
    if (!setchmodCmd.Success())
        return false;

    //�����ļ���
    AdbCreateDir createDirCmd(sn, strInstallDir);
    createDirCmd.Execute();   
    if (!createDirCmd.Success())
        return false;

    //����ļ����Ƿ����
    AdbCheckDir checkDirCmd(sn, strInstallDir);
    checkDirCmd.Execute();   
    if (!checkDirCmd.Success())
        return false;

    //�ϴ��ļ�
    AdbUploadFileCommand uploadCmd(sn, strPCPath, strInstallPath);
    uploadCmd.Execute();
    if (!uploadCmd.Success())
        return false;

//2��װ
    //��Ȩ
    AdbSetchmod setchmodCmd2(sn, strInstallPath);
    setchmodCmd2.Execute();
    if (!setchmodCmd2.Success())
        return false;

    if (strPackageName.length() > 0)
    {
        //ж��ԭ����
        AdbUninstallAppCommand uninstallCmd(sn, strPackageName, false);
        uninstallCmd.Execute();
    }

    //��װ����
    AdbInstallAppCommand installCmd(sn, strInstallPath, false, true, false);
    installCmd.Execute();
    if (!installCmd.Success())
        return false;
//3ɾ��
    //ɾ���ļ�
    AdbDeleteFile deleteCmd(sn, strInstallPath);
    deleteCmd.Execute();

    return true;
}

bool ND91Assistant::AdbInstallCommand::InstallNpk( wstring strPCPath )
{
    wstring strXMLPath; //NPK��XML·��
	wstring strNPKPath;	//NPK������Ŀ¼

    wstring strTempPath;
    OPTION->Get(GlobalOption::STR_ANDROID_NPK_TEMP_PATH, strTempPath);
    //��ѹNPK
    strTempPath += CFileOperation::GetFileNameNoExt(strPCPath);    
    strTempPath += L"\\";
    strXMLPath = strTempPath + L"npk.xml";
	CFileOperation::UnZipFile(strPCPath, strTempPath);

    //����xml
    string str = CFileOperation::ReadFile(strXMLPath, false);
	NpkData npkData(str);
	for (vector<NPK_ITEM*>::iterator it = npkData._items.begin(); it != npkData._items.end(); it++)
	{
		NPK_ITEM* pItem = (NPK_ITEM*)(*it);
		//��װAPK
		strNPKPath = strTempPath + CCodeOperation::GB2312ToUnicode(pItem->Name) + L"\\";
		if (!InstallApk(strNPKPath + CCodeOperation::GB2312ToUnicode(pItem->apkFileName)))
			break;
		for (vector<NPK_CopyData*>::iterator it2 = pItem->datas.begin(); it2 != pItem->datas.end(); it2++)
		{
			NPK_CopyData* pData = (NPK_CopyData*)(*it2);
			//����Data    

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