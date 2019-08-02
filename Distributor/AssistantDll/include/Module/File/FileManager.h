#pragma once

#include "Module/IManager.h"
#include "NodeData.h"
#include "FileData.h"
#include "LinkData.h"
#include "DirData.h"
#include "RootData.h"
#include "FileHelper.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL FileManager : public IManager
    {
    public:
        FileManager(const DeviceData* pDevice);
        virtual ~FileManager();

        CommandExecuteResult Load();
        //上传文件
        TransferResult UpLoadFile(const wstring& pathOnPc, const wstring& remotePath, ProgressObserver* pObserver = NULL);


    protected:
        RootData* _pRoot;
    };
}