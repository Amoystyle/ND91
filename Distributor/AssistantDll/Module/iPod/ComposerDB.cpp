#include "stdafx.h"
#ifdef MODULE_IPOD

#include "ComposerDB.h"

extern int G_DBver;
DComposer::DComposer()
{
	Pid = 0;
	name_order = 0;
	name_order_section = 0;
	name_blank = 0;
}
wstring DComposer::GetTableName()
{
	return L"composer";
}
DComposer* DComposer::FromValue(const vector<const char*>* pValues)
{
	DComposer* p = new DComposer();
	int i = 0;	
	p->Pid = _atoi64(pValues->at(i++));
	p->Name = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	p->name_order = atoi(pValues->at(i++));
	p->sort_name = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
	if(G_DBver>=310)
	{
		p->name_order_section = atoi(pValues->at(i++));
		p->name_blank = atoi(pValues->at(i++));
	}
	return p;
}

void DComposer::GetColumns(vector<wstring>& vector)
{
	vector.push_back(L"Pid");
	vector.push_back(L"Name");
	vector.push_back(L"name_order");
	vector.push_back(L"sort_name");
	if(G_DBver>=310)
	{
		if(G_DBver >=400)
			vector.push_back(L"name_order_section");
		else
			vector.push_back(L"sort_name_section");	
		vector.push_back(L"name_blank");
	}

}
const vector<wstring>* const DComposer::GetValues()
{
	vector<wstring>* pVector = GetVector();
	pVector->push_back(Int64ToStr(Pid));
	pVector->push_back(ConvertStr(Name));
	pVector->push_back(Int32ToStr(name_order));
	pVector->push_back(ConvertStr(sort_name));
	if(G_DBver>=310)
	{
		pVector->push_back(Int32ToStr(name_order_section));
		pVector->push_back(Int32ToStr(name_blank));
	}
	return pVector;
}
wstring DComposer::PrimaryKeyCondition() const
{
	wostringstream woss; 
	woss<<" Pid = "<<Pid;
	return woss.str();
}


#endif