#pragma  once

#include "Module/Contact/vCard/Extention/CustomItem.h"

namespace ND91Assistant
{
    class Item;
    class Anniversary : public CustomItem
    {
    public:
        Anniversary();
        virtual ~Anniversary();

        // ฑ๊สถ
        virtual wstring GetName() { return L"ANNIVERSARY"; }

        virtual void Init(Item* pItem);

        time_t GetDate();
        void SetDate(time_t date);

    private:
        time_t _date;
    };
}