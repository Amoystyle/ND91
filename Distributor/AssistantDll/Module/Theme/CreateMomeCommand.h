#pragma once


namespace ND91Assistant
{

	class CreateMomeCommand:public BaseCommand
	{
		public:
			CreateMomeCommand(const DeviceData* pDevice);
			virtual ~CreateMomeCommand(void);

			// 执行命令
			virtual void Execute();


			int CheckPHInstalled();


			int SetPCTheme(wstring themepathOnPhone);

			int GetThemeDataPath(map<wstring, wstring>& themepathname);

	protected:

			// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
			virtual std::string GetAndroidBodyData(){return "";}

			// 解析返回数据包
			virtual void SetAndroidReturnData(std::string data){;}


			virtual void SetIosReturnFile(std::wstring filename);

			// 命令传送到IOS设备上的文件名（全路径, unicode）
			// 默认放在IOS_COMMOND_NORMAL_FOLDER路径下，并对原命令文件名用CreatDaemonCypher进行加密处理
			virtual std::wstring GetIosCommandFilePath();

			// 生成ios文件内容。默认为空
			virtual std::wstring GetIosCommandFileContent();


			int CreateMomeError();

	private:

		string  _momeCmd;

		wstring  _ios_local_resultFile;
	};

}
