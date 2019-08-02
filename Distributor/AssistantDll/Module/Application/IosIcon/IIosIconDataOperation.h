#pragma once

#include "Module/Application/IosIcon/IosIconSet.h"
#include <map>

namespace ND91Assistant
{
	// ͼ���������
	class IIosIconDataOperation
	{
	public:
		IIosIconDataOperation(IosIconSet* pIconSet);
		~IIosIconDataOperation(void);

		// ���ͼ������
		virtual void* PackIconCFData() = 0;
		// ����ͼ������
		virtual void PraseIconCFData(void* pCFData) = 0;

	protected:
		void* PackNormalIconToCFData(IosIcon* pIcon);
		void AddIconInfo(void* pIconData, int nScreenIndex, int nLineIndex, int nRowIndex);

	protected:
		IosIconSet* _pIconSet;

	private:
		IosIcon* GetIcon(void* pIconData);
		IosIcon* GetNormalIcon(map<string, void*> mapInfo);
		IosFolderIcon* GetFolderIcon(map<string, void*> mapInfo);
	};
}
