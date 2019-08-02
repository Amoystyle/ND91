#pragma  once

namespace ND91Assistant
{
    class Item;

    class NickName
    {
    public:
        NickName(Item* pItem, wstring data);
        virtual ~NickName();

        // Parses delivery address from vCard ADR structure string.
        static NickName* Parse(Item* pItem);

        Item* GetItem() { return _pItem; }
        wstring GetData() { return _data; }
        void SetData(wstring data) { _data = data; Update(); }

    private:
        // This method is called when some property has changed, wee need to update underlaying vCard item.
        void Update();

    private:
        Item*       _pItem;    
        wstring     _data;     
    };
}