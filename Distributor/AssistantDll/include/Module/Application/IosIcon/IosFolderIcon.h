#pragma once

#pragma warning(disable : 4251)

#include "IosIcon.h"
#include <vector>

namespace ND91Assistant
{
	// �ļ���ͼ����Ϣ�洢��
	class CLASSINDLL_CLASS_DECL IosFolderIcon :public IosIcon
	{
	public:
		IosFolderIcon(void);
		~IosFolderIcon(void);

		// ��ȡͼ�������
		virtual eIosIconType GetIconType(){return eIosIconType_Folder;}

	public:
		vector<IosIcon*> _vtIconList; //�ļ��������ͨͼ����Ϣ
		string _strListType; // �ļ��е�����
	};
}
