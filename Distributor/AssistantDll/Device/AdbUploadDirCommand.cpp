#include "stdafx.h"

#include "AdbUploadDirCommand.h"
#include "AdbUploadFileCommand.h"

AdbUploadDirCommand::AdbUploadDirCommand(string sn,wstring id, wstring strPCPath, wstring strPhonePath)
{
	_sn = sn;
    _success = false;
    _strPCPath = strPCPath;
    _strPhonePath = strPhonePath;
	_id=id;
}

AdbUploadDirCommand::~AdbUploadDirCommand()
{
}

void ND91Assistant::AdbUploadDirCommand::Execute()
{	
#ifdef WIN32	
    _success = true;
    wstring strFind = CStrOperation::trimRightW(_strPCPath, L"\\") + L"\\*";
    WIN32_FIND_DATA fd;
    HANDLE hFindFile = FindFirstFile(strFind.c_str(), &fd);
    if (INVALID_HANDLE_VALUE != hFindFile)
    {
        bool bFinish = false;
        while (!bFinish && _success)
        {
            bool bDir = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

            if (_tcscmp(fd.cFileName, _T("."))==0 || _tcscmp(fd.cFileName, _T(".."))==0)
            {
                NULL;
            }
            else if (bDir)
            {
                AdbUploadDirCommand uploadDir(_sn,_id, _strPCPath + fd.cFileName + L"\\", _strPhonePath + fd.cFileName + L"/");
                uploadDir.Execute();
                _success = uploadDir.Success();
            }
            else
            {
				AdbUploadFileCommand uploadFile(CCodeOperation::UTF_8ToUnicode(_sn),_id, _strPCPath + fd.cFileName, _strPhonePath + fd.cFileName);
                uploadFile.Execute();
                _success = uploadFile.Success();
            }
            bFinish  = (FindNextFile(hFindFile, &fd) == false);
        }
    }
    FindClose(hFindFile);
#endif   
}