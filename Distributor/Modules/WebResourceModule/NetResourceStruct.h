#pragma once

/*******************************************************************************
�ļ����壺���������Դ�����ݽṹ����
*******************************************************************************/

enum ENUM_URL_OPREATE //������Դ�ϲ�����ҳ�Ĳ���
{

	Etype_UrlOperate_None,
	Etype_UrlOperate_Forword, //��ǰ
	Etype_UrlOperate_Back,    //���
	Etype_UrlOperate_Reflash, //ˢ��
	Etype_UrlOperate_Home     //��ҳ
};

//��Դվ��ṹ

enum ENUM_PLATFORM
{
	PLATFORM_Android,
	PLATFORM_Iphone,
	PLATFORM_Ipad,
};

// �豸����ϵͳ���� - �Ժ��ϲ���������滻��ABI��DDM���������
enum NetEnumPhoneOS
{
	ePhoneOS_None_Net = -1,     //������ϵͳ
	ePhoneOS_Android_Net = 0,
	ePhoneOS_Ios_Net = 1,       
	ePhoneOS_IosJailBreak = 0x0101,     // ��Խ��
	ePhoneOS_IosUnJailBreak = 0x0102,   // δԽ��
};

enum NetEnumAppSort  //����--Ӧ�÷���
{
	Etype_Application,  //Ӧ��
	Etype_Game,			//��Ϸ
	Etype_Theme,		//����
	Etype_Music,		//����
	Etype_Wallpaper,	//��ֽ
	Etype_Video,		//��Ƶ
	Etype_Ebook,		//������
	Etype_Phonelife	    //�ֻ�����
};


struct NetNode
{
	NetNode()
	{
		nodeId = L"";
		nodeName = L"";
		nodeIcon = L"";
		initUrl = L"";
		nodeUrl = L"";
		searchUrl = L"";
		isselect = false;
		ieid = L"";
		tagicon = L"";
		isshow = false;
		statid = L"";
		isPartner = false;
		strValue = L"";
	}

	NetNode(const NetNode& node)
	{
		nodeId = node.nodeId;
		nodeName = node.nodeName;
		nodeIcon = node.nodeIcon;
		initUrl = node.initUrl;
		nodeUrl = node.nodeUrl;
		searchUrl = node.searchUrl;
		isselect = node.isselect;
		ieid = node.ieid;
		tagicon = node.tagicon;
		isshow = node.isshow;
		statid = node.statid;
		isPartner = node.isPartner;
		strValue = node.strValue;
	}

	NetNode& operator=(const NetNode& node)
	{
		nodeId = node.nodeId;
		nodeName = node.nodeName;
		nodeIcon = node.nodeIcon;
		initUrl = node.initUrl;
		nodeUrl = node.nodeUrl;
		searchUrl = node.searchUrl;
		isselect = node.isselect;
		ieid = node.ieid;
		tagicon = node.tagicon;
		isshow = node.isshow;
		statid = node.statid;
		isPartner = node.isPartner;
		strValue = node.strValue;
		return *this;
	}
	tstring nodeId;		//�ӽڵ�Id
	tstring nodeName;	//�ӽڵ�����
	tstring nodeIcon;	//�ӽڵ�ͼ��
	wstring initUrl;	//��ʼ��ַ
	tstring nodeUrl;	//�ӽڵ㵼����ַ
	tstring searchUrl;	//�ӽڵ�������ַ
	bool isselect;		//�Ƿ�ѡ��
	tstring ieid;		//���õ�IE�ؼ���id
	tstring tagicon;	//��չ��ͼ��
	bool isshow;		//�Ƿ�Ҫ��ʾ
	tstring statid;		//����ͳ��Id
	bool isPartner;		//�Ƿ����վ�㣨Ĭ���ǣ�
	tstring strValue;   //��Ӧ����˵�valueֵ
};

struct NetSite
{
	NetSite()
	{
		siteId = L"";
		siteName = L"";
		layoutIndex = 0;
		isJailbreak = false;
		platform = ePhoneOS_None_Net;
	}
	NetSite(const NetSite& site)
	{
		siteId = site.siteId;
		siteName = site.siteName;
		layoutIndex = site.layoutIndex;
		isJailbreak = site.isJailbreak;
		platform    = site.platform;
		for (vector<NetNode>::const_iterator iter = site.nodes.begin(); iter != site.nodes.end(); iter++)
		{
			NetNode nodeTmp = (*iter);
			NetNode node(nodeTmp);
			nodes.push_back(node);
		}
	}

	NetSite& operator=(const NetSite& site)
	{
		siteId = site.siteId;
		siteName = site.siteName;
		layoutIndex = site.layoutIndex;
		isJailbreak = site.isJailbreak;
		for (vector<NetNode>::const_iterator iter = site.nodes.begin(); iter != site.nodes.end(); iter++)
		{
			NetNode nodeTmp = (*iter);
			NetNode node(nodeTmp);
			nodes.push_back(node);
		}
		return *this;
	}
	tstring siteId;
	tstring siteName;
	int layoutIndex;
	NetEnumPhoneOS platform;
	bool isJailbreak;
	vector<NetNode> nodes;
};

struct NetModuleData
{
	NetModuleData()
	{
		moduleId = L"";
	}
	tstring moduleId;
	NetEnumPhoneOS platform;
	vector<NetSite> sites;
};

struct NetResultData
{
    NetResultData()
    {
        //isSuccess = false;
        version = 0;
    };
	//bool isSuccess;
	int version;
	vector<NetModuleData> moduledatas;
};

//��Դģ��ṹ

struct NetModuleStruct
{
	tstring moduleid;
	tstring modulename;
	tstring moduleicon;
	int layoutindex;
};

struct NetResultModule
{
	bool isSuccess;
	tstring version;
	vector<NetModuleStruct> modules;
};

enum NetNodeType
{
	NetNodeTypeBegin,
	ItunesApp2 = NetNodeTypeBegin,//��Խ�����װ
	Index,						  //װ���ر�
	Search,						  //����
	News,					      //������Ѷ
	Cpa,                          //Cpa
	NetNodeTypeEnd
};

enum ButonStatus
{
	CpaButonStatus_NONE,			//��
	CpaButonStatus_DOWNLOAD,		//����
	CpaButonStatus_DOWNLOADFINISH,  //������
	CpaButonStatus_INSTALL			//��װ
};

enum CPaDispatcherID
{
	HomeWebCpa
};
