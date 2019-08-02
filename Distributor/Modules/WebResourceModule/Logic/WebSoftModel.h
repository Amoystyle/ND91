#pragma once
#include "WebSoftInfo.h"



class WebSoftModel
{
public:
	WebSoftModel(PlatFrom platType );
    ~WebSoftModel();
	
	// Summary  : ��ȡ24Сʱ����Ӧ���б�
	// Returns  : bool
	// Parameter: int total				�������ܸ���
	bool Get24HoursHotAppList();


	// Summary  : ��ȡ��Ϸ������������б�
	// Returns  : bool
	// Parameter: bool isGame			�Ƿ�����Ϸ
	bool GetSoftCategoryList(bool isGame);


	// Summary  : ��ȡ��������б�,����ҳ
	// Returns  : bool
	// Parameter: int categoryId	����id
	// Parameter: int size			ÿҳ����
	// Parameter: int page			ҳ��
	// Parameter: int sortType		����ʽ
	bool GetAppDataList( int categoryId, int count,int page, std::string sortType );


	// Summary  : ��ȡ��Ϸ�����б�
	// Returns  : bool
	// Parameter: int categoryId	����id
	// Parameter: int count			ÿҳ����
	// Parameter: int page			ҳ��
	// Parameter: int sortType		����ʽ
	bool GetGameDataList( int categoryId, int count,int page, std::string sortType );


	// Summary  : ��ȡ���������ϸ��Ϣ		
	// Returns  : void*
	// Parameter: std::string appId			Ӧ��id
	bool GetOneAppInfo( int appId );


	// Summary  : ��������ӿ�
	// Returns  : bool
	// Parameter: wstring categoryId	�����ؼ���
	// Parameter: int count				ÿҳ����
	// Parameter: int page				ҳ��
	bool SearchAppList(	wstring strKeyWord, int count,int page );


	// Summary  : ��ȡӦ�ý�����ʾ��ͼ��
	// Returns  : bool
	bool GetAppsIconFromWeb( vector<int> fids, vector<wstring> urls );



	// Summary  : UI��ȡӦ�����ݼ��ϵ�ͨ�ýӿ� (�����е������ɵ������ͷ�)
	// Returns  : bool
	// Parameter: vector<WebSoftData * > & verDatas			��ҳ������
	// Parameter: int& total								�÷���������	
	bool GetAppDatas(vector<WebSoftData*>& verDatas, int& total);
	// Summary  : UI��ȡӦ�÷��༯�ϵ�ͨ�ýӿ� (�����е������ɵ������ͷ�)
	// Returns  : bool
	// Parameter: vector<WebSoftCategory * > & verDatas
	bool GetCategoryDatas(vector<WebSoftCategory*>& verCats);



private:

// 	virtual bool Action(int nAction, BaseParam* pParam);  
// 	bool GetHotApps(BaseParam* pParam);
// 	bool GetSoftCategorys(BaseParam* pParam);
// 	bool GetAppDatas(BaseParam* pParam);
// 	bool GetGameDatas(BaseParam* pParam);
// 	bool GetSoftInfo(BaseParam* pParam);
// 	bool SearchApps(BaseParam* pParam);
// 	bool DownLoadApp(BaseParam* pParam);
// 	bool GetAppIcon(BaseParam* pParam);


private:

	std::string			_postData;			// ��������
	std::string			_CatsUrl;			// ��������
	std::string			_platForm;
	PlatFrom			_platType;

	int							_total;		// �÷��������Ӧ����
 	vector<WebSoftData*>		_VerDatas;	// ���������б�
 	vector<WebSoftCategory*>	_VerCats;	// ���������б�

	
	CRITICAL_SECTION   _slock;

};