#pragma once

#include "IIosIconDataOperation.h"

namespace ND91Assistant
{
	//3.X�̼���ͼ�����ݲ�����
	class IosIconDataOperation3 : public IIosIconDataOperation
	{
	public:
		// ���캯��
		// IosIconSet* pIconSet ��IosIconSet��ָ��
		IosIconDataOperation3(IosIconSet* pIconSet);
		~IosIconDataOperation3(void);

		// ���ͼ������
		virtual void* PackIconCFData();
		// ����ͼ������
		virtual void PraseIconCFData(void* pCFData);

	private:
		void* PackSingleIconToCFData(IosIcon* pIcon);

	};
}
