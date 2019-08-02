#pragma  once

#include "Module/Contact/vCard/Extention/CustomItem.h"

namespace ND91Assistant
{
    enum IMType_enum
    {
        IMType_NotSpecified    = 0,
        IMType_OTHER           = 1,
        IMType_WORK            = 2,
        IMType_HOME            = 4,
        IMType_CUSTOM          = 8,
    };

    class Item;
    enum IMTypesName;
    enum IMTypesName;
    class IMAddress : public CustomItem
    {
    public:
        IMAddress();
        virtual ~IMAddress();

        // 标识
        virtual wstring GetName() { return L"IM"; }

        virtual void Init(Item* pItem);

        wstring GetLabel() { return _label; }
        void SetLabel(wstring label) { _label = label; }
        IMType_enum GetType() { return _type; }
        void SetType(IMType_enum type) { _type = type; }
        wstring GetCustomName() { return _customName; }
        void SetCustomName(wstring customName) { _customName = customName; }

        wstring GetAddress() { return GetValue(); }
        void SetAddress(wstring address) { SetValue(address); }

        static wstring IMLabelConvert(IMTypesName imlabelSource);
        static IMTypesName IMLabelConvert(wstring label);

        // 预定义的IM通信方式参数
        static wstring IMAddress_QQ;
        static wstring IMAddress_MSN;
        static wstring IMAddress_GOOLETALK;
        static wstring IMAddress_WINDOWSLIVE;
        static wstring IMAddress_YAHOO;
        static wstring IMAddress_SKYPE;
        static wstring IMAddress_AIM;
        static wstring IMAddress_ICQ;
        static wstring IMAddress_JABBER;
        static wstring IMAddress_FETION;

    private:
        wstring     _label;
        IMType_enum _type;
        wstring     _customName;
    };
}