#pragma  once

namespace ND91Assistant
{
    class vCard;
    class NickName;

    class NickNameCollection
    {
    public:
        NickNameCollection(vCard* owner);
        virtual ~NickNameCollection();

        // Add new delivery address to the collection.    
        void Add(wstring data);

        // Removes specified item from the collection.
        void Remove(NickName* pItem);

        // Removes all items from the collection.
        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        NickName* GetNickNameByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<NickName*> _collection;
    };
}