#pragma  once

namespace ND91Assistant
{
    enum DateType_enum
    {
        DateType_NotSpecified    = 0,
        DateType_Other           = 1,
        DateType_Anniversary     = 2,
        DateType_Custom          = 4,
    };

    class Item;

    class Date
    {
    public:
        Date(Item* pItem, DateType_enum urlType, wstring customType, time_t date);
        virtual ~Date();

        static wstring DateTypeToString(DateType_enum type, wstring customType);

        // Parses delivery address from vCard ADR structure string.
        static Date* Parse(Item* pItem);

        Item* GetItem();

        DateType_enum GetType();
        void SetType(DateType_enum type);
        wstring GetCustomType();
        void SetCustomType(wstring customType);
        time_t GetDate();
        void SetDate(time_t date);

    private:
        // This method is called when some property has changed, wee need to update underlaying vCard item.
        void Update();

    private:
        Item*            _pItem;
        DateType_enum    _type;
        wstring          _customType;
        time_t           _date; 
    };
}