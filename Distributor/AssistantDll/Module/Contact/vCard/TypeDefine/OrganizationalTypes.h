#pragma  once


namespace ND91Assistant
{
    // These types are concerned with information associated with
    // characteristics of the organization or organizational units of the
    // object the vCard represents.
    class OrganizationalTypes
    {
    public:
        OrganizationalTypes();
        virtual ~OrganizationalTypes();

        // To specify the job title, functional position or function of the object the vCard represents.
        // TITLE:Director\, Research and Development
        static wstring TITLE;

        // To specify information concerning the role, occupation, or business category of the object the vCard represents.
        // ROLE:Programmer
        static wstring ROLE;

        // To specify a graphic image of a logo associated with the object the vCard represents.
        // LOGO;VALUE=uri:http://www.abc.com/pub/logos/abccorp.jpg
        // LOGO;ENCODING=b;TYPE=JPEG:MIICajCCAdOgAwIBAgICBEUwDQYJKoZIhvcN
        // AQEEBQAwdzELMAkGA1UEBhMCVVMxLDAqBgNVBAoTI05ldHNjYXBlIENvbW11bm
        // ljYXRpb25zIENvcnBvcmF0aW9uMRwwGgYDVQQLExNJbmZvcm1hdGlvbiBTeXN0
        static wstring LOGO;

        // To specify information about another person who will act on behalf of the individual or resource associated with the vCard.
        // AGENT;VALUE=uri: CID:JQPUBLIC.part3.960129T083020.xyzMail@host3.com
        // AGENT:BEGIN:VCARD\nFN:Susan Thomas\nTEL:+1-919-555-1234\nEMAIL\;INTERNET:sthomas@host.com\nEND:VCARD\n
        static wstring AGENT;

        // To specify the organizational name and units associated with the vCard.
        // ORG:ABC\, Inc.;North American Division;Marketing
        static wstring ORG;
    };
}