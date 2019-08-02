#pragma  once

namespace ND91Assistant
{
    class Item;

    // vCard item collection.
    class ItemCollection
    {
    public:
        ItemCollection();
        virtual ~ItemCollection();

        vector<Item*>* GetItemVec() { return &_items; }

        Item* Add(wstring name, wstring parametes, wstring value);
        void Remove(wstring name);
        void Remove(Item* item);
        void Clear();

        // Gets first item with specified name. Returns null if specified item doesn't exists.
        Item* GetFirst(wstring name);

        // Gets items with specified name.
        vector<Item*> Get(wstring name);

        // Sets first item with specified name value.  If item doesn't exist, item will be appended to the end.
        // If value is null, all items with specified name will be removed.
        void SetValue(wstring name, wstring value);
        void SetValue(wstring name, wstring parametes, wstring value);

        // Gets number of vCard items in the collection.
        size_t GetCount() { return _items.size(); }

    private:
        vector<Item*> _items;

    };
}