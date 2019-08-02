#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosIconDataOperation4.h"
#include "Device/CFConverter.h"

IosIconDataOperation4::IosIconDataOperation4(IosIconSet* pIconSet)
:IIosIconDataOperation(pIconSet)
{
}

IosIconDataOperation4::~IosIconDataOperation4(void)
{
}

void* IosIconDataOperation4::PackSingleIconToCFData(IosIcon* pIcon)
{
	if (!pIcon)
		return CCFConverter::NDCFDictionaryCreateMutable(NULL, 0, NULL, NULL);
	else if (pIcon->GetIconType() == eIosIconType_Folder)
		return PackFolderIconToCFData(pIcon);
	else
		return PackNormalIconToCFData(pIcon);
}

void* IosIconDataOperation4::PackFolderIconToCFData(IosIcon* pIcon)
{
	vector<void*> vtIconHandle;
	IosFolderIcon* pIconTemp = dynamic_cast<IosFolderIcon*>(pIcon);
	for (vector<IosIcon*>::iterator it=pIconTemp->_vtIconList.begin(); it!=pIconTemp->_vtIconList.end(); it++)
	{
		vtIconHandle.push_back(PackSingleIconToCFData(*it));
	}

	vector<void*> vtIconListsHandle;
	void* pBottomLineCFData = CCFConverter::ArrayToCFArray(vtIconHandle);
	if (pBottomLineCFData)
		vtIconListsHandle.push_back(pBottomLineCFData);

	void* pCFDict = CCFConverter::NDCFDictionaryCreateMutable(NULL, 2, NULL, NULL);

	void* pKey = CCFConverter::StringtoCFString("displayName");
	void* pVaule = CCFConverter::StringtoCFString(pIconTemp->_strDisPlayName);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	pKey = CCFConverter::StringtoCFString("iconLists");
	pVaule = CCFConverter::ArrayToCFArray(vtIconListsHandle);
	CCFConverter::NDCFDictionaryAddValue(pCFDict, pKey, pVaule);

	return pCFDict;
}

void* IosIconDataOperation4::PackIconCFData()
{
	if (!_pIconSet)
		return NULL;

	vector<void*> vtScreen;

	//ICON信息转CFData
	vector<void*> vtBottomIcon;
	for (int i=0; i<_pIconSet->_nBottomBarMaxIconCount; i++)
	{
		void* pIconCFData = PackSingleIconToCFData(_pIconSet->_BottomBar[i]);
		if (pIconCFData)
			vtBottomIcon.push_back(pIconCFData);
	}
	void* pBottomScreenCFData = CCFConverter::ArrayToCFArray(vtBottomIcon);
	if (pBottomScreenCFData)
		vtScreen.push_back(pBottomScreenCFData);

	int nScreenCount = 0;
	vector<IosScreen*>::iterator it;
	for (it=_pIconSet->_vtScreen.begin(); it!=_pIconSet->_vtScreen.end(); it++)
	{
		IosScreen* pIosScreenTemp = *it;
		if (pIosScreenTemp)
		{			
			vector<void*> vtScreenIconCFData;
			for (int i=0; i<_pIconSet->_nScreenLineCount; i++)
			{
				for (int j=0; j<_pIconSet->_nScreenRowCount; j++)
				{
					void* pRowCFData = PackSingleIconToCFData(_pIconSet->_vtScreen.at(nScreenCount)->_screenInfo[i][j]);
					if (pRowCFData)
						vtScreenIconCFData.push_back(pRowCFData);
				}				
			}

			void* pScreenCFData = CCFConverter::ArrayToCFArray(vtScreenIconCFData);
			if (pScreenCFData)
				vtScreen.push_back(pScreenCFData);
		}
		nScreenCount++;
	}

	return CCFConverter::ArrayToCFArray(vtScreen);
}

void IosIconDataOperation4::PraseIconCFData(void* pCFData)
{
	if (!_pIconSet || !pCFData)
		return ;

	//获取各个屏幕的句柄
	vector<void*> vtScreenHandle;
	if (CCFConverter::GetIosData(pCFData, vtScreenHandle))
	{		
		bool bBottomBar = true;
		int nScreenIndex = 0; //屏幕索引
		vector<void*>::iterator itScreenHandle;
		for (itScreenHandle=vtScreenHandle.begin(); itScreenHandle!=vtScreenHandle.end(); itScreenHandle++)
		{			
			int nMaxRowCount = _pIconSet->_nScreenRowCount;
			if (bBottomBar)
			{
				nMaxRowCount = _pIconSet->_nBottomBarMaxIconCount;
				bBottomBar = false;
			}
			//获取每屏的ICON句柄
			vector<void*> vtIconHandle;
			if (CCFConverter::GetIosData(*itScreenHandle, vtIconHandle))
			{
				int nIconIndex = 0;
				vector<void*>::iterator itIconHandle;
				for (itIconHandle=vtIconHandle.begin(); itIconHandle!=vtIconHandle.end(); itIconHandle++)
				{
					//解析并添加ICON信息
					int nLineIndex = nIconIndex/nMaxRowCount;
					int nRowIndex = nIconIndex%nMaxRowCount;
					AddIconInfo(*itIconHandle, nScreenIndex, nLineIndex, nRowIndex);

					CCFConverter::NDCFRelease(*itIconHandle);

					nIconIndex++;
				}						
				vtIconHandle.clear();
			}

			nScreenIndex++;
		}
		vtScreenHandle.clear();
	}
}

#endif