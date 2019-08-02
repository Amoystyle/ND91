
#ifndef WINDOWWND_H
#define WINDOWWND_H

class DUI_API CWindowBase;
typedef std::map<HWND,CWindowBase*> TmapWindowTable;
class DUI_API CWindowBase  
{
public:
	CWindowBase();
	virtual ~CWindowBase();

public:
	HWND Create(HWND hwndParent, LPCTSTR lpszName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy);
	void SetWndStyle(int index, DWORD dwStyle);
	void ModifyWndStyle(int index, DWORD dwRemove, DWORD dwAdd);
	//���໯�뷴���໯
	BOOL SubclassWindow(HWND hWnd);
	void UnsubclassWindow();
	
	void SetAutoDel(bool bAutoDel); //�Ƿ��Զ��ͷ�ָ��
	bool IsAutoDel(){return m_bAutoDel;}
	HWND GetHWND();
	operator HWND();
	bool GetWindowRect(LPRECT lpRect);
	static CWindowBase* FromHandle(HWND hWnd);
	virtual void OnClose();
	virtual bool OnClosing(WPARAM wParam, LPARAM lParam);
	virtual bool OnQueryEndSession();
	virtual UINT GetClassStyle();
	virtual LPCTSTR GetWindowClassName();
	virtual void SetWindowClassName(LPCTSTR lpszName);
	virtual void OnInactiveApp(){};
	void GetClientRect(LPRECT lpRect);
public:
	static TmapWindowTable s_mapNdCtrls; //���б������Ĵ�����
	virtual bool PreMessageHandler(const LPMSG pMsg, LRESULT& lRes); //��Ϣ
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	bool RegisterWindowClass();
	WNDPROC m_oldProc;      //ԭ�������
	HWND m_hWnd;      
	bool m_bAutoDel;
	tstring m_strWndClassName;

	DWORD m_dwExStyle;
	DWORD m_dwStyle;

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif


















