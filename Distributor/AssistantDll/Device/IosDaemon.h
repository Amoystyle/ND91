#pragma once

#include "IIosFileSystem.h"

namespace ND91Assistant
{
	class IosDaemon
	{
	public:
		IosDaemon(IIosFileSystem* pFileStream, wstring strUUID);
		~IosDaemon(void);

		//检查守护是否在运行  若在则检测守护是否开启交互
		bool PhoneDaemonIsStillRunning();

		bool GetIsTrunCypher(){return _bTurnCypher;}
		wstring GetDaemonCypherBytes(){return _szDaemonCypherBytes;}

		//创建交互指令,构造函数参数均为空，直接调用即可
		//strTalk : 需要创建的指令
		//bTurnCypher : 为DevInfo里的_ios_devicebTurnCypher
		//szDaemonCypherBytes : 为DevInfo里的_ios_deviceszDaemonCypherBytes.c_str()，无需编码转换
		wstring CreatDaemonCypher(wstring strTalk, bool bTurnCypher, wstring strDaemonCypherBytes);

		wstring GetDaemonVer();

	private:
		//获取交互口令，返回守护是否开启交互
		bool GetDaemonCypher();

		string ReadDaemonCypher(string iTalk, int nLen);

	private:
		IIosFileSystem* _pFileStream;
		bool _bTurnCypher; //守护是否开启交互
		wstring _szDaemonCypherBytes; //交互口令
		wstring _strUUID; //设备唯一ID
	};
}