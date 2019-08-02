#pragma once

namespace ND91Assistant 
{
	// ��������¼��ǰAPK����Ϣ
	class ApkData
	{
	public:
		ApkData();
		ApkData(wstring strApkPath);
		~ApkData();
		
	public:
		wstring PackageName;		//����
		wstring Version;			//�汾
		wstring VersionCode;		//�汾��
		wstring FormatFileSize;		//�ļ���С
		wstring SDKVersion;			//SDKҪ��汾��
		wstring AppName;			//��������
		bool DangerPermission;		//�Ƿ���Σ��Ȩ��
		wstring APKFilePath;		//�ļ�·��

	private:
		void parseData(string strData);
	};
}