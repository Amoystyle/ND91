#pragma  once

namespace ND91Assistant
{
    class ItemCollection;
    class DeliveryAddressCollection;
    class PhoneNumberCollection;
    class EmailAddressCollection;
    class UrlCollection;
    class NickNameCollection;
    class NoteCollection;
    class vName;

    // Rfc 2426 vCard implementation.
    class vCard
    {
    public:
        vCard();
        virtual ~vCard();

    public:
        void ToFile(wstring file);
        virtual wstring ToString();
        //void ToFile(wstring file, wstring pKey);

        void FromFile(wstring file);
        void FromString(wstring source);
        static vector<wstring> SplitLine(wstring line, wchar_t splitChar, size_t count);

    private:
        wstring ToString(wstring pKey);

#pragma region  get_and_set
    public:
        DeliveryAddressCollection*  GetDeliveryAddressCollection();
        PhoneNumberCollection*      GetPhoneNumberCollection();
        EmailAddressCollection*     GetEmailAddressCollection();
        UrlCollection*              GetUrlCollection();
        NickNameCollection*         GetNickNameCollection();
        NoteCollection*             GetNoteCollection();
        ItemCollection*             GetItemCollection();

        wstring GetUid();
        void SetUid(wstring uid);
        wstring GetVersion();
        void SetVersion(wstring version);
        wstring GetCategory();
        void SetCategory(wstring category);
        time_t GetLastModifyTime();
        void SetLastModifyTime(time_t lastModifyTime);
        wstring GetSort();
        void SetSort(wstring sort);
        wstring GetSoundURI();
        void SetSoundURI(wstring soundURI);
        wstring GetProdID();
        void SetProdID(wstring proid);

        vName* GetName();
        void SetName(vName* name);
        wstring GetFormattedName();
        void SetFormattedName(wstring formattedName);
        wstring GetImage();
        void SetImage(wstring filePath);
        wstring GetPhotoURI();
        void SetPhotoURI(wstring photoURI);
        time_t GetBirthdate();
        void SetBirthdate(time_t birthdate);
        wstring GetCustomRingtone();
        void SetCustomRingtone(wstring customRingtone);
        wstring GetAssistantName();
        void SetAssistantName(wstring assistantName);
        wstring GetAssistantTel();
        void SetAssistantTel(wstring assistantTel);
        wstring GetChildren();
        void SetChildren(wstring children);
        wstring GetSpouse();
        void SetSpouse(wstring spouse);
        wstring GetSuffix();
        void SetSuffix(wstring suffix);
        wstring GetManager();
        void SetManager(wstring manager);
        wstring GetStatusId();
        void SetStatusId(wstring statusId);
        wstring GetAccount();
        void SetAccount(wstring account);
        wstring GetCustomerId();
        void SetCustomerId(wstring customerId);

        wstring GetTimezone();
        void SetTimezone(wstring timezone);
        wstring GetGeo();
        void SetGeo(wstring geo);

        wstring GetTitle();
        void SetTitle(wstring title);
        wstring GetRole();
        void SetRole(wstring role);
        wstring GetOrganization();
        void SetOrganization(wstring organization);
//         Image GetLogo();
//         void SetLogo(Image logo);
        wstring GetLogoURI();
        void SetLogoURI(wstring logoURI);
        vCard* GetvCard() { return _pAgent; }
        void SetvCard(vCard* pAgent) { _pAgent = pAgent; }

#pragma endregion
    protected:
        void InitvCard();
        void UnInitvCard();
        virtual void OnInit();
        virtual void Parse(wstring source);

    private:
        // ·µ»ØÍ¼Æ¬Êý¾Ý×Ö·û´®
        wstring BuildHeadPhoto(wstring imageFilePath);

    public:
        static wstring BEGIN;
        static wstring END;
        static wstring Name_Param_Splitor;
        static wstring Name_Value_Splitor;
        static wstring BlankSpace;
        static wstring CUSTOM_TAG;

    private:
        DeliveryAddressCollection*  _pAddresses;
        PhoneNumberCollection*      _pPhoneNumbers;
        EmailAddressCollection*     _pEmailAddresses;
        UrlCollection*              _pUrls;
        NickNameCollection*         _pNickNames;
        NoteCollection*             _pNotes;

        ItemCollection*             _pItems;
        vCard*                      _pAgent;
    };
}