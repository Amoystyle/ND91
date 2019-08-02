#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "IPAPackageHelper.h"

#include "Core/DeviceManager.h"
#include "Common/NDZlib.h"
#include "Common/Path.h"

#define     CONTAINERNAME       "Container"                // 存档文件目录
#define     PAYLOADNAME         "Payload"                  // 主程序目录
#define     INFOPLISTFILENAME   "Info.plist"
#define     ITUNESMETADATANAME  "iTunesMetadata.plist"
#define     ITUNESARTWORKNAME   "iTunesArtwork"

IPAPackageHelper::IPAPackageHelper(const DeviceInfo* pDevInfoPtr, AFCFileSysemBase* fileStream, wstring ipaFolderOnPc)
{
    _pDevInfoPtr            = pDevInfoPtr;
    _fileStream             = fileStream;
    _ipaFolderOnPc          = ipaFolderOnPc;
    _strFolderTemp          = L"";

    // 获取临时目录
    DeviceData* pDevice = DEVICE_MANAGER->FindIosDevice(_pDevInfoPtr->_ios_devptr);
    if (pDevice)
        _strFolderTemp = pDevice->GetPath()->GetLocalAppPath()+CFileOperation::GetGuidW();
    //InitializeCriticalSection(&_cs);

    _result = false;
}

IPAPackageHelper::~IPAPackageHelper(void)
{
    CFileOperation::DeleteDirectory(_strFolderTemp);
    //DeleteCriticalSection(&_cs);
}

bool IPAPackageHelper::IsArchivedIPA(wstring ipaFolderOnPc)
{
    bool isArchivedIPA = false;
    unzFile uf = unzOpen(ipaFolderOnPc.c_str());
    unz_global_info gi;
    RETURN_FALSE_IF(unzGetGlobalInfo(uf,&gi) != UNZ_OK);

    for (uLong i = 0; i < gi.number_entry; i++)
    {
        char filename_inzip[256] = {0};
        if (UNZ_OK != unzGetCurrentFileInfo(uf,NULL,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0))
            break;

        string tempfilename = filename_inzip;
        if (CStrOperation::startsWith(tempfilename, "Container/"))
        {
            isArchivedIPA = true;
            break;
        }

        if ((i+1) < gi.number_entry)
            RETURN_FALSE_IF(unzGoToNextFile(uf) != UNZ_OK);
    }
    unzClose(uf);

    return isArchivedIPA;
}

bool IPAPackageHelper::CheckPackageCopyRight(wstring ipaFolderOnPc)
{
    unzFile uf = unzOpen(ipaFolderOnPc.c_str());
    unz_global_info gi;
    RETURN_FALSE_IF(unzGetGlobalInfo(uf,&gi) != UNZ_OK);

    int authorizeFileCounter = 0;
    for (uLong i = 0; i < gi.number_entry; i++)
    {
        char filename_inzip[256] = {0};
        if (UNZ_OK != unzGetCurrentFileInfo(uf,NULL,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0))
            break;

        string tempfilename = filename_inzip;
        if (CStrOperation::endsWith(tempfilename, ".sinf")
            || CStrOperation::endsWith(tempfilename, ".supp"))
        {
            authorizeFileCounter++;
            if (2 == authorizeFileCounter)
                break;
        }

        if ((i+1) < gi.number_entry)
            RETURN_FALSE_IF(unzGoToNextFile(uf) != UNZ_OK);
    }
    unzClose(uf);

    return ( authorizeFileCounter == 2 );
}

bool IPAPackageHelper::TransferIPAPayload(wstring& ipaFolder, ProgressSubject* pProgressSubject)
{
    _pProgressSubject = pProgressSubject;
    ipaFolder = L"PublicStaging/";

    wstring fileName = CCodeOperation::StringToWstring(CFileOperation::GetGuid());
    if (!_fileStream->Ios_Exists(ipaFolder))
        RETURN_FALSE_IF(!_fileStream->Ios_CreateDirectory(ipaFolder));

    ipaFolder = ipaFolder + fileName + L".app/";
    if (!_fileStream->Ios_Exists(ipaFolder))
    {
        if (!_fileStream->Ios_CreateDirectory(ipaFolder))
        {
            //发现有些用户设备由于PublicStaging 文件夹权限被修改,导致安装失败.
            RETURN_FALSE_IF(!_pDevInfoPtr->_ios_bJailBreaked);//未越狱的机器,如果无法创建目录,找老乔去吧(dev-team 那边看过有用户未越狱设备文件系统紊乱的)Edited By xyq@2012-1-14 11:07:28
            RETURN_FALSE_IF(!_fileStream->Ios_DeleteFile(L"/private/var/mobile/Media/PublicStaging"));

            //尝试修复'PublicStaging' 文件夹失败.放弃安装
            if (_fileStream->Ios_CreateDirectory(L"PublicStaging"))
                RETURN_FALSE_IF(!_fileStream->Ios_CreateDirectory(ipaFolder))
        }
    }

    try
    {
        // 开始上传文件
        _ipaFolderOnIos = ipaFolder;
        //_uf = unzOpen(_ipaFolderOnPc.c_str());

        clock_t begin, end;
        begin = clock();

        HANDLE hThreadPayload[2];
        hThreadPayload[0] = CreateThread(NULL, 0, IPAPackageHelper::TransferFilePayloadRun, (LPVOID)this, 0, NULL);
        //hThreadPayload[1] = CreateThread(NULL, 0, IPAPackageHelper::TransferFilePayloadRun, (LPVOID)this, 0, NULL);
        WaitForMultipleObjects(1, hThreadPayload, true, INFINITE);

        end = clock();
        wchar_t temp[128] = {0};
        wsprintf(temp, L"use time: %d\n", end-begin);
        OutputDebugStr(temp);

        //unzClose(_uf);
    }
    catch (...)
    {
        LOG->WriteError(L"TransferIPAFile Error");
        _result = false;
    }

    return _result;
}

void IPAPackageHelper::TransferIPAArchive(ProgressSubject* pProgressSubject)
{
    _pProgressSubject = pProgressSubject;

    try
    {
        // 开始上传文件
        _ipaFolderOnIos = L"/";
        //_uf = unzOpen(_ipaFolderOnPc.c_str());

        HANDLE hThreadPayload[2];
        hThreadPayload[0] = CreateThread(NULL, 0, IPAPackageHelper::TransferFileContainerRun, (LPVOID)this, 0, NULL);
        //hThreadPayload[1] = CreateThread(NULL, 0, IPAPackageHelper::TransferFileContainerRun, (LPVOID)this, 0, NULL);
        WaitForMultipleObjects(1, hThreadPayload, true, INFINITE);

        //unzClose(_uf);
    }
    catch (...)
    {
        LOG->WriteError(L"TransferIPAArchive Error");
    }
}

DWORD WINAPI IPAPackageHelper::TransferFilePayloadRun(LPVOID lpParameter)
{
    IPAPackageHelper* pThis = (IPAPackageHelper*)lpParameter;
    pThis->TransferFile(true);
    return 0;
}

DWORD WINAPI IPAPackageHelper::TransferFileContainerRun(LPVOID lpParameter)
{
    IPAPackageHelper* pThis = (IPAPackageHelper*)lpParameter;
    pThis->TransferFile(false);
    return 0;
}

bool IPAPackageHelper::TransferFile(bool isTransferPayload)
{
    unzFile uf = unzOpen(_ipaFolderOnPc.c_str());
    _result = do_extract(uf,isTransferPayload) == 0;
    unzClose(uf);
    return _result;
}

__int64 IPAPackageHelper::GetPackageUncompressedSize()
{
    __int64 totalSize = 0;
    unzFile uf = unzOpen(_ipaFolderOnPc.c_str());
    unz_global_info gi;
    RETURN_VALUE_IF(unzGetGlobalInfo(uf,&gi) != UNZ_OK, 1);

    for (uLong i = 0; i < gi.number_entry; i++)
    {
        unz_file_info file_info;
        if (UNZ_OK != unzGetCurrentFileInfo(uf,&file_info,NULL,0,NULL,0,NULL,0))
            break;

        totalSize += file_info.uncompressed_size;
        if ((i+1) < gi.number_entry)
            RETURN_VALUE_IF(unzGoToNextFile(uf) != UNZ_OK, 0);
    }
    unzClose(uf);

    return totalSize;
}

int IPAPackageHelper::do_extract(unzFile uf,bool isTransferPayload)
{
    int err = UNZ_OK;
    unz_global_info gi;
    RETURN_VALUE_IF(unzGetGlobalInfo(uf,&gi) != UNZ_OK, 1);

    for (uLong i = 0; i < gi.number_entry; i++)
    {
        int ret = do_extract_currentfile(uf,isTransferPayload);
        if (ret != UNZ_OK)
        {
            err = UNZ_ERRNO;
            break;
        }

        if ((i+1) < gi.number_entry)
            RETURN_VALUE_IF(unzGoToNextFile(uf) != UNZ_OK, 2);
    }

    return err;
}

int IPAPackageHelper::do_extract_currentfile(unzFile uf,bool isTransferPayload)
{
    char filename_inzip[256] = {0};
    char* filename_withoutpath = NULL;
    char* p = NULL;
    int err = UNZ_OK;
    FILE *fout = NULL;
    void* buf = NULL;
    uInt size_buf = 0;

    unz_file_info file_info;
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
    RETURN_VALUE_IF(err != UNZ_OK, err);

    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf == NULL)
        return UNZ_INTERNALERROR;

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p)=='/') || ((*p)=='\\'))
            filename_withoutpath = p+1;
        p++;
    }

    string tempfilename = filename_inzip;
    wstring filenameios = _ipaFolderOnIos+CCodeOperation::UTF_8ToUnicode(tempfilename);
    RETURN_VALUE_IF(_fileStream->Ios_Exists(filenameios), UNZ_OK);
    if ((*filename_withoutpath)!='\0')
    {
        const char* write_filename = filename_inzip;

        char szFullWriteFileName[260] = {0};
        string strFolderTempGB2312 = CStrOperation::trimRight(CCodeOperation::UnicodeToGB2312(_strFolderTemp), "\\") + "\\";
        strcat(szFullWriteFileName, strFolderTempGB2312.c_str());
        strcat(szFullWriteFileName, write_filename);
        string temp = szFullWriteFileName;
        NDZlib::makedir2(szFullWriteFileName);

        err = unzOpenCurrentFile(uf);
        //EnterCriticalSection(&_cs);
        if (err==UNZ_OK)
        {
            fout = fopen(szFullWriteFileName, "wb");
            // some zipfile don't contain directory alone before file
            if ((fout==NULL) && (filename_withoutpath!=(char*)filename_inzip))
            {
                char c=*(filename_withoutpath-1);
                *(filename_withoutpath-1)='\0';
                NDZlib::makedir(szFullWriteFileName);
                *(filename_withoutpath-1)=c;

                fout = fopen(szFullWriteFileName, "w+");
            }
        }

        if (fout != NULL)
        {
            do
            {
                err = unzReadCurrentFile(uf,buf,size_buf);
                if (err < 0)
                    break;

                if (err > 0)
                    if (fwrite(buf,err,1,fout)!=1)
                    {
                        err=UNZ_ERRNO;
                        break;
                    }
            }
            while (err > 0);

            fclose(fout);
            if (err == 0)
                NDZlib::change_file_date(szFullWriteFileName,file_info.dosDate,file_info.tmu_date);
        }
        err = unzCloseCurrentFile(uf);
        //LeaveCriticalSection(&_cs);

        string temp_filename_withoutpath = filename_withoutpath;
        vector<string> tempPath = CStrOperation::parseStrings(tempfilename, '/');
        string folder;
        for (size_t i = 0; i < tempPath.size(); i++)
        {
            if (tempPath.at(i) != temp_filename_withoutpath)
                folder+=tempPath.at(i)+"/";
            else
                continue;

            if (_fileStream->Ios_Exists(_ipaFolderOnIos+CCodeOperation::UTF_8ToUnicode(folder)))
                continue;

            if ( isTransferPayload && CStrOperation::startsWith(tempfilename, PAYLOADNAME) )
                _fileStream->Ios_CreateDirectory(_ipaFolderOnIos+CCodeOperation::UTF_8ToUnicode(folder));
            if ( !isTransferPayload && CStrOperation::startsWith(tempfilename, CONTAINERNAME) && tempfilename != "Container/" )
                _fileStream->Ios_CreateDirectory(_ipaFolderOnIos+CStrOperation::ReplaceW(CCodeOperation::UTF_8ToUnicode(folder), L"Container/", L""));
        }

        if ( isTransferPayload && CStrOperation::startsWith(tempfilename, PAYLOADNAME) )
  //          _fileStream->Ios_CopyToPhone(CCodeOperation::StringToWstring(szFullWriteFileName), filenameios, *_pProgressSubject);
             if (_fileStream->Ios_CopyToPhone(CCodeOperation::StringToWstring(szFullWriteFileName), filenameios, *_pProgressSubject) != Transfer_Success)
                 err = UNZ_ERRNO;
        if ( !isTransferPayload && CStrOperation::startsWith(tempfilename, CONTAINERNAME) )
            if(_fileStream->Ios_CopyToPhone(CCodeOperation::StringToWstring(szFullWriteFileName), 
                                            CStrOperation::ReplaceW(filenameios, L"Container/", L""), 
                                            *_pProgressSubject) != Transfer_Success)
				err = UNZ_ERRNO;

        //TODO: Library/SyncedPreferences 这个文件夹下面的文件可能是超链接.需要处理
        //todo:需要深入了解的,存在相同的文件,当前处理是跳过,不覆盖以免导致一些注册信息被覆写导致程序无法运行
        //一些程序运行时候,去覆盖相应文件会失败的,比如网易新闻这种有注册系统通知的
        if ( !isTransferPayload && IosPublicFunc::GetVerNum(_pDevInfoPtr->_deviceProductVersion) >= 500 && CStrOperation::startsWith(tempfilename, "Container/Library/SyncedPreferences/") )
            _fileStream->Ios_CopyToPhone(CCodeOperation::StringToWstring(szFullWriteFileName), 
                                            CStrOperation::ReplaceW(CStrOperation::ReplaceW(filenameios, L"SyncedPreferences", L"Preferences"), L"Container/", L""), 
                                            *_pProgressSubject);
    }
    else
    {
        if ( isTransferPayload && CStrOperation::startsWith(tempfilename, PAYLOADNAME) )
            _fileStream->Ios_CreateDirectory(filenameios);

        if ( !isTransferPayload && CStrOperation::startsWith(tempfilename, CONTAINERNAME) && tempfilename != "Container/" )
            _fileStream->Ios_CreateDirectory(CStrOperation::ReplaceW(filenameios, L"Container/", L""));
    }

    free(buf);
    return err;
}

#endif