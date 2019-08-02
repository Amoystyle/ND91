#pragma  once


namespace ND91Assistant
{
    // These types are concerned with information associated with
    // geographical positions or regions associated with the object the
    // vCard represents.
    class GeographicalTypes
    {
    public:
        GeographicalTypes();
        virtual ~GeographicalTypes();

        // To specify information related to the time zone of the object the vCard represents.
        // TZ:-05:00
        // TZ;VALUE=text:-05:00; EST; Raleigh/North America
        // ;This example has a single value, not a structure text value.
        static wstring TZ;

        // To specify information related to the global positioning of the object the vCard represents.
        // GEO:37.386013;-122.082932
        // GEO:¾­¶È;Î³¶È
        static wstring GEO;
    };
}