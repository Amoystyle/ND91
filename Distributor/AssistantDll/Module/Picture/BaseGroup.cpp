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

    // �����豸���ļ�·��
    wstring remotepath = _path;
    if ( *remotepath.rbegin() != '/' )
        remotepath.push_back('/');
     remotepath += CFileOperation::GetFileName(filePath);

    // �ϴ��ļ�
    DefaultProgressObserver observer(RENAME_ALL);  // �ļ��Ѵ�����������
    UploadFileCommand cmd(_pDevice);
    cmd.Attach(&observer);
    cmd.Upload(filePath, remotepath);
    remotepath = cmd.GetDestFilePath(); // ȡ�����������ļ���(��δ�������򲻱�)

    // ���ͼƬ��ͼ��
    AddPictureCommand  addPicCmd(_pDevice);
    addPicCmd.SetParam(remotepath, filePath, _id, _name);
    addPicCmd.Execute();
    return addPicCmd.GetRetPicture();
}

#endif