#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Module/Application/IosIcon/IosFolderIcon.h"

IosFolderIcon::IosFolderIcon(void)
{
	_strListType = "";
}

IosFolderIcon::~IosFolderIcon(void)
{
	vector<IosIcon*>::iterator it;
	for (it=_vtIconList.begin(); it!=_vtIconList.end(); it++)
	{
		if (*it)
			delete (*it);
		*it = NULL;
	}
}

#endif