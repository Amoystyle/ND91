#pragma  once

namespace ND91Assistant
{
    // vCal phone number type. Note this values may be flagged !
    enum PhoneNumberType_enum
    {
        PhoneNumber_NotSpecified    = 0,        // Phone number type not specified.(default)
        PhoneNumber_Preferred       = 1,        // Preferred phone number.
        PhoneNumber_Home            = 2,        // Telephone number associated with a residence.
        PhoneNumber_Msg             = 4,        // Telephone number has voice messaging support.
        PhoneNumber_Work            = 8,        // Telephone number associated with a place of work.
        PhoneNumber_Voice           = 16,       // Voice telephone number.
        PhoneNumber_Fax             = 32,       // Fax number.
        PhoneNumber_Cellular        = 64,       // Cellular phone number.
        PhoneNumber_Video           = 128,      // Video conferencing telephone number.
        PhoneNumber_Pager           = 256,      // Paging device telephone number.
        PhoneNumber_BBS             = 512,      // Bulletin board system telephone number.
        PhoneNumber_Modem           = 1024,     // Modem connected telephone number.
        PhoneNumber_Car             = 2048,     // Car-phone telephone number.
        PhoneNumber_ISDN            = 4096,     // ISDN service telephone number.
        PhoneNumber_PCS             = 8192,     // Personal communication services telephone number.
        PhoneNumber_Custom          = 16384,    // Custom type telephone number.
        PhoneNumber_Main            = 32768,    // main telephone number.
        PhoneNumber_CallbackTel     = 65536,    // callback telephone number.
        PhoneNumber_Company         = 131072,   // company telephone number
        PhoneNumber_Other           = 262144,   // other
        PhoneNumber_Radio           = 524288,   // radio telephone number
        PhoneNumber_Telex           = 1048576,  // telex number
        PhoneNumber_TtyTdd          = 2097152,  // ttytdd
        PhoneNumber_Assistant       = 4194304,  // assistant
        PhoneNumber_MMS             = 8388608,  // mms
    };


    class Item;

    // vCard phone number implementation.
    class PhoneNumber
    {
    public:
        PhoneNumber(Item* pItem, 
                    PhoneNumberType_enum type, 
                    wstring customType, 
                    wstring number);
        virtual ~PhoneNumber();

        // Converts PhoneNumberType_enum to vCard item parameters string.
        static wstring PhoneTypeToString(PhoneNumberType_enum type, wstring customType);

        // Parses phone from vCard TEL structure string.
        static PhoneNumber* Parse(Item* pItem);

        // Get is primary email address.
        bool IsPrimary();

        Item* GetItem();
        PhoneNumberType_enum GetType();
        void SetType(PhoneNumberType_enum type);
        wstring GetCustomType();
        void SetCustomType(wstring customType);
        wstring GetNumber();
        void SetNumber(wstring number);

    private:
        // This method is called when some property has changed, wee need to update underlaying vCard item.
        void Update();

    private:
        Item*                   _pItem;
        PhoneNumberType_enum    _type;
        wstring                 _customType;
        wstring                 _number;
    };
}