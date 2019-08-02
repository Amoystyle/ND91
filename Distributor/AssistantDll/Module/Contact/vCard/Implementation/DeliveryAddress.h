#pragma  once

namespace ND91Assistant
{
    enum DeliveryAddressType_enum
    {
        DeliveryAddress_NotSpecified    = 0,    // Delivery address type not specified.
        DeliveryAddress_Preferred       = 1,    // Preferred delivery address.
        DeliveryAddress_Domestic        = 2,    // Domestic delivery address.
        DeliveryAddress_Ineternational  = 4,    // International delivery address.
        DeliveryAddress_Postal          = 8,    // Postal delivery address.
        DeliveryAddress_Parcel          = 16,   // Parcel delivery address.
        DeliveryAddress_Home            = 32,   // Delivery address for a residence.
        DeliveryAddress_Work            = 64,   // Address for a place of work.
        DeliveryAddress_Custom          = 128,  // Custom delivery address.
    };

    class Item;

    // vCard delivery address implementation.
    class DeliveryAddress
    {
    public:
        DeliveryAddress(Item* pItem, 
                        DeliveryAddressType_enum type, 
                        wstring customType, 
                        wstring postOfficeAddress, 
                        wstring extendedAddress, 
                        wstring street, 
                        wstring locality, 
                        wstring region, 
                        wstring postalCode, 
                        wstring country,
                        wstring countryCode);

        virtual ~DeliveryAddress();

        // Converts DeliveryAddressType_enum to vCard item parameters string.
        static wstring AddressTypeToString(DeliveryAddressType_enum type, wstring customType);

        // Parses delivery address from vCard ADR structure string.
        static DeliveryAddress* Parse(Item* pItem);

        // Get is primary email address.
        bool IsPrimary();

        Item* GetItem();
        DeliveryAddressType_enum GetType();
        void SetType(DeliveryAddressType_enum type);
        wstring GetCustomType();
        void SetCustomType(wstring customType);
        wstring GetPostOfficeAddress();
        void SetPostOfficeAddress(wstring postOfficeAddress);
        wstring GetExtendedAddress();
        void SetExtendedAddress(wstring extendedAddress);
        wstring GetStreet();
        void SetStreet(wstring street);
        wstring GetLocality();
        void SetLocality(wstring locality);
        wstring GetRegion();
        void SetRegion(wstring region);
        wstring GetPostalCode();
        void SetpostalCode(wstring postalCode);
        wstring GetCountry();
        void SetCountry(wstring country);
        wstring GetCountryCode();
        void SetCountryCode(wstring countryCode);

    private:
        // This method is called when some property has changed, wee need to update underlaying vCard item.
        void Update();

    private:
        Item* _pItem;
        DeliveryAddressType_enum _type;
        wstring _customType;
        wstring _postOfficeAddress;
        wstring _extendedAddress;
        wstring _street;
        wstring _locality;
        wstring _region;
        wstring _postalCode;
        wstring _country;
        wstring _countryCode;

    };
}