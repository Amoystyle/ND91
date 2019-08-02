#include "stdafx.h"

#include "Module/File/FileManager.h"
#include "UploadFileCommand.h"

#include "Core/DeviceData.h"

FileManager::FileManager( const DeviceData* pDevice )
: IManager (pDevice)
{

}

FileManager::~FileManager()
{
    
}

TransferResult ND91Assistant::FileManager::UpLoadFile(const wstring& pathOnPc, const wstring& remotePath, ProgressObserver* pObserver)
{
    UploadFileCommand  upfilecmd(_pDevice);
    upfilecmd.Attach(pObserver);

    upfilecmd.setfilename(pathOnPc, remotePath);
    upfilecmd.Execute();

    return upfilecmd.GetUploadResult();
}

CommandExecuteResult ND91Assistant::FileManager::Load()
{
    return COMMAND_EXECUTE_UNSUPPORT;
}