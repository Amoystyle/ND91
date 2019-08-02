/*author:hattricker 2008-12-31*/
#include "Tree.h"
#include <stdlib.h>
#include <string.h>

/*to get rid of annoying compiler warnings*/
#pragma warning(disable:4133)
#pragma warning(disable:4550)

/*create a tree item that has no child,no parent*/
TreeItem_t* CreateTree(Data_t* pItem)
{
	TreeItem_t* pNew = NULL;
	if (!pItem)	return NULL;

	pNew = (TreeItem_t*)malloc(sizeof(TreeItem_t));
	if (!pNew)
	{
		return NULL;
	}

	memcpy(&pNew->data,pItem,sizeof(Data_t));
	pNew->firstchild = NULL;
	pNew->lastchild = NULL;
	pNew->nextsibling = NULL;
	pNew->previoussibling = NULL;
	pNew->parent = NULL;
	pNew->childCount = 0;

	return pNew;
}

/*append item after the tail of the tree's child list*/
TreeItem_t* AppendTreeItem(TreeItem_t* pTree,Data_t* pItem)
{
	return InsertTreeItem(pTree,pItem,0x7fffffff);
}

/*specifies a position,then insert a tree item before the position*/
TreeItem_t* InsertTreeItem(TreeItem_t* pTree,Data_t* pItem,long nPos)
{
	TreeItem_t* pChild = NULL;
	TreeItem_t* p = NULL;
	long i = 0;

	if (!pTree || !pItem)
	{
		return NULL;
	}

	//firstly create an tree item
	pChild = CreateTree(pItem);
	if (!pChild)
	{
		return NULL;
	}
	pChild->parent = pTree;

	//find the node which we insert item before it
	for (p = pTree->firstchild,i = 0; i < nPos && p; ++i,p = p->nextsibling);

	//insert
	if (!p)
	{
		if (pTree->lastchild)
		{
			pTree->lastchild->nextsibling = pChild;
		}
		else
		{
			pTree->firstchild = pChild;
		}
		pChild->previoussibling = pTree->lastchild;
		pTree->lastchild = pChild;
	}//end if
	else
	{
		if (p->previoussibling)
		{
			p->previoussibling->nextsibling = pChild;
		}
		else
		{
			pTree->firstchild = pChild;
		}
		pChild->previoussibling = p->previoussibling;
		pChild->nextsibling = p;
		p->previoussibling = pChild;
	}//end else

	pTree->childCount ++;			//子节点数量加1

	return pChild;
}

/*destroy the whole binary tree,not exposed,helper function of DestroyTree*/
void DestroyBiTree(TreeItem_t* pTree)
{
	if (!pTree) return;

	DestroyBiTree(pTree->firstchild);
	DestroyBiTree(pTree->nextsibling);

	//释放本节点资源
	if(pTree->parent)
	{
		pTree->parent->childCount -- ;		//释放完成后将父节点的子节点统计加1
	}

	free(pTree);
}

/*destroy tree,whether its descendants are destroy decided by the param fDeleteChilds or whether it has parent*/
void DestroyTree(TreeItem_t* pTree,C_BOOL fDeleteChilds)
{
	TreeItem_t* pRoot = NULL;
	TreeItem_t* p = NULL;

	if (!pTree)
	{
		return;
	}
	
	/*firstly we must isolate this branch from the whole tree*/
	if (pTree->previoussibling)
	{
		pTree->previoussibling->nextsibling = pTree->nextsibling;
	}
	else
	{
		if (pTree->parent)
		{
			pTree->parent->firstchild = pTree->nextsibling;
		}
	}

	if (pTree->nextsibling)
	{
		pTree->nextsibling->previoussibling = pTree->previoussibling;
	}
	else
	{
		if (pTree->parent)
		{
			pTree->parent->lastchild = pTree->previoussibling;
		}
	}//end else

	//reduce parent's children count
	if(pTree->parent)
	{
		pTree->parent->childCount -- ;		//释放完成后将父节点的子节点统计加1
	}

	/*now we begin to destroy this branch*/
	/*if the tree item has no parent,we won't have a node to merge its descendants,so we destroy th whole tree*/
	if (fDeleteChilds || !pTree->parent)
	{
		DestroyBiTree(pTree->firstchild);
	}
	/*we just erase the tree item,but merge its descendants to the root node*/
	else
	{
		pRoot = GetTreeRoot(pTree);

		//add root's children count
		pRoot->childCount += pTree->childCount;

		for (p = pTree->firstchild; p; p = p->nextsibling) p->parent = pRoot;
		if (pTree->firstchild)
		{
			if (!pRoot->firstchild)
			{
				pRoot->firstchild = pTree->firstchild;
				pRoot->lastchild = pTree->lastchild;
			}
			else
			{
				pRoot->lastchild->nextsibling = pTree->firstchild;
				pTree->firstchild->previoussibling = pRoot->lastchild;
				pRoot->lastchild = pTree->lastchild;
			}
		}
	}

	/*finally we free the space the tree item node occupies*/

	free(pTree);
}

/*Search function of binary tree,by content,not exposed,helper function of GetTreeItem*/
TreeItem_t* GetBiTreeItem(TreeItem_t* pTree,Data_t* pItem)
{
	TreeItem_t* retVal = NULL;

	if (!pItem || !pTree)
	{
		return NULL;
	}

	if (!memcmp(pItem,&pTree->data,sizeof(Data_t)))
	{
		return pTree;
	}

	if (retVal = GetBiTreeItem(pTree->firstchild,pItem))
	{
		return retVal;
	}

	return GetBiTreeItem(pTree->nextsibling,pItem);
}

/*Search function of ordinary tree,by content*/
TreeItem_t* GetTreeItem(TreeItem_t* pTree,Data_t* pItem)
{
	if (!pItem || !pTree)
	{
		return NULL;
	}
	if (!memcmp(pItem,&pTree->data,sizeof(Data_t)))
	{
		return pTree;
	}
	return GetBiTreeItem(pTree->firstchild,pItem);
}

/*Search function of ordinary tree,specifying position*/
TreeItem_t* GetSubTreeItem(TreeItem_t* pTree,long nPos)
{
	TreeItem_t* p = NULL;
	long i = 0;

	if (!pTree || nPos < 0)
	{
		return NULL;
	}

	for (i = 0,p = pTree->firstchild; i < nPos && p; ++i,p = p->nextsibling);
	return p;
}

/*get item count of an binary tree,not exposed,helper function of GetTreeItemCount*/
long GetBiTreeItemCount(TreeItem_t* pTree)
{
	if (!pTree)
	{
		return 0;
	}

	return GetBiTreeItemCount(pTree->firstchild) + GetBiTreeItemCount(pTree->nextsibling) + 1;
}

/*get item count of an ordinary tree*/
long GetTreeItemCount(TreeItem_t* pTree)
{
	if (!pTree)
	{
		return 0;
	}
	else
	{
		return GetBiTreeItemCount(pTree->firstchild) + 1;
	}
}

/*get childs count of an ordinary tree*/
long GetSubTreeItemCount(TreeItem_t* pTree)
{
	long count = 0;
	TreeItem_t* p = NULL;

	if (!pTree)
	{
		return 0;
	}

	for (p = pTree->firstchild; p; p = p->nextsibling,++count);

	return count;
}

/*get the first child of a tree item*/
TreeItem_t* GetFirstTreeChild(TreeItem_t* pTree)
{
	if (pTree)
	{
		return pTree->firstchild;
	}
	else
	{
		return NULL;
	}
}

/*get the last child of a tree item*/
TreeItem_t* GetLastTreeChild(TreeItem_t* pTree)
{
	if (pTree)
	{
		return pTree->lastchild;
	}
	else
	{
		return NULL;
	}
}

/*get the next sibling of a tree item*/
TreeItem_t* GetNextTreeSibling(TreeItem_t* pTree)
{
	if (pTree)
	{
		return pTree->nextsibling;
	}
	else
	{
		return NULL;
	}
}

/*get previous sibling of a tree item*/
TreeItem_t* GetPreTreeSibling(TreeItem_t* pTree)
{
	if (pTree)
	{
		return pTree->previoussibling;
	}
	else
	{
		return NULL;
	}
}

/*get parent node of a tree item*/
TreeItem_t* GetTreeParent(TreeItem_t* pTree)
{
	if (pTree)
		return pTree->parent;
	else
		return NULL;
}

/*get root node of a tree item*/
TreeItem_t* GetTreeRoot(TreeItem_t* pTree)
{
	TreeItem_t* p = NULL;
	if (!pTree)
	{
		return NULL;
	}
	else
	{
		for (p = pTree; p->parent; p = p->parent);
		return p;
	}
}

/*destroy a link list,exposed,also invoked by traverse functions*/
void FreeTreeItemList(LinkListNode_t* p)
{
	LinkListNode_t* q = NULL;

	while (p->previous) p = p->previous;

	while (p)
	{
		q = p->next;
		free(p);
		p = q;
	}
}

/*helper function of GetBiTreeTraverseList,not exposed*/
C_BOOL AppendListItem(LinkListNode_t** ppFrontNode,LinkListNode_t** ppTailNode,Data_t* pData)
{
	LinkListNode_t* p = (LinkListNode_t*)malloc(sizeof(LinkListNode_t));
	if (!p)
	{
		FreeTreeItemList(*ppFrontNode);
		return C_FALSE;
	}
	memcpy(&p->data,pData,sizeof(Data_t));
	if (*ppTailNode)
	{
		(*ppTailNode)->next = p;
	}
	else
	{
		*ppFrontNode = p;
	}
	p->previous = *ppTailNode;
	p->next = NULL;
	*ppTailNode = p;
	return C_TRUE;
}

/*get traverse list of a binary tree,not exposed,helper function of GetTreeTraverseList*/
C_BOOL GetBiTreeTraverseList(LinkListNode_t** ppFrontNode,LinkListNode_t** ppTailNode,TreeItem_t* pTree,long nMode)
{
	LinkListNode_t* p = NULL;

	if (!pTree)
	{
		return C_TRUE;
	}

	if (TRAVERSE_PRE_ORDER == nMode)
	{
		return AppendListItem(ppFrontNode,ppTailNode,&pTree->data)
			&& GetBiTreeTraverseList(ppFrontNode,ppTailNode,pTree->firstchild,nMode)
			&& GetBiTreeTraverseList(ppFrontNode,ppTailNode,pTree->nextsibling,nMode);
	}
	else if (TRAVERSE_IN_ORDER == nMode)
	{
		return GetBiTreeTraverseList(ppFrontNode,ppTailNode,pTree->firstchild,nMode)
			&& AppendListItem(ppFrontNode,ppTailNode,&pTree->data)
			&& GetBiTreeTraverseList(ppFrontNode,ppTailNode,pTree->nextsibling,nMode);
	}
	else
	{
		return GetBiTreeTraverseList(ppFrontNode,ppTailNode,pTree->firstchild,nMode)
			&& GetBiTreeTraverseList(ppFrontNode,ppTailNode,pTree->nextsibling,nMode)
			&& AppendListItem(ppFrontNode,ppTailNode,&pTree->data);
	}
}

/*get traverse list of an ordinary tree*/
C_BOOL GetTreeTraverseList(LinkListNode_t** ppFrontNode,LinkListNode_t** ppTailNode,TreeItem_t* pTree,long nMode)
{
	LinkListNode_t* pTmp = NULL;
	C_BOOL retVal = C_FALSE;

	if (!ppFrontNode || !ppTailNode || !pTree || nMode < TRAVERSE_PRE_ORDER || nMode > TRAVERSE_POST_ORDER)
	{
		return C_FALSE;
	}
	
	*ppFrontNode = NULL;
	*ppTailNode = NULL;
	pTmp = pTree->nextsibling;
	pTree->nextsibling = NULL;
	retVal = GetBiTreeTraverseList(ppFrontNode,ppTailNode,pTree,nMode);
	pTree->nextsibling = pTmp;
	return retVal;
}

/*traverse(in order) the whole binary tree with callback visit function,not exposed,helper function of TreeTraverse_InOrder*/
C_BOOL BiTreeTraverse(TreeItem_t* pTree,VISIT_FUNC pFunc,long nSatisfied,C_BOOL fRetWhenSatisfied,long nMode)
{
	if (!pTree)
	{
		return fRetWhenSatisfied ? C_FALSE : C_TRUE;
	}

	if (TRAVERSE_PRE_ORDER == nMode)
	{
		if (!fRetWhenSatisfied)
		{
			(*pFunc)(pTree);
			BiTreeTraverse(pTree->firstchild,pFunc,nSatisfied,fRetWhenSatisfied,nMode);
			BiTreeTraverse(pTree->nextsibling,pFunc,nSatisfied,fRetWhenSatisfied,nMode);
			return C_TRUE;
		}
		else
		{
			return ((*pFunc)(pTree) == nSatisfied
				|| BiTreeTraverse(pTree->firstchild,pFunc,nSatisfied,fRetWhenSatisfied,nMode)
				|| BiTreeTraverse(pTree->nextsibling,pFunc,nSatisfied,fRetWhenSatisfied,nMode))
				? C_TRUE : C_FALSE;
		}
	}
	else if (TRAVERSE_IN_ORDER == nMode)
	{
		if (!fRetWhenSatisfied)
		{
			BiTreeTraverse(pTree->firstchild,pFunc,nSatisfied,fRetWhenSatisfied,nMode);
			(*pFunc)(pTree);
			BiTreeTraverse(pTree->nextsibling,pFunc,nSatisfied,fRetWhenSatisfied,nMode);
			return C_TRUE;
		}
		else
		{
			return (BiTreeTraverse(pTree->firstchild,pFunc,nSatisfied,fRetWhenSatisfied,nMode)
				|| (*pFunc)(pTree)
				|| BiTreeTraverse(pTree->nextsibling,pFunc,nSatisfied,fRetWhenSatisfied,nMode))
				? C_TRUE : C_FALSE;
		}
	}
	else
	{
		if (!fRetWhenSatisfied)
		{
			BiTreeTraverse(pTree->firstchild,pFunc,nSatisfied,fRetWhenSatisfied,nMode);
			BiTreeTraverse(pTree->nextsibling,pFunc,nSatisfied,fRetWhenSatisfied,nMode);
			(*pFunc)(pTree);
			return C_TRUE;
		}
		else
		{
			return (BiTreeTraverse(pTree->firstchild,pFunc,nSatisfied,fRetWhenSatisfied,nMode)
				|| BiTreeTraverse(pTree->nextsibling,pFunc,nSatisfied,fRetWhenSatisfied,nMode)
				|| (*pFunc)(pTree))
				? C_TRUE : C_FALSE;
		}
	}
}

/*traverse(in order) the whole tree with callback visit function*/
C_BOOL TreeTraverse(TreeItem_t* pTree,VISIT_FUNC pFunc,long nSatisfied,C_BOOL fRetWhenSatisfied,long nMode)
{
	TreeItem_t* pTmp = NULL;
	C_BOOL retVal = C_FALSE;

	if (!pTree || !pFunc || nMode < TRAVERSE_PRE_ORDER || nMode > TRAVERSE_POST_ORDER)
	{
		return C_FALSE;
	}

	pTmp = pTree->nextsibling;
	pTree->nextsibling = NULL;
	retVal = BiTreeTraverse(pTree,pFunc,nSatisfied,fRetWhenSatisfied,nMode);
	pTree->nextsibling = pTmp;
	return retVal;
}
