#pragma  once


namespace ND91Assistant
{
    // These types are concerned with the security of communication pathways
    // or access to the vCard.
    class SecurityTypes
    {
    public:
        SecurityTypes();
        virtual ~SecurityTypes();

        // To specify the access classification for a vCard object.
        // CLASS:PUBLIC
        // CLASS:PRIVATE
        // CLASS:CONFIDENTIAL
        static wstring CLASS;

        // To specify a public key or authentication certificate
        // associated with the object that the vCard represents.
        // KEY;ENCODING=b:MIICajCCAdOgAwIBAgICBEUwDQYJKoZIhvcNAQEEBQA
        // wdzELMAkGA1UEBhMCVVMxLDAqBgNVBAoTI05ldHNjYXBlIENbW11bmljYX
        // Rpb25zIENvcnBvcmF0aW9uMRwwGgYDVQQLExNJbmZvcm1hdGlvbiBTeXN0
        // ZW1zMRwwGgYDVQQDExNyb290Y2EubmV0c2NhcGUuY29tMB4XDTk3MDYwNj
        // E5NDc1OVoXDTk3MTIwMzE5NDc1OVowgYkxCzAJBgNVBAYTAlVTMSYwJAYD
        // VQQKEx1OZXRzY2FwZSBDb21tdW5pY2F0aW9ucyBDb3JwLjEYMBYGA1UEAx
        // MPVGltb3RoeSBBIEhvd2VzMSEwHwYJKoZIhvcNAQkBFhJob3dlc0BuZXRz
        // Y2FwZS5jb20xFTATBgoJkiaJk/IsZAEBEwVob3dlczBcMA0GCSqGSIb3DQ
        // EBAQUAA0sAMEgCQQC0JZf6wkg8pLMXHHCUvMfL5H6zjSk4vTTXZpYyrdN2
        // dXcoX49LKiOmgeJSzoiFKHtLOIboyludF90CgqcxtwKnAgMBAAGjNjA0MB
        // EGCWCGSAGG+EIBAQQEAwIAoDAfBgNVHSMEGDAWgBT84FToB/GV3jr3mcau
        // +hUMbsQukjANBgkqhkiG9w0BAQQFAAOBgQBexv7o7mi3PLXadkmNP9LcIP
        // mx93HGp0Kgyx1jIVMyNgsemeAwBM+MSlhMfcpbTrONwNjZYW8vJDSoi//y
        // rZlVt9bJbs7MNYZVsyF1unsqaln4/vy6Uawfg8VUMk1U7jt8LYpo4YULU7
        // UZHPYVUaSgVttImOHZIKi4hlPXBOhcUQ==
        static wstring KEY;

#pragma  region - About KEY -

        /*
        Type special notes: The type can also include the type parameter TYPE
        to specify the public key or authentication certificate format. The
        parameter type should specify an IANA registered public key or
        authentication certificate format. The parameter type can also
        specify a non-standard format.
        */

#pragma endregion
    };
}