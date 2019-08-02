#pragma  once


namespace ND91Assistant
{
    class vCard;
    class OrganizationEx;

    class OrganizationExCollection
    {
    public:
        OrganizationExCollection(vCard* owner);
        virtual ~OrganizationExCollection();

        void Add(wstring title, wstring location);

        void Add(wstring label, wstring title, wstring location);

        void AddToCollecton(wstring value);

        void AddToCollecton(wstring parameters, wstring value);

        void Remove(OrganizationEx* pItem);

        void Clear();

    private:
        vCard* _pOwner;
        vector<OrganizationEx*> _collection;
    };
}