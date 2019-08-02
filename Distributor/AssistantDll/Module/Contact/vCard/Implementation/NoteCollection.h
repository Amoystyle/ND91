#pragma  once

namespace ND91Assistant
{
    class vCard;
    class Note;

    class NoteCollection
    {
    public:
        NoteCollection(vCard* owner);
        virtual ~NoteCollection();

        // Add new delivery address to the collection. 
        void Add(wstring data);

        // Removes specified item from the collection.
        void Remove(Note* pItem);

        // Removes all items from the collection.
        void Clear();

        // Gets number of items in the collection.
        size_t GetCount() { return _collection.size(); }

        // Gets item at the specified index.
        Note* GetNoteByIndex(int index) { return _collection.at(index); }

    private:
        vCard* _pOwner;
        vector<Note*> _collection;
    };
}