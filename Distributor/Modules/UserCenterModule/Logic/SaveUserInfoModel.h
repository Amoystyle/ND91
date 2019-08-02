#pragma once	 
#include "Common/UserCenter/UserCenterMsg.h"
#include "Common/Device/DeviceMessage.h"

class AblDeviceInfo;
class SaveUserInfoModel 
	: public BaseHandler<RequestUserInfoMsg> 
	, public BaseHandler<RequestSaveUserInfoMsg>
	, public BaseHandler<ResponseDeviceImageMsg>
	, public BaseGetData<GetUserLoginInfoMsg>
{
public:
    SaveUserInfoModel();
    ~SaveUserInfoModel();

	// 发送客户资料
	bool SendUserInfo(wstring name, wstring userTel, wstring comment);

	bool GetImgPath( wstring& strPath, bool& bIspad );

private:

	virtual bool Handle(const RequestUserInfoMsg* pMsg);

	virtual bool Handle(const RequestSaveUserInfoMsg* pMsg);

	virtual bool Handle( const ResponseDeviceImageMsg* pMsg );

	virtual bool Handle( GetUserLoginInfoMsg* pMsg );

	void ReleaseData();

private:

	bool							_bIsPad;
	wstring							_strDeviceID;
	
	map<wstring, wstring>			_mapImgPath;
	map<wstring, AblDeviceInfo*>	_mapDeviceData;

};