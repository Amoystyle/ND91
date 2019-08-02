#pragma once

//功能大全列表

struct FunctionItem 
{
	wstring st_ID;
	wstring st_Name;//功能名字
	wstring st_IconDownPath;//图片下载地址
	wstring st_IconSavePath;//图片保存路径
	wstring st_InstallPkDownPath;//安装包下载地址
	int     st_SuitPCPlate;//使用运行的pc平台, 0:all; 1:xp; 2:win7
	wstring st_wsExeBinPath;//运行的bin路径
	wstring st_FileSize;//文件大小
	wstring st_Desc;//文件描述
	wstring st_FileVersion;//版本号
};

//主动发送功能大全
struct ReponsePcSuitFunctionMsg: BaseMessage<ReponsePcSuitFunctionMsg>
{
	wstring wsPlatform;//平台
	vector<FunctionItem> vec_Function;
};

//请求功能大全列表
struct GetPcSuitFunctionMsg: BaseMessage<GetPcSuitFunctionMsg>
{
	wstring wsPlatform;//平台
	vector<FunctionItem> vec_Function;
};


//下载任务ID
struct GetDownloadFileReportTaskIDMsg: BaseMessage<GetDownloadFileReportTaskIDMsg>
{
	GetDownloadFileReportTaskIDMsg():unTaskID(0)
	{

	}
	UINT unTaskID;
};

//下载文件
struct DownloadFileReportMsg: BaseMessage<DownloadFileReportMsg>
{
	DownloadFileReportMsg()
	{
		unTaskID = 0;
	}
	wstring wsDownPath;
	wstring wsSavePath;
	UINT    unTaskID;
};


//下载任务进度信息
struct DownloadInfo
{
	DownloadInfo()
	{
		unTaskid = 0;
		unCurDownSize = 0;
		unAllSize = 0;
	}
	UINT unTaskid;
	UINT unCurDownSize;
	UINT unAllSize;
};

//下载进度信息
struct ReponseDownloadReportMsg: BaseMessage<ReponseDownloadReportMsg>
{
	ReponseDownloadReportMsg():bDownloadSuccess(false)
	{

	}
	bool bDownloadSuccess;
	DownloadInfo st_DoenloadInfo;
};
