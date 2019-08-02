#ifndef _UTILITY_H
#define _UTILITY_H
#include <Windows.h>

typedef bool (*array_compare_t)(LPVOID ptrA, LPVOID ptrB);
/// 序列容器.
class DUI_API CStdPtrArray
{
public:
	CStdPtrArray(int iPreallocSize = 0);
	virtual ~CStdPtrArray();

	void Empty();
	void EmptyAndFree();
	void Resize(int iSize);
	bool IsEmpty() const;
	int Find(LPVOID iIndex) const;
	bool Add(LPVOID pData);
	bool SetAt(int iIndex, LPVOID pData);
	bool InsertAt(int iIndex, LPVOID pData);
	bool Remove(int iIndex);
	bool Remove(LPVOID iIndex);
	void Swap(int nStart, int nEnd);
	/// 区间交换
	void Swap(int nStart1, int nEnd1, int nStart2, int nEnd2);
	void Move(int nStart, int nEnd, int nTo);
	int GetSize() const;
	LPVOID* GetData();

	LPVOID GetAt(int iIndex) const;
	LPVOID operator[] (int nIndex) const;

	void Sort(array_compare_t cmp);
protected:
	LPVOID* m_ppVoid;
	int m_nCount;
	int m_nAllocated;
};

class  DUI_API Charset
{
public:
	// ANSI转为Unicode
	static wstring AnsiToUnicode(const char* src);

	// ANSI转为UTF8
	static string AnsiToUTF8(const char* src);
	// UTF8转为ANSI
	static string UTF8ToAnsi(const char* src);

	// UTF8转为UNICODE
	static wstring UTF8ToUnicode(const char* src);
	// UNICODE转为UTF8
	static string UnicodeToUTF8(const wchar_t*src);

	// UTF8转为ANSI或Unicode
	static tstring UTF8ToNative(const char* src);
	// ANSI或Unicode转为UTF8
	static string  NativeToUTF8(LPCTSTR src);

	static wstring  TCHARToUnicode(LPCTSTR src);

	static string UnicodeTochar(const wchar_t*src);
	
};
////////////////////////////////////////////////////////////////////////////////////////
class  DUI_API CDelegateBase
{
public:
	CDelegateBase(void* pObject, void* pFn);
	CDelegateBase(const CDelegateBase& rhs);
	virtual ~CDelegateBase();
	bool Equals(const CDelegateBase& rhs) const;
	bool operator() (TEventUI& event);
	virtual CDelegateBase* Copy();

	void* GetFn();
	void* GetObject();
	virtual bool Invoke(TEventUI& event)
	{
		return false;
	}
private:
	void* m_pObject;
	void* m_pFn;
};


template <class T>
class CDelegate : public CDelegateBase
{
	typedef bool (T::* Fn)(TEventUI&);
public:
	CDelegate(T* pObj, Fn pFn) : CDelegateBase(pObj, &pFn), m_pFn(pFn) { }
	CDelegate(const CDelegate& rhs) : CDelegateBase(rhs) { m_pFn = rhs.m_pFn; } 
protected:
	virtual bool Invoke(TEventUI& event)
	{
		T* pObject = (T*) GetObject();
		return (pObject->*m_pFn)(event); 
	}  
	virtual CDelegateBase* Copy()
	{
		return new CDelegate(*this);
	}
private:
	Fn m_pFn;
};

template <class T>
CDelegate<T>  MakeDelegate(T* pObject, bool (T::* pFn)(TEventUI&))
{
	return CDelegate<T>(pObject, pFn);
}


class  DUI_API CEventDelegate
{
	typedef bool (*FnType)(TEventUI&);
public:
	CEventDelegate();
	~CEventDelegate();
	int GetCount();
	CDelegateBase* GetItem(int nIndex);
	void operator+= (CDelegateBase& d);
	void operator-= (CDelegateBase& d);
	bool Invoke(TEventUI& event);
	void Clear();
	bool IsExist(CDelegateBase& d);

protected:
	CStdPtrArray m_aDelegates;
};
///////////////////////////////////////////////////////////////////////////////////////////////////


#define   DEFAULT_SKIN_COLOR RGB(0, 159, 234)
class  DUI_API ColorHelper
{
public:
	static COLORREF AvgColor(HBITMAP hBitmap, int &nR, int &nG, int &nB);
	static void UpdateHLS(HBITMAP hBitmap, int nHue, int nSaturation, int lightness);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DUI_API CTweenAlgorithm
{
public:
	static double Linear( double t, double b, double c, double d );
	static double Quad( double t, double b, double c, double d, int iEaseType = 0 ); 
	static double Cubic( double t, double b, double c, double d, int iEaseType = 0 ); 
	static double Quart( double t, double b, double c, double d, int iEaseType = 0 );
	static double Quint( double t, double b, double c, double d, int iEaseType = 0 );
	static double Sine( double t, double b, double c, double d, int iEaseType = 0 );
	static double Expo( double t, double b, double c, double d, int iEaseType = 0 );
	static double Circ( double t, double b, double c, double d, int iEaseType = 0 );
	static double Elastic( double t, double b, double c, double d, double a, double p, int iEaseType = 0 );
	static double Back( double t, double b, double c, double d,  int iEaseType = 0, double s = 1.70158);
	static double Bounce( double t, double b, double c, double d, int iEaseType = 0 );

private:
	enum{ EASEIN, EASEOUT, EASEINOUT } eEaseType;
	static double BounceEaseIn( double t, double b, double c, double d );
	static double BounceEaseOut( double t, double b, double c, double d );
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DUI_API StrUtil
{
public:
	static tstring Replace(tstring &str, const TCHAR* string_to_replace, const TCHAR* new_string);
    static void ToLowerCase( tstring& str );
    static void ToUpperCase( tstring& str );
	static void Trim( tstring& str, bool left = true, bool right = true );
    static tstring Printf(const TCHAR *pszFormat, ...);
    static vector<tstring> Split( const tstring& str, const tstring& delims = _T("\t\n "), unsigned int maxSplits = 0);
};

extern "C" 
{
	DUI_API const char* GetDirectUIVersion();
	DUI_API void ShowDirectUIVersion();
}

#endif //_UTILITY_H