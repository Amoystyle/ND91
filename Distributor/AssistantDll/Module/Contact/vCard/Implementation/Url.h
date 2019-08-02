#pragma  once

namespace ND91Assistant
{
    enum UrlType_enum
    {
        Url_NotSpecified    = 0,
        Url_Preferred       = 1,
        Url_Work            = 2,
        Url_Home            = 4,
        Url_Other           = 8,
        Url_Custom          = 16,
    };

    class Item;

    class Url
    {
    public:
        Url(Item* pItem, UrlType_enum urlType, wstring customType, wstring data);
        virtual ~Url();

        // Converts DeliveryAddressType_enum to vCard item parameters string.
        static wstring UrlTypeToString(UrlType_enum type, wstring customType);

        // Parses delivery address from vCard ADR structure string.
        static Url* Parse(Item* pItem);

        Item* GetItem();
        UrlType_enum GetType();
        void SetType(UrlType_enum type);
        wstring GetCustomType();
        void SetCustomType(wstring customType);
        wstring GetData();
        void SetData(wstring data);

    private:
        // This method is called when some property has changed, wee need to update underlaying vCard item.
        void Update();

    private:
        Item*           _pItem;
        UrlType_enum    _type;
        wstring         _customType;
        wstring         _data; 
    };
}