#pragma  once


namespace ND91Assistant
{
    // These types are concerned with additional explanations, such as that
    // related to informational notes or revisions specific to the vCard.
    class ExplanatoryTypes
    {
    public:
        ExplanatoryTypes();
        virtual ~ExplanatoryTypes();

        // To specify application category information about the vCard.
        // CATEGORIES:TRAVEL AGENT
        // CATEGORIES:INTERNET,IETF,INDUSTRY,INFORMATION TECHNOLOGY
        static wstring CATEGORIES;

        // To specify supplemental information or a comment that is associated with the vCard.
        // NOTE:This fax number is operational 0800 to 1715 EST\, Mon-Fri.
        static wstring NOTE;

        /// To specify the identifier for the product that created the vCard object.
        /// PRODID:-//ONLINE DIRECTORY//NONSGML Version 1//EN
        static wstring PRODID;

#pragma  region - About PRODID -

        /*
        Type special notes: Implementations SHOULD use a method such as that
        specified for Formal Public Identifiers in ISO 9070 to assure that
        the text value is unique.
        */

#pragma endregion

        // To specify revision information about the current vCard.
        // REV:1995-10-31T22:27:10Z
        // REV:1997-11-15
        static wstring REV;

        // To specify the family name or given name text to be
        // used for national-language-specific sorting of the FN and N types.
        // FN:Rene van der Harten
        // N:van der Harten;Rene;J.;Sir;R.D.O.N.
        // SORT-STRING:Harten
        // 
        // FN:Robert Pau Shou Chang
        // N:Pau;Shou Chang;Robert
        // SORT-STRING:Pau
        static wstring SORT_STRING;

        // To specify a digital sound content information that
        // annotates some aspect of the vCard. By default this type is used to
        // specify the proper pronunciation of the name type value of the vCard.
        // SOUND;TYPE=BASIC;VALUE=uri:CID:JOHNQPUBLIC.part8.19960229T080000.xyzMail@host1.com
        // SOUND;TYPE=BASIC;ENCODING=b:MIICajCCAdOgAwIBAgICBEUwDQYJKoZIhvcN
        // AQEEBQAwdzELMAkGA1UEBhMCVVMxLDAqBgNVBAoTI05ldHNjYXBlIENvbW11bm
        // ljYXRpb25zIENvcnBvcmF0aW9uMRwwGgYDVQQLExNJbmZvcm1hdGlvbiBTeXN0
        static wstring SOUND;

        // To specify a value that represents a globally unique
        // identifier corresponding to the individual or resource associated
        // with the vCard.
        // UID:19950401-080045-40000F192713-0052
        static wstring UID;

        // To specify a uniform resource locator associated with
        // the object that the vCard refers to.
        // URL:http://www.swbyps.restaurant.french/~chezchic.html
        static wstring URL;

        // To specify the version of the vCard specification used to format this vCard.
        // VERSION:3.0
        static wstring VERSION;
    };
}