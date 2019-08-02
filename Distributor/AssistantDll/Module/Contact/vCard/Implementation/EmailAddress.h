#pragma  once

namespace ND91Assistant
{
    // vCal email address type. Note this values may be flagged !
    enum EmailAddressType_enum
    {   
        EmailAddress_NotSpecified    = 0,    // Email address type not specified.
        EmailAddress_Preferred       = 1,    // Preferred email address.
        EmailAddress_Internet        = 2,    // Internet addressing type.
        EmailAddress_X400            = 4,    // X.400 addressing type.
        EmailAddress_Custom          = 8,    // Custom addressing number.
    };

    class Item;
    enum EmailAddressType_enum;

    // vCard email address implementation.
    class EmailAddress
    {
    public:
        EmailAddress(Item* pItem, 
                     EmailAddressType_enum type, 
                     wstring customType, 
                     wstring emailAddress);
        virtual ~EmailAddress();

        // Converts EmailAddressType_enum to vCard item parameters string.
        static wstring EmailTypeToString(EmailAddressType_enum type, wstring customType);

        // Parses email address from vCard EMAIL structure string.
        static EmailAddress* Parse(Item* pItem);

        // Get is primary email address.
        bool IsPrimary();

        Item* GetItem();
        EmailAddressType_enum GetType();
        void SetType(EmailAddressType_enum type);
        wstring GetCustomType();
        void SetCustomType(wstring customType);
        wstring GetEmailAddress();
        void SetEmailAddress(wstring emailAddress);

        static wstring Home;
        static wstring Work;
        static wstring Other;
        static wstring Custom;
        static wstring Pager;
        static wstring Mobile;
        static wstring Anniversary;

    private:
        // This method is called when some property has changed, wee need to update underlaying vCard item.
        void Update();

    private:
        Item*                   _pItem;
        EmailAddressType_enum   _type;
        wstring                 _customType;
        wstring                 _emailAddress;
    };
}