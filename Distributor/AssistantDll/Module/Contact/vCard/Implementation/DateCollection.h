#pragma  once

namespace ND91Assistant
{
    class vCard;
    class Date;
    enum DateType_enum;

    // vCard email address collection implementation.
    class DateCollection
    {
    public:
        DateCollection(vCard* owner);
        virtual ~DateCollection();

        void Add(DateType_enum type, time_t date);

        void Add(DateType_enum type, wstring customType, time_t date);

        // Removes specified item from the collection.
        void Remove(Date* pItem);

        // Removes all items from the collection.
        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        Date* GetDateByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<Date*> _collection;
    };
}