#pragma once

class FileManagerInterface
{
public:
    // ��ʼ��
    virtual void OnInit(DirData* pRootDir) = 0;

    // ��ʾָ��Ŀ¼/�ļ�
    virtual bool Goto(wstring path) = 0;

    // ���õ�����ť�Ƿ����
    virtual void EnableButton(bool enablePrevButton, bool enableNextButton, bool enableParentButton) = 0;

    // ˢ��Ŀ¼��
    virtual void Refresh() = 0;

    // ��ʾ�ռ���Ϣ
    virtual void ShowSpaceInfo(wstring info) = 0;


    // ����ָ���ļ����ڵ��б���
    virtual void SelectListLine(vector<wstring> paths) = 0;

	virtual void MyTrim(wstring & str) = 0;		//ȥ���ַ����ҿո�

	// �൱����������
	virtual void Release() = 0;

	virtual void ShowLoading(bool visible = true) = 0;

};
