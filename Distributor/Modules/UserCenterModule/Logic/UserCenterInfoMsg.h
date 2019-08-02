#pragma once

//�û���¼�ɹ�
struct UserLoginInfoMsg : public BaseMessage<UserLoginInfoMsg>
{
	bool		bLongin;		// �û��Ƿ��ѵ���
	wstring		userName;		// �û���	
	wstring		userID;			// �û�ID
	int			nCpaUser;		// 1Ϊcpa�û���0Ϊ��ͨ�û�
	std::string	channelID;

	UserLoginInfoMsg ()
	{
		bLongin		= false;
		userName	= L"";
		userID		= L"";	
		nCpaUser	= 0;
		channelID	= "";		
	}
};

//���������Ƿ�������Ϣ
struct CheckConnectInfoMsg : public BaseMessage<CheckConnectInfoMsg>
{
	bool bConnectOK;

	CheckConnectInfoMsg ()
	{
		bConnectOK = false;
	}

};

//��ȡ�û���Ϣ
struct GetCustomerInfoMsg : public BaseMessage<GetCustomerInfoMsg>
{
	wstring userName;
	wstring phoneNum;
	wstring comment;
	wstring strSaveTime;

	GetCustomerInfoMsg ()
	{
		userName	= L"";
		phoneNum	= L"";
		comment		= L"";
		strSaveTime = L"";
	}
};

//�����û���Ϣ
struct SendCustomerInfoMsg : public BaseMessage<SendCustomerInfoMsg>
{
	wstring strSaveTime;

	SendCustomerInfoMsg ()
	{
		strSaveTime = L"";
	}
};

