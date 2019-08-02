#pragma  once


namespace ND91Assistant
{
    // These types are concerned with information related to the delivery
    // addressing or label for the vCard object.
    class DeliveryAddressingTypes
    {
    public:
        DeliveryAddressingTypes();
        virtual ~DeliveryAddressingTypes();

        // To specify the components of the delivery address for the vCard object.
        // ADR;TYPE=dom,home,postal,parcel:;;123 Main Street;Any Town;CA;91921-1234
        static wstring ADR;

#pragma  region - About ADR -

        /*
        Type special notes: The structured type value consists of a sequence
        of address components. The component values MUST be specified in
        their corresponding position. The structured type value corresponds,
        in sequence, to the post office box; the extended address; the street
        address; the locality (e.g., city); the region (e.g., state or
        province); the postal code; the country name. When a component value
        is missing, the associated component separator MUST still be
        specified.

        The text components are separated by the SEMI-COLON character (ASCII
        decimal 59). Where it makes semantic sense, individual text
        components can include multiple text values (e.g., a "street"
        component with multiple lines) separated by the COMMA character
        (ASCII decimal 44).

        The type can include the type parameter "TYPE" to specify the
        delivery address type. The TYPE parameter values can include "dom" to
        indicate a domestic delivery address; "intl" to indicate an
        international delivery address; "postal" to indicate a postal
        delivery address; "parcel" to indicate a parcel delivery address;
        "home" to indicate a delivery address for a residence; "work" to
        indicate delivery address for a place of work; and "pref" to indicate
        the preferred delivery address when more than one address is
        specified. These type parameter values can be specified as a
        parameter list (i.e., "TYPE=dom;TYPE=postal") or as a value list
        (i.e., "TYPE=dom,postal"). This type is based on semantics of the
        X.520 geographical and postal addressing attributes. The default is
        "TYPE=intl,postal,parcel,work". The default can be overridden to some
        other set of values by specifying one or more alternate values. For
        example, the default can be reset to "TYPE=dom,postal,work,home" to
        specify a domestic delivery address for postal delivery to a
        residence that is also used for work.
        */

#pragma endregion

        // To specify the formatted text corresponding to delivery address of the object the vCard represents.
        // LABEL;TYPE=dom,home,postal,parcel:Mr.John Q. Public\, Esq.\n
        // Mail Drop: TNE QB\n123 Main Street\nAny Town\, CA  91921-1234
        // \nU.S.A.
        static wstring LABEL;

#pragma region - About LABEL -

        /*
        Type special notes: The type value is formatted text that can be used
        to present a delivery address label for the vCard object. The type
        can include the type parameter "TYPE" to specify delivery label type.
        The TYPE parameter values can include "dom" to indicate a domestic
        delivery label; "intl" to indicate an international delivery label;
        "postal" to indicate a postal delivery label; "parcel" to indicate a
        parcel delivery label; "home" to indicate a delivery label for a
        residence; "work" to indicate delivery label for a place of work; and
        "pref" to indicate the preferred delivery label when more than one
        label is specified. These type parameter values can be specified as a
        parameter list (i.e., "TYPE=dom;TYPE=postal") or as a value list
        (i.e., "TYPE=dom,postal"). This type is based on semantics of the
        X.520 geographical and postal addressing attributes. The default is
        "TYPE=intl,postal,parcel,work". The default can be overridden to some
        other set of values by specifying one or more alternate values. For
        example, the default can be reset to "TYPE=intl,post,parcel,home" to
        specify an international delivery label for both postal and parcel
        delivery to a residential location.
        */

#pragma endregion

    };
}