#pragma once

// Ä£¿é»ùÀà
class SourceModule
	: public BaseModule
{
public:
	virtual bool Init(CContainerUI* pLeft, CContainerUI* pContainer) = 0;
};