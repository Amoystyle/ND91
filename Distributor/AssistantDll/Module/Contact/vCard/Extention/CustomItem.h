#pragma  once


namespace ND91Assistant
{
    class Item;

    class CustomItem
    {
    public:
        CustomItem();
        virtual ~CustomItem();

        virtual wstring GetName() = 0;

        virtual void Init(Item* pItem);

        Item* GetItem() { return _pItem; }

    protected:
        void Update();

        virtual wstring GetParametersString() { return _parameter; }
        virtual void SetParametersString(wstring parameter) { _parameter = parameter; Update(); }
        virtual wstring GetValue() { return _value; }
        virtual void SetValue(wstring value) { _value = value; Update(); }

    protected:
        Item* _pItem;

    private:
        wstring _parameter;
        wstring _value;

    };
}