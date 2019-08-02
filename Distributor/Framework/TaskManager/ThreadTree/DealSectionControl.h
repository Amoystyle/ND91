
#ifndef __DEAL_SECTION_CONTROL__
#define __DEAL_SECTION_CONTROL__

#pragma once

#ifdef _LANG_TRANSLATE                  //                  //
#define LANG_EXPORT __declspec(dllexport)                   //                  //
#else                                   //                  //
#define LANG_EXPORT                     //                  //
#endif  

class LANG_EXPORT DealSectionControl
{
private:
	CRITICAL_SECTION	_m_csDealSection;					//����������ٽ�����

public:
	DealSectionControl(void);
	~DealSectionControl(void);

	void Lock(void);
	void UnLock(void);
};

#endif
