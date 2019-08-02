#include "stdafx.h"
#include "RealParser.h"

const std::string Real_DRMF = ".RMF";
const std::string Real_PROP = "PROP";
const std::string Real_MDPR = "MDPR";
const std::string Real_CONT = "CONT";

const int FLAG = 1179472430;


RealParser::RealParser( const wstring path )
: MediaBase(path)
{
}

RealParser::~RealParser(void)
{
	for (size_t k=0;k<_recordef.size();k++)
	{
		RELEASE_VECTOR(_recordef[k]->_fieldDef);
	}
	RELEASE_VECTOR (_recordef);
}


bool ND91Assistant::RealParser::ParserFormat()
{
	fstream fs(CCodeOperation::WstringToString(_path).c_str(), ios::in|ios::out|ios::binary);
	fs.seekg(0, ios::end);
	_media->_fileSize = fs.tellg();
	fs.seekg(0, ios::beg);

	if (_media->_fileSize < 4)
	{
		fs.close();
		return false;
	}

	unsigned char buf[5] = {0};
	fs.read((char*)buf, 4);

	int realValue = ((((((int)buf[3]<<8) + (int)buf[2])<<8) + (int)buf[1])<<8) + (int)buf[0];

	if (FLAG != realValue)
	{
		fs.close();
		return false;
	}

	fs.close();
	return true;	
}


long ND91Assistant::RealParser::GetMediaDuration()
{	
	/*__int64*/int64_t duration = 0;
	char buf[5] = {0};

	BinaryBufferReader binarybuf(_path);

	GetInitData();

	while (1)
	{
		int offset = binarybuf.GetOffset();
		binarybuf.ReadBuffer(buf, 4);
		std::string strID = buf;
		unsigned int size = ReadUInt32BE(&binarybuf);
		unsigned int datasize = size - 8;

		for (int i=0;i<(int)_recordef.size();++i)
		{
			if ( strID == _recordef[i]->_name )
			{
				for (int j=0;j<(int)_recordef[i]->_fieldDef.size();++j)
				{
					duration = Decode(_recordef[i]->_fieldDef[j], &binarybuf);

					if (duration > 0 && _recordef[i]->_name == Real_MDPR)
					{
						_media->_duration = (int)duration / 1000;
						return _media->_duration;
					}
				}	
			}
		}

		binarybuf.SetOffset(offset+size);

		RETURN_FALSE_IF ( strID!= Real_DRMF && strID!=Real_PROP && strID!=Real_MDPR && strID!=Real_CONT );

		if ( 0 == size )
			break;
		if ( strID.empty() )
			break;
	}

	return _media->_duration;
	
}

unsigned int ND91Assistant::RealParser::ReadUInt32BE( BinaryBufferReader* binaryBuf )
{
	unsigned char buf[4] = {0};
	RETURN_VALUE_IF ( !binaryBuf->ReadBuffer( (char*)buf, 4 ), 0);

	unsigned int nvalue = ((((((unsigned int)buf[0] << 8) + (unsigned int)buf[1]) << 8) + (unsigned int)buf[2]) << 8) + (unsigned int)buf[3];

	return nvalue;
}

unsigned short ND91Assistant::RealParser::ReadUInt16BE( BinaryBufferReader* binaryBuf )
{
	unsigned char buf[2] = {0};
	RETURN_VALUE_IF ( !binaryBuf->ReadBuffer( (char*)buf, 2 ), 0);

	unsigned short nvalue = ((((((unsigned short)buf[0] << 8) + (unsigned short)buf[1]) << 8) + (unsigned short)buf[2]) << 8) + (unsigned short)buf[3];

	return nvalue;
}

void ND91Assistant::RealParser::GetInitData()
{
	RealData*	realDrmf = new RealData;

	realDrmf->_name = Real_DRMF;

	FIELDDEF* field1 = new FIELDDEF;
	field1->name	= "object_version";
	field1->type	= "UInt";
	field1->size	= 2;
	field1->endian	= "BE";
	realDrmf->_fieldDef.push_back(field1);
	FIELDDEF* field2 = new FIELDDEF;
	field2->name	= "file_version";
	field2->type	= "UInt";
	field2->size	= 2;
	field2->endian	= "BE";
	realDrmf->_fieldDef.push_back(field2);
	FIELDDEF* field3 = new FIELDDEF;
	field3->name	= "num_headers";
	field3->type	= "UInt";
	field3->size	= 4;
	field3->endian	= "BE";
	realDrmf->_fieldDef.push_back(field3);
	_recordef.push_back(realDrmf);

	RealData*	realProp = new RealData;

	realProp->_name = Real_PROP;

	FIELDDEF* field4 = new FIELDDEF;
	field4->name	= "object_version";
	field4->type	= "UInt";
	field4->size	= 2;
	field4->endian= "BE";
	realProp->_fieldDef.push_back(field4);

	FIELDDEF* field5 = new FIELDDEF;
	field5->name	= "max_bit_rate";
	field5->type	= "UInt";
	field5->size	= 4;
	field5->endian= "BE";
	realProp->_fieldDef.push_back(field5);

	FIELDDEF* field6 = new FIELDDEF;
	field6->name	= "avg_bit_rate";
	field6->type	= "UInt";
	field6->size	= 4;
	field6->endian= "BE";
	realProp->_fieldDef.push_back(field6);

	FIELDDEF* field7 = new FIELDDEF;
	field7->name	= "max_packet_size";
	field7->type	= "UInt";
	field7->size	= 4;
	field7->endian= "BE";
	realProp->_fieldDef.push_back(field7);

	FIELDDEF* field8 = new FIELDDEF;
	field8->name	= "avg_packet_size";
	field8->type	= "UInt";
	field8->size	= 4;
	field8->endian= "BE";
	realProp->_fieldDef.push_back(field8);

	FIELDDEF* field9 = new FIELDDEF;
	field9->name	= "num_packets";
	field9->type	= "UInt";
	field9->size	= 4;
	field9->endian= "BE";
	realProp->_fieldDef.push_back(field9);

	FIELDDEF* field10 = new FIELDDEF;
	field10->name	= "duration";
	field10->type	= "UInt";
	field10->size	= 4;
	field10->endian= "BE";
	realProp->_fieldDef.push_back(field10);

	FIELDDEF* field11 = new FIELDDEF;
	field11->name	= "preroll";
	field11->type	= "UInt";
	field11->size	= 4;
	field11->endian= "BE";
	realProp->_fieldDef.push_back(field11);

	FIELDDEF* field12 = new FIELDDEF;
	field12->name	= "index_offset";
	field12->type	= "UInt";
	field12->size	= 4;
	field12->endian= "BE";
	realProp->_fieldDef.push_back(field12);

	FIELDDEF* field13 = new FIELDDEF;
	field13->name	= "data_offset";
	field13->type	= "UInt";
	field13->size	= 4;
	field13->endian= "BE";
	realProp->_fieldDef.push_back(field13);

	FIELDDEF* field14 = new FIELDDEF;
	field14->name	= "num_streams";
	field14->type	= "UInt";
	field14->size	= 4;
	field14->endian= "BE";
	realProp->_fieldDef.push_back(field14);

	FIELDDEF* field15 = new FIELDDEF;
	field15->name	= "flags";
	field15->type	= "UInt";
	field15->size	= 2;
	field15->endian= "BE";
	realProp->_fieldDef.push_back(field15);
	_recordef.push_back(realProp);

	RealData*	realCont = new RealData;

	realCont->_name = Real_CONT;

	FIELDDEF* field16 = new FIELDDEF;
	field16->name	= "object_version";
	field16->type	= "UInt";
	field16->size	= 2;
	field16->endian= "BE";
	realCont->_fieldDef.push_back(field16);

	FIELDDEF* field17 = new FIELDDEF;
	field17->name	= "title_len";
	field17->type	= "UInt";
	field17->size	= 2;
	field17->endian= "BE";
	realCont->_fieldDef.push_back(field17);

	FIELDDEF* field18 = new FIELDDEF;
	field18->name	= "title";
	field18->type	= "AnsiString";
	field18->size	= 0;
	field18->szieField = "title_len";
	field18->endian= "BE";
	realCont->_fieldDef.push_back(field18);

	FIELDDEF* field19 = new FIELDDEF;
	field19->name	= "author_len";
	field19->type	= "UInt";
	field19->size	= 2;
	field19->endian= "BE";
	realCont->_fieldDef.push_back(field19);

	FIELDDEF* field20 = new FIELDDEF;
	field20->name	= "author";
	field20->type	= "AnsiString";
	field20->size	= 0;
	field20->szieField = "author_len";
	field20->endian= "BE";
	realCont->_fieldDef.push_back(field20);

	FIELDDEF* field21 = new FIELDDEF;
	field21->name	= "copyright_len";
	field21->type	= "UInt";
	field21->size	= 2;
	field21->endian= "BE";
	realCont->_fieldDef.push_back(field21);

	FIELDDEF* field22 = new FIELDDEF;
	field22->name	= "copyright";
	field22->type	= "AnsiString";
	field22->size	= 0;
	field22->szieField = "copyright_len";
	field22->endian= "BE";
	realCont->_fieldDef.push_back(field22);

	FIELDDEF* field23 = new FIELDDEF;
	field23->name	= "comment_len";
	field23->type	= "UInt";
	field23->size	= 2;
	field23->endian= "BE";
	realCont->_fieldDef.push_back(field23);

	FIELDDEF* field24 = new FIELDDEF;
	field24->name	= "comment";
	field24->type	= "AnsiString";
	field24->size	= 0;
	field24->szieField = "copyright_len";
	field24->endian= "BE";
	realCont->_fieldDef.push_back(field24);
	_recordef.push_back(realCont);

	RealData*	realMdpr = new RealData;

	realMdpr->_name = Real_MDPR;

	FIELDDEF* field25 = new FIELDDEF;
	field25->name	= "object_version";
	field25->type	= "UInt";
	field25->size	= 2;
	field25->endian= "BE";
	realMdpr->_fieldDef.push_back(field25);

	FIELDDEF* field26 = new FIELDDEF;
	field26->name	= "stream_number";
	field26->type	= "UInt";
	field26->size	= 4;
	field26->endian= "BE";
	realMdpr->_fieldDef.push_back(field26);

	FIELDDEF* field27 = new FIELDDEF;
	field27->name	= "max_bit_rate";
	field27->type	= "UInt";
	field27->size	= 4;
	field27->endian= "BE";
	realMdpr->_fieldDef.push_back(field27);

	FIELDDEF* field28 = new FIELDDEF;
	field28->name	= "avg_bit_rate";
	field28->type	= "UInt";
	field28->size	= 4;
	field28->endian= "BE";
	realMdpr->_fieldDef.push_back(field28);

	FIELDDEF* field29 = new FIELDDEF;
	field29->name	= "max_packet_size";
	field29->type	= "UInt";
	field29->size	= 4;
	field29->endian= "BE";
	realMdpr->_fieldDef.push_back(field29);

	FIELDDEF* field30 = new FIELDDEF;
	field30->name	= "avg_packet_size";
	field30->type	= "UInt";
	field30->size	= 4;
	field30->endian= "BE";
	realMdpr->_fieldDef.push_back(field30);

	FIELDDEF* field31 = new FIELDDEF;
	field31->name	= "start_time";
	field31->type	= "UInt";
	field31->size	= 4;
	field31->endian= "BE";
	realMdpr->_fieldDef.push_back(field31);

	FIELDDEF* field32 = new FIELDDEF;
	field32->name	= "preroll";
	field32->type	= "UInt";
	field32->size	= 4;
	field32->endian= "BE";
	realMdpr->_fieldDef.push_back(field32);

	FIELDDEF* field33 = new FIELDDEF;
	field33->name	= "duration";
	field33->type	= "UInt";
	field33->size	= 4;
	field33->endian= "BE";
	realMdpr->_fieldDef.push_back(field33);

	FIELDDEF* field34 = new FIELDDEF;
	field34->name	= "stream_name_size";
	field34->type	= "UInt";
	field34->size	= 1;
	field34->endian= "BE";
	realMdpr->_fieldDef.push_back(field34);

	FIELDDEF* field35 = new FIELDDEF;
	field35->name	= "stream_name";
	field35->type	= "UInt";
	field35->size	= 0;
	field35->szieField = "stream_name_size";
	field35->endian= "BE";
	realMdpr->_fieldDef.push_back(field35);

	FIELDDEF* field36 = new FIELDDEF;
	field36->name	= "mime_type_size";
	field36->type	= "UInt";
	field36->size	= 1;
	field36->endian= "BE";
	realMdpr->_fieldDef.push_back(field36);

	FIELDDEF* field37 = new FIELDDEF;
	field37->name	= "mime_type";
	field37->type	= "UInt";
	field37->size	= 1;
	field37->szieField = "mime_type_size";
	field37->endian= "BE";
	realMdpr->_fieldDef.push_back(field37);

	FIELDDEF* field38 = new FIELDDEF;
	field38->name	= "type_specific_len";
	field38->type	= "UInt";
	field38->size	= 4;
	field38->endian= "BE";
	realMdpr->_fieldDef.push_back(field38);

	FIELDDEF* field39 = new FIELDDEF;
	field39->name	= "type_specific_data";
	field39->type	= "UInt";
	field39->size	= 4;
	field39->szieField = "type_specific_len";
	field39->endian= "BE";
	realMdpr->_fieldDef.push_back(field39);
	_recordef.push_back(realMdpr);
	
}

unsigned int ND91Assistant::RealParser::Decode( FIELDDEF* field, BinaryBufferReader* binaryBuf )
{
	char* data = NULL;
	/*__int32*/int32_t nCache = 0;
	std::string temp = field->type;

	RETURN_VALUE_IF (field == NULL, 0);

	if ( (int)temp.find("Record") >= 0)
	{
		int size = GetSize(temp);
		data = new char[size];
		binaryBuf->ReadBuffer(data, size);
		return 0;
	}
	else if ( (int)temp.find("[]") >=0 )
	{
		int pos = temp.find(']');
		temp = temp.substr(1,pos);
		int size = GetSize(temp);
		if (temp == "Byte")
		{
			data = new char[size];
			binaryBuf->ReadBuffer(data, size);
			return 0;
		}
		else if (temp == "UInt2")
		{
			data = new char[size*2];
			binaryBuf->ReadBuffer(data, size*2);
			return 0;
		}
		else if (temp == "UInt4")
		{
			data = new char[size*4];
			binaryBuf->ReadBuffer(data, size*4);
			return 0;
		}
		else
			return 0;
	}
	else
	{
		char buff[3] = {0};
		//sprintf_s( buff, 3, "%d", field->size * 8 );
		sprintf( buff, "%d", field->size * 8 );
		temp = buff;
		temp = field->type + temp + field->endian;

		if ( temp== "UInt8LE" || temp== "UInt8BE" )
		{
			binaryBuf->ReadChar(buff[0]);
			return 0;
		}
		else if ( temp== "UInt32LE")
		{
			binaryBuf->ReadInt32(nCache);
			return 0;
		}
		else if ( temp== "UInt16LE")
		{
			ReadUInt16BE(binaryBuf);
			return 0;
		}
		else if ( temp== "UInt32BE")
		{
			int len = ReadUInt32BE(binaryBuf);
			RETURN_VALUE_IF ( field->name == "duration", len);
			return 0;		
		}
	}
	return 0;

}

int ND91Assistant::RealParser::GetSize( std::string type )
{
	for (size_t i=0;i<_recordef.size();i++ )
	{
		for (size_t j=0;j<_recordef[i]->_fieldDef.size();i++)
		{
			if (type == _recordef[i]->_fieldDef[j]->type)
			{
				return _recordef[i]->_fieldDef[j]->size;
			}
		}

	}
	return 0;
}