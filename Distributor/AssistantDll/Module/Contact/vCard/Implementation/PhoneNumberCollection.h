#pragma  once

namespace ND91Assistant
{
    class vCard;
    class PhoneNumber;
    enum PhoneNumberType_enum; 

    // vCard phone number collection implementation.
    class PhoneNumberCollection
    {
    public:
        PhoneNumberCollection(vCard* owner);
        virtual ~PhoneNumberCollection();

        void Add(PhoneNumberType_enum type, wstring number);

        // Add new phone number to the collection.
        void Add(PhoneNumberType_enum type, wstring customType, wstring number);

        // Removes specified item from the collection.
        void Remove(PhoneNumber* pItem);

        // Removes all items from the collection.
        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        PhoneNumber* GetPhoneNumberByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<PhoneNumber*> _collection;
    };
}