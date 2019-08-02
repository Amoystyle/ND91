#pragma  once

#include "Module/Contact/vCard/Extention/CustomItem.h"

namespace ND91Assistant
{
    class Item;
    class WebPage : public CustomItem
    {
    public:
        WebPage();
        virtual ~WebPage();

        // ��ʶ
        virtual wstring GetName() { return L"WEBPAGE"; }

        virtual void Init(Item* pItem);

        wstring GetLabel() { return GetParametersString(); }
        void SetLabel(wstring label) { SetParametersString(label); } 
        wstring GetURL() { return GetValue(); }
        void SetURL(wstring url) { SetValue(url); }

        // Ԥ�����WebPage����
        static wstring WebPage_WORK;
        static wstring WebPage_HOME;
        static wstring WebPage_OTHER;

    private:

    };
}