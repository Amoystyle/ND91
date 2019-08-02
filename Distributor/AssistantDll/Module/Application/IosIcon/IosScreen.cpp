#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Module/Application/IosIcon/IosScreen.h"

IosScreen::IosScreen(void)
{
	for (int i=0; i<5; i++)
	{
		for (int j=0; j<5; j++)
		{
			_screenInfo[i][j] = NULL;
		}
	}
}

IosScreen::~IosScreen(void)
{
	for (int i=0; i<5; i++)
	{
		for (int j=0; j<5; j++)
		{
			SAFE_DELETE(_screenInfo[i][j]);
		}
	}
}

#endif