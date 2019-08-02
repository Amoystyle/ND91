#pragma  once

namespace ND91Assistant
{
    class vCard;
    class DeliveryAddress;
    enum DeliveryAddressType_enum;

    // vCard item collection.
    class DeliveryAddressCollection
    {
    public:
        DeliveryAddressCollection(vCard* owner);
        virtual ~DeliveryAddressCollection();

        void Add(DeliveryAddressType_enum type, 
                 wstring customType, 
                 wstring postOfficeAddress, 
                 wstring extendedAddress, 
                 wstring street, 
                 wstring locality, 
                 wstring region, 
                 wstring postalCode, 
                 wstring countryCode);  

        void Add(DeliveryAddressType_enum type, 
                 wstring postOfficeAddress, 
                 wstring extendedAddress, 
                 wstring street, 
                 wstring locality, 
                 wstring region, 
                 wstring postalCode, 
                 wstring country);

        // Add new delivery address to the collection.
        void Add(DeliveryAddressType_enum type, 
                 wstring customType, 
                 wstring postOfficeAddress, 
                 wstring extendedAddress, 
                 wstring street, 
                 wstring locality, 
                 wstring region, 
                 wstring postalCode, 
                 wstring country, 
                 wstring countryCode);

        // Removes specified item from the collection.
        void Remove(DeliveryAddress* pItem);

        // Removes all items from the collection.
        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        DeliveryAddress* GetDeliveryAddressByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<DeliveryAddress*> _collection;
    };
}