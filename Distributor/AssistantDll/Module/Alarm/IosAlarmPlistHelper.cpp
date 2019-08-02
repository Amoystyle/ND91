#include "pch_module.h"
#include "IosAlarmPlistHelper.h"

#ifdef MODULE_ALARM

#include "Module/Alarm/AlarmData.h"

IosAlarmPlistHelper::IosAlarmPlistHelper(void)
{
}

IosAlarmPlistHelper::~IosAlarmPlistHelper(void)
{
}


void ND91Assistant::IosAlarmPlistHelper::ParseAlarms( wstring plistfilePath, deque<AlarmData*>& alarms, bool ios4)
{
	deque<string> nodenames;
	if(!ios4)
	{
		nodenames.push_back("active");
		nodenames.push_back("allows snooze");
	}
	else
	{
		nodenames.push_back("alarmId");
		nodenames.push_back("allowsSnooze");
	}
	nodenames.push_back("hour");
	nodenames.push_back("minute");
	nodenames.push_back("repeats");
	if(!ios4)
	{
		nodenames.push_back("setting");
		nodenames.push_back("sound path");
	}
	else
	{
		nodenames.push_back("daySetting");
		nodenames.push_back("sound");
	}
	nodenames.push_back("title");

	IosPlistHelper plistHelper(plistfilePath, eXmlType_UTF8);
	XML_NODE_INDEX index;
	index.AddChild(0);
	vector<rapidxml::xml_node<char>*> vtValues;
	IosPlistHelper::GetNodesFromNode(plistHelper.SearchKeyNode(index, ios4?"Alarms":"AlarmList", "array"), vtValues);
	for (vector<rapidxml::xml_node<char>*>::iterator it=vtValues.begin(); it!=vtValues.end(); it++)
	{
		AlarmData* palarm = new AlarmData();
		for (size_t i = 0 ; i < 8; i++)
		{
			rapidxml::xml_node<char>* pNode = IosPlistHelper::SearchKeyNodeFromXmlNode(*it, nodenames[i]);
			if (!pNode)
				continue;
			switch(i)
			{
				case 0:
					{
						palarm->_enabled = (strcmp(pNode->name(), "true")==0);
						if(ios4)
							palarm->_ios_guid = CCodeOperation::UTF_8ToUnicode(pNode->value());
					}
					break;
				case 1:
					palarm->_ios_allowSnooze = (strcmp(pNode->name(), "true")==0);
					break;
				case 2:
					palarm->_hour = atoi(pNode->value());
					break;
				case 3:
					palarm->_minutes = atoi(pNode->value());
					break;
				case 4:
					palarm->_ios_repeat = (strcmp(pNode->name(), "true")==0);
					break;
				case 5:
					palarm->_daysofweek = atoi(pNode->value());
					break;
				case 6:
					palarm->_ringuri = CCodeOperation::UTF_8ToUnicode(pNode->value());
					break;
				case 7:
					palarm->_message = CCodeOperation::UTF_8ToUnicode(pNode->value());
					break;
			}
		}
		alarms.push_back(palarm);
	}
}

void ND91Assistant::IosAlarmPlistHelper::WriteAlarmPlist( wstring plistfilePath, const deque<AlarmData*> alarms, bool ios4 )
{
	IosPlistHelper helper(XML_PI, XML_DOCTYPE, eXmlType_UTF8, XML_ELEMENT(L"plist", L"", L"version", L"1.0"));
	XML_NODE_INDEX index;
	helper.AddElement(index, XML_ELEMENT(XML_DICT));
	index.AddChild(0);
	helper.AddElement(index, XML_ELEMENT(XML_KEY, ios4?L"Alarms":L"AlarmList"));
	helper.AddElement(index, XML_ELEMENT(XML_ARRAY));
	index.AddChild(1);
	for (size_t i = 0; i < alarms.size(); i++)
	{
		helper.AddElement(index, XML_ELEMENT(XML_DICT));
	}
	index.AddChild(0);
	for (size_t i = 0 ; i < alarms.size(); i++)
	{
		index.SetChildIndex(2, i);
		AlarmData* palarm = alarms[i];
		for (size_t j = 0; j < 8; j++ )
		{
			switch(j)
			{
			case 0:
				{
					helper.AddElement(index, XML_ELEMENT(XML_KEY, !ios4?L"active":L"alarmId"));
					if (!ios4)
						helper.AddElement(index, XML_ELEMENT(palarm->_enabled ?XML_TRUE:XML_FALSE));
					else
						helper.AddElement(index, XML_ELEMENT(XML_STRING, palarm->_ios_guid));

				}
				break;
			case 1:
				{
					helper.AddElement(index, XML_ELEMENT(XML_KEY, !ios4?L"allows snooze":L"allowsSnooze"));
					helper.AddElement(index, XML_ELEMENT(palarm->_ios_allowSnooze ?XML_TRUE:XML_FALSE));
				}
				break;
			case 2:
				{
					helper.AddElement(index, XML_ELEMENT(XML_KEY, L"hour"));
					helper.AddElement(index, XML_ELEMENT(XML_INTEGER,CStrOperation::IntToWString(palarm->_hour)));
				}
				break;
			case 3:
				{
					helper.AddElement(index, XML_ELEMENT(XML_KEY, L"minute"));
					helper.AddElement(index, XML_ELEMENT(XML_INTEGER,CStrOperation::IntToWString(palarm->_minutes)));
				}
				break;
			case 4:
				{
					helper.AddElement(index, XML_ELEMENT(XML_KEY, L"repeats"));
					helper.AddElement(index, XML_ELEMENT(palarm->_ios_repeat ?XML_TRUE:XML_FALSE));
				}
				break;
			case 5:
				{
					helper.AddElement(index, XML_ELEMENT(XML_KEY, !ios4?L"setting":L"daySetting"));
					helper.AddElement(index, XML_ELEMENT(XML_INTEGER,CStrOperation::IntToWString(palarm->_daysofweek)));
				}
				break;
			case 6:
				{
					helper.AddElement(index, XML_ELEMENT(XML_KEY, !ios4?L"sound path":L"sound"));
					wstring ringuri= palarm->_ringuri;
					if(ios4 && (int)ringuri.find(L"system:")<0 
						&& (int)ringuri.find(L"itunes:")<0 
						&& (int)ringuri.find(L"default")<0 )
						ringuri = L"itunes:"+ringuri;
					helper.AddElement(index, XML_ELEMENT(XML_STRING, ringuri));
				}
				break;
			case 7:
				{
					helper.AddElement(index, XML_ELEMENT(XML_KEY, L"title"));
					helper.AddElement(index, XML_ELEMENT(XML_STRING, palarm->_message));
				}
				break;
			}
		}
	}
	helper.WriteToFile(plistfilePath);
	IosPlistConverter::GetPlistFromXml(plistfilePath, plistfilePath);
}


#endif