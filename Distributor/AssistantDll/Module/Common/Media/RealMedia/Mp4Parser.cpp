#include "stdafx.h"
#include "Mp4Parser.h"

static int SAMPLE_RATES[]  = {96000, 88200, 64000, 48000, 44100, 32000,24000, 22050, 16000, 12000, 11025, 8000};

const /*byte*/unsigned char COPYRIGHT_SYMBOL = 0xA9;

const int ATOM_SIZE = 8;
const int MAX_TRACKS = 1024;
const int TRACK_UNKNOWN = 0;
const int TRACK_AUDIO = 1;
const int TRACK_VIDEO = 2;
const int TRACK_SYSTEM = 3;
const int SUBATOMIC = 128;

//atoms without subatoms 
const int ATOM_FTYP = 129;
const int ATOM_MDAT = 130;
const int ATOM_MVHD = 131;
const int ATOM_TKHD = 132;
const int ATOM_TREF = 133;
const int ATOM_MDHD = 134;
const int ATOM_VMHD = 135;
const int ATOM_SMHD = 136;
const int ATOM_HMHD = 137;
const int ATOM_STSD = 138;
const int ATOM_STTS = 139;
const int ATOM_STSZ = 140;
const int ATOM_STZ2 = 141;
const int ATOM_STCO = 142;
const int ATOM_STSC = 143;
const int ATOM_MP4A = 144;
const int ATOM_MP4V = 145;
const int ATOM_MP4S = 146;
const int ATOM_ESDS = 147;
const int ATOM_META = 148; // iTunes Metadata box 
const int ATOM_NAME = 149; // iTunes Metadata name box 
const int ATOM_DATA = 150; // iTunes Metadata data box 
const int ATOM_CTTS = 151;
const int ATOM_FRMA = 152;
const int ATOM_IVIV = 153;
const int ATOM_PRIV = 154;
const int ATOM_USER = 155;
const int ATOM_KEY = 156;

const int ATOM_ALBUM_ARTIST = 157;
const int ATOM_CONTENTGROUP = 158;
const int ATOM_LYRICS = 159;
const int ATOM_DESCRIPTION = 160;
const int ATOM_NETWORK = 161;
const int ATOM_SHOW = 162;
const int ATOM_EPISODENAME = 163;
const int ATOM_SORTTITLE = 164;
const int ATOM_SORTALBUM = 165;
const int ATOM_SORTARTIST = 166;
const int ATOM_SORTALBUMARTIST = 167;
const int ATOM_SORTWRITER = 168;
const int ATOM_SORTSHOW = 169;
const int ATOM_SEASON = 170;
const int ATOM_EPISODE = 171;
const int ATOM_PODCAST = 172;

const int ATOM_UNKNOWN = 255;
const int ATOM_FREE = 255;
const int ATOM_SKIP = 255;

// atoms with subatoms 
const int ATOM_MOOV = 1;
const int ATOM_TRAK = 2;
const int ATOM_EDTS = 3;
const int ATOM_MDIA = 4;
const int ATOM_MINF = 5;
const int ATOM_STBL = 6;
const int ATOM_UDTA = 7;
const int ATOM_ILST = 8; // iTunes Metadata list 
const int ATOM_TITLE = 9;
const int ATOM_ARTIST = 10;
const int ATOM_WRITER = 11;
const int ATOM_ALBUM = 12;
const int ATOM_DATE = 13;
const int ATOM_TOOL = 14;
const int ATOM_COMMENT = 15;
const int ATOM_GENRE1 = 16;
const int ATOM_TRACK = 17;
const int ATOM_DISC = 18;
const int ATOM_COMPILATION = 19;
const int ATOM_GENRE2 = 20;
const int ATOM_TEMPO = 21;
const int ATOM_COVER = 22;
const int ATOM_DRMS = 23;
const int ATOM_SINF = 24;
const int ATOM_SCHI = 25;


Mp4Parser::Mp4Parser( const wstring path )
: MediaBase(path)
{
	_currentTrack	= 0;
	_fs				= NULL;
	ParserHeader();
}

Mp4Parser::~Mp4Parser(void)
{
	SAFE_DELETE(_fs);
	
	for ( size_t i=0; i<_ltTrack.size(); ++i )
	{
		SAFE_DELETE ( _ltTrack[i]->stts_sample_delta );
		SAFE_DELETE ( _ltTrack[i]->stts_sample_delta );
		SAFE_DELETE ( _ltTrack[i]->decoderConfig );
	}

	RELEASE_VECTOR(_ltTrack);

    for (map<string, unsigned char*>::iterator it = _dicTag.begin(); it != _dicTag.end(); it++)
        SAFE_DELETE(it->second);
}


long ND91Assistant::Mp4Parser::GetMediaDuration()
{

	RETURN_VALUE_IF ( !ParserFormat(), 0 );		// 解析失败或非该格式文件，返回0.

	int pos = _fs->tellg();

	while (	pos < _media->_fileSize - ATOM_SIZE)
	{
		int atomType = 0, atomSize = 0;

		unsigned char buf[ATOM_SIZE] = {0};
		_fs->read( (char*)buf, ATOM_SIZE );

		atomSize = GetInt(buf, 0);
		atomType = mp4ff_atom_name_to_type(buf);

		RETURN_VALUE_IF ( atomSize == 0, -1 );

		pos = (int)_fs->tellg() + atomSize - ATOM_SIZE;
		if (atomType == ATOM_MOOV)
		{
			parse_sub_atoms(atomSize);
		}

		_fs->seekg(pos, ios::beg);
		pos = _fs->tellg();

	}
	_fs->close();

	Set_mp4AudioSpecificConfig();

	return _media->_duration;
}

bool ND91Assistant::Mp4Parser::ParserFormat()
{
	bool result = false;
	_fs		= new fstream(CCodeOperation::WstringToString(_path).c_str(), ios::in|ios::out|ios::binary);
	_fs->seekg(0, ios::end);
	_media->_fileSize = _fs->tellg();
	_fs->seekg(0, ios::beg);

	if (_media->_fileSize < ATOM_SIZE)
	{
		_fs->close();
		return false;
	}

	unsigned char buf[8] = {0};
	_fs->read((char*)buf, 8);

	if (mp4ff_atom_name_to_type(buf) == ATOM_FTYP)
		result = true;

	_fs->seekg(0, ios::beg);

	return result;

}


int ND91Assistant::Mp4Parser::mp4ff_atom_name_to_type( unsigned char* buf )
{
	unsigned char buffer[5] = {0};

	//sprintf_s( (char*)buffer, 5, "%c%c%c%c", buf[4], buf[5], buf[6], buf[7] );
	sprintf( (char*)buffer, "%c%c%c%c", buf[4], buf[5], buf[6], buf[7] );

	std::string strbuf = (char*)buffer;
	if (buffer[0] == 'm')
	{
		if ( "moov" == strbuf )
			return ATOM_MOOV;
		else if ( "minf" == strbuf )
			return ATOM_MINF;
		else if ( "mdia" == strbuf )
			return ATOM_MDIA;
		else if ( "mdat" == strbuf )
			return ATOM_MDAT;
		else if ( "mdhd" == strbuf )
			return ATOM_MDHD;
		else if ( "mvhd" == strbuf )
			return ATOM_MVHD;
		else if ( "mp4a" == strbuf )
			return ATOM_MP4A;
		else if ( "mp4v" == strbuf )
			return ATOM_MP4V;
		else if ( "mp4s" == strbuf )
			return ATOM_MP4S;
		else if ( "meta" == strbuf )
			return ATOM_META;
	}
	else if (buffer[0] == 't')
	{
		if ( "trak" == strbuf )
			return ATOM_TRAK;
		else if ( "tkhd" == strbuf )
			return ATOM_TKHD;
		else if ( "tref" == strbuf )
			return ATOM_TREF;
		else if ( "trkn" == strbuf )
			return ATOM_TRACK;
		else if ( "tmpo" == strbuf )
			return ATOM_TEMPO;
		else if ( "tvnn" == strbuf )
			return ATOM_NETWORK;
		else if ( "tvsh" == strbuf )
			return ATOM_SHOW;
		else if ( "tven" == strbuf )
			return ATOM_EPISODENAME;
		else if ( "tvsn" == strbuf )
			return ATOM_SEASON;
		else if ( "tves" == strbuf )
			return ATOM_EPISODE;
	}
	else if (buffer[0] == 's')
	{
		if ( "stbl" == strbuf )
			return ATOM_STBL;
		else if ( "smhd" == strbuf )
			return ATOM_SMHD;
		else if ( "stsd" == strbuf )
			return ATOM_STSD;
		else if ( "stts" == strbuf )
			return ATOM_STTS;
		else if ( "stco" == strbuf )
			return ATOM_STCO;
		else if ( "stsc" == strbuf )
			return ATOM_STSC;
		else if ( "stsz" == strbuf )
			return ATOM_STSZ;
		else if ( "stz2" == strbuf )
			return ATOM_STZ2;
		else if ( "skip" == strbuf )
			return ATOM_SKIP;
		else if ( "sinf" == strbuf )
			return ATOM_SINF;
		else if ( "schi" == strbuf )
			return ATOM_SCHI;
		else if ( "sonm" == strbuf )
			return ATOM_SORTTITLE;
		else if ( "soal" == strbuf )
			return ATOM_SORTALBUM;
		else if ( "soar" == strbuf )
			return ATOM_SORTARTIST;
		else if ( "soaa" == strbuf )
			return ATOM_SORTALBUMARTIST;
		else if ( "soco" == strbuf )
			return ATOM_SORTWRITER;
		else if ( "sosn" == strbuf )
			return ATOM_SORTSHOW;
	}
	else if (buffer[0] == COPYRIGHT_SYMBOL)
	{
		std::string strTemp = strbuf.substr(1, 3);

		if ( "nam" == strTemp )
			return ATOM_TITLE;
		else if ( "ART" == strTemp )
			return ATOM_ARTIST;
		else if ( "wrt" == strTemp )
			return ATOM_WRITER;
		else if ( "alb" == strTemp )
			return ATOM_ALBUM;
		else if ( "day" == strTemp )
			return ATOM_DATE;
		else if ( "too" == strTemp )
			return ATOM_TOOL;
		else if ( "cmt" == strTemp )
			return ATOM_COMMENT;
		else if ( "gen" == strTemp )
			return ATOM_GENRE1;
		else if ( "grp" == strTemp )
			return ATOM_CONTENTGROUP;
		else if ( "lyr" == strTemp )
			return ATOM_LYRICS;
	}

	if ( "edts" == strbuf )
		return ATOM_EDTS;
	else if ( "esds" == strbuf )
		return ATOM_ESDS;
	else if ( "ftyp" == strbuf )
		return ATOM_FTYP;
	else if ( "free" == strbuf )
		return ATOM_FREE;
	else if ( "hmhd" == strbuf )
		return ATOM_HMHD;
	else if ( "vmhd" == strbuf )
		return ATOM_VMHD;
	else if ( "udta" == strbuf )
		return ATOM_UDTA;
	else if ( "ilst" == strbuf )
		return ATOM_ILST;
	else if ( "name" == strbuf )
		return ATOM_NAME;
	else if ( "data" == strbuf )
		return ATOM_DATA;
	else if ( "disk" == strbuf )
		return ATOM_DISC;
	else if ( "gnre" == strbuf )
		return ATOM_GENRE2;
	else if ( "covr" == strbuf )
		return ATOM_COVER;
	else if ( "cpil" == strbuf )
		return ATOM_COMPILATION;
	else if ( "ctts" == strbuf )
		return ATOM_CTTS;
	else if ( "drms" == strbuf )
		return ATOM_DRMS;
	else if ( "frma" == strbuf )
		return ATOM_FRMA;
	else if ( "priv" == strbuf )
		return ATOM_PRIV;
	else if ( "iviv" == strbuf )
		return ATOM_IVIV;
	else if ( "user" == strbuf )
		return ATOM_USER;
	else if ( "key " == strbuf )
		return ATOM_KEY;
	else if ( "aART" == strbuf )
		return ATOM_ALBUM_ARTIST;
	else if ( "desc" == strbuf )
		return ATOM_DESCRIPTION;
	else if ( "pcst" == strbuf )
		return ATOM_PODCAST;
	else
		return ATOM_UNKNOWN;

}

void ND91Assistant::Mp4Parser::parse_sub_atoms( long totalAtomSize )
{
	int atomType = 0;
	long atomSize = 0;
	long counted_size = 0;
	long pos = (long)_fs->tellg() + totalAtomSize;
	while (counted_size < totalAtomSize - ATOM_SIZE)
	{
		read_atom_header(atomType, atomSize);
		counted_size += atomSize;

		if (atomSize == 0 || totalAtomSize - counted_size <= 0)
		{
			_fs->seekg(pos, ios::beg);
			break;
		}

		if (atomType == ATOM_TRAK)
		{
 			_currentTrack++;
			MP4FF_TRACK_T* pmp4ff = new MP4FF_TRACK_T;
			_ltTrack.push_back(pmp4ff);
		}

		if (atomType < SUBATOMIC)
		{
			parse_sub_atoms(atomSize);
		}
		else
		{
			mp4ff_atom_read(atomType, atomSize);
		}
	}
}

void ND91Assistant::Mp4Parser::read_atom_header( int& atomType, long& atomSize )
{
	unsigned char buf[ATOM_SIZE];
	_fs->read((char*)buf, ATOM_SIZE);
	atomSize = GetInt(buf, 0);

	atomType = mp4ff_atom_name_to_type(buf);
}

void ND91Assistant::Mp4Parser::Set_mp4AudioSpecificConfig()
{
	int track = -1, idex = 0;
	_mp4AudioSpecificConfig.sbr_present_flag = -1;

	for (int i=0;i<(int)_ltTrack.size();i++,idex++)
	{
		if (_ltTrack[i]->decoderConfigLen > 0 && _ltTrack[i]->decoderConfig != NULL)
		{
			track = idex;
			break;
		}
	}

	if (track >= 0)
	{
		long samples = 0;
		/*u_int*/unsigned int flag = 0;
		int loop = 3;

		MP4FF_TRACK_T *theTrack = _ltTrack[track];

		for (int i = 0; i < theTrack->decoderConfigLen && i < 4; i++)
		{
			flag += (/*u_int*/unsigned int)(theTrack->decoderConfig[i] << (loop * 8));
			loop--;
		}

		_mp4AudioSpecificConfig.objectTypeIndex = (int)(flag & 0xF8000000) >> 27; //5bit
		_mp4AudioSpecificConfig.samplingFrequencyIndex = (int)(flag & 0x07800000) >> 23; //4bit
		_mp4AudioSpecificConfig.channelsConfiguration = (int)(flag & 0x00780000) >> 19; // 4bit
		_mp4AudioSpecificConfig.samplingFrequency = SAMPLE_RATES[_mp4AudioSpecificConfig.samplingFrequencyIndex];

		if (_mp4AudioSpecificConfig.objectTypeIndex != 5)
		{
			if (_mp4AudioSpecificConfig.samplingFrequency <= 24000)
			{
				//_mp4AudioSpecificConfig.samplingFrequency *= 2;
				_mp4AudioSpecificConfig.forceUpSampling = 1;
			}
			else /* > 24000*/
			{
				_mp4AudioSpecificConfig.downSampledSBR = 1;
			}
		}
		else
		{
			_mp4AudioSpecificConfig.sbr_present_flag = 1;
		}

		for (int i = 0; i < theTrack->stts_entry_count ; i++)
		{
			int temp = theTrack->stts_sample_count[i];
			samples += temp;
		}

		int f = 1024 ;
		if (_mp4AudioSpecificConfig.sbr_present_flag == 1)
			f = f * 2;

	 	_media->_duration = samples * f / _mp4AudioSpecificConfig.samplingFrequency;

	}
}

void ND91Assistant::Mp4Parser::mp4ff_atom_read( int atomType, long atomSize )
{
	long dest_position = (int)_fs->tellg() + atomSize - 8;
	if (atomType == ATOM_STSZ)
	{
		/* sample size box */
		//mp4ff_read_stsz(f);
	}
	else if (atomType == ATOM_STTS)
	{
		/* time to sample box */
		mp4ff_read_stts();
	}
	else if (atomType == ATOM_CTTS)
	{
		/* composition offset box */
		//mp4ff_read_ctts(f);
	}
	else if (atomType == ATOM_STSC)
	{
		/* sample to chunk box */
		//mp4ff_read_stsc(f);
	}
	else if (atomType == ATOM_STCO)
	{
		/* chunk offset box */
		//mp4ff_read_stco(f);
	}
	else if (atomType == ATOM_STSD)
	{
		/* sample description box */
		mp4ff_read_stsd();
	}
	else if (atomType == ATOM_MVHD)
	{
		/* movie header box */
		//mp4ff_read_mvhd(f);
	}
	else if (atomType == ATOM_MDHD)
	{
		/* track header */
		//mp4ff_read_mdhd(f);
	}
	else if (atomType == ATOM_META)
	{
		/* iTunes Metadata box */
		mp4ff_read_meta(atomSize);
	}

	_fs->seekg(dest_position, ios::beg);
}

void ND91Assistant::Mp4Parser::mp4ff_read_stts()
{
	MP4FF_TRACK_T* currentTrack = _ltTrack[_currentTrack - 1];

	if (currentTrack->stts_entry_count > 0)
		return ;

	int pos = _fs->tellg();
	_fs->seekg(pos+4, ios::beg);			/* version + flags */
	unsigned char buf[4] = {0};
	_fs->read((char*)buf, 4);
	currentTrack->stts_entry_count = GetInt(buf,0);
	RETURN_IF (currentTrack->stts_entry_count == 0);

	currentTrack->stts_sample_count = new int[currentTrack->stts_entry_count];
	currentTrack->stts_sample_delta = new int[currentTrack->stts_entry_count];

	for (int i = 0; i < currentTrack->stts_entry_count; i++)
	{
		_fs->read((char*)buf, 4);
		currentTrack->stts_sample_count[i] = GetInt(buf, 0);
		currentTrack->stts_sample_delta[i] = GetInt(buf, 0);
	}
}

void ND91Assistant::Mp4Parser::mp4ff_read_stsd()
{
	MP4FF_TRACK_T* currentTrack = _ltTrack[_currentTrack - 1];
	unsigned char buf[8] = {0};
	_fs->read((char*)buf, 8);

	int trackCount = GetInt(buf, 4);
	currentTrack->stsd_entry_count = trackCount;
	for (int i = 0; i < trackCount; i++)
	{
		long skip = _fs->tellg();
		int atomType = 0;
		long atomSize = 0;
		read_atom_header(atomType, atomSize);
		skip += atomSize;

		if (atomType == ATOM_MP4A)
		{
			currentTrack->ntype = TRACK_AUDIO;
			mp4ff_read_mp4a();
		}
		else if (atomType == ATOM_MP4V)
		{
			currentTrack->ntype = TRACK_VIDEO;
		}
		else if (atomType == ATOM_MP4S)
		{
			currentTrack->ntype = TRACK_SYSTEM;
		}
		else
		{
			currentTrack->ntype = TRACK_UNKNOWN;
		}

		_fs->seekg(skip, ios::beg);
	}
}

void ND91Assistant::Mp4Parser::mp4ff_read_mp4a()
{
	MP4FF_TRACK_T* currentTrack = _ltTrack[_currentTrack - 1];
	unsigned char buf[28] = {0};
	_fs->read((char*)buf, 28);

	currentTrack->channelCount = buf[17] + (buf[16] << 8);
	currentTrack->sampleSize = buf[19] + (buf[18] << 8);
	currentTrack->sampleRate = buf[25] + (buf[24] << 8);

	int atomType = 0;
	long atomSize = 0;
	read_atom_header(atomType, atomSize);
	if (atomType == ATOM_ESDS)
	{
		mp4ff_read_esds();
	}
}

void ND91Assistant::Mp4Parser::mp4ff_read_esds()
{
	MP4FF_TRACK_T* currentTrack = _ltTrack[_currentTrack - 1];
	int pos = _fs->tellg();
	_fs->seekg(pos+4);

	unsigned char readtag[2] = {0};
	_fs->read((char*)readtag, 1);
	if (readtag[0] == 0x03)
	{
		if (mp4ff_read_mp4_descr_length() < 20)
			return;
		pos = _fs->tellg();
		_fs->seekg(pos+3);
	}
	else
	{
		pos = _fs->tellg();
		_fs->seekg(pos+3);
	}

	_fs->read((char*)readtag, 1);
	if (readtag[0] != 0x04)
		return;

	int temp = mp4ff_read_mp4_descr_length();
	if (temp < 13)
		return ;

	_fs->read((char*)readtag, 1);
	currentTrack->audioType = readtag[0];
	pos = _fs->tellg();
	_fs->seekg(pos+4);

	unsigned char buf[4] = {0};
	_fs->read((char*)buf, 4);
	currentTrack->maxBitrate = GetInt(buf,0);
	_fs->read((char*)buf, 4);
	currentTrack->avgBitrate = GetInt(buf, 0);

	_fs->read((char*)readtag, 1);
	if (readtag[0] != 0x05)
		return;

	currentTrack->decoderConfigLen = mp4ff_read_mp4_descr_length();
	currentTrack->decoderConfig = new unsigned char[currentTrack->decoderConfigLen];
	_fs->read((char*)currentTrack->decoderConfig, currentTrack->decoderConfigLen);
}

int ND91Assistant::Mp4Parser::mp4ff_read_mp4_descr_length()
{
	unsigned char b[2] = {0};
	/*byte*/unsigned char numBytes = 0;
	int length = 0;

	do
	{
		_fs->read((char*)b, 1);
		numBytes++;
		length = (length << 7) | (b[0] & 0x7F);
	} while (((b[0] & 0x80) != 0) && numBytes < 4);

	return length;	

}

void ND91Assistant::Mp4Parser::mp4ff_read_meta( long atomSize )
{
	long subsize = 0, sumsize = 0;
	int atom_type = 0;
	long header_size = 8;

	_fs->seekg( (int)_fs->tellg()+4, ios::beg);

	while (sumsize < (atomSize - (header_size + 4)))
	{
		read_atom_header(atom_type, subsize);
		if (subsize <= header_size + 4)
			return;
		if (atom_type == ATOM_ILST)
		{
			mp4ff_parse_metadata(subsize - header_size - 4);
		}
		else
		{
			_fs->seekg((int)_fs->tellg() + subsize - header_size, ios::beg);
		}
		sumsize += subsize;
	}
}

void ND91Assistant::Mp4Parser::mp4ff_parse_metadata( long atomSize )
{
	long subsize = 0, sumsize = 0;
	int atom_type = 0;
	int header_size = 8;

	while (sumsize < atomSize)
	{
		read_atom_header(atom_type, subsize);
		if (subsize == 0)
			break;
		mp4ff_parse_tag( atom_type, subsize - header_size);
		sumsize += subsize;
	}
}

void ND91Assistant::Mp4Parser::mp4ff_parse_tag( int parent_atom_type, long size )
{
	int atom_type = 0;
	int header_size = 8;
	long subsize = 0, sumsize = 0;
	std::string name ;
	unsigned char* data = NULL;
	int done = 0;

	while (sumsize < size)
	{
		long destpos;
		read_atom_header(atom_type, subsize);
		destpos = (int)_fs->tellg() + subsize - header_size;
		if (done == 0)
		{
			if (atom_type == ATOM_DATA)
			{
				_fs->seekg( (int)_fs->tellg() + 8);

				if (parent_atom_type == ATOM_GENRE2 || parent_atom_type == ATOM_TEMPO)
				{
					if (subsize - header_size >= 8 + 2)
					{
                        unsigned char* bufVal = new unsigned char[2];
						_fs->read((char*)bufVal, 2);
						int val = (int)bufVal[1] + ((int)bufVal[0] << 8);

						if (parent_atom_type == ATOM_TEMPO)
						{
							mp4ff_tag_add_field("tempo", bufVal);
						}
						else
						{
							mp4ff_tag_add_field("genre", bufVal);
						}
						done = 1;
					}
				}
				else if (parent_atom_type == ATOM_TRACK || parent_atom_type == ATOM_DISC)
				{
					if (done == 0 && (subsize - header_size) >= 14)
					{
						int total;
						_fs->seekg( (int)_fs->tellg() + 2);

						unsigned char* bufIndex = new unsigned char[2];
						_fs->read((char*)bufIndex, 2);

						unsigned char* bufTotal = new unsigned char[2];
						_fs->read((char*)bufTotal, 2);
						total = bufTotal[1] + (bufTotal[0] << 8);

						mp4ff_tag_add_field(parent_atom_type == ATOM_TRACK ? "track" : "disc", bufIndex);
						if (total > 0)
						{
							mp4ff_tag_add_field(parent_atom_type == ATOM_TRACK ? "totaltracks" : "totaldiscs", bufTotal);
						}
						done = 1;
					}
				}
				else
				{
					data = mp4ff_read_string(subsize - (header_size + 8));
				}
			}
			else if (atom_type == ATOM_NAME)
			{
				if (done == 0)
				{
					_fs->seekg( (int)_fs->tellg() + 4);

					if (!name.empty())
						name = "";
					unsigned char* bufName = mp4ff_read_string(subsize - (header_size + 4));
					name = (char*) bufName;
					SAFE_DELETE (bufName);
				}
			}

			_fs->seekg( (int)_fs->tellg() + destpos);
			sumsize += subsize;
		}
	}

	if (data != NULL)
	{
		if (done == 0)
		{
			if (name.empty())
				name = mp4ff_set_metadata_name(parent_atom_type);

			if (!name.empty())
				mp4ff_tag_add_field(name, data);
		}
	}
}


std::string ND91Assistant::Mp4Parser::mp4ff_set_metadata_name( int parent_atom_type )
{
	std::string tag_names[] = {"unknown", "title", "artist", "writer", "album",
		"date", "tool", "comment", "genre", "track",
		"disc", "compilation", "genre", "tempo", "cover",
		"album_artist", "contentgroup", "lyrics", "description",
		"network", "show", "episodename",
		"sorttitle", "sortalbum", "sortartist", "sortalbumartist",
		"sortwriter", "sortshow",
		"season", "episode", "podcast" };
	int tag_idx = 0;

	switch (parent_atom_type)
	{
	case ATOM_TITLE: tag_idx = 1; break;
	case ATOM_ARTIST: tag_idx = 2; break;
	case ATOM_WRITER: tag_idx = 3; break;
	case ATOM_ALBUM: tag_idx = 4; break;
	case ATOM_DATE: tag_idx = 5; break;
	case ATOM_TOOL: tag_idx = 6; break;
	case ATOM_COMMENT: tag_idx = 7; break;
	case ATOM_GENRE1: tag_idx = 8; break;
	case ATOM_TRACK: tag_idx = 9; break;
	case ATOM_DISC: tag_idx = 10; break;
	case ATOM_COMPILATION: tag_idx = 11; break;
	case ATOM_GENRE2: tag_idx = 12; break;
	case ATOM_TEMPO: tag_idx = 13; break;
	case ATOM_COVER: tag_idx = 14; break;
	case ATOM_ALBUM_ARTIST: tag_idx = 15; break;
	case ATOM_CONTENTGROUP: tag_idx = 16; break;
	case ATOM_LYRICS: tag_idx = 17; break;
	case ATOM_DESCRIPTION: tag_idx = 18; break;
	case ATOM_NETWORK: tag_idx = 19; break;
	case ATOM_SHOW: tag_idx = 20; break;
	case ATOM_EPISODENAME: tag_idx = 21; break;
	case ATOM_SORTTITLE: tag_idx = 22; break;
	case ATOM_SORTALBUM: tag_idx = 23; break;
	case ATOM_SORTARTIST: tag_idx = 24; break;
	case ATOM_SORTALBUMARTIST: tag_idx = 25; break;
	case ATOM_SORTWRITER: tag_idx = 26; break;
	case ATOM_SORTSHOW: tag_idx = 27; break;
	case ATOM_SEASON: tag_idx = 28; break;
	case ATOM_EPISODE: tag_idx = 29; break;
	case ATOM_PODCAST: tag_idx = 30; break;
	default: tag_idx = 0; break;
	}

	return tag_names[tag_idx];
}

unsigned char* ND91Assistant::Mp4Parser::mp4ff_read_string( long readSize )
{
	RETURN_NULL_IF (readSize == 0);
		
	unsigned char* buf = new unsigned char[readSize];
	_fs->read((char*)buf, readSize);
	return buf;
}

void ND91Assistant::Mp4Parser::mp4ff_tag_add_field( std::string name, unsigned char* data )
{
    map<string, unsigned char*>::iterator it = _dicTag.find(name);
    if (it == _dicTag.end())
        _dicTag.insert(pair<string, unsigned char*>(name, data));
}

bool ND91Assistant::Mp4Parser::ParserHeader()
{
    RETURN_FALSE_IF( !ParserFormat() ); // 解析失败或非该格式文件，返回false

    int pos = _fs->tellg();
    while (	pos < _media->_fileSize - ATOM_SIZE)
    {
        int atomType = 0; 
        int atomSize = 0;
        unsigned char buf[ATOM_SIZE] = {0};
        _fs->read( (char*)buf, ATOM_SIZE );

        atomSize = GetInt(buf, 0);
        atomType = mp4ff_atom_name_to_type(buf);
        RETURN_FALSE_IF ( atomSize == 0 );

        pos = (int)_fs->tellg() + atomSize - ATOM_SIZE;
        if (atomType == ATOM_MOOV)
            parse_sub_atoms(atomSize);

        _fs->seekg(pos, ios::beg);
        pos = _fs->tellg();
    }
    _fs->close();

    Set_mp4AudioSpecificConfig();
    GenerateID31Tag();
    GenerateTrack();
    GenerateOther();
    return true;
}

bool ND91Assistant::Mp4Parser::GenerateID31Tag()
{
    RETURN_FALSE_IF( _dicTag.empty() );

    for (map<string, unsigned char*>::iterator it = _dicTag.begin(); it != _dicTag.end(); it ++)
    {
        string temp = (const char*)it->second;
        if (it->first == "title")
            _media->_title = CCodeOperation::StringToWstring(temp);
        else if (it->first == "artist")
            _media->_artist = CCodeOperation::StringToWstring(temp);
        else if (it->first == "album")
            _media->_album = CCodeOperation::StringToWstring(temp);
        else if (it->first == "comment")
            _media->_comment = CCodeOperation::StringToWstring(temp);
    }

    return true;
}

bool ND91Assistant::Mp4Parser::GenerateTrack()
{
    for (size_t i = 0; i < _ltTrack.size(); i++)
    {
        if (_ltTrack.at(i)->ntype != TRACK_VIDEO)
            continue;

        _media->_isVideo = true;
        break;
    }

    RETURN_FALSE_IF( _ltTrack.size() < 2 );
    _media->_nSampleRate    = _ltTrack.at(1)->sampleRate;

    return true;
}

bool ND91Assistant::Mp4Parser::GenerateOther()
{
    _media->_fileSize       = CFileOperation::GetFileSizeW(_path);
    if (_media->_title.empty())
        _media->_title      = CStrOperation::parseStringsW(CFileOperation::GetFileName(_path), L".").at(0);

    return true;
}