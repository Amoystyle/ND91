#include "stdafx.h"

#ifdef MODULE_IPOD

#include "Ios5BaseLocation.h"


DBaseLocaltion5::DBaseLocaltion5()
{
	base_location_id=0;
	path=L"";
}
wstring DBaseLocaltion5::GetTableName()
{
	return L"base_location";
}
DBaseLocaltion5* DBaseLocaltion5::FromValue(const vector<const char*>* pValues)
{
	DBaseLocaltion5* p = new DBaseLocaltion5();
	int i = 0;	
	p->base_location_id = _atoi64(pValues->at(i++));
	p->path = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	return p;
}

void DBaseLocaltion5::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"base_location_id");
	vector.push_back(L"path");

}
const vector<wstring>* const DBaseLocaltion5::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(base_location_id));
	pVector->push_back(ConvertStr(path));
	return pVector;
}
wstring DBaseLocaltion5::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" base_location_id = "<<base_location_id;
	return woss.str();
}



#endif