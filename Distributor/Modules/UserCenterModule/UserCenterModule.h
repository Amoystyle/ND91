#ifdef USERCENTERMODULE_EXPORTS
#define USERCENTERMODULE_API __declspec(dllexport)
#else
#define USERCENTERMODULE_API __declspec(dllimport)
#endif


USERCENTERMODULE_API SourceModule* CreateUserCenterModule();

class UserCenterController;
class UserCenterModule
	: public SourceModule
{
public:
	UserCenterModule();
	~UserCenterModule();

	static UserCenterModule* GetInstance();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init(CContainerUI* pLeft, CContainerUI* pMain );

	virtual void Release(){}

private:
	UserCenterController* _pController;

	static UserCenterModule* _pInstance;
};

