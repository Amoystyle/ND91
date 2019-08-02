#include "stdafx.h"

#include "Module/File/FileData.h"
#include "DirectoryOperationCommand.h"
#include "ListDirectoryCommand.h"
#include "DownloadFileCommand.h"

FileData::FileData()
: NodeData(NodeData::FILE)
{

}

FileData::~FileData()
{

}

bool ND91Assistant::FileData::Rename( wstring dest )
{
	DirectoryOperationCommand cmd(_Parent->_pDeviceData);
	RETURN_FALSE_IF ( ! cmd.Rename(GetFullPath(), _Parent->GetFullPath() + dest) );

    this->_name = dest;
    return true;
}

bool ND91Assistant::FileData::ChangeMode(string mode)
{
	DirectoryOperationCommand cmd(_Parent->_pDeviceData);
	RETURN_FALSE_IF ( ! cmd.ChangeMode(GetFullPath(), mode) );

    this->_mode = mode;
    return true;
}

bool ND91Assistant::FileData::GetMode(string& mode, wstring& owner)
{
    DirectoryOperationCommand cmd(_Parent->_pDeviceData);
    RETURN_FALSE_IF ( ! cmd.GetMode(GetFullPath(), mode, owner) );
    return true;
}

bool ND91Assistant::FileData::Refresh()
{
	ListDirectoryCommand lsCmd(_Parent->_pDeviceData, this);
	lsCmd.Execute();
    return lsCmd.DoneSuccess();
}

std::wstring ND91Assistant::FileData::GetFullPath() const
{
    if ( ! _Parent )
        THROW_ASSISTANT_EXCEPTION(PROGRAM_ERROR);

    return _Parent->GetFullPath() + _name;
}

NodeData* ND91Assistant::FileData::Clone( DirData* pParent ) const
{
    FileData * pNewFile = new FileData();

    pNewFile->_name       = this->_name      ;      
    pNewFile->_mode       = this->_mode      ;      
    pNewFile->_owner      = this->_owner     ;     
    pNewFile->_group      = this->_group     ;     
    pNewFile->_size       = this->_size      ;		
    pNewFile->_modifyTime = this->_modifyTime;
    pNewFile->_createTime = this->_createTime;
    pNewFile->_Parent     = pParent;

    return pNewFile;
}

bool ND91Assistant::FileData::DoDelete(DirectoryOperationCommand* pCmd,ProgressObserver* pObserver)
{
    RETURN_FALSE_IF ( !_Parent );

    RETURN_FALSE_IF ( ! pCmd->DeleteFileLink(GetFullPath()) );

    pCmd->SetCurrentTaskName(GetFullPath());
    pCmd->SetCurrentTaskSum(0);

    _Parent->RemoveFromChildrens(this);

    pCmd->IncreaseTaskNumberComplete();
    pCmd->SendProgress();

    return true;
}

TransferResult FileData::DoDownload(wstring destFile, DownloadFileCommand* pCmd)
{
    RETURN_VALUE_IF(!pCmd, Transfer_Unknown);

    pCmd->SetFileNames(destFile, GetFullPath());

    pCmd->Execute();
    
	if (!pCmd->Success())
		return Transfer_Cancel;

	LOG->WriteDebug(_T("ÎÄ¼þÏÂÔØ ²âÊÔ"));

    pCmd->IncreaseTaskNumberComplete();
    pCmd->SendProgress();

    _fileToPC = pCmd->GetFileToPC();

    return pCmd->getResult();
}
