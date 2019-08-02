#include "stdafx.h"

const int FILE_HEADER_LENGTH = 16;
const std::string ATOM_FTYP = "ftyp";
const std::string ATOM_3GP4 = "3gp";
const std::string ATOM_MOOV = "moov";
const std::string ATOM_TKHD = "tkhd";
const std::string ATOM_MVHD = "mvhd";
const std::string ATOM_TRAK = "trak";


Gp3Parser::Gp3Parser( const wstring path )
: MediaBase(path)
{		
}

Gp3Parser::~Gp3Parser(void)
{

}


bool ND91Assistant::Gp3Parser::ParserFormat()
{

	fstream fs(_path.c_str(), ios::in|ios::out|ios::binary);

	fs.seekg(0, ios::end);
	_media->_fileSize = fs.tellg();
	fs.seekg(0, ios::beg);

	if (_media->_fileSize < FILE_HEADER_LENGTH)
	{
		fs.close();
		return false;
	}

	char buffer[FILE_HEADER_LENGTH] = {0};

	fs.read(buffer, FILE_HEADER_LENGTH);
	fs.close();

	char szftyp[5] = {0}, sz3gp4[4] = {0};
	std::string strftyp, str3gp4;


	sprintf_s(szftyp, 5, "%c%c%c%c", buffer[4],buffer[5],buffer[6],buffer[7] );
	sprintf_s(sz3gp4, 4, "%c%c%c", buffer[8],buffer[9],buffer[10]);
 	strftyp = szftyp;
 	str3gp4 = sz3gp4;

	RETURN_FALSE_IF ( strftyp != ATOM_FTYP || str3gp4 != ATOM_3GP4);

	return true;
}


long ND91Assistant::Gp3Parser::GetMediaDuration()
{
	RETURN_VALUE_IF ( ! ParserFormat(), 0 );	// 解析失败或非该格式文件，返回0.

	fstream fs(_path.c_str(), ios::in|ios::out|ios::binary);
	int position = fs.tellg();

	while( position + 8 < _media->_fileSize)
	{
		int atomSize = GetAtomSize(&fs);
		string tmpType = GetAtomType(&fs);

		if (atomSize == 0)
			break;
		position = fs.tellg();
		long tmpPos = atomSize + position - 8;
		
		if (tmpType == ATOM_MOOV)
		{
			ParserMoov(&fs, atomSize);
		}

		fs.seekg(tmpPos, ios::beg);
		position = tmpPos;
	}

	return _media->_duration;
}

int ND91Assistant::Gp3Parser::GetAtomSize( fstream *fs )
{
	int pos = fs->tellg();
	if ( pos + 4 > _media->_fileSize )
		return -1;

	unsigned char buf[4] = {0};
	fs->read((char*)buf, 4);
	return GetInt(buf, 0);
}

std::string ND91Assistant::Gp3Parser::GetAtomType( fstream *fs )
{
	std::string strType = "";
	int pos = fs->tellg();
	if ( pos + 4 > _media->_fileSize )
		return strType;

	char buf[5] = {0};
	fs->read(buf, 4);
	strType = buf;

	return strType;
}

void ND91Assistant::Gp3Parser::ParserMoov( fstream *fs, int moovSize )
{
	long moovPos = fs->tellg();

	while ( (int)fs->tellg() - moovPos + 8 < moovSize)
	{
		int atomSize = GetAtomSize(fs);
		string tmpType = GetAtomType(fs);

		if (atomSize == 0)
			break;

		long tmpPos = atomSize + fs->tellg() - 8;
		if (tmpType == ATOM_MVHD)
		{
			unsigned char buf[20] = {0};
			fs->read((char*)buf, 20);
			_media->_duration = GetInt(buf, 16) / GetInt(buf, 12);
		}
		else if (tmpType == ATOM_TRAK)
		{
			unsigned char buf[46] = {0};
			fs->read((char*)buf, 46);
			if (buf[44] == 0)
				_media->_isVideo = true;
		}
		fs->seekg(tmpPos, ios::beg);
	}
}