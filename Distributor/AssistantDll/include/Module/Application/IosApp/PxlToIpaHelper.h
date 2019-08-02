#pragma once

#include <iostream>

using namespace std;

namespace ND91Assistant
{
	//用于PXL转IPA
	class CLASSINDLL_CLASS_DECL PxlToIpaHelper
	{
	public:
		PxlToIpaHelper(void);
		~PxlToIpaHelper(void);

		// Summary  : 将PXL转换为IPA格式
		// Returns  : bool 成功返回TRUE
		// Parameter: wstring strPxlFileName ：源PXL文件名（含路径）
		// Parameter: wstring strIpaFileName ：返回转换后的IPA文件名（含路径）
        // Parameter: wstring tempFolder : 工作中需要用到的临时目录
		static bool ConverPxlToIpa(wstring strPxlFileName, wstring& strIpaFileName, wstring tempFolder);

	private:
		// Summary  : 解压PXL包，并对解压后的文件进行相应处理
		// Returns  : bool 成功返回TRUE
		// Parameter: wstring strPxlFileName ：PXL包名（含路径）
		// Parameter: wstring strExtractPath ：解压后存放的文件夹名（含路径）
		static bool ExtractPxl(wstring strPxlFileName, wstring strExtractPath);

		// Summary  : 打包IPA包
		// Returns  : bool 成功返回TRUE
		// Parameter: wstring strExtractPath ：PXL解压后的文件夹（含路径）
		// Parameter: wstring strIpaFileName ：IPA打包后的文件名（含路径）
        // Parameter: wstring tempFolder : 工作中需要用到的临时目录
		static bool CompressIpa(wstring strExtractPath, wstring strIpaFileName, wstring tempFolder);
	};
}
