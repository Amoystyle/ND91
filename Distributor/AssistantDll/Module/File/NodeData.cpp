#include "stdafx.h"

#include "Module/File/NodeData.h"
#include "Module/File/DirData.h"

#include "Module/AndroidDefine.h"

NodeData::NodeData(NODE_TYPE t)
: _type(t)
{
	_name		= L"";              // ����

	_mode		= "";             // Ȩ��

	_owner		= "";             // ӵ����

	_group		= "";             // ��

	_size		= 0;				// ��С

	_modifyTime	= 0;        // �޸�ʱ��

	_createTime	= 0;		// ����ʱ��

	_Parent		= NULL;	
}


NodeData::~NodeData(void)
{
}

std::wstring ND91Assistant::NodeData::GetParentPath() const
{
    if (_Parent)
        return _Parent->GetFullPath();

    if ( ! _name.empty() )
    {
        int offset = _name.rfind('/');
        if ( offset != wstring::npos )
        {
            return _name.substr(0, offset);
        }
    }

    return L"/";
}

bool ND91Assistant::NodeData::IsUserSpace()
{
    return CStrOperation::startsWith(GetFullPath(), L"/private/var");
}