#pragma once

class LeftView
	: public BaseView
{
public:
	LeftView();
	~LeftView();

	virtual wstring GetStyleID();

	void OnCreate();

	CTreeUI* _pTree;
};