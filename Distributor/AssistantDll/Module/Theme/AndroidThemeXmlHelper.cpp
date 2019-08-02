#include "pch_module.h"

#ifdef MODULE_THEME

#include "AndroidThemeXmlHelper.h"

const wchar_t* themeinfos[]=
{
	L"name",
	L"en_name",
	L"ext",
	L"version",
	L"textsize",
	L"textcolor",
	L"textbackcolor",
	L"ptextsize",
	L"ptextcolor",
	L"ptextbackcolor",
	L"wallpaper",
	L"pwallpaper",
	L"keyconfig"
};

const wchar_t* APPIDS[]=
{
	L"com.android.contacts.DialtactsActivity", //打电话                                        
	L"com.android.contacts.DialtactsContactsEntryActivity", //联系人                                        
	L"com.android.browser.BrowserActivity",//浏览器                                       
	L"com.google.android.maps.MapsActivity", //地图                                        
	L"clock_dial", //闹钟主图                                           
	L"clock_hour",
	L"clock_minute"
};

AndroidThemeXmlHelper::AndroidThemeXmlHelper(void)
{
}

AndroidThemeXmlHelper::~AndroidThemeXmlHelper(void)
{
}

ThemeData* ND91Assistant::AndroidThemeXmlHelper::ResolveAllThemes( wstring localpath, deque<ThemeData*>* themeList )
{
	wstring  curthemeId =L"";
	ThemeData* curTheme = NULL;
	string strData = CFileOperation::ReadFile(localpath, false);
	//wstring wstrData = CCodeOperation::UTF_8ToUnicode(strData);

	xml_document<char> doc;
	doc.parse<0>(const_cast<char *>(strData.c_str()));

	xml_node<char> * themeinfo = doc.first_node("themeinfo");
	while(themeinfo)
	{
		xml_node<char> * curtheme = themeinfo->first_node("currenttheme");
		if (curtheme)
			curthemeId = CCodeOperation::UTF_8ToUnicode(curtheme->value());
		xml_node<char> * themenode = themeinfo->first_node("theme");
		if(themenode)
		{
			ThemeData* themedata = ResolveDetailTheme(themenode);
			if(themedata)
			{
				themeList->push_back(themedata);
				if ( curthemeId == themedata->_id )
					curTheme = themedata;
			}
		}
		themeinfo = themeinfo->next_sibling();
	}

	return curTheme;
}

ThemeData* ND91Assistant::AndroidThemeXmlHelper::ResolveSmartThemes( wstring localpath, deque<ThemeData*>* themeList )
{
	wstring  curthemeId =L"";
	ThemeData* curTheme = NULL;
	string strData = CFileOperation::ReadFile(localpath, false);
	//wstring wstrData = CCodeOperation::UTF_8ToUnicode(strData);

	xml_document<char> doc;
	doc.parse<0>(const_cast<char *>(strData.c_str()));

	xml_node<char> * themeinfo = doc.first_node("themeinfo");
	RETURN_NULL_IF(!themeinfo);
	xml_node<char> * curtheme = themeinfo->first_node("currenttheme");
	if (curtheme)
		curthemeId = CCodeOperation::UTF_8ToUnicode(curtheme->value());
	xml_node<char> * themenode = themeinfo->first_node("themelist");
	RETURN_NULL_IF(!themenode);
	themeinfo = themenode->first_node("themeinfo");
	while(themeinfo)
	{
		ThemeData*  newtheme = new ThemeData();
		newtheme->_themeType = ThemeType_SmartHome;
		xml_node<char> * item = themeinfo->first_node();
		while (item)
		{
			wstring name = CCodeOperation::UTF_8ToUnicode(item->name());
			wstring itemvalue = CCodeOperation::UTF_8ToUnicode(item->value());
			if(name == L"id")
				newtheme->_id=itemvalue;
			if(name == L"name")
				newtheme->_name = itemvalue;
			item = item->next_sibling();
		}
		themeList->push_back(newtheme);
		if ( curthemeId == newtheme->_id )
			curTheme = newtheme;
		themeinfo = themeinfo->next_sibling();
	}
	return curTheme;
}

string ND91Assistant::AndroidThemeXmlHelper::urldecode(string valuestr)
{
	string result;
	for (size_t i=0; i<valuestr.length();i++)
	{
		switch(valuestr[i])
		{
			case '+':
				result += ' ';
				break;
			case '%':
				if(isxdigit(valuestr[i + 1]) && isxdigit(valuestr[i + 2]))
				{
					char res = 0;                                         
					res = (valuestr[i + 1]<='9')?valuestr[i + 1]-'0':(tolower(valuestr[i + 1])-'a')+10; 
					res <<= 4;                                            
					res |= (valuestr[i + 2]<='9')?valuestr[i + 2]-'0':(tolower(valuestr[i + 2])-'a')+10;
					i += 2;
					result+=res;
				}
				break;
			default:
				result+=valuestr[i];
				break;
		}
	}
	return result;
}

ThemeData* ND91Assistant::AndroidThemeXmlHelper::ResolveDetailTheme( wstring localPath )
{
	string strData = CFileOperation::ReadFile(localPath, false);
	RETURN_NULL_IF(strData.length()<1);
	//wstring wstrData = CCodeOperation::UTF_8ToUnicode(strData);
	
	xml_document<char> doc;
	doc.parse<0>(const_cast<char *>(strData.c_str()));
	xml_node<char> * root = doc.first_node("themeinfo");
	RETURN_NULL_IF(!root);
	xml_node<char> * item = root->first_node("theme");

	if(item)
	{
		return ResolveDetailTheme(item);
	}
	return NULL;
}


ND91Assistant::ThemeBriefInfo ND91Assistant::AndroidThemeXmlHelper::ResolveBriefTheme( wstring localPath )
{
	 ThemeBriefInfo  BriefInfo;
	string strData = CFileOperation::ReadFile(localPath, false);
	//wstring wstrData = CCodeOperation::UTF_8ToUnicode(strData);

	xml_document<char> doc;
	doc.parse<0>(const_cast<char *>(strData.c_str()));

	xml_node<char> * root = doc.first_node("themeinfo");
	if(!root)
		return BriefInfo;

	xml_node<char> * item = root->first_node("currenttheme");
	
	 BriefInfo._currentThemeId = CCodeOperation::UTF_8ToUnicode(item->value());

	 item = root->first_node("count");
	 BriefInfo._count = atoi( item->value());

	 item = root->first_node("themelist");

	 if(item)
	 {
		xml_node<char> * nodeId = item->first_node();
		while(nodeId)
		{
			string name = nodeId->name();
			 if(name == "id")
			 {
				wstring id = CCodeOperation::UTF_8ToUnicode(nodeId->value());
				BriefInfo._themeIdList.push_back(id);
			 }
			 nodeId = nodeId->next_sibling();
		}
	 }
	 return BriefInfo;
}

ThemeData* ND91Assistant::AndroidThemeXmlHelper::ResolveTotalTheme( wstring localPath )
{
	string strData = CFileOperation::ReadFile(localPath, false);
	//wstring wstrData = CCodeOperation::UTF_8ToUnicode(strData);

	wstring temppath = CFileOperation::GetFileDir(localPath);

	xml_document<char> doc;
	doc.parse<0>(const_cast<char *>(strData.c_str()));

	xml_node<char> * root = doc.first_node("theme-config");
	if (!root)
		return NULL;

	ThemeData* newTheme = new ThemeData();

	newTheme->_themeType = ThemeType_PandaHome;
	xml_attribute<char>*  attrName = root->first_attribute("name");
	newTheme->_name = CCodeOperation::UTF_8ToUnicode(attrName->value());

	attrName = root->first_attribute("id_flag");
	if(attrName)
	newTheme->_id = CCodeOperation::UTF_8ToUnicode(attrName->value());

	attrName = root->first_attribute("id");
	if(attrName)
	newTheme->_id = CCodeOperation::UTF_8ToUnicode(attrName->value());

	/*xml_node<char> * keyConfigNode = root->first_node("keyconfig");
	if(keyConfigNode)
	{
		xml_node<char>* lstAppNodes = root->first_node("app");
		xml_node<char>* appNode = lstAppNodes->first_node("intent");
		if(appNode )
		{
			string valuestr=appNode->value();
			if(valuestr=="panda_dock_sys_switch")
			{
				newTheme->_themeType=ThemeType_SmartHome;
			}
		}
	}*/



	xml_attribute<char>*  attrEnName = root->first_attribute("en_name");
	if(attrEnName)
	newTheme->_en_name = CCodeOperation::UTF_8ToUnicode(attrEnName->value());

	xml_attribute<char>*  attrver = root->first_attribute("ver");
	if(attrver)
	newTheme->_version = CCodeOperation::UTF_8ToUnicode(attrver->value());

	xml_node<char> * workspace = root->first_node("workspace");
	if(workspace)
	{
		xml_node<char> * item = workspace->first_node();
		while (item)
		{
			wstring itemvalue = CCodeOperation::UTF_8ToUnicode(item->value());
			wstring itemname = CCodeOperation::UTF_8ToUnicode(item->name());
			if(itemname == L"textcolor")
			{
				newTheme->_textColor = itemvalue;
			}
			else if(itemname == L"textsize")
			{
				newTheme->_textSize = _wtoi(itemvalue.c_str());
			}
			else if(itemname == L"textbackcolor")
			{
				newTheme->_textBackColor = itemvalue;
			}
			else if(itemname == L"background")
			{
				newTheme->_wallPaperPath = temppath +CStrOperation::ReplaceW(itemvalue, L"/",L"\\");
				newTheme->_localWallPaperPath = newTheme->_wallPaperPath; 
			}
			item = item->next_sibling();
		}
	}

	xml_node<char> * drawer = root->first_node("drawer");
	if(drawer)
	{
		xml_node<char> *item = drawer->first_node();
		while(item)
		{
			wstring itemvalue = CCodeOperation::UTF_8ToUnicode(item->value());
			wstring itemname = CCodeOperation::UTF_8ToUnicode(item->name());
			if(itemname == L"textcolor")
			{
				newTheme->_pTextColor = itemvalue;
			}
			else if(itemname == L"textsize")
			{
				newTheme->_pTextSize = _wtoi(itemvalue.c_str());
			}
			else if(itemname == L"textbackcolor")
			{
				newTheme->_pTextBackColor = itemvalue;
			}
			else if(itemname == L"background")
			{
				newTheme->_panelWallPaperPath = temppath +CStrOperation::ReplaceW(itemvalue, L"/",L"\\");
				newTheme->_localPanelWallPaperPath = newTheme->_panelWallPaperPath; 
			}
			item = item->next_sibling();
		}
	}

	xml_node<char> * keyconfig = root->first_node("keyconfig");
	if(keyconfig)
	{
		xml_node<char> *item = keyconfig->first_node();
		while(item)
		{
			wstring itemname = CCodeOperation::UTF_8ToUnicode(item->name());
			if(itemname == L"app")
			{
				xml_node<char> * iconnode = item->first_node("icon");
				xml_node<char> * idnode = item->first_node("intent");
				if(iconnode || idnode)
				{
					string intentvalue=idnode->value();
					if("theme_preview"==intentvalue && iconnode)
					{
						wstring previewpath=CCodeOperation::UTF_8ToUnicode(iconnode->value());
						newTheme->_wallPaperPath = temppath +CStrOperation::ReplaceW(previewpath, L"/",L"\\");
						newTheme->_localWallPaperPath = newTheme->_wallPaperPath; 
					}
					if(intentvalue=="panda_dock_sys_switch")
					{
						newTheme->_themeType=ThemeType_SmartHome;
					}
					ThemeAppIcon  icon;
					icon._themeId = newTheme->_id;
					icon._appId = CCodeOperation::UTF_8ToUnicode(intentvalue);
					int index = IsNeededAppIcon(icon._appId);
					if(android_AppIcon_contacts_Dialtacts<=index && index <= android_AppIcon_clock_minute && iconnode)
					{
						icon._iconPath = CCodeOperation::UTF_8ToUnicode(iconnode->value());
						icon._LocalIconPath = temppath + CStrOperation::ReplaceW(icon._iconPath, L"/",L"\\");
						newTheme->_themeIconList.insert(pair<ThemeAppIconType,ThemeAppIcon>((ThemeAppIconType)index, icon));
						//newTheme->_themeIconList.push_back(icon);
					}
				}
			}
			item = item->next_sibling();
		}
	}

	return newTheme;
}

ThemeData* ND91Assistant::AndroidThemeXmlHelper::ResolveDetailTheme( xml_node<char> * themeNode )
{
	if (!themeNode)
		return NULL;

	string nodename = themeNode->name();
	if(nodename != "theme")
		return NULL;

	ThemeData*  newtheme = new ThemeData();

	xml_attribute<char>*  attrId = themeNode->first_attribute("id");
	newtheme->_id = CCodeOperation::UTF_8ToUnicode(attrId->value());

	xml_node<char> * item = themeNode->first_node();
	while(item)
	{
		wstring name = CCodeOperation::UTF_8ToUnicode(item->name());
		wstring itemvalue = CCodeOperation::UTF_8ToUnicode(item->value());
		for(int i =0 ; i< 13; i++)
		{
			if(themeinfos[i] == name)
			{
				switch(i)
				{
				case 0:
					{
						newtheme->_name = itemvalue;
					}
					break;
				case 1:
					{
						newtheme->_en_name =itemvalue;
					}
					break;
				case 2:
					{
						wstring _ext = itemvalue;
						if (_ext == L"apt")
							newtheme->_themeType = ThemeType_PandaHome;
						else if(_ext == L"apk")
							newtheme->_themeType = ThemeType_APK;
					}
					break;
				case 3:
					{
						newtheme->_version = itemvalue;
					}
					break;
				case 4:
					{
						newtheme->_textSize = _wtoi(itemvalue.c_str());
					}
					break;
				case 5:
					{
						newtheme->_textColor = itemvalue;
					}
					break;
				case 6:
					{
						newtheme->_textBackColor = itemvalue;
					}
					break;
				case 7:
					{
						newtheme->_pTextSize = _wtoi(itemvalue.c_str());
					}
					break;
				case 8:
					{
						newtheme->_pTextColor = itemvalue;
					}
					break;
				case 9:
					{
						newtheme->_pTextBackColor = itemvalue;
					}
					break;
				case 10:
					{
						newtheme->_wallPaperPath = itemvalue;
					}
					break;
				case 11:
					{
						newtheme->_panelWallPaperPath = itemvalue;
					}
					break;
				case 12:
					{
						xml_node<char>* appidnode = item->first_node("appid");
						ThemeAppIcon  appicon;
						appicon._appId = CCodeOperation::UTF_8ToUnicode(appidnode->value());
						appicon._themeId = newtheme->_id;
						xml_node<char>* iconnode = item->first_node("icon");
						appicon._iconPath = CCodeOperation::UTF_8ToUnicode(iconnode->value());
						//newtheme->_themeIconList.push_back(appicon);
						int index = IsNeededAppIcon(appicon._appId);
						if(android_AppIcon_contacts_Dialtacts<=index && index <= android_AppIcon_clock_minute)
							newtheme->_themeIconList.insert(pair<ThemeAppIconType,ThemeAppIcon>((ThemeAppIconType)index, appicon));
					}
					break;
				default:
					break;
				}
				break;
			}
		}
		item = item->next_sibling();
	}

	return newtheme;
}

int ND91Assistant::AndroidThemeXmlHelper::IsNeededAppIcon(wstring  appid)
{
	bool  found = false;
	for (int k = 0; k < 7; k++)
	{
		wstring temp = APPIDS[k];
		
		int pos = appid.find(temp);
		if(pos>-1)
		{
			found = true;
			return k;
		}
	}
	return -1;
}


#endif