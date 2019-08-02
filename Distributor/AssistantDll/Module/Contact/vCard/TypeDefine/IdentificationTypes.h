#pragma  once


namespace ND91Assistant
{
    // These types are used in the vCard profile to capture information
    // associated with the identification and naming of the person or
    // resource associated with the vCard.
    class IdentificationTypes
    {
    public:
        IdentificationTypes();
        virtual ~IdentificationTypes();

        // To specify the formatted text corresponding to the name of the object the vCard represents.
        // The property MUST be present in the vCard object.
        // FN:Mr. John Q. Public\, Esq.
        static wstring FN;

        // To specify the components of the name of the object the vCard represents.
        // N:Public;John;Quinlan;Mr.;Esq.
        // N:Stevenson;John;Philip,Paul;Dr.;Jr.,M.D.,A.C.P.
        static wstring N;

        // To specify the text corresponding to the nickname of the object the vCard represents.
        // NICKNAME:Robbie
        // NICKNAME:Jim,Jimmie
        static wstring NICKNAME;

        // To specify an image or photograph information that annotates some aspect of the object the vCard represents.
        // The encoding MUST be reset to "b" using the ENCODING parameter in order to specify inline, encoded binary data.
        // PHOTO;VALUE=uri:http://www.abc.com/pub/photos/jqpublic.gif
        // PHOTO;ENCODING=b;TYPE=JPEG:MIICajCCAdOgAwIBAgICBEUwDQYJKoZIhvcN
        // AQEEBQAwdzELMAkGA1UEBhMCVVMxLDAqBgNVBAoTI05ldHNjYXBlIENvbW11bm
        // ljYXRpb25zIENvcnBvcmF0aW9uMRwwGgYDVQQLExNJbmZvcm1hdGlvbiBTeXN0
        static wstring PHOTO;

        /// To specify the birth date of the object the vCard represents.
        /// BDAY:1996-04-15
        /// BDAY:1953-10-15T23:10:00Z
        /// BDAY:1987-09-27T08:30:00-06:00
        static wstring BDAY;

        static wstring ASSISTANTNAME;
        static wstring ASSISTANTTEL;
        static wstring CHILDREN;
        static wstring SPOUSE;
        static wstring SUFFIX;
        static wstring MANAGER;
        static wstring ACCOUNT;
        static wstring CUSTOMERID;
        static wstring STATUSID;
    };
}