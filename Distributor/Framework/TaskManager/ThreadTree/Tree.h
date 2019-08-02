/*author:hattricker 2008-12-30*/

#ifndef __TREE_H__
#define __TREE_H__

#if _MSC_VER > 1000


#pragma once

#endif //_MSC_VER > 1000

#define C_BOOL long
#define C_TRUE 1
#define C_FALSE 0

typedef struct
{
	void * customThreadInfo;		//线程相关信息,存储的是CUSTOMTHREADINFO结构体指针
}Data_t;

typedef struct tagTreeItem 
{
	Data_t data;
	int childCount;					//子节点统计
	struct tagTreeItem* firstchild;
	struct tagTreeItem* lastchild;
	struct tagTreeItem* nextsibling;
	struct tagTreeItem* previoussibling;
	struct tagTreeItem* parent;
}TreeItem_t;

typedef struct tagLinkListNode 
{
	Data_t data;
	struct tagLinkListNode* previous;
	struct tagLinkListNode* next;
	/*double unused[65535];*/
}LinkListNode_t;

enum
{
	TRAVERSE_PRE_ORDER,
	TRAVERSE_IN_ORDER,
	TRAVERSE_POST_ORDER,
};

#if defined (__cplusplus)
extern "C" {
#endif


typedef long (VISIT_FUNC)(TreeItem_t*);

TreeItem_t* CreateTree(Data_t* pItem);

TreeItem_t* AppendTreeItem(TreeItem_t* pTree,Data_t* pItem);
TreeItem_t* InsertTreeItem(TreeItem_t* pTree,Data_t* pItem,long nPos);

void DestroyTree(TreeItem_t* pTree,C_BOOL fDeleteChilds);

TreeItem_t* GetTreeItem(TreeItem_t* pTree,Data_t* pItem);
TreeItem_t* GetSubTreeItem(TreeItem_t* pTree,long nPos);
long GetTreeItemCount(TreeItem_t* pTree);
long GetSubTreeItemCount(TreeItem_t* pTree);
TreeItem_t* GetFirstTreeChild(TreeItem_t* pTree);
TreeItem_t* GetLastTreeChild(TreeItem_t* pTree);
TreeItem_t* GetNextTreeSibling(TreeItem_t* pTree);
TreeItem_t* GetPreTreeSibling(TreeItem_t* pTree);
TreeItem_t* GetTreeParent(TreeItem_t* pTree);
TreeItem_t* GetTreeRoot(TreeItem_t* pTree);

void FreeTreeItemList(LinkListNode_t* p);

C_BOOL GetTreeTraverseList(LinkListNode_t** ppFrontNode,LinkListNode_t** ppTailNode,TreeItem_t* pTree,long nMode);
C_BOOL TreeTraverse(TreeItem_t* pTree,VISIT_FUNC pFunc,long nSatisfied,C_BOOL fRetWhenSatisfied,long nMode);

#if defined (__cplusplus)
}
#endif


#endif /*_TREE_H_*/