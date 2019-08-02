#include "stdafx.h"
#include "Location.h"

//#ifdef MODULE_IPOD
DLocation::DLocation()
{
    item_pid = 0;
    sub_id = 0;
    base_location_id = 0;
    location_type = 0;
    // location = 0;
    extension = 0;
    kind_id = 1;
    date_created = 0;
    file_size = 0;
    file_creator = 0;
    file_type = 0;
    num_dir_levels_file = 0;
    num_dir_levels_lib = 0;
}

const vector<wstring>* const DLocation::GetValues()
{
    vector<wstring>* pVector = GetVector();

    pVector->push_back(Int64ToStr(item_pid));
    pVector->push_back(Int64ToStr(sub_id));
    pVector->push_back(Int32ToStr(base_location_id));
    pVector->push_back(Int32ToStr(location_type));
    pVector->push_back(ConvertStr(location));
    pVector->push_back(Int32ToStr(extension));
    pVector->push_back(Int32ToStr(kind_id));
    pVector->push_back(Int32ToStr((/*__int32*/int32_t)PcToMac(date_created)));
    pVector->push_back(Int32ToStr(file_size));
    pVector->push_back(Int32ToStr(file_creator));
    pVector->push_back(Int32ToStr(file_type));
    pVector->push_back(Int32ToStr(num_dir_levels_file));
    pVector->push_back(Int32ToStr(num_dir_levels_lib));

    return pVector;
}

DLocation* DLocation::FromValue( const vector<const char*>* pValues )
{
    DLocation* p = new DLocation();
    int i = 0;
    
    p->item_pid             = _atoi64(pValues->at(i++));
    p->sub_id               = atoi(pValues->at(i++));
    p->base_location_id     = atoi(pValues->at(i++));
    p->location_type        = atoi(pValues->at(i++));
    p->location             = CCodeOperation::UTF_8ToUnicode(pValues->at(i++));
    p->extension            = atoi(pValues->at(i++));
    p->kind_id              = atoi(pValues->at(i++));
    p->date_created         = MacToPc(atoi(pValues->at(i++)));
    p->file_size            = atoi(pValues->at(i++));
    p->file_creator         = atoi(pValues->at(i++));
    p->file_type            = atoi(pValues->at(i++));
    p->num_dir_levels_file  = atoi(pValues->at(i++));
    p->num_dir_levels_lib   = atoi(pValues->at(i++));

    return p;
}

void DLocation::GetColumns( vector<wstring>& vector )
{
    vector.push_back(L"item_pid");
    vector.push_back(L"sub_id");
    vector.push_back(L"base_location_id");
    vector.push_back(L"location_type");
    vector.push_back(L"location");
    vector.push_back(L"extension");
    vector.push_back(L"kind_id");
    vector.push_back(L"date_created");
    vector.push_back(L"file_size");
    vector.push_back(L"file_creator");
    vector.push_back(L"file_type");
    vector.push_back(L"num_dir_levels_file");
    vector.push_back(L"num_dir_levels_lib");
}

wstring DLocation::GetTableName()
{
    return L"Location";
}

wstring DLocation::PrimaryKeyCondition() const
{
    wchar_t buffer[128];
    //swprintf_s(buffer, 127, L" item_pid = %lld AND sub_id = %d", item_pid, sub_id);
    swprintf(buffer, 127, L" item_pid = %lld AND sub_id = %d", item_pid, sub_id);
    return buffer;
	wostringstream woss; 
	woss<<" item_pid = "<<item_pid<<" AND sub_id = "<<sub_id;
	return woss.str();
}

wstring DLocation::ToString()
{
    wstring s;
    const vector<wstring>* const pVector = GetValues();
    for(size_t i = 0; i < pVector->size(); ++i)
    {
        if ( i == 7 )
            s += timeToStr( date_created )  + L" | ";
        else
            s += pVector->at(i) + L" | ";
    }
    s.erase(s.size()-3, 3);
    return s;
}
//#endif

