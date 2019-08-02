#pragma once

class MainView
	: public BaseView
{
public:
	MainView();
	~MainView();

protected:
	virtual wstring GetStyleID();
};