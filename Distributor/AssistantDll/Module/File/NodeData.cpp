#include "stdafx.h"

#include "Module/File/NodeData.h"
#include "Module/File/DirData.h"

#include "Module/AndroidDefine.h"

NodeData::NodeData(NODE_TYPE t)
: _type(t)
{
	_name		= L"";              // 名称

	_mode		= "";             // 权限

	_owner		= "";             // 拥有者

	_group		= "";             // 组

	_size		= 0;				// 大小

	_modifyTime	= 0;        // 修改时间

	_createTime	= 0;		// 创建时间

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