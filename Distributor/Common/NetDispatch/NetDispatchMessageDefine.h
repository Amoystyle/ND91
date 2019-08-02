#pragma once

//���ܴ�ȫ�б�

struct FunctionItem 
{
	wstring st_ID;
	wstring st_Name;//��������
	wstring st_IconDownPath;//ͼƬ���ص�ַ
	wstring st_IconSavePath;//ͼƬ����·��
	wstring st_InstallPkDownPath;//��װ�����ص�ַ
	int     st_SuitPCPlate;//ʹ�����е�pcƽ̨, 0:all; 1:xp; 2:win7
	wstring st_wsExeBinPath;//���е�bin·��
	wstring st_FileSize;//�ļ���С
	wstring st_Desc;//�ļ�����
	wstring st_FileVersion;//�汾��
};

//�������͹��ܴ�ȫ
struct ReponsePcSuitFunctionMsg: BaseMessage<ReponsePcSuitFunctionMsg>
{
	wstring wsPlatform;//ƽ̨
	vector<FunctionItem> vec_Function;
};

//�����ܴ�ȫ�б�
struct GetPcSuitFunctionMsg: BaseMessage<GetPcSuitFunctionMsg>
{
	wstring wsPlatform;//ƽ̨
	vector<FunctionItem> vec_Function;
};


//��������ID
struct GetDownloadFileReportTaskIDMsg: BaseMessage<GetDownloadFileReportTaskIDMsg>
{
	GetDownloadFileReportTaskIDMsg():unTaskID(0)
	{

	}
	UINT unTaskID;
};

//�����ļ�
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


//�������������Ϣ
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

//���ؽ�����Ϣ
struct ReponseDownloadReportMsg: BaseMessage<ReponseDownloadReportMsg>
{
	ReponseDownloadReportMsg():bDownloadSuccess(false)
	{

	}
	bool bDownloadSuccess;
	DownloadInfo st_DoenloadInfo;
};
