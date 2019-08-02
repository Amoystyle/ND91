#include "stdafx.h"

#include "IosDaemon.h"
#include "Common/Path.h"
#include "Common/NDBase64.h"
#include "Common/Path.h"
#include "Device/IosPublicFunc.h"
#include "Device/NDCrEnLib.h"


unsigned char CRYPT_KEY[17] = 
{
    0xef,
    0xab,
    0xe,
    0x39,
    0xaa,
    0xbb,
    0x89,
    0x20,
    0x3,
    0x67,
    0xcb,
    0xec,
    0xaa,
    0xa4,
    0xa9,
    0x99,
    0x00
};

IosDaemon::IosDaemon(IIosFileSystem* pFileStream, wstring strUUID)
{
    _pFileStream = pFileStream;
    _bTurnCypher =  false; //守护是否运行及开启交互
    _szDaemonCypherBytes = L""; //交互口令
    _strUUID = strUUID;
}

IosDaemon::~IosDaemon(void)
{
}

bool IosDaemon::PhoneDaemonIsStillRunning()
{
    if (!_pFileStream || _strUUID.length() <= 0)
        return false;

    bool blnReturn = false;
    try
    {
        std::wstring folder = Path::GlobalTemp();

        wstring strCmd = folder + L"TestDaemon";
        wstring strCmdOniPhone = Path::IosDeviceCommandPath() + L"TestDaemon.s" + _strUUID;
        wstring strTqServerOniPhone = Path::DeviceDeamonPath();

        if (!CFileOperation::IsFileExist(strCmd.c_str()))
        {
            FILE* pFile = fopen(CCodeOperation::WstringToString(strCmd).c_str(), "w+");
            if (!pFile)
            {
                THROW_SYSTEM_EXCETPION(FILE_IO_FORBIDDEN);
            }
            fclose(pFile);
        }

        if (_pFileStream->Ios_Exists(strTqServerOniPhone) && 
            _pFileStream->Ios_Exists(Path::IosDeviceCommandPath()))
        {
            if (_pFileStream->Ios_CopyToPhone(strCmd, strCmdOniPhone) == Transfer_Success)
            {
                // 判断 mstrPXL_PxlPickup 文件是否还在，不在了就表示安装成功，
                //或者超时（6 秒）.测试3秒，有些机器会守护起来比较慢，会造成误判为未安装守护
                unsigned long ulLastTime = GetTickCount();
                //unsigned long ulLastTime = IosPublicFunc::MyGetTickCount();
                do 
                {
                    Sleep(500);
                } 
                while (_pFileStream->Ios_Exists(strCmdOniPhone) && (GetTickCount() - ulLastTime < 6000));
                //while (_pFileStream->Ios_Exists(strCmdOniPhone) && (IosPublicFunc::MyGetTickCount() - ulLastTime < 6000));

                // 文件还存在就是失败
                if (_pFileStream->Ios_Exists(strCmdOniPhone))
                {
                    _pFileStream->Ios_DeleteFile(strCmdOniPhone);
                    blnReturn = false;
                }
                else
                {
                    _pFileStream->Ios_DeleteFile(strCmdOniPhone + L".done");
                    blnReturn = true;
                }
            }
        }
    }
    catch (...)
    {
        return false;
    }

    if (blnReturn)
        _bTurnCypher = GetDaemonCypher();

    return blnReturn;
}

bool IosDaemon::GetDaemonCypher()
{
    if (!_pFileStream || _strUUID.length() <= 0)
        return false;

    //DeviceData* pDevice = DEVICE_MANAGER->FindDevice(_strUUID);
    //RETURN_FALSE_IF( ! pDevice );
    //std::wstring folder = pDevice->GetPath()->GetLocalDeamonFolder();
    std::wstring folder = Path::GlobalTemp() + L"DaemonFolderTemp\\";
    CFileOperation::MakeLocalDir(folder);

    wostringstream woss;
    woss << time(NULL);

    wstring strCmd = folder + L"session" + woss.str();
    wstring cyfile = L"/var/root/Media/ndDaemon/session.plist";
    string sKey = "";

    if (_pFileStream->Ios_Exists(cyfile))
    {
        sKey = CCodeOperation::WstringToString(IosPublicFunc::GetTypeValue(_pFileStream, cyfile, L"session"));
        sKey = ReadDaemonCypher(sKey, 16);
        if (sKey.length() > 0)
        {
            string newKey = sKey;
            vector<string> keyList = CStrOperation::parseStrings(sKey.c_str(), sKey.length(), ',');
            if (keyList.size() > 0)
                newKey = keyList.at(0);

            //公用密钥 
            _szDaemonCypherBytes = CCodeOperation::StringToWstring(newKey.c_str(), newKey.length());
        }
    }

    return (sKey.length() > 0);
}

wstring IosDaemon::CreatDaemonCypher(wstring strTalk, bool bTurnCypher, wstring strDaemonCypherBytes)
{
    if (!bTurnCypher || strDaemonCypherBytes.length() <= 0)
        return strTalk;

    string strDaemonCypherBytesUtf8 = CCodeOperation::UnicodeToUTF_8(strDaemonCypherBytes);
    unsigned char* szDaemonCypherBytes = (unsigned char*)(strDaemonCypherBytesUtf8.c_str());

    string iTalk = CCodeOperation::UnicodeToUTF_8(strTalk);

    string cipher = iTalk;
    string newTalk = iTalk;

    int nMaxValue = (int)ceil((double)iTalk.length()/8 - 1);

    unsigned char* pCryptBuffTemp = new unsigned char[8];
    memset(pCryptBuffTemp, 0, 8);
    int nCryptBuff = 8*(nMaxValue+1);
    unsigned char* pCryptBuff = new unsigned char[nCryptBuff + 1];
    memset(pCryptBuff, 0, nCryptBuff + 1);

    for (int i=0; i<=nMaxValue; i++)
    {
        int nMinLen = iTalk.length() - i*8;
        int sLen = (nMinLen<8?nMinLen:8);
        string s = iTalk.substr(i*8, sLen);
        memset(pCryptBuffTemp, 0, 8);
        CNDCrEnLib::Ios_Encrypt(s.c_str(), szDaemonCypherBytes, pCryptBuffTemp);
        memcpy(pCryptBuff+8*i, pCryptBuffTemp, 8);
    }

    int nSizeOut = CNDBase64::Base64encode_len(nCryptBuff);
    char* b64Out = new char[nSizeOut];
    memset(b64Out, 0, nSizeOut);
    CNDBase64::Base64encode(b64Out, (const char*)pCryptBuff, nCryptBuff);

    cipher = b64Out;
    cipher = CStrOperation::Replace(cipher, "/", "_");
    newTalk = "nd_com_cn" + cipher;

    if (b64Out)
    {
        delete []b64Out;
        b64Out = NULL;
    }

    if (pCryptBuffTemp)
    {
        delete []pCryptBuffTemp;
        pCryptBuffTemp = NULL;
    }

    if (pCryptBuff)
    {
        delete []pCryptBuff;
        pCryptBuff = NULL;
    }

    return CCodeOperation::UTF_8ToUnicode(newTalk);
}


string IosDaemon::ReadDaemonCypher(string iTalk, int nLen)
{
    string newTalk = "";

    string cipher = CStrOperation::Replace(iTalk, "_", "/");
    
    int nSizeOut = CNDBase64::Base64decode_len(cipher.c_str());
    char* plaintText = new char[nSizeOut];
    memset(plaintText, 0, nSizeOut);

    int nSizeRet = CNDBase64::Base64decode(plaintText, cipher.c_str());

    unsigned char* szKey = new unsigned char[8];
    memset(szKey, 0, 8);
    char szTemp[/*MAX_PATH*/260] = {0};
    int nMaxValue = (int)ceil((double)nSizeRet/8 - 1);
    for (int i=0; i<=nMaxValue; i++)
    {
        memcpy(szKey, plaintText+i*8, 8);
        memset(szTemp, 0, 260);
        CNDCrEnLib::Ios_Decrypt(szKey, CRYPT_KEY, szTemp);
        newTalk += szTemp;
    }

    if (szKey)
    {
        delete []szKey;
        szKey = NULL;
    }

    if (plaintText)
    {
        delete []plaintText;
        plaintText = NULL;
    }

    return newTalk;
}

wstring IosDaemon::GetDaemonVer()
{
    wstring strVer = L"";

    if (_pFileStream)
    {
        wstring strVerFileOnDev = L"/private/var/root/Media/ndDaemon/Info.plist";
        if (_pFileStream->Ios_Exists(strVerFileOnDev + L".done"))
            _pFileStream->Ios_Rename(strVerFileOnDev + L".done", strVerFileOnDev);

        strVer = IosPublicFunc::GetTypeValue(_pFileStream, strVerFileOnDev, L"Version");
    }

    return strVer;
}