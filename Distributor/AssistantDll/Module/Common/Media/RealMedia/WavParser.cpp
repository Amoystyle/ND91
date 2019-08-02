#include "stdafx.h"
#include "WavParser.h"

const int RIFF = 0x46464952;
const int RIFX = 0x58464952;
const int WAVE = 0x45564157;
const int FMT  = 0x20746D66;
const int FACT = 0x74636166;
const int DATA = 0x61746164;

const short DA = 0x6164;
const short FA = 0x6166;


WavParser::WavParser( const wstring path )
: MediaBase(path)
{		
	_pos		= 0;	

}

WavParser::~WavParser(void)
{
}


bool ND91Assistant::WavParser::ParserFormat()
{
	_media->_fileSize = CFileOperation::GetFileSizeW(_path);
	
	BinaryBufferReader binarybuf( _path );

	//Read riff header
	binarybuf.ReadInt32(_riffHeader.RiffID);
	binarybuf.ReadInt32(_riffHeader.fileSize);
	binarybuf.ReadInt32(_riffHeader.fmt);

	//check header
	RETURN_FALSE_IF (_riffHeader.RiffID != RIFF && _riffHeader.RiffID != RIFX && _riffHeader.fmt != WAVE);


	//read format block
	binarybuf.ReadInt32(_format.FmtID);
	binarybuf.ReadInt32(_format.BlockSize);

	RETURN_FALSE_IF ( _format.FmtID != FMT );

	_pos = binarybuf.GetOffset();
	return true;
	
}


long ND91Assistant::WavParser::GetMediaDuration()
{
	int dataSize = 0;
	RETURN_VALUE_IF ( ! ParserFormat(), 0 );	// 解析失败或非该格式文件，返回0.

	BinaryBufferReader binarybuf( _path );
	binarybuf.SetOffset(_pos);

	binarybuf.ReadShort(_format.FormatTag);
	binarybuf.ReadShort(_format.Channels);
	binarybuf.ReadInt32(_format.SamplesPerSec);
	binarybuf.ReadInt32(_format.AvgBytesPerSec);
	binarybuf.ReadShort(_format.lockAlign);
	binarybuf.ReadShort(_format.BitsPerSample);


	//read fact block optional
	short otherInfo = 0;
	binarybuf.ReadShort(otherInfo);

	int pos = 0, factSize = 0, chunkType = 0;

	if (otherInfo == DA || otherInfo == FA)
	{
		pos = binarybuf.GetOffset();
		binarybuf.SetOffset(pos-2);
	}

	binarybuf.ReadInt32(chunkType);

	if (chunkType == FACT)
	{
		binarybuf.ReadInt32(factSize);
		binarybuf.SetOffset(factSize);

		// read data block
		binarybuf.ReadInt32(chunkType);
		binarybuf.ReadInt32(dataSize);

		RETURN_VALUE_IF ( chunkType != DATA, 0);
	}
	else if (chunkType == DATA)
	{
		binarybuf.ReadInt32(dataSize);
	}

	if (_format.AvgBytesPerSec != 0)
	{
		_media->_duration = dataSize / _format.AvgBytesPerSec;
	}

	return _media->_duration;

}