#include "stdafx.h"
#include "LeftView.h"

LeftView::LeftView()
{

}

LeftView::~LeftView()
{

}

std::wstring LeftView::GetStyleID()
{
	return L"TaskCenter_View_Left";
}

CTreeUI* LeftView::GetTree()
{
	return  dynamic_cast<CTreeUI*>(GetItem(L"Tree"));
}

