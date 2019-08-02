#pragma  once

namespace ND91Assistant
{
    class vName
    {
    public:
        vName(wstring lastName, 
             wstring firstName, 
             wstring middleName, 
             wstring honorificPrefix, 
             wstring honorificSuffix);
        vName(wstring lastName, 
             wstring firstName, 
             wstring middleName, 
             wstring honorificPrefix, 
             wstring honorificSuffix, 
             wstring lastPhonetic, 
             wstring firstPhonetic, 
             wstring middlePhonetic);

        virtual ~vName();

        // Name string. (eg. Mr. LastName MiddleName FirstName Sir)
        virtual wstring ToString();

        // vCard N structure string
        wstring ToValueString();

        // Parses name info from vCard N structure string.
        static vName* Parse(wstring value);

        static vName* Instance(wstring lastName, 
                              wstring firstName, 
                              wstring middleName, 
                              wstring honorificPrefix, 
                              wstring honorificSuffix, 
                              wstring lastPhonetic, 
                              wstring firstPhonetic, 
                              wstring middlePhonetic);

        wstring GetLastName() { return _lastName; }
        wstring GetLastPhonetic() { return _lastPhonetic; }
        wstring GetFirstName() { return _firstName; }
        wstring GetFirstPhonetic() { return _firstPhonetic; }
        wstring GetMiddleName() { return _middleName; }
        wstring GetMiddlePhonetic() { return _middlePhonetic; }
        wstring GetHonorificPrefix() { return _honorificPrefix; }
        wstring GetHonorificSuffix() { return _honorificSuffix; }

    private:
        wstring _lastName;
        wstring _lastPhonetic;
        wstring _firstName;
        wstring _firstPhonetic;
        wstring _middleName;
        wstring _middlePhonetic;
        wstring _honorificPrefix;
        wstring _honorificSuffix;
    };
}