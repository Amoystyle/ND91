#pragma once

namespace ND91Assistant 
{
	// 解析并记录当前APK的信息
	class ApkData
	{
	public:
		ApkData();
		ApkData(wstring strApkPath);
		~ApkData();
		
	public:
		wstring PackageName;		//包名
		wstring Version;			//版本
		wstring VersionCode;		//版本号
		wstring FormatFileSize;		//文件大小
		wstring SDKVersion;			//SDK要求版本好
		wstring AppName;			//程序名称
		bool DangerPermission;		//是否有危险权限
		wstring APKFilePath;		//文件路径

	private:
		void parseData(string strData);
	};
}