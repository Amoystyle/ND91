#pragma once

#include "IIosIconDataOperation.h"
#include <map>

using namespace std;

namespace ND91Assistant
{
	//4.X�̼���ͼ�����ݲ�����
	class IosIconDataOperation4 : public IIosIconDataOperation
	{
	public:
		IosIconDataOperation4(IosIconSet* pIconSet);
		~IosIconDataOperation4(void);

		// ���ͼ������
		virtual void* PackIconCFData();
		// ����ͼ������
		virtual void PraseIconCFData(void* pCFData);

	private:
		void* PackSingleIconToCFData(IosIcon* pIcon);
		void* PackFolderIconToCFData(IosIcon* pIcon);
	};
}
