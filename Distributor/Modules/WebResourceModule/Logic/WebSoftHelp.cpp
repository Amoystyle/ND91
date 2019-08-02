#include "stdafx.h"
#include "WebSoftHelp.h"

WebSoftHelp::WebSoftHelp(PlatFrom platType)
{
	_platType = platType;
}

WebSoftHelp::~WebSoftHelp()
{

}

bool WebSoftHelp::Pare24HotSoftDatas( std::string str, vector<WebSoftData*>& pVerDatas )
{
	RETURN_FALSE_IF(str.empty())
	cJSON* pRoot = cJSON_Parse(str.c_str());
	RETURN_FALSE_IF(!pRoot);

	int arraySize = cJSON_GetArraySize(pRoot);
	for(int i=0; i < arraySize; ++i)
	{
		cJSON * pItem = cJSON_GetArrayItem(pRoot, i);
		if(!pItem)continue;
		WebSoftData* p = PareData(pItem);
		if(!p)continue;
		p->_appType=HOT24APP;
		pVerDatas.push_back(p);
	}

	return true;	
}

bool WebSoftHelp::PareSoftDatas( std::string str, vector<WebSoftData*>& pVerDatas, int& total, bool isSearch)
{
	total=0;
	RETURN_FALSE_IF(str.empty());

	cJSON* pJson = cJSON_Parse(str.c_str());
	RETURN_FALSE_IF ( !pJson );
	
	cJSON *pChild = cJSON_GetObjectItem(pJson, "code");
	RETURN_FALSE_IF ( !pChild );
	
	if (pChild->type == cJSON_String)
	{
		std::string strTemp;
		strTemp.assign(pChild->valuestring, strlen(pChild->valuestring));
		RETURN_FALSE_IF ( "0" != strTemp );
	}
	else if ( pChild->type == cJSON_Number )
	{
		RETURN_FALSE_IF ( 0 != pChild->valueint );
	}


	total = CjsonNumber(pJson, "cnt");

	cJSON* pRoot = cJSON_GetObjectItem(pJson, "data");
	RETURN_FALSE_IF (!pRoot);

	int arraySize = cJSON_GetArraySize(pRoot);
	for(int i=0; i < arraySize; ++i)
	{
		cJSON * pItem = cJSON_GetArrayItem(pRoot, i);
		WebSoftData* p = PareData(pItem, isSearch);
		pVerDatas.push_back(p);
	}
	if(total==0)total=pVerDatas.size();
	return true;
}

bool WebSoftHelp::PareCategoryData( std::string str, vector<WebSoftCategory*>& pVerCats )
{
	RETURN_FALSE_IF(str.empty())
	cJSON* pRoot = cJSON_Parse(str.c_str());

	RETURN_FALSE_IF (!pRoot);
	int arraySize = cJSON_GetArraySize(pRoot);
	for(int i=0; i < arraySize; ++i)
	{
		cJSON * pItem = cJSON_GetArrayItem(pRoot, i);
		if ( pItem && pItem->type == cJSON_Object )
		{
			WebSoftCategory* p = new WebSoftCategory;

			p->_id = CjsonNumber(pItem, "c_id");

			p->_name = CjsonString(pItem,"c_name");

			p->_imgSrc = CjsonString(pItem,"c_imgsrc");

			p->_sortNum = CjsonNumber(pItem, "c_sortnum");

			pVerCats.push_back(p);
		}
	}
	return true;
}

WebSoftData* WebSoftHelp::PareAppData( std::string str )
{
	RETURN_FALSE_IF(str.empty())
	cJSON* pRoot = cJSON_Parse(str.c_str());

	RETURN_FALSE_IF (!pRoot);

	return PareData(pRoot);
}


wstring WebSoftHelp::CjsonString(cJSON* pRoot,std::string key)
{
	if (!pRoot)
		return _T("");
	cJSON *pChild = cJSON_GetObjectItem(pRoot, key.c_str());
	if(pChild&& pChild->type == cJSON_String)
	{
		std::string data(pChild->valuestring, strlen(pChild->valuestring));
		return CCodeOperation::UTF_8ToUnicode(data);
	}
	return _T("");
}

int WebSoftHelp::CjsonNumber(cJSON* pRoot,std::string key)
{
	if (!pRoot)
		return 0;

	cJSON *pChild = cJSON_GetObjectItem(pRoot, key.c_str());
	if ( pChild && pChild->type == cJSON_Number )
		return pChild->valueint;

	return 0;
}

WebSoftData* WebSoftHelp::PareData( cJSON* pRoot, bool isSearch )
{
	
	RETURN_NULL_IF ( !pRoot || pRoot->type != cJSON_Object );
		
	WebSoftData* p = new WebSoftData;

	p->_id = CjsonNumber(pRoot, "f_id");

	p->_name = CjsonString(pRoot,"alias");

	if(p->_name.empty())
		p->_name = CjsonString(pRoot,"f_name");

	p->_namesLug = CjsonString(pRoot,"f_name_slug");

	p->_size = CjsonNumber(pRoot, "f_size");

	p->_imgSrc = CjsonString(pRoot,"f_imgsrc");

	p->_summary = CjsonString(pRoot,"f_summary");

	p->_desc = CjsonString(pRoot,"desc");

	p->_cateid = CjsonNumber(pRoot, "f_cateid");
	
	p->_categoryName = CjsonString(pRoot,"f_category_name");

	p->_identifier = CjsonString(pRoot,"f_identifier");

	p->_version = CjsonString(pRoot,"f_version");

	p->_addTime = CjsonString(pRoot,"f_add_time");

	cJSON *pChild = cJSON_GetObjectItem(pRoot, "f_platform");
	if ( pChild && pChild->type == cJSON_Number)
		p->_bIpad = pChild->valueint == 7 ? true : false;

	p->_bIpad = _platType == IPAD;

	p->_demand = CjsonString(pRoot,"url2");

	p->_downNum = CjsonNumber(pRoot, "f_downnum");

	p->_ratingScore = CjsonNumber(pRoot, "f_ratingscore");

	p->_ratingNum = CjsonNumber(pRoot, "f_ratingnum");

	p->_author = CjsonString(pRoot,"f_author");

	pChild = cJSON_GetObjectItem(pRoot, "f_isgrant");
	if ( pChild )
	{
		bool is = true ? false : pChild->type == cJSON_True;
	}

	pChild = cJSON_GetObjectItem(pRoot, "f_ischinese");
	if ( pChild )
	{
		bool is = true ? false : pChild->type == cJSON_True;
	}

	pChild = cJSON_GetObjectItem(pRoot, "ipa");
	if ( pChild )
	{
		p->_isIpa = true ? false : pChild->type == cJSON_True;
	}

	p->_Price = CjsonNumber(pRoot, "Price");

	if ( isSearch )
	{
		p->_downUrl = "http://itunesapp2.sj.91.com/xmlController.ashx?action=dostat&f_id=" + CStrOperation::Int2string(p->_id);
		p->_downUrl	+= "&price="		+ CStrOperation::Int2string(p->_Price);
		p->_downUrl	+= "&f_name="		+ CNetOperation::URLEncode(CCodeOperation::UnicodeToUTF_8(p->_name));
		p->_downUrl	+= "&f_version="	+ CCodeOperation::UnicodeToUTF_8(p->_version);
		p->_downUrl	+= "&f_img="		+ CCodeOperation::UnicodeToUTF_8(p->_imgSrc);
		p->_downUrl	+= "&f_identifier=" + CCodeOperation::UnicodeToUTF_8(p->_identifier);
		p->_downUrl	+= "&f_summary="	+ CNetOperation::URLEncode(CCodeOperation::UnicodeToUTF_8(p->_summary));
		p->_downUrl += "&secure=SAFE&ext=ipa";
	}
	else
	{
		pChild = cJSON_GetObjectItem(pRoot, "f_downurl");
		if ( pChild && pChild->type == cJSON_String )
		{
			std::string data(pChild->valuestring, strlen(pChild->valuestring));
			p->_downUrl = data;
		}
	}


	p->_categoryName = CjsonString(pRoot,"caten");
	
	return p;
}

void WebSoftHelp::GetDescPicture( std::string str, vector<std::string>& verPics  )
{
	int pos1=0, endpos1=0, pos2=0, endpos2=0;
	std::string temp;

	pos1 = str.find("<img src=");
	endpos1 = str.find(">");

	if ( pos1 >= 0 && endpos1 >0 )
	{
		temp = str.substr(pos1+9, endpos1-pos1-10);
		verPics.push_back( temp );
	}

	pos2 = str.find("<img src=", endpos1);
	endpos2 = str.find_first_of(">", pos2);

	if ( pos2 > 0 && endpos2 >0 )
	{
		temp = str.substr(pos2);
		GetDescPicture(temp, verPics);
	}
	else
		return ;
		 
}
