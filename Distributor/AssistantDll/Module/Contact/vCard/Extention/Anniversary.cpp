#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/Anniversary.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/Util.h"

Anniversary::Anniversary()
{
    _date = 0;
}

Anniversary::~Anniversary()
{
}

void Anniversary::Init(Item* pItem)
{
    CustomItem::Init(pItem);
    _date = Util::PareDateTime(pItem->GetValue());
}

time_t Anniversary::GetDate()
{
    return _date;
}

void Anniversary::SetDate(time_t date)
{
    _date = date;
    // update
    if (_date != 0)
    {
        CDealTime dTime(date);
        SetValue(CCodeOperation::StringToWstring(dTime.TimeFormat()));
    }
    else
        SetValue(L"");
}

//#endif