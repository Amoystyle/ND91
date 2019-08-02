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

        // 标识
        virtual wstring GetName() { return L"WEBPAGE"; }

        virtual void Init(Item* pItem);

        wstring GetLabel() { return GetParametersString(); }
        void SetLabel(wstring label) { SetParametersString(label); } 
        wstring GetURL() { return GetValue(); }
        void SetURL(wstring url) { SetValue(url); }

        // 预定义的WebPage参数
        static wstring WebPage_WORK;
        static wstring WebPage_HOME;
        static wstring WebPage_OTHER;

    private:

    };
}