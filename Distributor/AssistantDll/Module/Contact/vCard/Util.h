#pragma  once

namespace ND91Assistant
{
    class Util
    {
    public:
        static time_t PareDateTime(wstring dateTimeStr);
        static wstring FoldLine(wstring line);
        static bool StartLine(wstring line);
        static bool StartWith(wstring line, wstring KeyWord);
    };
}