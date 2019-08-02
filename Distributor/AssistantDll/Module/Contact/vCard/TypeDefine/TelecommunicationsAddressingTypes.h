#pragma  once


namespace ND91Assistant
{
    // These types are concerned with information associated with the
    // telecommunications addressing of the object the vCard represents.
    class TelecommunicationsAddressingTypes
    {
    public:
        TelecommunicationsAddressingTypes();
        virtual ~TelecommunicationsAddressingTypes();

        // To specify the telephone number for telephony communication with the object the vCard represents.
        // TEL;TYPE=work,voice,pref,msg:+1-213-555-1234
        static wstring TEL;

#pragma  region - About TEL -

        /*
        Type special notes: The value of this type is specified in a
        canonical form in order to specify an unambiguous representation of
        the globally unique telephone endpoint. This type is based on the
        X.500 Telephone Number attribute.

        The type can include the type parameter "TYPE" to specify intended
        use for the telephone number. The TYPE parameter values can include:
        "home" to indicate a telephone number associated with a residence,
        "msg" to indicate the telephone number has voice messaging support,
        "work" to indicate a telephone number associated with a place of
        work, "pref" to indicate a preferred-use telephone number, "voice" to
        indicate a voice telephone number, "fax" to indicate a facsimile
        telephone number, "cell" to indicate a cellular telephone number,
        "video" to indicate a video conferencing telephone number, "pager" to
        indicate a paging device telephone number, "bbs" to indicate a
        bulletin board system telephone number, "modem" to indicate a MODEM
        connected telephone number, "car" to indicate a car-phone telephone
        number, "isdn" to indicate an ISDN service telephone number, "pcs" to
        indicate a personal communication services telephone number. The
        default type is "voice". These type parameter values can be specified
        as a parameter list (i.e., "TYPE=work;TYPE=voice") or as a value list
        (i.e., "TYPE=work,voice"). The default can be overridden to another
        set of values by specifying one or more alternate values. For
        example, the default TYPE of "voice" can be reset to a WORK and HOME,
        VOICE and FAX telephone number by the value list
        "TYPE=work,home,voice,fax".
        */

#pragma endregion

        // To specify the electronic mail address for communication with the object the vCard represents.
        // EMAIL;TYPE=internet:jqpublic@xyz.dom1.com
        // EMAIL;TYPE=internet:jdoe@isp.net
        // EMAIL;TYPE=internet,pref:jane_doe@abc.com
        static wstring EMAIL;

        // To specify the type of electronic mail software that is used by the individual associated with the vCard.
        // MAILER:PigeonMail 2.1
        static wstring MAILER;

        static wstring DATE;


    };
}