#include "stdafx.h"

const int ADIF = 0x46494441;
const int ADTS_SAMPLE_RATES[] = { 96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000, 7350, 0, 0, 0 };

AacParser::AacParser( const wstring path )
: MediaBase(path)
{		
	_isADIF = false;
	_isADTS = false;

}

AacParser::~AacParser(void)
{

}

unsigned char* ND91Assistant::AacParser::ParserHeader()
{
	fstream fs(_path.c_str(), ios::in|ios::out|ios::binary);

	fs.seekg(0, ios::end);
	_media->_fileSize = fs.tellg();
	fs.seekg(0, ios::beg);

	if (_media->_fileSize<10)
	{
		fs.close();
		return NULL;
	}

	unsigned char* buf = new unsigned char[10];
	fs.read((char*)buf, 10);

	if (buf[0] == ('I') && buf[1] == ('D') && buf[2] == ('3'))
	{
		/* high bit is not used */
		int tagsize = ((int)buf[6] << 21) | ((int)buf[7] << 14) |
			((int)buf[8] << 7) | ((int)buf[9] << 0);

		if ( _media->_fileSize < tagsize + 20)
		{
			fs.close();
			return NULL;
		}
		fs.seekg(tagsize, ios::cur);
		fs.read((char*)buf, 10);		
	}
	fs.close();

	int header = (int)buf[0] + ((int)buf[1] << 8) + ((int)buf[2] << 16) + ((int)buf[3] << 24);

	if (buf[0] == 0xFF && (buf[1] & 0xF6) == 0xF0)
	{
		_isADTS = true;
	}
	if ( header == ADIF )
	{
		_isADIF = true;
	}

	return buf;
}

bool ND91Assistant::AacParser::ParserFormat()
{
	unsigned char* buf = ParserHeader();

	if ( _isADTS )
	{
		SAFE_DELETE(buf);
		return true;
	}
	if ( _isADIF )
	{
		SAFE_DELETE(buf);
		return true;
	}
	
	SAFE_DELETE(buf);
	return false;
	
}

void ND91Assistant::AacParser::ADTS_Parser( fstream* fs )
{
	fs->seekg(0, ios::beg);
	int currentPosition = 0;
	int bufSize = 6;
	unsigned char buf[6] = {0};

	int samplerate = 0;
	int frames = 0;
	int frameLength = 0;
	int t_framelength = 0;
	double frames_per_sec = 0;
	double bytes_per_frame = 0;

	for (; ; frames++)
	{
		if (currentPosition + bufSize >= _media->_fileSize)
			break;
		fs->seekg(currentPosition, ios::beg);
		fs->read((char*)buf, bufSize);

		if (buf[0] != 0xFF && (buf[1] & 0xF6) != 0xF0)
			break;

		if (frames == 0)
			samplerate = ADTS_SAMPLE_RATES[(buf[2] & 0x3c) >> 2];

		frameLength = ((int)(buf[3] & 0x3) << 11) | ((int)buf[4] << 3) | ((int)buf[5] >> 5);

		t_framelength += frameLength;
		currentPosition += frameLength;

		if (currentPosition + 5 > _media->_fileSize)
			break;

		if (currentPosition >= _media->_fileSize)
			break;
	}

	frames_per_sec = samplerate / 1024.0;
	if (frames != 0)
		bytes_per_frame = (float)t_framelength / (float)(frames * 1000);
	else
		bytes_per_frame = 0;
	_media->_bitrate = (int)(8 * bytes_per_frame * frames_per_sec);
	if (frames_per_sec != 0)
		_media->_duration = (int)(frames / frames_per_sec);
	else
		_media->_duration = 0;	
}

void ND91Assistant::AacParser::ADIF_Parser( unsigned char* buf )
{
	int skip_size =  0;
	if ((buf[4] & 0x80) != 0)
		skip_size = 9;
	_media->_bitrate = ((int)(buf[4 + skip_size] & 0x0F) << 19) | ((int)buf[5 + skip_size] << 11) | ((int)buf[6 + skip_size] << 3) | (int)(buf[7 + skip_size] & 0xE0);

	if (_media->_fileSize > 0)
	{
		_media->_duration = (_media->_fileSize * 80) / _media->_bitrate;
	}

	_media->_bitrate = _media->_bitrate / 1000;
}


long ND91Assistant::AacParser::GetMediaDuration()
{
	unsigned char* buf = ParserHeader();

	if ( NULL == buf )
	{
		SAFE_DELETE(buf);
		return 0;
	}

	fstream fs(_path.c_str(), ios::in|ios::out|ios::binary);

	if ( _isADTS )
	{
		ADTS_Parser(&fs);
		SAFE_DELETE(buf);
		return _media->_duration;
	}
	if ( _isADIF )
	{
		ADIF_Parser(buf);
		SAFE_DELETE(buf);
		return _media->_duration;
	}

	return 0;
}
