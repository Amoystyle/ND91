#pragma  once


namespace ND91Assistant
{
    class vCard;
    class IMAddress;
    enum IMType_enum;

    class IMAddressCollection
    {
    public:
        IMAddressCollection(vCard* owner);
        virtual ~IMAddressCollection();

        void Add(wstring label, wstring address);

        void Add(wstring label, wstring address, IMType_enum type, wstring customName);

        void AddToCollecton(wstring value);

        void AddToCollecton(wstring parameters, wstring value);

        void Remove(IMAddress* pItem);

        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        IMAddress* GetIMAddressByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<IMAddress*> _collection;
    };
}