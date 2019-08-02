#pragma  once

namespace ND91Assistant
{
    // vCard structure item.
    class Item
    {
    public:
        Item(wstring name, wstring parameters, wstring value);
        virtual ~Item();

        wstring GetName() { return _name; }
        void SetName(wstring name) { _name = name; }
        wstring GetParameters() { return _parameters; }
        void SetParameters(wstring parameters) { _parameters = parameters; }
        wstring GetValue() { return _value; }
        void SetValue(wstring value) { _value = value; }

        virtual wstring ToString();
        bool IsEmpty();

    private:
        wstring _name;
        wstring _parameters;
        wstring _value;
    };
}