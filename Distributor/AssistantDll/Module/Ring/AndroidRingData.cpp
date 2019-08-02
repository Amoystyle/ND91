#include "pch_module.h"

#ifdef MODULE_RING

#include "Module/Ring/AndroidRingData.h"
#include "Module/Ring/RingOperationCommand.h"
#include "Module/Music/MusicPlayCommand.h"

bool ND91Assistant::AndroidRingData::ParseAndroidData( BinaryBufferReader& reader)
{
	 reader.ReadInt32(_Id);
	 reader.ReadStringFromUTF8(_remotePath);
	 reader.ReadStringFromUTF8(_title);
	 string str;
	 reader.ReadInt32(_duration);
	 reader.ReadStringFromUTF8(_artist);
	 reader.ReadStringFromUTF8(_album);
	 bool type;
	 reader.ReadBool(type);
	 if (type)
		 _ringType |= Ring;
	 reader.ReadBool(type);
	 if (type)
		 _ringType |= Music;
	 reader.ReadBool(type);
	 if (type)
		 _ringType |= Alarm;
	 reader.ReadBool(type);
	 if (type)
		 _ringType |= Notif;
	 int64_t size;
	 reader.ReadInt64(size);
	 _size = (long)size;

	 _name = CFileOperation::GetFileName(_remotePath);
	 if (_title.empty())
		 _title = CFileOperation::GetFileNameNoExt(_name);
	 return true;

}

bool ND91Assistant::AndroidRingData::SettingAndroidRing( MediaFileKind kind )
{
	RETURN_FALSE_IF( _Parent->_pDeviceData->GetDevInfo()->_deviceType != Android || Alarm ==kind);
	
	RingOperationCommand cmd(_Parent->_pDeviceData);
	return cmd.SettingRing(this, kind);
}

void ND91Assistant::AndroidRingData::PlayRing()
{
#ifdef MODULE_MUSIC
	MusicPlayCommand cmd(_Parent->_pDeviceData);
	cmd.Play(_remotePath);
#endif
}

bool ND91Assistant::AndroidRingData::RenameRing( wstring title )
{
	RingOperationCommand cmd(_Parent->_pDeviceData);
	cmd.RenameRing(this, title);
	if(cmd.DoneSuccess())
		this->_title = title;
	return cmd.DoneSuccess();
}

bool ND91Assistant::AndroidRingData::ChangeRingType( MediaFileKind kind )
{
	RingOperationCommand cmd(_Parent->_pDeviceData);
	if(cmd.ChangeRingType(this, kind))
		this->_ringType = kind;
	return cmd.DoneSuccess();
}

NodeData* ND91Assistant::AndroidRingData::Clone( DirData* pParent ) const
{
	AndroidRingData* pNode = new AndroidRingData();
	pNode->Assign(this);
	pNode->_Parent = pParent;

	return pNode;
}


#endif