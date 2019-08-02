#include "StdAfx.h"
#include "DealSectionControl.h"

DealSectionControl::DealSectionControl(void)
{INTERFUN;
	InitializeCriticalSection(&_m_csDealSection);
}

DealSectionControl::~DealSectionControl(void)
{INTERFUN;
	DeleteCriticalSection(&_m_csDealSection);
}

void DealSectionControl::Lock()
{INTERFUN;
	EnterCriticalSection(&_m_csDealSection);
}

void DealSectionControl::UnLock()
{INTERFUN;
	LeaveCriticalSection(&_m_csDealSection);
}
