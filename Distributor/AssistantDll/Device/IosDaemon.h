#pragma once

#include "IIosFileSystem.h"

namespace ND91Assistant
{
	class IosDaemon
	{
	public:
		IosDaemon(IIosFileSystem* pFileStream, wstring strUUID);
		~IosDaemon(void);

		//����ػ��Ƿ�������  ���������ػ��Ƿ�������
		bool PhoneDaemonIsStillRunning();

		bool GetIsTrunCypher(){return _bTurnCypher;}
		wstring GetDaemonCypherBytes(){return _szDaemonCypherBytes;}

		//��������ָ��,���캯��������Ϊ�գ�ֱ�ӵ��ü���
		//strTalk : ��Ҫ������ָ��
		//bTurnCypher : ΪDevInfo���_ios_devicebTurnCypher
		//szDaemonCypherBytes : ΪDevInfo���_ios_deviceszDaemonCypherBytes.c_str()���������ת��
		wstring CreatDaemonCypher(wstring strTalk, bool bTurnCypher, wstring strDaemonCypherBytes);

		wstring GetDaemonVer();

	private:
		//��ȡ������������ػ��Ƿ�������
		bool GetDaemonCypher();

		string ReadDaemonCypher(string iTalk, int nLen);

	private:
		IIosFileSystem* _pFileStream;
		bool _bTurnCypher; //�ػ��Ƿ�������
		wstring _szDaemonCypherBytes; //��������
		wstring _strUUID; //�豸ΨһID
	};
}