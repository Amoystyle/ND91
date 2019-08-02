#include "stdafx.h"
#include "SearchFileThread.h"
#include "../AblFileManager.h"
#include <stack>
#include "LOGX.h"
#include "regexpr2.h"

using namespace std;
using namespace regex;

#define WaitForEvent_Search	{\
	DWORD res = WaitForMultipleObjects(3, _events, FALSE, INFINITE);\
	if(res == WAIT_OBJECT_0 + 1 || res == WAIT_OBJECT_0 + 2)\
	{\
		LOGX::Write(L"开始退出", CCodeOperation::StringToWstring(_id));\
		return 0;\
	}\
}

SearchFileThread::SearchFileThread( string threadID, int nRequestID, wstring deviceID )
: BaseAblThread(threadID, nRequestID, deviceID)
{
	_requestSearchFileMsg = NULL;
	_waitEvent = NULL;
	_waitEvent = CreateEvent(NULL, TRUE, TRUE, NULL);  
	_events[0] = _waitEvent;
	_events[1] = __super::_hExitEvent;
	_events[2] = __super::_hSystemExitEvent;
}

SearchFileThread::~SearchFileThread()
{
	SAFE_DELETE(_requestSearchFileMsg);
	if(_waitEvent)
		CloseHandle(_waitEvent);
}

inline bool isMatch(tstring src, tstring key)
{
	match_results results;
	//REGEX_FLAGS dw = GLOBAL | ALLBACKREFS;
	REGEX_FLAGS dw = NOFLAGS;
	
	match_results::backref_type br ;
	rpattern url_pat(key, dw);
	br = url_pat.match(src, results);
	LOGX::Write(L"match ", br.matched ? L"1_":L"0_", src);
	return br.matched;
}

void SearchFileThread::Pause()
{
	ResetEvent(_waitEvent) ;
	LOGX::Write(L"请求暂停", CCodeOperation::StringToWstring(_id));
}

void SearchFileThread::Resume()
{
	SetEvent(_waitEvent);
	LOGX::Write(L"请求还原", CCodeOperation::StringToWstring(_id));
}
 

inline tstring PathSeparator() 
{
	return L"/"; //android, ios 都是这个分隔符
}

inline tstring ConvertiPath(tstring path)
{
	return CStrOperation::ReplaceW(CStrOperation::ReplaceW(path, L"\\\\", L"/"), L"\\", L"/");
}

inline tstring Combine(tstring path1, tstring path2)
{
	path1 = ConvertiPath(path1);
	path2 = ConvertiPath(path2);
	path1 = CStrOperation::trimRightW(path1, PathSeparator());
	path2 = CStrOperation::trimLeftW(path2, PathSeparator());
	return path1 + L"/" + path2;
}
inline FileItem_ABL NewItem(NodeData* node, tstring par_path)
{
	FileItem_ABL* item = new FileItem_ABL;
	item->nType = (eNodeInfoType)node->_type;
	item->strName = (node->_name);
	item->nSize = node->_size;
	item->modifyTime = node->_modifyTime;
	item->createTime = node->_createTime;
	item->permissions = (node->_mode);
	item->owner = (node->_owner);
	item->group = (node->_group);
	item->path = Combine(par_path, item->strName);
	item->user_data = 0;
	FileItem_ABL result = *item;
	delete(item);
	return result;
}

int SearchFileThread::SearchItem(NodeData* cur_node, FileItem_ABL cur_item)
{
	// TODO iTools 搜索的时候， 不停顿。 我们获取子目录很多的时候， 会停顿一下。 
	if(cur_node->_type == NodeData::LINK)
	{
		LinkData* pLink = dynamic_cast<LinkData*> ( cur_node );
		if ( pLink )
			cur_node = pLink->GetTarget();
	}
	if ( cur_node && cur_node->_type ==  NodeData::DIR)
	{
		DirData* dir = dynamic_cast<DirData*>(cur_node);
		if(dir)
		{
			FileItem_ABL sub_item;
			NodeData* sub_node;
			deque<NodeData*> sub_nodes = dir->GetChildrens();
			vector<FileItem_ABL> subitem_dir;
			// 先检查本级的子集再递归检查子集的子集
			for (deque<NodeData*>::iterator it = sub_nodes.begin(); it != sub_nodes.end(); it++)
			{
				WaitForEvent_Search;
				sub_node = *it;
				sub_item = NewItem(sub_node, cur_item.path);

				ResponseSearchFileMsg msg;
				msg.nRequestID = _nGUID;
				msg.item = sub_item;
				msg.isMatch = isMatch(sub_item.strName, _key);
				
				msg.SendToUI();

				WaitForEvent_Search;
				if(sub_node->_type !=  NodeData::FILE)
				{
					sub_item.user_data = (int)sub_node;
					subitem_dir.push_back(sub_item);
				}
			}
			// 递归检查所有子集
			for (vector<FileItem_ABL>::iterator it = subitem_dir.begin(); it != subitem_dir.end(); it++)
			{
				FileItem_ABL* sub_item = &(*it);
				NodeData* sub_node = (NodeData*)sub_item->user_data;
				SearchItem(sub_node, *sub_item);	
				sub_item->user_data = 0;
			}
		}
	}
	return 0;
}

bool SearchFileThread::Action(DeviceData* pDevice)
{
	AutoLock; //TODO 锁要改成全局的。  
    WAIT_DEVICE_ACTIVE_NORMAL;
 
	tstring path = _requestSearchFileMsg->root_path;
	_key = _requestSearchFileMsg->key ;
	_key = CStrOperation::ReplaceW(_key, L"^", L"\\^");
	_key = CStrOperation::ReplaceW(_key, L"$", L"\\$");
	_key = CStrOperation::ReplaceW(_key, L"[", L"\\[");
	_key = CStrOperation::ReplaceW(_key, L"]", L"\\]");
	_key = CStrOperation::ReplaceW(_key, L"{", L"\\{");
	_key = CStrOperation::ReplaceW(_key, L"}", L"\\}");
	_key = CStrOperation::ReplaceW(_key, L"(", L"\\(");
	_key = CStrOperation::ReplaceW(_key, L")", L"\\)");
	_key = CStrOperation::ReplaceW(_key, L".", L"\\.");
	_key = CStrOperation::ReplaceW(_key, L"+", L"\\+");
	if(_key.find('*') != tstring::npos)
	{
		if(!CStrOperation::startsWith(_key, L"*"))
		{
			// 如果输入 "a*" 那么搜索以a打头的
			_key = L"^" + _key;
		}
		else if(!CStrOperation::endsWithW(_key, L"*"))
		{
			// 如果输入 "a*" 那么搜索以a打头的
			_key =  _key + L"$";
		}
	}
	_key = CStrOperation::ReplaceW(_key, L"*", L".*");
	
	


	NodeData *cur_node;
	FileItem_ABL cur_item;
	cur_node = pDevice->CreateNoParentDirWithPath(path);

	WaitForEvent_Search;

	cur_node->Refresh();
	cur_item = NewItem(cur_node, L"");
	// 从子集开始搜索
	/*ResponseSearchFileMsg msg;
	msg.nRequestID = _nGUID;
	msg.item = cur_item;
	msg.isMatch = isMatch(cur_item.strName, _key);
	msg.SendToUI();*/

	SearchItem(cur_node, cur_item);

	// 搜索结束 
	ResponseSearchFileMsg msg;
	msg.nRequestID = _nGUID;
	msg.nParam = 0;
	msg.SendToUI();

    return true;
}

 