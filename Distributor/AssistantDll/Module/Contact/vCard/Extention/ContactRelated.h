#pragma  once

#include "Module/Contact/vCard/Extention/CustomItem.h"

namespace ND91Assistant
{
    // 联系人关系类型
    enum RelatedKind
    {
        RelatedKind_Parent,
        RelatedKind_Brother,
        RelatedKind_Sister,
        RelatedKind_Mother,
        RelatedKind_Father,
        RelatedKind_Child,
        RelatedKind_Friend,
        RelatedKind_Spouse,
        RelatedKind_Partner,
        RelatedKind_Assistant,
        RelatedKind_Manager,
        RelatedKind_Custom,
        RelatedKind_Other,
    };

    class Item;
    enum RelatedTypes;
    class ContactRelated : public CustomItem
    {
    public:
        ContactRelated();
        virtual ~ContactRelated();

        // 标识
        virtual wstring GetName() { return L"ContactRelated"; }

        virtual void Init(Item* pItem);

        wstring GetLabel() { return _label; }
        void SetLabel(wstring label) { _label = label; }

        // 关系称谓
        RelatedKind GetKind() { return _type; }
        void SetKind(RelatedKind type) { _type = type; }

        // 自定义名称
        wstring GetCustomName() { return _customName; }
        void SetCustomName(wstring customName) { _customName = customName; }

        // 关系人名称
        wstring GetRelatedName() { return GetValue(); }
        void SetRelatedName(wstring value) { SetValue(value); }

        static wstring RelatedLabelConvert(RelatedTypes imlabelSource);

        static wstring Related_Parent;
        static wstring Related_Brother;
        static wstring Related_Sister;
        static wstring Related_Mother;
        static wstring Related_Father;
        static wstring Related_Child;
        static wstring Related_Friend;
        static wstring Related_Spouse;
        static wstring Related_Partner;
        static wstring Related_Assistant;
        static wstring Related_Manager;
        static wstring Related_Custom;
        static wstring Related_Other;

    private:
        wstring     _label;
        RelatedKind _type;
        wstring     _customName;
    };
}