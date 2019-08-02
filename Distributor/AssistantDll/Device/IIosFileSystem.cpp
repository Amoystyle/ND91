#include "stdafx.h"
#include "IIosFileSystem.h"

#include "Module/File/DirData.h"
#include "Common/Path.h"
#include "Common/plist/PlistOperation.h"
#include "Module/Application/AppData.h"

bool IIosFileSystem::SkipFile(wstring file)
{
    //wchar_t * pIgnore = L"thumbs.db";
    wchar_t pIgnore[] = L"thumbs.db";

    wstring str = CStrOperation::trimRightW(file, L" ");
    if ( str.length() < wcslen(pIgnore) )
        return false;


    return str.substr(str.length() - wcslen(pIgnore)) == pIgnore;
}

#define   IOSAPPLICATIONPATH        L"/private/var/mobile/Applications/"
void IIosFileSystem::GetDisplayName(NodeData* pNode)
{
    pNode->_displayName = pNode->_name;

    RETURN_IF(pNode->_type != NodeData::DIR
        || pNode->GetParentPath() != IOSAPPLICATIONPATH);

    DirData* pDir = (DirData*)pNode;
    deque<NodeData*> childs = pDir->GetChildrens();
    for (size_t i = 0; i < childs.size(); i++)
    {
        NodeData* pFile = childs[i];

        if (!CStrOperation::endsWithW(pFile->_name, L".app"))
            continue;
        
        wstring tempDir = Path::GlobalTemp()+CFileOperation::GetGuidW()+L"\\";
        wstring fileName = L"Info.plist";
        RETURN_IF(Ios_DownFromPhone(pFile->GetFullPath()+fileName, tempDir+fileName) != Transfer_Success);

        AppData* pAppData = PlistOperation::ParseIpaAppInfoPlist(tempDir+fileName);
        RETURN_IF(pAppData == NULL);

        pNode->_displayName = pAppData->_AppName;
        SAFE_DELETE(pAppData);
        CFileOperation::DeleteDirectory(tempDir);
    }
}