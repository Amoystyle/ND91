#include "stdafx.h"
#include "IosFileIO.h"
#include "Common/MyFileOperation.h"
#include "Common/Path.h"

IosFileIO::IosFileIO(const DeviceInfo* pDevInfo) : BaseIO()
{
	_pFileStream = new AFCFileSysemBase(pDevInfo);
	_nWaitTime = 30000;
}

IosFileIO::IosFileIO(int nHandle) : BaseIO()
{
	_pFileStream = new IosWiFiFileSystem(nHandle);
}

IosFileIO::~IosFileIO()
{
	if (_pFileStream)
	{
		delete _pFileStream;
		_pFileStream = NULL;
	}
}

void IosFileIO::DeleteAllCmdDoneFile()
{
// 	vector<IosFileData> vtFile;
// 	_pFileStream->Ios_GetFileInfos(Path::GetIosCommandDevicePath(), eFileGetType_Files, vtFile);
// 	vector<IosFileData>::iterator it;
// 	for (it=vtFile.begin(); it!=vtFile.end(); it++)
// 	{
// 		wstring name = ((IosFileData)(*it))._name; 
// 		name = CStrOperation::toLowerW(name);
// 		if (CStrOperation::endsWithW(name, L".done"))
// 			_pFileStream->Ios_DeleteFile(IOS_COMMOND_NORMAL_FOLDER + ((IosFileData)(*it))._name);
// 	}
// 	vtFile.clear();
}

void IosFileIO::CreateDirectory( wstring path ) 
{
    if ( *path.rbegin() == '/' )
        path.erase(path.size() - 1);

    RETURN_IF ( _pFileStream->Ios_Exists(path) );

    wstring parentFolder = CFileOperation::GetFileDir(path);
    _pFileStream->Ios_CreateDirectory(parentFolder);

    _pFileStream->Ios_CreateDirectory(path);
}

wstring IosFileIO::SendIosFile(wstring strCmdFileOnPhone,
                               wstring strResultFileOnPhone,
                               wstring strCmdFileContent,
                               bool bReceiveResult)
{
	//��ɾ���豸�ϵĽ���ļ�
	if (!_pFileStream)
	{
		_nWaitTime = 30000;
		return L"";	
	}

	_cmdFileOnPhone = strCmdFileOnPhone;
	_resultFileOnPhone = strResultFileOnPhone;

	wstring strCmdFileName = CFileOperation::GetFileName(strCmdFileOnPhone);
	wstring filename = CreateIosCommandFile(strCmdFileName, strCmdFileContent);
	if (filename.length() <= 0)
	{
		_nWaitTime = 30000;
		return L"";
	}

    CreateDirectory(CFileOperation::GetFileDir(strCmdFileOnPhone));

	if (_resultFileOnPhone.length() > 0 && _pFileStream->Ios_Exists(_resultFileOnPhone))
		_pFileStream->Ios_DeleteFile(_resultFileOnPhone);

	// copy�ļ����豸	
	if (_pFileStream->Ios_CopyToPhone(filename, _cmdFileOnPhone) != Transfer_Success)
	{
		_nWaitTime = 30000;
		return L"";
	}

    //ɾ����ʱ�ļ�
    CFileOperation::DeleteFile(filename);

	if (!bReceiveResult)
	{
		_nWaitTime = 30000;
		return L"";
	}

	// ����ļ�����ʱĿ¼
	wstring folder = Path::GlobalTemp();

	// �ȴ����ؽ���ļ�
	bool blnCompleted = false;
	unsigned long timeout = _nWaitTime; //30��ȴ�ʱ��
	unsigned long lFirstTick = GetTickCount();
	unsigned long lNextTick = 0;	
	do 
	{
		if (_pFileStream->Ios_Exists(_resultFileOnPhone))
		{
			blnCompleted = true;

			// �ѽ���ļ�copy����ʱĿ¼			
			wstring fileTemp = folder + WSTRING_PATH_SEPARATER + CFileOperation::GetFileName(_resultFileOnPhone);
			if (_pFileStream->Ios_DownFromPhone(_resultFileOnPhone, fileTemp) == Transfer_Success)
			{
				_nWaitTime = 30000;
				return fileTemp;
			}
		}

		Sleep(500);
		lNextTick = GetTickCount();
		
	} while (!blnCompleted && (lNextTick-lFirstTick) < timeout);		

	_nWaitTime = 30000;
	return L"";
}

string IosFileIO::SendAndroidDatagram( string data, bool bReceiveResult /*= true*/ )
{
    THROW_ASSISTANT_EXCEPTION(PROGRAM_ERROR);
	return "";
}

wstring IosFileIO::CreateIosCommandFile(wstring strCmdFileName, wstring strCmdFileContent)
{
	if (strCmdFileName.length() <= 0)
		return L"";

	//���������ļ�
	wstring strCmdOnPC = Path::GlobalTemp() + strCmdFileName;
    void* hFile = MyFileOperation::MyCreateFile(strCmdOnPC);
    if (!hFile)
        return L"";

	if (strCmdFileContent.length() > 0)
	{
		string strBuffUTF8 = CCodeOperation::UnicodeToUTF_8(strCmdFileContent);
		unsigned long nWrite = 0;
        MyFileOperation::MyWriteFile(hFile, strBuffUTF8, nWrite);
	}
	
    MyFileOperation::MyCloseFile(hFile);

	return strCmdOnPC;
}
