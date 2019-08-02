#pragma once
#include <string>

using namespace std;

namespace ND91Assistant
{
	class BatteryInfo
	{
	public:
		BatteryInfo() {}; 

		void ParseData(string strData)
		{
			vector<string> lines = CStrOperation::parseLines(strData);
			string line;
			for (vector<string>::iterator it = lines.begin(); it != lines.end(); it++)
			{
				line = *it;
				int nIndex = line.find(':');
				if (nIndex > 0)
				{
					string strProp(line,0, nIndex);
					string strValue(line, nIndex+1);
					strProp = CStrOperation::toUpper(strProp);
					strValue = CStrOperation::toUpper(strValue);
					if ((int)strProp.find("AC POWERED") >= 0)
						_isACPower = (int)strValue.find("TRUE") >= 0 ? true : false;
					else if ((int)strProp.find("USB POWERED") >= 0)
						_isUSBPower = (int)strValue.find("TRUE") >= 0 ? true : false;
					else if ((int)strProp.find("STATUS") >= 0)
						_nStatus = atoi(strValue.c_str());
					else if ((int)strProp.find("HEALTH") >= 0)
						_nHealth = atoi(strValue.c_str());
					else if ((int)strProp.find("PRESENT") >= 0)
						_nPresent = atoi(strValue.c_str());
					else if ((int)strProp.find("LEVEL") >= 0)
						_nLever = atoi(strValue.c_str());
					else if ((int)strProp.find("SCALE") >= 0)
						_nScale = atoi(strValue.c_str());
					else if ((int)strProp.find("VOLTAGE") >= 0)
						_nVoltage = atoi(strValue.c_str());
					else if ((int)strProp.find("TEMPERATURE") >= 0)
						_nTemperature = atoi(strValue.c_str());
					else if ((int)strProp.find("TECHNOLOGY") >= 0)
						_strTechnology = CCodeOperation::UTF_8ToUnicode(CStrOperation::trimLeft(strValue, " "));					
				}
			}
		}
	public:
		bool _isACPower;		//是否是交流电充电
		bool _isUSBPower;		//是否是USB充电
		int	 _nStatus;		
		int	 _nHealth;
		int  _nPresent;
		int  _nLever;
		int  _nScale;
		int  _nVoltage;
		int	 _nTemperature;
		wstring _strTechnology;
	};
}