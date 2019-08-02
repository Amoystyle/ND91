#pragma  once

namespace ND91Assistant
{
    class Item;

    class Note
    {
    public:
        Note(Item* pItem, wstring data);
        virtual ~Note();

        // Parses delivery address from vCard ADR structure string.
        static Note* Parse(Item* pItem);

        Item* GetItem() { return _pItem; }
        wstring GetData() { return _data; }
        void SetData(wstring data) { _data = data; Update(); }

    private:
        // This method is called when some property has changed, wee need to update underlaying vCard item.
        void Update();

    private:
        Item*           _pItem;
        wstring         _data;
    };
}