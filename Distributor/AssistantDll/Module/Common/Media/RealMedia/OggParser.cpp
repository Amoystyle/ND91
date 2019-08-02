#include "stdafx.h"
#include "OggParser.h"

const unsigned char vorbis1	[]	= { 0x01, 0x76, 0x6F, 0x72, 0x62, 0x69, 0x73 };
const unsigned char vorbis3	[]	= { 0x03, 0x76, 0x6F, 0x72, 0x62, 0x69, 0x73 };
const unsigned char fLaC	[]	= { 0x66, 0x4C, 0x61, 0x43 };
const unsigned char Speex	[]	= { 0x53, 0x70, 0x65, 0x65, 0x78, 0x20, 0x20, 0x20 };
const std::string	Oggs		= "OggS";


OggParser::OggParser( const wstring path )
: MediaBase(path)
{
	_oggType					= (OggHeaderType)3;	

	_samples					= 0;
	_sampleRate					= 0;

	_segMents					= NULL;

	_pos						= 0;
	_firstBufLen				= 0;	

	_abGranulePostion			= 0;
	_multiplexedStreamDetected  = false;

}

OggParser::~OggParser(void)
{
	SAFE_DELETE(_segMents);
}


bool ND91Assistant::OggParser::ParserFormat()
{
	fstream fs(CCodeOperation::WstringToString(_path).c_str(), ios::in|ios::out|ios::binary);
	fs.seekg(0, ios::end);
	_media->_fileSize = fs.tellg();
	fs.seekg(0, ios::beg);

	if (_media->_fileSize < 3)
	{
		fs.close();
		return false;
	}

	char buf[4] = {0};
	fs.read(buf, 3);
	wstring strogg = CCodeOperation::StringToWstring(buf);

	strogg = CStrOperation::toLowerW(strogg);

	if (OGG != strogg)
	{
		fs.close();
		return false;
	}

	fs.close();
	return true;	
}


long ND91Assistant::OggParser::GetMediaDuration()
{	
	bool success = true;

	RETURN_VALUE_IF ( !ParserFormat(), 0 );		// 解析失败或非该格式文件，返回0.

	RETURN_VALUE_IF ( !ReadData(false), 0 );	// 解析文件不符合要求，返回0.

	BinaryBufferReader binarybuf((char*)_segMents, _firstBufLen);

	if (strncmp((char*)_segMents, (char*)vorbis1, 7) == 0)
	{
		binarybuf.SetOffset(12);
		binarybuf.ReadInt32(_sampleRate);
		binarybuf.SetOffset(31);

		while( success )
		{
			success = ReadData(true);
		}

		RETURN_VALUE_IF ( !_multiplexedStreamDetected, 0 );

		_samples = (int)_abGranulePostion;
	}
	else if (strncmp((char*)_segMents, (char*)fLaC, 4) == 0)
	{
		unsigned char buffer[8] = {0};
		binarybuf.ReadBuffer((char*)buffer, 8);

		_sampleRate = buffer[0];
		_sampleRate = (_sampleRate << 8) | (int)buffer[1];
		_sampleRate = (_sampleRate << 4) | ((int)buffer[2] >> 4);

		int channels = (((int)buffer[2] >> 1) & 7) + 1;

		int FlacBitsPerSample = ((((int)buffer[2] & 1) << 4) | ((int)buffer[3] >> 4)) + 1;

		_samples = ((int)buffer[3] & 15);
		_samples = (_samples << 8) | (int)buffer[4];
		_samples = (_samples << 8) | (int)buffer[5];
		_samples = (_samples << 8) | (int)buffer[6];
		_samples = (_samples << 8) | (int)buffer[7];

	}
	else if (strncmp((char*)_segMents, (char*)vorbis1, 8) == 0)
	{
		binarybuf.SetOffset(36);
		binarybuf.ReadInt32(_sampleRate);
		binarybuf.SetOffset(60);

		while( success )
		{
			success = ReadData(true);
		}
		RETURN_VALUE_IF ( !_multiplexedStreamDetected, 0 );

		_samples = (int)_abGranulePostion;
	}
	else
	{
		return 0;
	}

	_media->_duration = (_samples / _sampleRate)/1;

	return _media->_duration;
}

bool ND91Assistant::OggParser::ReadData( bool onlyLast )
{
	int nTemp = 0, nNum = 0, totalPos = 0;
	char buffer[5] = {0};
	unsigned char buf[2] = {0};
	/*__int32*/int32_t serial = 0;
	/*__int32*/int32_t	checksum = 0, pageSequenceNumber = 0;

	RETURN_FALSE_IF (_pos >= _media->_fileSize );

	BinaryBufferReader binarybuf( _path );
	binarybuf.SetOffset(_pos);

	binarybuf.ReadBuffer(buffer, 4);
	std::string strTemp = buffer;
	RETURN_FALSE_IF ( Oggs != strTemp );

	binarybuf.ReadBuffer((char*)buf, 1);
	RETURN_FALSE_IF ( (int)buf[0] != 0 );

	binarybuf.ReadBuffer((char*)buf, 1);
	_oggType = (OggHeaderType)(int)buf[0];

	binarybuf.ReadInt64(_abGranulePostion);
	binarybuf.ReadInt32(serial);

	_multiplexedStreamDetected = true;	

	binarybuf.ReadInt32(pageSequenceNumber);
	binarybuf.ReadInt32(checksum);
	binarybuf.ReadBuffer((char*)buf, 1);
	nNum = (int)buf[0];

	int* sizeSegments = new int[nNum];

	for (int i = 0; i < nNum; i++)
	{
		binarybuf.ReadBuffer((char*)buf, 1);
		sizeSegments[i] = (int)buf[0];
		totalPos += sizeSegments[i];
	}

	_firstBufLen = sizeSegments[0];

	if (!onlyLast || IsLastInLogicalBitstream())
	{
		_segMents = new unsigned char[_firstBufLen];
		binarybuf.ReadBuffer( (char*)_segMents, _firstBufLen );
	}
	else
	{
		_pos = binarybuf.GetOffset();
		binarybuf.SetOffset( _pos + totalPos );
	}

	SAFE_DELETE(sizeSegments);
	_pos = binarybuf.GetOffset();
	
	return true;
}

bool ND91Assistant::OggParser::IsLastInLogicalBitstream()
{
	return (_oggType == ContinuedPaquetInLastLogicalBitstream ||
			_oggType == FreshPaquetInLastLogicalBitstream ||
			_oggType == StartAndEndLogicalBitstream);
}
