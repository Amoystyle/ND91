#pragma once

#include <iostream>
#include "AssistantDll.h"

using namespace std;

namespace ND91Assistant
{
	typedef enum eIosIconType
	{
		eIosIconType_Normal,
		eIosIconType_Folder
	};

	// ��ͨͼ����Ϣ�洢��
	class CLASSINDLL_CLASS_DECL IosIcon
	{
	public:
		IosIcon(void);
		~IosIcon(void);

		// ��ȡͼ�������
		virtual eIosIconType GetIconType(){return eIosIconType_Normal;}

	public:
		string _strDisPlayName; //��ʾ����
		time_t _iconModeDate; // ʱ��
		string _strBundleIdentifier; // Ψһ��־ֵ
		string _strDisplayIndentifier; // ��ʾ��Ψһ��־ֵ
		wstring _strIconImgFile; //ͼ���ͼƬ��������
	};
}
