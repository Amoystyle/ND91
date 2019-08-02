#include "pch_module.h"

#ifdef MODULE_CONTACT

#include "Module/Contact/GetContactPhotoCommand.h"
#include "Module/Contact/ContactData.h"
#include "Common/Path.h"
#include "Module/AndroidDefine.h"

GetContactPhotoCommand::GetContactPhotoCommand(const DeviceData* pDevice, int contactId)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = PIM;
	_nAndroidCommandID = GET_CONTACT_PHOTO;
	_contactId = contactId;
	_contantPhotoLocalPath = L"";
}

GetContactPhotoCommand::~GetContactPhotoCommand(void)
{
}

void ND91Assistant::GetContactPhotoCommand::Execute()
{
	_contantPhotoLocalPath = L"";
	DefaultExecute();
}

std::string ND91Assistant::GetContactPhotoCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteInt(_nAndroidCommandID);
	bufwriter.WriteInt(_contactId);
	return bufwriter.GetBuffer();
}

void ND91Assistant::GetContactPhotoCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	RETURN_IF ( ! parseAndroidPackageHead(&reader) );

    int kind;
	reader.ReadInt32(kind);

    if (METHOD_PHOTO == kind)
	{
		int id;
		int contactId;
        int  length = 0;

        reader.ReadInt32(id);
		reader.ReadInt32(contactId);
		reader.ReadInt32(length);

        if( length > 0 )
		{
			string  binaryData;
			reader.ReadString(binaryData, length);
			RETURN_IF( binaryData.empty() );

            wstring localpath = _pDevice->GetPath()->GetLocalPimPhotoPath();
			CFileOperation::MakeLocalDir(localpath);

			localpath += CCodeOperation::GB2312ToUnicode(CFileOperation::GetGuid())+L".jpg";
			CFileOperation::WriteBinFile(localpath, binaryData.c_str(), binaryData.length());

			_contantPhotoLocalPath = localpath;
		}
	}
}

#endif