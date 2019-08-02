#pragma once

class LeftView
	: public BaseView
{
public:
	LeftView();
	~LeftView();

	CTreeUI* GetTree();
	virtual wstring GetStyleID();
};