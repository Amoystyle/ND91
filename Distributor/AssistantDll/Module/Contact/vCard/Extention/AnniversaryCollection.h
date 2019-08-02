#pragma  once


namespace ND91Assistant
{
    class vCard;
    class Anniversary;

    class AnniversaryCollection
    {
    public:
        AnniversaryCollection(vCard* owner);
        virtual ~AnniversaryCollection();

        void Add(time_t value);

        void Add(wstring label, time_t value);

        void AddToCollecton(wstring value);

        void AddToCollecton(wstring parameters, wstring value);

        void Remove(Anniversary* pItem);

        void Clear();

    private:
        vCard* _pOwner;
        vector<Anniversary*> _collection;
    };
}