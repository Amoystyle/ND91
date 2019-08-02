#pragma once

// Ä£¿é»ùÀà
class BaseModule
{
public:
    virtual tstring Id() = 0;
	virtual tstring Name() = 0;
	virtual tstring Icon() = 0;
	virtual int LayoutIndex()=0;
    virtual void Show() = 0;
	virtual void Hide() = 0;
};