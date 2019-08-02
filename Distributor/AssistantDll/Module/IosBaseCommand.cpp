#include "pch_module.h"
#include "IosBaseCommand.h"

#include "Device/IosDaemon.h"
#include "Device/IosFileIO.h"
#include "Common/Path.h"

#define RETURN_FILE_SUFFIX  L".done"

IosBaseCommand::IosBaseCommand(const DeviceData* pDevice)
:IBaseCommand(pDevice)
{
}

IosBaseCommand::~IosBaseCommand(void)
{
}

void ND91Assistant::IosBaseCommand::Execute()
{

}

std::wstring ND91Assistant::IosBaseCommand::GetIosCommandFilePath()
{
    //����IOS�豸�ϵ������ļ�
    IosDaemon daemon(NULL, L"");

    wstring strFileName_OnPhone = daemon.CreatDaemonCypher(_strIosCmdFileName, 
        GetDeviceInfo()->_ios_devicebTurnCypher, 
        GetDeviceInfo()->_ios_deviceszDaemonCypherBytes);

    return Path::IosDeviceCommandPath() + strFileName_OnPhone;
}

std::wstring ND91Assistant::IosBaseCommand::GetIosResultFilePath()
{
    return Path::IosDeviceCommandPath() + _strIosCmdFileName + L".done";
}

std::wstring ND91Assistant::IosBaseCommand::GetIosCommandFileContent()
{
    return L"";
}

std::wstring ND91Assistant::IosBaseCommand::GetIosDoneInfoFromFileName()
{
    IosFileIO* pFileIO = dynamic_cast<IosFileIO*>(GetIO());
    if ( ! pFileIO )
    {
        THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);
    }

    IIosFileSystem* pFileStream = pFileIO->GetIosFileStream();
    if (!pFileStream)
    {
        THROW_ASSISTANT_EXCEPTION(INVALID_IO_OBJECT);
    }

    // �ȴ����ؽ���ļ�
    bool blnCompleted = false; 
    unsigned long timeout = 30000; //30��ȴ�ʱ��
    unsigned long lFirstTick = GetTickCount();
    unsigned long lNextTick = 0;	
    wstring strResultFileFirstName = _strIosCmdFileName;
    wstring strInfo = L"";

    // ѭ���ȴ�����ķ��ؽ��
    do 
    {  	
        Sleep(100);			      
        if ( LookForResultFile(pFileStream, strResultFileFirstName, strInfo) )
            break;

        lNextTick = GetTickCount();

    } while ((lNextTick-lFirstTick) < timeout);	

    return strInfo;
}

bool ND91Assistant::IosBaseCommand::LookForResultFile( IIosFileSystem* pFileStream, std::wstring filePrefix, wstring& strInfo )
{
    strInfo = L"";
    // ���������Ŀ¼
    DirData resultDir(_pDevice);
    resultDir._name = Path::IosDeviceCommandPath();
    resultDir._name.erase(resultDir._name.size()-1); // ȥ����β��'/'

    // ȡĿ¼�µ��ļ�
    pFileStream->Ios_GetFileInfos(&resultDir, eFileGetType_Files);

    deque<NodeData*> childs = resultDir.GetChildrens();
    for ( size_t i=0; i < childs.size(); ++i )
    {
        NodeData* pFile = childs[i];
        wstring name = pFile->_name;

        // ����ļ����ǡ������ļ��� + XXXX + .done����ʽ���Ͷ���
        if (CStrOperation::startsWith(name, filePrefix) &&
            CStrOperation::endsWithW(name, RETURN_FILE_SUFFIX))
        {
            pFileStream->Ios_DeleteFile(pFile->GetFullPath());

            int left = filePrefix.length()+1;
            int right = wstring(RETURN_FILE_SUFFIX).length();
            strInfo = name.substr(left, name.length() - left - right);

            return true;
        }
    }

    return false;
}
