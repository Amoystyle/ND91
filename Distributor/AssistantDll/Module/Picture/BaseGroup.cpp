#include "pch_module.h"

#ifdef MODULE_PICTURE

#include "Module/Picture/BaseGroup.h"
#include "Module/Picture/AddPictureCommand.h"

#include "Monitor/DefaultProgressObserver.h"

BaseGroup::BaseGroup(const DeviceData* pDevice)
: _pDevice(pDevice)
{
    _type = PICTURE_TYPE_UNSET;
}

const PictureData* BaseGroup::AndroidAddPicture( wstring filePath ) const
{
    RETURN_NULL_IF( !MyFileOperation::IsSupportImageFile(filePath) );

    // 生成设备端文件路径
    wstring remotepath = _path;
    if ( *remotepath.rbegin() != '/' )
        remotepath.push_back('/');
     remotepath += CFileOperation::GetFileName(filePath);

    // 上传文件
    DefaultProgressObserver observer(RENAME_ALL);  // 文件已存在则重命名
    UploadFileCommand cmd(_pDevice);
    cmd.Attach(&observer);
    cmd.Upload(filePath, remotepath);
    remotepath = cmd.GetDestFilePath(); // 取得重命名的文件名(若未重命名则不变)

    // 添加图片到图库
    AddPictureCommand  addPicCmd(_pDevice);
    addPicCmd.SetParam(remotepath, filePath, _id, _name);
    addPicCmd.Execute();
    return addPicCmd.GetRetPicture();
}

#endif