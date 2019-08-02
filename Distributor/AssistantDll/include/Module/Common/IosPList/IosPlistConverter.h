#pragma once

/************************************************************************/
/* plist��ʽ�ļ���XML��ת����
   ��ʱֻ����WIFI�����½����豸��
   ����������޸�IOS�豸�����Ի������ļ���
   2011-6-21	by ytg
*/
/************************************************************************/
#include <iostream>
using namespace std;

class CLASSINDLL_CLASS_DECL IosPlistConverter
{
public:

	// Summary  : ��plist�ļ�ת��ΪXML��ʽ
	// Returns  : �Ƿ�ɹ�
	// Parameter: wstring srcPlistFile ԭplist�ļ�������·����
	// Parameter: wstring desXmlFile ת����洢��XML�ļ�������·����
	static bool GetXmlFromPlist(wstring srcPlistFile, wstring desXmlFile);

	// Summary  : ��XML�ļ�ת��Ϊplist��ʽ
	// Returns  : �Ƿ�ɹ�
	// Parameter: wstring sourceXmlFile ԭXML�ļ�������·����
	// Parameter: wstring destPlistFile ת����洢��plist�ļ�������·����
	static bool GetPlistFromXml(wstring sourceXmlFile, wstring destPlistFile);
};
