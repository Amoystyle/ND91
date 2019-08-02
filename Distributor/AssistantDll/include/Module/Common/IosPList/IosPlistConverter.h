#pragma once

/************************************************************************/
/* plist格式文件与XML的转化类
   暂时只用于WIFI连接下解析设备名
   后面可用于修改IOS设备的属性或配置文件用
   2011-6-21	by ytg
*/
/************************************************************************/
#include <iostream>
using namespace std;

class CLASSINDLL_CLASS_DECL IosPlistConverter
{
public:

	// Summary  : 将plist文件转化为XML格式
	// Returns  : 是否成功
	// Parameter: wstring srcPlistFile 原plist文件名（含路径）
	// Parameter: wstring desXmlFile 转化后存储的XML文件名（含路径）
	static bool GetXmlFromPlist(wstring srcPlistFile, wstring desXmlFile);

	// Summary  : 将XML文件转化为plist格式
	// Returns  : 是否成功
	// Parameter: wstring sourceXmlFile 原XML文件名（含路径）
	// Parameter: wstring destPlistFile 转化后存储的plist文件名（含路径）
	static bool GetPlistFromXml(wstring sourceXmlFile, wstring destPlistFile);
};
