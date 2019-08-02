#ifdef WEBRESOURCE_EXPORTS
#define WEBRESOURCE_API __declspec(dllexport)
#else
#define WEBRESOURCE_API __declspec(dllimport)
#endif

WEBRESOURCE_API SourceModule* CreateWebResourceModule();

class WebResourceController;
class CCheckLocalCpaValid;
class WebResourceModule
	: public SourceModule
{
public:
	WebResourceModule(void);
	~WebResourceModule();

	static const wchar_t* LoadString(const wchar_t* id) { return I18NSTR(id); }

	static WebResourceModule* GetInstance();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init( CContainerUI* pLeft, CContainerUI* pMain );

private:
	static WebResourceModule* _pInstance;
	WebResourceController* _pController;
	CCheckLocalCpaValid*   _pCheckCpaValid;
};

