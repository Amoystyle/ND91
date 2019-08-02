#pragma once

#include <iostream>

using namespace std;

namespace ND91Assistant
{
	//����PXLתIPA
	class CLASSINDLL_CLASS_DECL PxlToIpaHelper
	{
	public:
		PxlToIpaHelper(void);
		~PxlToIpaHelper(void);

		// Summary  : ��PXLת��ΪIPA��ʽ
		// Returns  : bool �ɹ�����TRUE
		// Parameter: wstring strPxlFileName ��ԴPXL�ļ�������·����
		// Parameter: wstring strIpaFileName ������ת�����IPA�ļ�������·����
        // Parameter: wstring tempFolder : ��������Ҫ�õ�����ʱĿ¼
		static bool ConverPxlToIpa(wstring strPxlFileName, wstring& strIpaFileName, wstring tempFolder);

	private:
		// Summary  : ��ѹPXL�������Խ�ѹ����ļ�������Ӧ����
		// Returns  : bool �ɹ�����TRUE
		// Parameter: wstring strPxlFileName ��PXL��������·����
		// Parameter: wstring strExtractPath ����ѹ���ŵ��ļ���������·����
		static bool ExtractPxl(wstring strPxlFileName, wstring strExtractPath);

		// Summary  : ���IPA��
		// Returns  : bool �ɹ�����TRUE
		// Parameter: wstring strExtractPath ��PXL��ѹ����ļ��У���·����
		// Parameter: wstring strIpaFileName ��IPA�������ļ�������·����
        // Parameter: wstring tempFolder : ��������Ҫ�õ�����ʱĿ¼
		static bool CompressIpa(wstring strExtractPath, wstring strIpaFileName, wstring tempFolder);
	};
}
