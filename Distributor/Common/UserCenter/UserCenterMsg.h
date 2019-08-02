#pragma once

//�û���¼�ɹ�
struct ResponseUserLoginMsg : public BaseMessage<ResponseUserLoginMsg>
{
	bool		bLongin;		// �û��Ƿ��ѵ���
	wstring		userName;		// �û���	
	wstring		userID;			// �û�ID
	int			nCpaUser;		// 1Ϊcpa�û���0Ϊ��ͨ�û�
	std::string	channelID;

	ResponseUserLoginMsg ()
	{
		bLongin		= false;
		userName	= L"";
		userID		= L"";	
		nCpaUser	= 0;
		channelID	= "";		
	}
};

//��ȡ�û���Ϣ
struct RequestUserInfoMsg : public BaseMessage<RequestUserInfoMsg>
{
	wstring deviceID;
};

//�����û���Ϣ
struct ResponseUserInfoMsg : public BaseMessage<ResponseUserInfoMsg>
{
	wstring deviceID;	// �û��˳�����������û�����ʱ����������
	wstring lastTime;
};

//�����û���Ϣ
struct RequestSaveUserInfoMsg : public BaseMessage<RequestSaveUserInfoMsg>
{
	wstring deviceID;
};

//�û���¼�ɹ�
struct GetUserLoginInfoMsg : public BaseMessage<GetUserLoginInfoMsg>
{
	wstring		userId;
	bool		bLongin;		// �û��Ƿ��ѵ���
	bool		bCpaUser;		// 1Ϊcpa�û���0Ϊ��ͨ�û�
	std::string	channelID;

	GetUserLoginInfoMsg ()
	{
		userId		= L"";
		bLongin		= false;
		bCpaUser	= false;
		channelID	= "";		
	}
};

//��ʼ�����ػ�������
struct BeginDownLoadDeamonMsg : public BaseMessage<BeginDownLoadDeamonMsg>
{
	
};

//��ʼ��װ�ػ�������
struct BeginInstallDeamonMsg : public BaseMessage<BeginInstallDeamonMsg>
{
	wstring deviceID;
	bool	bAppSync;

	BeginInstallDeamonMsg ()
	{
		deviceID = L"";
		bAppSync = false;	
	}
};

//�ػ���װ��ɷ���
struct RequestInstallDeamonOverMsg : public BaseMessage<RequestInstallDeamonOverMsg>
{
	wstring deviceID;           // �豸ID
};
