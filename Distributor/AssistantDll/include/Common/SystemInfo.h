#pragma once


class CVersion;
// �����࣬������ȡϵͳ����Ϣ�����Ƿ�64λ������ϵͳ�汾��
class CLASSINDLL_CLASS_DECL CSystemInfo
{
public:
	CSystemInfo();
	~CSystemInfo();

public:
	// �жϲ���ϵͳ�Ƿ���64λ
	static bool is64();
	//�Ƿ���Vistaϵͳ
	static bool IsVista();
	//�Ƿ���Windows7ϵͳ
	static bool IsWindows7();
	//�Ƿ���XPϵͳ
	static bool IsXP();
    //��ȡϵͳ��Ϣ
    static wstring GetSystemInfo();
	//��ȡ���Ե�MAC��ַ
	static std::string GetMac();
	//��ȡwinϵͳ�汾
	static std::string GetOs();
	static bool IsNeedInstalliTunes(wstring minVer = _T("7,5,0,20"));
private:
	//��ȡ����ϵͳ�汾
	static bool GetOSVer(CVersion* pVer);
};
