#pragma once
#include "AdbShellCommand.h"
#include "AdbCheckDir.h"

namespace ND91Assistant
{
    //创建目录，多级递归创建
    class AdbCreateDir : public AdbShellCommand
    {
    public:
        AdbCreateDir(const wstring sn, const wstring strPhonePath)
        {
            _sn = CCodeOperation::UnicodeToUTF_8(sn);
            _strPhonePath = strPhonePath;
        };

        virtual void Execute()
        {            
            wstring path = _strPhonePath;
            if(path.length() < 1 || path == L"/")
            {
                _success = false;
                return;
            }
            int nIndex = path.find(L'/', 1);
            while (nIndex > 0)
            {
                wstring dir(path, 0, nIndex);
				AdbCheckDir checkDir(CCodeOperation::UTF_8ToUnicode(_sn), dir);
                checkDir.Execute();
                if (!checkDir.Success())//目录不存在则创建
                    if (!createSingerDir(dir))
                        break;
                nIndex = path.find('/', nIndex+1);
            }
        };
    protected:
        virtual bool ParseReturnData()
        {
            if (_returnData.length() == 0 || _returnData.find("mkdir: can't set permissions of directory") >= 0)
                _success = true;
            else
                _success = false;
            return _success;
        };  

    private:
        //创建单级目录
        bool createSingerDir(const wstring path)
        {
            if(path.length()<1 || path == L"/")
            {
                return false;
            }
			_strCommand = "mkdir \"" + CCodeOperation::UnicodeToUTF_8(path) + "\"";
            CommonExecute(_strCommand);
            return _success;
        };
    private:
        wstring _strPhonePath;
    };
}