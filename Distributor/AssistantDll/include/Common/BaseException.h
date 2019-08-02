#pragma once

#include "Common/Log.h"
#include "Common/CodeOperation.h"

#include <string>
#include <sstream>

using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL BaseException
    {
    public:
        BaseException(int code, string filename, int lineNo)
            : _errorCode(code), _filename(filename), _lineNo(lineNo)

        {
            wstring s = CCodeOperation::GB2312ToUnicode(filename);
            wostringstream woss;
            woss << L" Exception : error code = " << code
                << L" in file " << s
                << L"(" << lineNo << L")";
            LOG->WriteError(woss.str());
        }

        int GetErrorCode()
        {
            return _errorCode;
        }
    private:
        int _errorCode;

        string _filename;
        int _lineNo;
    };
}