#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IosIconDataOperation3.h"
#include "Device/CFConverter.h"

IosIconDataOperation3::IosIconDataOperation3(IosIconSet* pIconSet)
:IIosIconDataOperation(pIconSet)
{
}

IosIconDataOperation3::~IosIconDataOperation3(void)
{
}

void* IosIconDataOperation3::PackSingleIconToCFData(IosIcon* pIcon)
{
	if (!pIcon)
		return CCFConverter::NDkCFBooleanFalse();
	else
		return PackNormalIconToCFData(pIcon);
}

void* IosIconDataOperation3::PackIconCFData()
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
	vector<void*> vtBottomLine;
	void* pBottomLineCFData = CCFConverter::ArrayToCFArray(vtBottomIcon);
	if (pBottomLineCFData)
		vtBottomLine.push_back(pBottomLineCFData);
	void* pBottomScreenCFData = CCFConverter::ArrayToCFArray(vtBottomLine);
	if (pBottomScreenCFData)
		vtScreen.push_back(pBottomScreenCFData);

	int nScreenCount = 0;
	vector<IosScreen*>::iterator it;
	for (it=_pIconSet->_vtScreen.begin(); it!=_pIconSet->_vtScreen.end(); it++)
	{
		IosScreen* pIosScreenTemp = *it;
		if (pIosScreenTemp)
		{			

			vector<void*> vtLineCFData;
			for (int i=0; i<_pIconSet->_nScreenLineCount; i++)
			{
				vector<void*> vtRowCFData;

				for (int j=0; j<_pIconSet->_nScreenRowCount; j++)
				{
					void* pRowCFData = PackSingleIconToCFData(_pIconSet->_vtScreen.at(nScreenCount)->_screenInfo[i][j]);
					if (pRowCFData)
						vtRowCFData.push_back(pRowCFData);
				}

				void* pLineCFData = CCFConverter::ArrayToCFArray(vtRowCFData);
				if (pLineCFData)
					vtLineCFData.push_back(pLineCFData);
			}

			void* pScreenCFData = CCFConverter::ArrayToCFArray(vtLineCFData);
			if (pScreenCFData)
				vtScreen.push_back(pScreenCFData);
		}
		nScreenCount++;
	}

	return CCFConverter::ArrayToCFArray(vtScreen);
}

void IosIconDataOperation3::PraseIconCFData(void* pCFData)
{
	if (!_pIconSet || !pCFData)
		return ;

	//获取各个屏幕的句柄
	 vector<void*> vtScreenHandle;
	if (CCFConverter::GetIosData(pCFData, vtScreenHandle))
	{		
		int nScreenIndex = 0; //屏幕索引
		vector<void*>::iterator itScreenHandle;
		for (itScreenHandle=vtScreenHandle.begin(); itScreenHandle!=vtScreenHandle.end(); itScreenHandle++)
		{
			//获取每屏的行句柄
			vector<void*> vtScreenLineHandle;
			if (CCFConverter::GetIosData(*itScreenHandle, vtScreenLineHandle))
			{				
				int nLineIndex = 0; //行索引
				vector<void*>::iterator itScreenLineHandle;
				for (itScreenLineHandle=vtScreenLineHandle.begin(); itScreenLineHandle!=vtScreenLineHandle.end(); itScreenLineHandle++)
				{
					//获取每行的ICON句柄
					vector<void*> vtIconHandle;
					if (CCFConverter::GetIosData(*itScreenLineHandle, vtIconHandle))
					{
						int nRowIndex = 0; //列索引
						vector<void*>::iterator itIconHandle;
						for (itIconHandle=vtIconHandle.begin(); itIconHandle!=vtIconHandle.end(); itIconHandle++)
						{
							//解析并添加ICON信息
							AddIconInfo(*itIconHandle, nScreenIndex, nLineIndex, nRowIndex);

							CCFConverter::NDCFRelease(*itIconHandle);

							nRowIndex++;
						}						
						vtIconHandle.clear();
					}

					//CCFConverter::NDCFRelease(*itScreenLineHandle);

					nLineIndex++;
				}
				vtScreenLineHandle.clear();
			}

			//CCFConverter::NDCFRelease(*itScreenHandle);

			nScreenIndex++;
		}
		vtScreenHandle.clear();
	}
}

#endif