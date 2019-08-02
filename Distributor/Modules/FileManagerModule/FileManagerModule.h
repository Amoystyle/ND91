#ifdef FILEMANAGER_EXPORTS
#define FILEMANAGER_API __declspec(dllexport)
#else
#define FILEMANAGER_API __declspec(dllimport)
#endif

FILEMANAGER_API DeviceModule* CreateFileManagerModule(wstring deviceID, void* phone);

class FileManagerController;
class FileManagerModule
	: public DeviceModule
{
public:
	FileManagerModule(wstring deviceID, void* phone);
	~FileManagerModule();

	virtual tstring Id();

	virtual tstring Name();

	virtual tstring Icon();

	virtual int LayoutIndex();

	virtual void Show();

	virtual void Hide();

	virtual bool Init(CContainerUI* pMain);

	void Release();

private:
	FileManagerController* _pController;
};

