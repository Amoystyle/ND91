#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IIosIconDataOperation.h"
#include "Device/CFConverter.h"

IIosIconDataOperation::IIosIconDataOperation(IosIconSet* pIconSet)
{
	_pIconSet = pIconSet;
}

IIosIconDataOperation::~IIosIconDataOperation(void)
{
}

void* IIosIconDataOperation::PackNormalIconToCFData(IosIcon* pIcon)
{
	if (!pIcon || pIcon->GetIconType() == eIosIconType_Folder)
		return NULL;

	void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 4, NULL, NULL);

	void* pKey = CCFConverter::StringtoCFString("displayName");
	void* pVaule = CCFConverter::StringtoCFString(pIcon->_strDisPlayName);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	pKey = CCFConverter::StringtoCFString("iconModDate");
	pVaule = CCFConverter::TimeToCFDate(pIcon->_iconModeDate);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	pKey = CCFConverter::StringtoCFString("bundleIdentifier");
	pVaule = CCFConverter::StringtoCFString(pIcon->_strBundleIdentifier);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	pKey = CCFConverter::StringtoCFString("displayIdentifier");
	pVaule = CCFConverter::StringtoCFString(pIcon->_strDisplayIndentifier);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	return pCFDict;
}

void IIosIconDataOperation::AddIconInfo(void* pIconData, int nScreenIndex, int nLineIndex, int nRowIndex)
{
	if (!_pIconSet || !pIconData)
		return ;

	IosIcon* pIcon = GetIcon(pIconData);

	if (nScreenIndex < 0 || nLineIndex < 0 ||
		nRowIndex < 0 || !pIcon)
		return;

	if (nScreenIndex == 0 && nLineIndex == 0 && nRowIndex < _pIconSet->_nBottomBarMaxIconCount) //µ×²¿ICON
	{
		if (_pIconSet->_BottomBar[nRowIndex])
			delete _pIconSet->_BottomBar[nRowIndex];

		_pIconSet->_BottomBar[nRowIndex] = pIcon;
	}
	else if (nLineIndex < _pIconSet->_nScreenLineCount && nRowIndex < _pIconSet->_nScreenRowCount)
	{
		IosScreen* pScreen = NULL;

		if ((int)_pIconSet->_vtScreen.size() < nScreenIndex)
		{
			pScreen = new IosScreen();
			_pIconSet->_vtScreen.push_back(pScreen);
		}
		else
			pScreen = _pIconSet->_vtScreen.at(nScreenIndex-1);

		if (pScreen->_screenInfo[nLineIndex][nRowIndex])
			delete pScreen->_screenInfo[nLineIndex][nRowIndex];
		pScreen->_screenInfo[nLineIndex][nRowIndex] = pIcon;
	}
}

IosIcon* IIosIconDataOperation::GetIcon(void* pIconData)
{
	IosIcon* pIcon = NULL;

	map<string, void*> mapInfo;
	if (CCFConverter::GetIosData(pIconData, mapInfo))
	{
		if (mapInfo.size() > 0)
		{
			if (mapInfo.find("iconLists") != mapInfo.end())
				pIcon = GetFolderIcon(mapInfo);
			else
				pIcon = GetNormalIcon(mapInfo);	
		}	
	}

	return pIcon;
}

IosIcon* IIosIconDataOperation::GetNormalIcon(map<string, void*> mapInfo)
{
	IosIcon* pIcon = new IosIcon();

	map<string, void*>::iterator it;
	for (it=mapInfo.begin(); it!=mapInfo.end(); it++)
	{
		if (it->first.compare("displayName") == 0)
			CCFConverter::GetIosData(it->second, pIcon->_strDisPlayName);
		else if (it->first.compare("iconModDate") == 0)
			CCFConverter::GetIosData(it->second, pIcon->_iconModeDate);
		else if (it->first.compare("bundleIdentifier") == 0)
			CCFConverter::GetIosData(it->second, pIcon->_strBundleIdentifier);
		else if (it->first.compare("displayIdentifier") == 0)
			CCFConverter::GetIosData(it->second, pIcon->_strDisplayIndentifier);
	}

	return pIcon;
}

IosFolderIcon* IIosIconDataOperation::GetFolderIcon(map<string, void*> mapInfo)
{
	IosFolderIcon* pIcon = new IosFolderIcon();

	map<string, void*>::iterator it;
	for (it=mapInfo.begin(); it!=mapInfo.end(); it++)
	{
		if (it->first.compare("displayName") == 0)
			CCFConverter::GetIosData(it->second, pIcon->_strDisPlayName);
		else if (it->first.compare("iconLists") == 0)
		{
			vector<void*> vtLineHandle;
			if (CCFConverter::GetIosData(it->second, vtLineHandle))
			{
				for (vector<void*>::iterator itLineHandle=vtLineHandle.begin(); itLineHandle!=vtLineHandle.end(); itLineHandle++)
				{
					vector<void*> vtIconHandle;
					if (CCFConverter::GetIosData(*itLineHandle, vtIconHandle))
					{
						for (vector<void*>::iterator itIconHandle=vtIconHandle.begin(); itIconHandle!=vtIconHandle.end(); itIconHandle++)
						{
							IosIcon* pIconChild = GetIcon(*itIconHandle);
							pIcon->_vtIconList.push_back(pIconChild);
						}
					}
				}						
				vtLineHandle.clear();
			}
		}
	}

	return pIcon;
}
#endif