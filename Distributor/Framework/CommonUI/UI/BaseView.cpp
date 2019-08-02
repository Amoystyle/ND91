#include "stdafx.h"
#include "BaseView.h"

BaseView::BaseView()
{
	_bAdded = false;
}

BaseView::~BaseView()
{

}

void BaseView::Init(CContainerUI* pContainer)
{
	if(pContainer == NULL)
		return;

	wstring styleId = this->GetStyleID();

	if(_bAdded || pContainer->Contain(this))
		return;

	_bAdded = true;

	__super::Create(styleId.c_str(), pContainer);

	this->Resize();
	this->Invalidate();
}

void BaseView::Show()
{
	this->SetVisible(true);
	this->Resize();
	this->Invalidate();
}

void BaseView::Hide()
{
	this->SetVisible(false);
	this->Resize();
	this->Invalidate();
}

