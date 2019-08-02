#include "stdafx.h"
#include "LeftView.h"

LeftView::LeftView()
{
	_pTree = NULL;
}

LeftView::~LeftView()
{

}

std::wstring LeftView::GetStyleID()
{
	return L"InstallCase_View_Left";
}

void LeftView::OnCreate()
{
	_pTree = dynamic_cast<CTreeUI*>(GetItem(L"Tree"));
	assert(_pTree);
}
