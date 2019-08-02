#pragma  once

namespace ND91Assistant
{
    class vCard;
    class Url;
    enum UrlType_enum;

    // vCard email address collection implementation.
    class UrlCollection
    {
    public:
        UrlCollection(vCard* owner);
        virtual ~UrlCollection();

        void Add(UrlType_enum type, wstring data);

        // Add new delivery address to the collection.
        void Add(UrlType_enum type, wstring customType, wstring data);

        // Removes specified item from the collection.
        void Remove(Url* pItem);

        // Removes all items from the collection.
        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        Url* GetUrlByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<Url*> _collection;

    };
}