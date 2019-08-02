#include "stdafx.h"

const int RIFF = 0x46464952;
const int IAVI = 0x20495641;
const int LIST = 0x5453494C;
const int HDRL = 0x6C726468;
const int AVIH = 0x68697661;
const int STRL = 0x6C727473;
const int STRH = 0x68727473;
const int STRF = 0x66727473;
const int STRN = 0x6E727473;
const int STRD = 0x64727473;
const int IDX1 = 0x31786469;
const int INDX = 0x78646E69;
const int VIDS = 0x73646976;
const int AUDS = 0x73647561;
const int MIDS = 0x7364696D;
const int TXTS = 0x73747874;
const int  DIB = 0x20424944;
const int MOVI = 0x69766F6D;
const int JUNK = 0x4B4E554A;


AviParser::AviParser( const wstring path )
: MediaBase(path)
{		
	_pos	= 0;	 

}

AviParser::~AviParser(void)
{

}


std::string ND91Assistant::AviParser::ParserHeader(BinaryBufferReader* binarybuf, int headSize)
{
	std::string errMsg = "";
	int ReadBytes = 0;
	int chunkType = 0;
	int chunkSize = 0;

	while (ReadBytes < headSize)
	{
		//读取信息块aviDef.AviMainHeader tag
		binarybuf->ReadInt32(chunkType);
		ReadBytes += CHUNK_SIZE;

		//MainHeader struct
		if (chunkType == AVIH)
		{
			//读取结构数据
			int structSize = 0;
			binarybuf->ReadInt32(structSize);
			ReadBytes += CHUNK_SIZE;
			if (structSize == 0)
			{
				errMsg = "文件头格式出错。";
				break;
			}

			//读取avi的头文件信息
			_aviHeader.fcc = chunkType;
			_aviHeader.cb = chunkSize;
			binarybuf->ReadInt32(_aviHeader.dwMicroSecPerFrame);
			binarybuf->ReadInt32(_aviHeader.dwMaxBytesPerSec);
			binarybuf->ReadInt32(_aviHeader.dwPaddingGranularity);
			binarybuf->ReadInt32(_aviHeader.dwFlags);
			binarybuf->ReadInt32(_aviHeader.dwTotalFrames);
			binarybuf->ReadInt32(_aviHeader.dwInitialFrames);
			binarybuf->ReadInt32(_aviHeader.dwStreams);
			binarybuf->ReadInt32(_aviHeader.dwSuggestedBufferSize);
			binarybuf->ReadInt32(_aviHeader.dwWidth);
			binarybuf->ReadInt32(_aviHeader.dwHeight);
			binarybuf->ReadInt32(_aviHeader.dwSuggestedBufferSize);
			binarybuf->ReadInt32(_aviHeader.dwReserved1);
			binarybuf->ReadInt32(_aviHeader.dwReserved2);
			binarybuf->ReadInt32(_aviHeader.dwReserved3);
			binarybuf->ReadInt32(_aviHeader.dwReserved4);

			ReadBytes += structSize;
		}
		else if (chunkType == LIST)
		{
			binarybuf->ReadInt32(chunkSize);
			ReadBytes += CHUNK_SIZE;
		}
	}

	return errMsg;
}

bool ND91Assistant::AviParser::ParserFormat()
{
	_media->_fileSize = CFileOperation::GetFileSizeW(_path);

	BinaryBufferReader binarybuf( _path );

	int tempRead=0, filesize=0;
	binarybuf.ReadInt32(tempRead);
	if (tempRead != RIFF)
	{
		return false;
	}

	//read file size
	binarybuf.ReadInt32(filesize);
	
	//read file type
	binarybuf.ReadInt32(tempRead);
	if (tempRead != IAVI)
	{
		return false;
	}

	_pos = binarybuf.GetOffset();
	return true;
	
}


long ND91Assistant::AviParser::GetMediaDuration()
{
	RETURN_VALUE_IF ( ! ParserFormat(), 0 );    // 解析失败或非该格式文件，返回0.

	BinaryBufferReader binarybuf( _path );
	binarybuf.SetOffset(_pos);

	int ListSize = 0, chunkType = 0;
	int ReadBytes = CHUNK_SIZE;

	while (ReadBytes < _media->_fileSize)
	{
		binarybuf.ReadInt32(chunkType);

		if (chunkType == LIST)
		{
			binarybuf.ReadInt32(ListSize);

		}
		else if (chunkType == HDRL)
		{
			int headSize = ListSize - CHUNK_SIZE;
			ParserHeader(&binarybuf,headSize);
			break;
		}
	}

	if (_aviHeader.dwTotalFrames != 0)
	{
		long framePerSecond = 1000000 / _aviHeader.dwMicroSecPerFrame;
		_media->_duration = _aviHeader.dwTotalFrames / framePerSecond;
	}

	return _media->_duration;
}

