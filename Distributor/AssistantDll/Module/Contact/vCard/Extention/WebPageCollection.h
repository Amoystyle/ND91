#pragma  once


namespace ND91Assistant
{
    class vCard;
    class WebPage;

    class WebPageCollection
    {
    public:
        WebPageCollection(vCard* owner);
        virtual ~WebPageCollection();

        void Add(wstring label, wstring url);

        void AddToCollecton(wstring value);

        void AddToCollecton(wstring parameters, wstring value);

        void Remove(WebPage* pItem);

        void Clear();

    private:
        vCard* _pOwner;
        vector<WebPage*> _collection;
    };
}