#pragma once


namespace ND91Assistant
{

	class CreateMomeCommand:public BaseCommand
	{
		public:
			CreateMomeCommand(const DeviceData* pDevice);
			virtual ~CreateMomeCommand(void);

			// ִ������
			virtual void Execute();


			int CheckPHInstalled();


			int SetPCTheme(wstring themepathOnPhone);

			int GetThemeDataPath(map<wstring, wstring>& themepathname);

	protected:

			// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
			virtual std::string GetAndroidBodyData(){return "";}

			// �����������ݰ�
			virtual void SetAndroidReturnData(std::string data){;}


			virtual void SetIosReturnFile(std::wstring filename);

			// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
			// Ĭ�Ϸ���IOS_COMMOND_NORMAL_FOLDER·���£�����ԭ�����ļ�����CreatDaemonCypher���м��ܴ���
			virtual std::wstring GetIosCommandFilePath();

			// ����ios�ļ����ݡ�Ĭ��Ϊ��
			virtual std::wstring GetIosCommandFileContent();


			int CreateMomeError();

	private:

		string  _momeCmd;

		wstring  _ios_local_resultFile;
	};

}
