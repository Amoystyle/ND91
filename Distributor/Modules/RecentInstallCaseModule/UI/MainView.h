#pragma once

class MainView
	: public BaseView
{
public:
	MainView();
	~MainView();

private:
	virtual wstring GetStyleID();
};