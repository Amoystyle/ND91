#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    class AdbUploadDirCommand
    {
    public:
        AdbUploadDirCommand(string sn,wstring id, wstring wstrPCPath, wstring wstrPhonePath);
        ~AdbUploadDirCommand();

		virtual void Execute();

        bool Success(){return _success;}
        
    private:
        string _sn;
		wstring _id;
        wstring _strPCPath;
        wstring _strPhonePath;
        bool   _success;
    };
}