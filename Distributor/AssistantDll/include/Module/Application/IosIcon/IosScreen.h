#pragma once

#pragma warning(disable : 4251)

#include "IosFolderIcon.h"

namespace ND91Assistant
{
	//��Ļ��Ϣ�洢��
	class CLASSINDLL_CLASS_DECL IosScreen
	{
	public:
		IosScreen(void);
		~IosScreen(void);

	public:
		IosIcon* _screenInfo[5][5]; // ÿ����Ļ�е�ͼ����Ϣ���������б߽�ֵ����IosIconSet���е�_nScreenLineCount��_nScreenRowCount����
	};
}
