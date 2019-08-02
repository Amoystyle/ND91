#include "stdafx.h"

#include "Module/File/LinkData.h"

#include "DownloadFileCommand.h"
#include "CopyFileCommand.h"
#include "DirectoryOperationCommand.h"
#include "ListDirectoryCommand.h"

LinkData::LinkData() : NodeData(NodeData::LINK)
{
	_targetNode = TARGET_INIT_VALUE;
	_linkTarget = L"";
}

LinkData::~LinkData()
{

}

bool LinkData::IsDir()
{
    NodeData* pNode = GetTarget();
    if ( pNode)
        return pNode->IsDir();
    else
        return false;
}

bool LinkData::Rename( wstring dest )
{
	return false; 
}

bool LinkData::ChangeMode( string mode )
{
	return true;
}

bool LinkData::GetMode(string& mode, wstring& owner)
{
    return true;
}

bool LinkData::Refresh()
{
    _targetNode = TARGET_INIT_VALUE;

	ListDirectoryCommand lsCmd(_Parent->_pDeviceData, this);
	lsCmd.Execute();
    return lsCmd.DoneSuccess();
}

NodeData* LinkData::Clone( DirData* pParent ) const
{
    LinkData * pNew = new LinkData();

    pNew->_name       = this->_name      ;
    pNew->_mode       = this->_mode      ;
    pNew->_owner      = this->_owner     ;
    pNew->_group      = this->_group     ;
    pNew->_size       = this->_size      ;
    pNew->_modifyTime = this->_modifyTime;
    pNew->_createTime = this->_createTime;
    pNew->_Parent     = pParent;
    pNew->_linkTarget = this->_linkTarget;
    pNew->_targetNode = this->_targetNode;

    return pNew;
}

std::wstring LinkData::GetFullPath() const
{
	if ( ! _Parent )
		THROW_ASSISTANT_EXCEPTION(PROGRAM_ERROR);

	return _Parent->GetFullPath() + _name;
}

bool LinkData::DoDelete(DirectoryOperationCommand* pCmd,ProgressObserver* pObserver)
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

TransferResult LinkData::DoDownload( wstring dest, DownloadFileCommand* pCmd )
{
    pCmd->IncreaseTaskNumberComplete();
    pCmd->SendProgress();

    MyFileOperation::MyCloseFile(MyFileOperation::MyCreateFile(dest));

    return Transfer_Success;
}

NodeData* LinkData::GetTarget()
{
    if ( TARGET_INIT_VALUE == _targetNode )
        _targetNode = FindTarget();

    return _targetNode;
}

NodeData* LinkData::FindTarget()
{
    vector<wstring> names = CStrOperation::parseStringsW(_linkTarget, L"/");
    RETURN_NULL_IF(names.empty());

    // 从哪里找起：如果是/开头就从根目录找起，否则从当前目录找起
    DirData*  currentDir = _Parent;

    if ( _linkTarget[0] == '/' )
    {
        while( currentDir->GetParent() )
            currentDir = currentDir->GetParent();
    }

    for(size_t i = 0; i < names.size()-1; i++)
    {
        if ( names[i] == L".." )
        {
            currentDir = currentDir->GetParent();
            continue;
        }

        if ( names[i] == L"." )
        {
            continue;
        }

        NodeData* pNode = currentDir->findChildrenbyName(names[i]);
        RETURN_NULL_IF( ! pNode );

        // 应该是目录，或指向目录的链接
        if ( pNode->_type == DIR )
        {
            currentDir = dynamic_cast<DirData*>(pNode);
            continue;
        }
        else if ( pNode->_type == LINK )
        {
            // 如果是链接，一直找到不是链接为止
            while ( pNode->_type == LINK )
            {
                pNode = dynamic_cast<LinkData*>(pNode)->GetTarget();
                RETURN_NULL_IF( ! pNode );
            }

            // 最后出来的必须是目录
            RETURN_NULL_IF( pNode->_type != DIR ); // 必须是目录
            currentDir = dynamic_cast<DirData*>(pNode);
        }
        else
        {
            return NULL;    // 其他类型都是错
        }

    }

    // 最后一层，直接找名字了
    return currentDir->findChildrenbyName(names[names.size()-1]);
}