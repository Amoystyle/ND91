#pragma  once


namespace ND91Assistant
{
    class vCard;
    class ContactRelated;
    enum RelatedKind;

    class ContactRelatedCollection
    {
    public:
        ContactRelatedCollection(vCard* owner);
        virtual ~ContactRelatedCollection();

        void Add(wstring label, wstring address, RelatedKind type, wstring customName);

        void AddToCollecton(wstring value);

        void AddToCollecton(wstring parameters, wstring value);

        void Remove(ContactRelated* pItem);

        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        ContactRelated* GetContactRelatedByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<ContactRelated*> _collection;
    };
}