#ifdef ABL_EXPORTS
#define ABL_API __declspec(dllexport)
#else
#define ABL_API __declspec(dllimport)
#endif

class TaskHelpModel;
class ConnectionManager;
class DeviceInfoManager;
class ApplicationManager;
class CStatistics;
class NetInterfaceManager;
class StatisticHelp;
class ABL_API Cabl 
{
public:
	static Cabl* GetInstance();
	static void Release() { SAFE_DELETE(_pInstance); }

	//��ʼ��
	bool Init();

private:
	Cabl(void);
	~Cabl(void);

	static Cabl* _pInstance;

	ConnectionManager* _pConnectionManager;
	DeviceInfoManager* _pDeviceInfoManager;
	ApplicationManager*  _pApplicationManager; // �������
	CStatistics* _statistics;
	NetInterfaceManager* _pNetInterfaceManager;//������ع���
	StatisticHelp* _pStatisticHelp;
	TaskHelpModel* _pTaskHelpModel;
};