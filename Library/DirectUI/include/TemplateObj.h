#ifndef _TEMPALTEOBJ_H
#define _TEMPALTEOBJ_H

class  TemplateObj
{
public:
	TemplateObj(void);
	~TemplateObj(void);
	void SetType(LPCTSTR lpszType);
	LPCTSTR GetType();

	void SetStore(bool bStore);
	bool IsStore();

	void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	int GetAttrCount();
	LPCTSTR GetAttr(LPCTSTR lpszName);
	MAPAttr GetAttr();

	void Add(TemplateObj* pTemplate);
	void Insert(int nIndex, TemplateObj* pTemplate);
	TemplateObj* GetChild(int nIndex);
	int NumChild();
protected:
	tstring m_strType;
	MAPAttr m_mapAttr;
	bool m_bStore;
	CStdPtrArray m_arrChild;
};

//////////////////////////////////////////////////////////////////////////
#endif