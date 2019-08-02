#include "stdafx.h"
#include "WmvParser.h"

#ifndef WIN32
	#include <uuid/uuid.h>
#endif

#define _Asf_Header_Object					"75B22630-668E-11CF-A6D9-00AA0062CE6C"
#define _Asf_File_Properties_Object			"8CABDCA1-A947-11CF-8EE4-00C00C205365"
#define _Asf_Codec_List_Object				"86D15240-311D-11D0-A3A4-00A0C90348F6"
#define _Asf_Stream_Properties_Object		"B7DC0791-A9B7-11CF-8EE6-00C00C205365"
#define _Asf_Header_Extension_Object		"5FBF03B5-A92E-11CF-8EE3-00C00C205365"
#define _Asf_Extended_Description_Object	"D2D0A440-E307-11D2-97F0-00A0C95EA850"

WmvParser::WmvParser( const wstring path )
: MediaBase(path)
{
	_pos		= 0;		
	_header_num	= 0;
	_bufsize	= 0;
	
}

WmvParser::~WmvParser(void)
{

}

bool EqualsId(GUID id, const char* idString)
{
#ifdef WIN32
	GUID id2;
	assert(S_OK == UuidFromStringA ((RPC_CSTR)idString, &id2));
#else
	uuid_t id2;
	assert(0 == uuid_parse(idString, id2));
#endif
	return 0 == memcmp(&id, &id2, 16);
}

bool ND91Assistant::WmvParser::ParserFormat()
{
	RETURN_FALSE_IF ( !Verification() );

	BinaryBufferReader binaryBuf( _path );
	binaryBuf.SetOffset(_pos);

	//read header object
	for (int i = 0; i < _header_num; i++)
	{
		GUID id;
		binaryBuf.ReadBuffer((char*)&id, 16);

		int64_t objectSize; 		
		binaryBuf.ReadInt64(objectSize);
		_pos = binaryBuf.GetOffset();

		long nextPosition = _pos + (int)objectSize - 24;

		if ( EqualsId(id, _Asf_Codec_List_Object))
		{
		    return Read_ASF_Codec_List_Object(&binaryBuf);
		}

		binaryBuf.SetOffset(nextPosition);
	}

	return false;
	
}


long ND91Assistant::WmvParser::GetMediaDuration()
{
	RETURN_VALUE_IF (!Verification(), 0);

	BinaryBufferReader binaryBuf( _path );
	binaryBuf.SetOffset(_pos);

	for (int i = 0; i < _header_num; i++)
	{
		GUID id;
		binaryBuf.ReadBuffer((char*)&id, 16);

		int64_t objectSize; 		
		binaryBuf.ReadInt64(objectSize);
		_pos = binaryBuf.GetOffset();

		long nextPosition = _pos + (int)objectSize - 24;

		if ( EqualsId(id, _Asf_File_Properties_Object))
			return Read_ASF_File_Properties_Object(&binaryBuf);

		binaryBuf.SetOffset(nextPosition);

		if ( nextPosition >= _bufsize )
			break;
	}

	return 0;
}

long ND91Assistant::WmvParser::Read_ASF_File_Properties_Object(BinaryBufferReader* binaryBuf)
{
	binaryBuf->SetOffset(_pos + 40);

	int64_t play_duration, send_duration, preroll;

	RETURN_VALUE_IF( !binaryBuf->ReadInt64(play_duration), 0);
	RETURN_VALUE_IF( !binaryBuf->ReadInt64(send_duration), 0);
	RETURN_VALUE_IF( !binaryBuf->ReadInt64(preroll), 0);

	long duration = (int)( send_duration / 10000000 - 1.3);
	
	return duration;
}

bool ND91Assistant::WmvParser::Read_ASF_Codec_List_Object(BinaryBufferReader* binaryBuf)
{
	int pos = binaryBuf->GetOffset();
	binaryBuf->SetOffset(pos + 16);

	/*__int32*/int32_t length;
	binaryBuf->ReadInt32(length);

	for (int i = 0; i < length; i++)
	{
		short type, name_length, description_length, information_length;
		std::string strname, strdescriptions;	

		binaryBuf->ReadShort(type);
		binaryBuf->ReadShort(name_length);
		char* names = new char[name_length*2];
		binaryBuf->ReadBuffer( names, name_length * 2 );
		strname = names;
		binaryBuf->ReadShort(description_length);
		char* descriptions = new char[description_length*2];
		binaryBuf->ReadBuffer( descriptions, description_length * 2 );
		strdescriptions = descriptions;
		binaryBuf->ReadShort(information_length);
		char* informations = new char[description_length];
		binaryBuf->ReadBuffer(informations, information_length);

		if (type == 2)
			return true;
		else if (type == 1)
			return true;
		else
			return false;
	}
	return true;
}

void ND91Assistant::WmvParser::ReadASF_Extended_Content_Description_Object()
{

}

bool ND91Assistant::WmvParser::Verification()
{
	_media->_fileSize = CFileOperation::GetFileSizeW(_path);

	BinaryBufferReader binaryBuf( _path );

	GUID id;
	binaryBuf.ReadBuffer((char*)&id, 16);

	binaryBuf.ReadInt64(_bufsize);

	if ( EqualsId(id, _Asf_Header_Object))
	{
		binaryBuf.ReadInt32(_header_num);
		_pos = binaryBuf.GetOffset() + 2;
		binaryBuf.SetOffset(_pos);

		return true;
	}
	else
		return false;
	
}
