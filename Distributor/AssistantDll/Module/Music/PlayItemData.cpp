#include "pch_module.h"

#ifdef MODULE_MUSIC

#include "Module/Music/PlayItemData.h"

PlayItemData::PlayItemData(void)
{
	_id = -1;      // id
	_trackId = -1; // ÇúÄ¿id
	_index = -1;   // Ë÷Òý
	_owner = NULL;
}

PlayItemData::~PlayItemData(void)
{
	_owner = NULL;
}

bool ND91Assistant::PlayItemData::ParseAndroidData( BinaryBufferReader& reader )
{
	reader.ReadInt32(_id);
	reader.ReadInt32(_trackId);
	reader.ReadInt32(_index);
	return true;
}


#endif