#pragma  once

#include "Module/Contact/vCard/Extention/CustomItem.h"
#include "Module/Contact/vCard/vCard.h"

namespace ND91Assistant
{
    class Item;
    class OrganizationEx : public CustomItem
    {
    public:
        OrganizationEx();
        virtual ~OrganizationEx();

        // 标识
        virtual wstring GetName() { return L"ORG"; }

        virtual void Init(Item* pItem);

        wstring GetLabel() { return GetParametersString(); }
        void SetLabel(wstring label) { SetParametersString(label); }
        wstring GetTitle() { return _title; }
        void SetTitle(wstring title) 
        { 
            _title = title;
            SetValue(_title+vCard::Name_Param_Splitor+_location);
        }
        wstring GetLocation() { return _location; }
        void SetLocation(wstring location) { _location = location; }


        // 预定义的OrgAddress参数
        static wstring ORG_Home;
        static wstring ORG_Work;
        static wstring ORG_Other;
        static wstring ORG_Custom;

    private:
        wstring _title;
        wstring _location;
    };
}