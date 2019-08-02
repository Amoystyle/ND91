#pragma  once

namespace ND91Assistant
{
    class vCard;
    class EmailAddress;
    enum EmailAddressType_enum;

    // vCard email address collection implementation.
    class EmailAddressCollection
    {
    public:
        EmailAddressCollection(vCard* owner);
        virtual ~EmailAddressCollection();

        EmailAddress* Add(EmailAddressType_enum type, wstring email);

        // Add new email address to the collection.
        EmailAddress* Add(EmailAddressType_enum type, wstring customType, wstring email);

        // Removes specified item from the collection.
        void Remove(EmailAddress* pItem);

        // Removes all items from the collection.
        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        EmailAddress* GetEmailAddressByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<EmailAddress*> _collection;

    };
}