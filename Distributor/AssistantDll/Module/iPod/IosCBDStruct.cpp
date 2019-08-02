#include "stdafx.h"
#ifdef MODULE_IPOD

#include "IosCBDStruct.h"

extern int G_DBver;
cdbbasehead::cdbbasehead()
{
	memset(_name, 0, 5);
	_headSize = 0;
}

void cdbbasehead::Read(BinaryBufferReader* pReader,const char * pName)
{
	pReader->ReadBuffer(_name, 4);
	pReader->ReadInt32(_headSize);

	if( pName && strncmp(_name, pName, 4))
		THROW_ASSISTANT_EXCEPTION(CDB_FILE_FORMAT_ERROR);
}

void cdbbasehead::Write(BinaryBufferWriter* pWrite)
{
	pWrite->WriteBuffer(_name, 4);
	pWrite->WriteInt(_headSize);
}
CbaseMhod::CbaseMhod(int type)
: _type(type)
{
}


void CMhbd::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhbd");
	pReader->ReadInt32(_nOffsetCompress);
	pReader->ReadInt32(_nIsCompress);
	pReader->ReadInt32(_niTunesVersion);
	pReader->ReadInt32(_nItemNum);
	pReader->ReadInt64(_nItdID);
	pReader->ReadBuffer(_OSItunes,sizeof(_OSItunes));
	pReader->ReadBuffer(_unknow1,sizeof(_unknow1));
	pReader->ReadInt64(_nItunesLibID);
	pReader->ReadBuffer(_unknow2,sizeof(_unknow2));
	pReader->ReadBuffer(hashtype,sizeof(hashtype));
	pReader->ReadBuffer(_unknow3,sizeof(_unknow3));
	pReader->ReadBuffer(_language,sizeof(_language));
	pReader->ReadInt64(_pid);
	pReader->ReadInt32(_TrackFlg);
	pReader->ReadInt32(_unknowflg);
	pReader->ReadBuffer(_hash58,sizeof(_hash58));
	pReader->ReadInt32(_timeZone);
	pReader->ReadBuffer(_hashtype,sizeof(_hashtype));
	pReader->ReadBuffer(_hash72,sizeof(_hash72));
	pReader->ReadBuffer(_audiolang,sizeof(_audiolang));
	pReader->ReadBuffer(_subtitlelang,sizeof(_subtitlelang));
	pReader->ReadBuffer(_unknow4,sizeof(_unknow4));
	pReader->ReadBuffer(_hashAB,sizeof(_hashAB));
	pReader->ReadBuffer(_unknow5,sizeof(_unknow5));
	pReader->AddOffset(_panddingLen);	
}

void CMhbd::Write(BinaryBufferWriter* pWrite)
{
	 _head.Write(pWrite);
	 pWrite->WriteInt32(_nOffsetCompress);
	 pWrite->WriteInt32(_nIsCompress);
	 pWrite->WriteInt32(_niTunesVersion);
	 pWrite->WriteInt32(_nItemNum);
	 pWrite->WriteInt64(_nItdID);
	 pWrite->WriteBuffer(_OSItunes,sizeof(_OSItunes));
	 pWrite->WriteBuffer(_unknow1,sizeof(_unknow1));
	 pWrite->WriteInt64(_nItunesLibID);
	 pWrite->WriteBuffer(_unknow2,sizeof(_unknow2));
	 pWrite->WriteBuffer(hashtype,sizeof(hashtype));
	 pWrite->WriteBuffer(_unknow3,sizeof(_unknow3));
	 pWrite->WriteBuffer(_language,sizeof(_language));
	 pWrite->WriteInt64(_pid);
	 pWrite->WriteInt32(_TrackFlg);
	 pWrite->WriteInt32(_unknowflg);
	 pWrite->WriteBuffer(_hash58,sizeof(_hash58));
	 pWrite->WriteInt32(_timeZone);
	 pWrite->WriteBuffer(_hashtype,sizeof(_hashtype));
	 pWrite->WriteBuffer(_hash72,sizeof(_hash72));
	 pWrite->WriteBuffer(_audiolang,sizeof(_audiolang));
	 pWrite->WriteBuffer(_subtitlelang,sizeof(_subtitlelang));
	 pWrite->WriteBuffer(_unknow4,sizeof(_unknow4));
	 pWrite->WriteBuffer(_hashAB,sizeof(_hashAB));
	 pWrite->WriteBuffer(_unknow5,sizeof(_unknow5));
	 pWrite->WriteRepeatChar(0,_panddingLen);		
}

void CMhsd::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhsd");
	pReader->ReadInt32(_nBodylen);
	pReader->ReadInt32(_type);	
	pReader->AddOffset(_panddingLen);	
}

void CMhsd::Write(BinaryBufferWriter* pWrite)
{
	 _head.Write(pWrite);
	pWrite->WriteInt32(_nBodylen);
	pWrite->WriteInt32(_type);	
	pWrite->WriteRepeatChar(0,_panddingLen);		
}

void CMhla::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhla");
	pReader->ReadInt32(_nAlbumsNum);	//ר������
	pReader->AddOffset(_panddingLen);	
}

void CMhla::Write(BinaryBufferWriter* pWrite)
{
	 _head.Write(pWrite);
	pWrite->WriteInt32(_nAlbumsNum);	
	pWrite->WriteRepeatChar(0,_panddingLen);	
}


void CMhia::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhia");
	 pReader->ReadInt32(_nBodylen);
	pReader->ReadInt32(_nTrackNum);	//����
	pReader->ReadInt32(_nAlbumid);	//album id����iTunes�ڲ�ID
	pReader->ReadInt64(_nAlbumDBid); //Album id����sqlitedb��pid
	pReader->ReadInt32(_unknow);
	pReader->ReadBuffer(_unknow1,sizeof(_unknow1));
	pReader->ReadBuffer(_unknow2,sizeof(_unknow2));
	pReader->AddOffset(_panddingLen);	
}

void CMhia::Write(BinaryBufferWriter* pWrite)
{
	 _head.Write(pWrite);
	 pWrite->WriteInt32(_nBodylen);
	pWrite->WriteInt32(_nTrackNum);	//����
	pWrite->WriteInt32(_nAlbumid);	//album id����iTunes�ڲ�ID
	pWrite->WriteInt64(_nAlbumDBid); //Album id����sqlitedb��pid
	pWrite->WriteInt32(_unknow);
	pWrite->WriteBuffer(_unknow1,sizeof(_unknow1));
	pWrite->WriteBuffer(_unknow2,sizeof(_unknow2));
	pWrite->WriteRepeatChar(0,_panddingLen);		
}

void CMhli::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhli");
	pReader->ReadInt32(_nArtistsNum);
	pReader->AddOffset(_panddingLen);	
}

void CMhli::Write(BinaryBufferWriter* pWrite)
{
	_head.Write(pWrite);
	pWrite->WriteInt32(_nArtistsNum);	
	pWrite->WriteRepeatChar(0,_panddingLen);		
}

void CMhii::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhii");
	pReader->ReadInt32(_nBodylen);		//������
	pReader->ReadInt32(_nTrackNum);		//����
	pReader->ReadInt32(_nArtistid);		// Artist id����iTunes�ڲ�ID
	pReader->ReadInt64(_nArtistDBid);	// Artist id����sqlitedb��pid
	pReader->ReadInt32(_unknow);
	pReader->AddOffset(_panddingLen);	
}

void CMhii::Write(BinaryBufferWriter* pWrite)
{
	_head.Write(pWrite);
	pWrite->WriteInt32(_nBodylen);		//������
	pWrite->WriteInt32(_nTrackNum);		//����
	pWrite->WriteInt32(_nArtistid);		// Artist id����iTunes�ڲ�ID
	pWrite->WriteInt64(_nArtistDBid);	// Artist id����sqlitedb��pid
	pWrite->WriteInt32(_unknow);
	pWrite->WriteRepeatChar(0,_panddingLen);		
}

void CMhlt::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhlt");
	pReader->ReadInt32(_nTracksNum);
	pReader->AddOffset(_panddingLen);	
}

void CMhlt::Write(BinaryBufferWriter* pWrite)
{
	_head.Write(pWrite);
	pWrite->WriteInt32(_nTracksNum);	
	pWrite->WriteRepeatChar(0,_panddingLen);		
}
void CMhit::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhit");
	pReader->ReadInt32(_nBodylen);			//������
	pReader->ReadInt32(_nInfoNum);			//����
	pReader->ReadInt32(_ntrackid);			//album id����iTunes�ڲ�ID
	pReader->ReadInt32(_nIsHide);			//track�Ƿ�ɼ�
	pReader->ReadBuffer(_nFileType,sizeof(_nFileType));			//track���ļ����ͱ�ʾ
	pReader->ReadChar(_type1);
	pReader->ReadChar(_type2);
	pReader->ReadChar(_compilation);
	pReader->ReadChar(_rating);
	pReader->ReadInt32(_timeMac);			//MACʱ���
	pReader->ReadInt32(_size);				//��С
	pReader->ReadInt32(_longer);			//����ʱ�� ��λms
	pReader->ReadInt32(_nIdInAlbuum);		//ר���еı��
	pReader->ReadInt32(_nAlbumtrackNum);	//����ר���ĸ�������
	pReader->ReadInt32(_issueyear);			//�������
	pReader->ReadInt32(_bytespeed);			//������
	pReader->ReadInt32(_nSamplingrate);		//������
	pReader->ReadInt32(_volume);			//����
	pReader->ReadInt32(_begintime);			//��ʼʱ��
	pReader->ReadInt32(_endtime);			//����ʱ��
	pReader->ReadInt32(_teststring);		//����
	pReader->ReadInt32(_playcount);			//���Ŵ���
	pReader->ReadInt32(_playcount2);		//���Ŵ��� ���ֳ�ʼ��ֵ
	pReader->ReadInt32(_lastplaytime);		//���һ�β���ʱ��
	pReader->ReadInt32(_cdid);				//cd���
	pReader->ReadInt32(_cdNum);				//CD�ĸ���
	pReader->ReadInt32(_nDRMID);			//DRM�û�ID
	pReader->ReadInt32(_nAddTime);			//����ʱ��
	pReader->ReadInt32(_nBoolmarkTime);		//��ǩʱ��
	pReader->ReadInt64(_ndbID);
	
	pReader->ReadChar(_isSelect);				//�Ƿ�track��ѡ��
	pReader->ReadChar(_playerSpeed);			//Ӧ�ó�������
	pReader->ReadBuffer(_bpm,sizeof(_bpm));				//ÿ���ӽ�����
	pReader->ReadBuffer(_artworkcount,sizeof(_artworkcount));			//��ͼ��Ŀ
	pReader->ReadBuffer(_unknow,sizeof(_unknow));
	pReader->ReadInt32(artworksize);		//��ͼ��С
	pReader->ReadBuffer(_unknow2,sizeof(_unknow2));
	pReader->ReadInt32(_samplerate);		//������
	pReader->ReadInt32(_releasetime);		//�ͷ�ʱ��
	pReader->ReadBuffer(_unknow3,sizeof(_unknow3));
	pReader->ReadBuffer(_detailflag,sizeof(_detailflag));			//������־
	pReader->ReadBuffer(_unknow4,sizeof(_unknow4));
	pReader->ReadInt32(_skipcount);
	pReader->ReadInt32(_lastskipped);
	pReader->ReadChar(_hasartwork);
	pReader->ReadChar(_skipwhenshuffing);
	pReader->ReadChar(_rememberplaybackposition);
	pReader->ReadChar(flag4);
	pReader->ReadInt64(_dbid2);
	pReader->ReadChar(_haslyric);				//��ʱ�־
	pReader->ReadChar( _isMoving);			//��Ӱ��־
	pReader->ReadChar(_cannotplay);			//���ܲ��ű�־
	pReader->ReadChar(_unknow5);
	pReader->ReadBuffer(_unknow6,sizeof(_unknow6));
	pReader->ReadInt32(_pregap);
	pReader->ReadInt64(_usecount);		//���ø���
	pReader->ReadBuffer(_unknow7,sizeof(_unknow7));		
	pReader->ReadInt32(_postgap);
	pReader->ReadBuffer(_unknow8,sizeof(_unknow8));
	pReader->ReadInt32(_mediaType);
	pReader->ReadInt32(_seasonnr);
	pReader->ReadInt32(_episodenr);
	for(int i=0;i<7;i++)
	{
		pReader->ReadInt32(_unknowArray[i]);
	}
	pReader->ReadInt32(gaplessdata);
	pReader->ReadInt32(_unknow9);
	pReader->ReadBuffer(_gaplessTrackFlag,sizeof(_gaplessTrackFlag));
	pReader->ReadBuffer(_gaplessAlbumFlag,sizeof(_gaplessAlbumFlag));
	pReader->ReadBuffer(_keep,sizeof(_keep));
	pReader->ReadInt32(_alumbID);
	pReader->ReadInt64(_ID);
	pReader->ReadInt32(_size2);			//track��С
	pReader->ReadInt32(_unknow10);		//=0
	pReader->ReadBuffer(_unknow11,sizeof(_unknow11));		//0x808080808080LL
	pReader->ReadBuffer(_unknow12,sizeof(_unknow12));
	pReader->ReadBuffer(_unknow13,sizeof(_unknow13));
	pReader->ReadBuffer(_unknow14,sizeof(_unknow14));		//mediatypeΪbook��Ϊ0x00010001������Ϊ0
	pReader->ReadBuffer(_keep1,sizeof(_keep1));
	pReader->ReadInt32(_mhiiLink);
	pReader->ReadInt32(_unknow15);
	pReader->ReadInt32(_unknow16);
	pReader->ReadInt32(_unknow17);
	pReader->AddOffset(_ReadEnd);//��ʱ��mhit�����ƺ������
	if(pReader->PreView(4).compare("mhod") != 0)
	{
		pReader->AddOffset(_panddingLen1);	
		pReader->ReadInt32(_artistid);
		pReader->ReadBuffer(_keep2,sizeof(_keep2));
		pReader->ReadInt32(_composerId);
		pReader->MoveTo("mhod");
// 		pReader->AddOffset(_panddingLen);
// 		if(pReader->PreView(4).compare("mhod") == 0)
// 			return;
// 		pReader->AddOffset(_ReadEnd1);	
	}
	else
	{
		_artistid=0;
		_composerId=0;
		memset(_keep2,0,sizeof(_keep2));
	}

}

void CMhit::Write(BinaryBufferWriter* pWrite)
{
	_head.Write(pWrite);
	pWrite->WriteInt32(_nBodylen);			//������
	pWrite->WriteInt32(_nInfoNum);			//����
	pWrite->WriteInt32(_ntrackid);			//album id����iTunes�ڲ�ID
	pWrite->WriteInt32(_nIsHide);			//track�Ƿ�ɼ�
	pWrite->WriteBuffer(_nFileType,sizeof(_nFileType));			//track���ļ����ͱ�ʾ
	pWrite->WriteChar(_type1);
	pWrite->WriteChar(_type2);
	pWrite->WriteChar(_compilation);
	pWrite->WriteChar(_rating);
	pWrite->WriteInt32(_timeMac);			//MACʱ���
	pWrite->WriteInt32(_size);				//��С
	pWrite->WriteInt32(_longer);			//����ʱ�� ��λms
	pWrite->WriteInt32(_nIdInAlbuum);		//ר���еı��
	pWrite->WriteInt32(_nAlbumtrackNum);	//����ר���ĸ�������
	pWrite->WriteInt32(_issueyear);			//�������
	pWrite->WriteInt32(_bytespeed);			//������
	pWrite->WriteInt32(_nSamplingrate);		//������
	pWrite->WriteInt32(_volume);			//����
	pWrite->WriteInt32(_begintime);			//��ʼʱ��
	pWrite->WriteInt32(_endtime);			//����ʱ��
	pWrite->WriteInt32(_teststring);		//����
	pWrite->WriteInt32(_playcount);			//���Ŵ���
	pWrite->WriteInt32(_playcount2);		//���Ŵ��� ���ֳ�ʼ��ֵ
	pWrite->WriteInt32(_lastplaytime);		//���һ�β���ʱ��
	pWrite->WriteInt32(_cdid);				//cd���
	pWrite->WriteInt32(_cdNum);				//CD�ĸ���
	pWrite->WriteInt32(_nDRMID);			//DRM�û�ID
	pWrite->WriteInt32(_nAddTime);			//����ʱ��
	pWrite->WriteInt32(_nBoolmarkTime);		//��ǩʱ��
	pWrite->WriteInt64(_ndbID);
	pWrite->WriteChar(_isSelect);				//�Ƿ�track��ѡ��
	pWrite->WriteChar(_playerSpeed);			//Ӧ�ó�������
	pWrite->WriteBuffer(_bpm,sizeof(_bpm));				//ÿ���ӽ�����
	pWrite->WriteBuffer(_artworkcount,sizeof(_artworkcount));			//��ͼ��Ŀ
	pWrite->WriteBuffer(_unknow,sizeof(_unknow));
	pWrite->WriteInt32(artworksize);		//��ͼ��С
	pWrite->WriteBuffer(_unknow2,sizeof(_unknow2));
	pWrite->WriteInt32(_samplerate);		//������
	pWrite->WriteInt32(_releasetime);		//�ͷ�ʱ��
	pWrite->WriteBuffer(_unknow3,sizeof(_unknow3));
	pWrite->WriteBuffer(_detailflag,sizeof(_detailflag));			//������־
	pWrite->WriteBuffer(_unknow4,sizeof(_unknow4));
	pWrite->WriteInt32(_skipcount);
	pWrite->WriteInt32(_lastskipped);
	pWrite->WriteChar(_hasartwork);
	pWrite->WriteChar(_skipwhenshuffing);
	pWrite->WriteChar(_rememberplaybackposition);
	pWrite->WriteChar(flag4);
	pWrite->WriteInt64(_dbid2);
	pWrite->WriteChar(_haslyric);				//��ʱ�־
	pWrite->WriteChar( _isMoving);			//��Ӱ��־
	pWrite->WriteChar(_cannotplay);			//���ܲ��ű�־
	pWrite->WriteChar(_unknow5);
	pWrite->WriteBuffer(_unknow6,sizeof(_unknow6));
	pWrite->WriteInt32(_pregap);
	pWrite->WriteInt64(_usecount);		//���ø���
	pWrite->WriteBuffer(_unknow7,sizeof(_unknow7));		
	pWrite->WriteInt32(_postgap);
	pWrite->WriteBuffer(_unknow8,sizeof(_unknow8));
	pWrite->WriteInt32(_mediaType);	
	pWrite->WriteInt32(_seasonnr);
	pWrite->WriteInt32(_episodenr);
	for(int i=0;i<sizeof(_unknowArray)/sizeof(__int32);i++)
	{
		pWrite->WriteInt32(_unknowArray[i]);
	}
	pWrite->WriteInt32(gaplessdata);
	pWrite->WriteInt32(_unknow9);
	pWrite->WriteBuffer(_gaplessTrackFlag,sizeof(_gaplessTrackFlag));
	pWrite->WriteBuffer(_gaplessAlbumFlag,sizeof(_gaplessAlbumFlag));
	pWrite->WriteBuffer(_keep,sizeof(_keep));
	pWrite->WriteInt32(_alumbID);
	pWrite->WriteInt64(_ID);
	pWrite->WriteInt32(_size2);			//track��С
	pWrite->WriteInt32(_unknow10);		//=0
	pWrite->WriteBuffer(_unknow11,sizeof(_unknow11));		//0x808080808080LL
	pWrite->WriteBuffer(_unknow12,sizeof(_unknow12));
	pWrite->WriteBuffer(_unknow13,sizeof(_unknow13));
	pWrite->WriteBuffer(_unknow14,sizeof(_unknow14));		//mediatypeΪbook��Ϊ0x00010001������Ϊ0
	pWrite->WriteBuffer(_keep1,sizeof(_keep1));
	pWrite->WriteInt32(_mhiiLink);
	pWrite->WriteInt32(_unknow15);
	pWrite->WriteInt32(_unknow16);
	pWrite->WriteInt32(_unknow17);
	if(G_DBver==400 || G_DBver >500)
		pWrite->WriteRepeatChar(0,20);
	pWrite->WriteRepeatChar(0,_panddingLen1);	
	pWrite->WriteInt32(_artistid);
	pWrite->WriteBuffer(_keep2,sizeof(_keep2));
	pWrite->WriteInt32(_composerId);
	pWrite->WriteRepeatChar(0,_panddingLen);
	if(G_DBver==400 || G_DBver >500)
		pWrite->WriteRepeatChar(0,20);
}
void CMhlp::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhlp");
	pReader->ReadInt32(_nMhodNum);
	pReader->AddOffset(_panddingLen);	
}

void CMhlp::Write(BinaryBufferWriter* pWrite)
{
	_head.Write(pWrite);
	pWrite->WriteInt32(_nMhodNum);	
	pWrite->WriteRepeatChar(0,_panddingLen);		
}
void CMhyp::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhyp");	
	pReader->ReadInt32(_nBodylen);		//������
	pReader->ReadInt32(_nMhodNum);		//����
	pReader->ReadInt32(_nmhips);		//mhips���������Կ���tracks����
	pReader->ReadChar(_type);				//type=0 visiable��=1main
	pReader->ReadBuffer(_unknow,sizeof(_unknow));
	pReader->ReadInt32(_timeMAC);		//MACʱ���
	pReader->ReadInt64(_playlistID);
	pReader->ReadInt32(_unknow1);
	pReader->ReadBuffer(_mhodcount,sizeof(_mhodcount));	//mhod����
	pReader->ReadBuffer(_Podcastflag,sizeof(_Podcastflag));
	pReader->ReadInt32(_nSortOrder);
	pReader->ReadBuffer(_unknow2,sizeof(_unknow2));
	pReader->ReadBuffer(_unknow3,sizeof(_unknow3));
	pReader->MoveTo("mhod");
	//pReader->AddOffset(_head._headSize - 96);
	//pReader->AddOffset(_panddingLen);	
	//pReader->AddOffset(_panddingLen1);	

}

void CMhyp::Write(BinaryBufferWriter* pWrite)
{
	_head.Write(pWrite);
	pWrite->WriteInt32(_nBodylen);		//������
	pWrite->WriteInt32(_nMhodNum);		//����
	pWrite->WriteInt32(_nmhips);		//mhips���������Կ���tracks����
	pWrite->WriteChar(_type);				//type=0 visiable��=1main
	pWrite->WriteBuffer(_unknow,sizeof(_unknow));
	pWrite->WriteInt32(_timeMAC);		//MACʱ���
	pWrite->WriteInt64(_playlistID);
	pWrite->WriteInt32(_unknow1);
	pWrite->WriteBuffer(_mhodcount,sizeof(_mhodcount));	//mhod����
	pWrite->WriteBuffer(_Podcastflag,sizeof(_Podcastflag));
	pWrite->WriteInt32(_nSortOrder);
	pWrite->WriteBuffer(_unknow2,sizeof(_unknow2));
	pWrite->WriteBuffer(_unknow3,sizeof(_unknow3));
	pWrite->WriteRepeatChar(0,_panddingLen);		
	pWrite->WriteRepeatChar(0,_panddingLen1);		
}

void CMhip::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhip");
	pReader->ReadInt32(_nBodylen);		//������
	pReader->ReadInt32(_nMhodNum);		//����
	pReader->ReadInt32(_Podcastgroupflag);
	pReader->ReadInt32(_Podcastgroupid);
	pReader->ReadInt32(_Trackid);
	pReader->ReadInt32(_timeMAC);		//MACʱ���
	pReader->ReadInt32(_Podcastgroupref);
	pReader->ReadBuffer(_unknow,sizeof(_unknow));
	pReader->ReadBuffer(_unknow1,sizeof(_unknow1));
	pReader->AddOffset(_panddingLen);	
}

void CMhip::Write(BinaryBufferWriter* pWrite)
{
	_head.Write(pWrite);
	pWrite->WriteInt32(_nBodylen);		//������
	pWrite->WriteInt32(_nMhodNum);		//����
	pWrite->WriteInt32(_Podcastgroupflag);
	pWrite->WriteInt32(_Podcastgroupid);
	pWrite->WriteInt32(_Trackid);
	pWrite->WriteInt32(_timeMAC);		//MACʱ���
	pWrite->WriteInt32(_Podcastgroupref);
	pWrite->WriteBuffer(_unknow,sizeof(_unknow));
	pWrite->WriteBuffer(_unknow1,sizeof(_unknow1));
	pWrite->WriteRepeatChar(0,_panddingLen);		
}

CMhod::CMhod()
{
	_pbody = NULL;
}
CMhod::~CMhod()
{
	SAFE_DELETE(_pbody);
}

cdbBase::cdbBase()
{

}

void CMhod::Read(BinaryBufferReader* pReader)
{
	_head.Read(pReader,"mhod");
	pReader->ReadInt32(_nBodylen);		//������
	pReader->ReadInt32(_type);			//����
	pReader->AddOffset(_panddingLen);	
	char *body = (char*)malloc(_nBodylen - _head._headSize);
	pReader->ReadBuffer(body,_nBodylen - _head._headSize);
	BinaryBufferReader ReaderBody(body,_nBodylen);	
	//if(_type==100)
	//{
//		_pbody = new CMhodBodyPLAYLIST(_type);
//	}
//	else if(_type==102)
//	{
//		_pbody = new CMhodBody102(_type);
//	}
	//if(_type == 15 || _type == 16  || _type == 17 || _type == 32 || _type == 50|| _type == 51|| _type == 52|| _type == 100|| _type == 102)
	if(_type<15 || (_type>17 && _type <32) || _type>=200)
	{
		_pbody = new CMhodBody(_type);
		_pbody->Read(&ReaderBody);	
	//	string filename("d:\\iTunesPackUnCompress");
	//	filename += _type;
	//	ofstream ofstemp(filename.c_str(), ios_base::out|ios_base::binary);
	//	ofstemp.write(_pbody->GetInfo(), _pbody->GetInfo().size());
	//	ofstemp.close();

	}
	else
	{
		_pbody = new CMhNodyUnknow(_type,_nBodylen - _head._headSize);
		_pbody->Read(&ReaderBody);		
	}
	
	free(body);

}

void CMhod::Write(BinaryBufferWriter* pWrite)
{
	_head.Write(pWrite);
	pWrite->WriteInt32(_nBodylen);		//������
	pWrite->WriteInt32(_type);			//����
	pWrite->WriteRepeatChar(0,_panddingLen);		
	_pbody->Write(pWrite);
}

CMhodBody::CMhodBody(int type)
: CbaseMhod(type)
{
}
CMhodBody::~CMhodBody()
{

}
void CMhodBody::Read(BinaryBufferReader* pReader)
{
	pReader->ReadInt32(_Code);			//�ַ������ͣ�=1��ʾ�ַ���UTF16���ͣ�=2��ʾUTF8����
	pReader->ReadInt32(_size);			//�ַ�������
	pReader->ReadInt32( _unknow);
	pReader->ReadInt32( _unknow1);	
	string str;
	pReader->ReadString(str,_size);		//Body����
	if(_Code == 2)
		_value = CCodeOperation::UTF_8ToUnicode(str);
	else
		_value.assign((wchar_t*) str.c_str(), _size/2);	
	
}
void CMhodBody::Write(BinaryBufferWriter* pWrite)
{
	pWrite->WriteInt32(_Code);			//�ַ������ͣ�=1��ʾ�ַ���UTF16���ͣ�=2��ʾUTF8����
	pWrite->WriteInt32(_size);			//�ַ�������
	pWrite->WriteInt32( _unknow);
	pWrite->WriteInt32( _unknow1);
	string s;
	if ( _Code == 2 )   // UTF-8 
	{;
	s = CCodeOperation::UnicodeToUTF_8(_value);
	}
	else if ( _Code == 1 )   // UTF-16
	{
		s.assign((char*) _value.c_str(), _value.size() * 2);
	}
	pWrite->WriteBuffer(s.c_str(),_size);		//Body����

}
 CMhodBodyPLAYLIST::CMhodBodyPLAYLIST(int type)
:CbaseMhod(type)
{
}
 CMhodBodyPLAYLIST::~CMhodBodyPLAYLIST()
 {
 }
void CMhodBodyPLAYLIST::Read(BinaryBufferReader* pReader)
{
	pReader->ReadBuffer(_unknow,sizeof(_unknow));
	pReader->ReadBuffer( _unknow1 ,sizeof(_unknow1));
	pReader->ReadBuffer( _unknow2 ,sizeof(_unknow2));
	pReader->ReadBuffer( _unknow3 ,sizeof(_unknow3));
	pReader->ReadBuffer( _unknow4 ,sizeof(_unknow4));
	pReader->AddOffset(_panddingLen);	
	pReader->ReadBuffer( _unknow5,sizeof(_unknow5) );
	pReader->AddOffset(_panddingLen1);	
}
void CMhodBodyPLAYLIST::Write(BinaryBufferWriter* pWrite)
{
	pWrite->WriteBuffer(_unknow,sizeof(_unknow));
	pWrite->WriteBuffer( _unknow1 ,sizeof(_unknow1));
	pWrite->WriteBuffer( _unknow2 ,sizeof(_unknow2));
	pWrite->WriteBuffer( _unknow3,sizeof(_unknow3) );
	pWrite->WriteBuffer( _unknow4 ,sizeof(_unknow4));
	pWrite->WriteRepeatChar(0,_panddingLen);		
	pWrite->WriteBuffer( _unknow5,sizeof(_unknow5) );
	pWrite->WriteRepeatChar(0,_panddingLen1);		
}
CMhodBody102::CMhodBody102(int type)
:CbaseMhod(type)
{
}
CMhodBody102::~CMhodBody102()
{

}
void CMhodBody102::Read(BinaryBufferReader* pReader)
{
	pReader->ReadInt32(_unknow);
	pReader->ReadInt32(_unknow1);
	pReader->ReadInt32(_unknow2);
	pReader->ReadInt32(_unknow3);
	pReader->AddOffset(_panddingLen);	
}
void CMhodBody102::Write(BinaryBufferWriter* pWrite)
{
	pWrite->WriteInt32(_unknow);
	pWrite->WriteInt32(_unknow1);
	pWrite->WriteInt32(_unknow2);
	pWrite->WriteInt32(_unknow3);
	pWrite->WriteRepeatChar(0,_panddingLen);		
}
CMhuk::CMhuk(int nvalue)
{
   _nBodylen = nvalue;
   temp = NULL;
}
CMhuk::~CMhuk()
{
	if(temp)
	{
		free(temp);
		temp = NULL;
	}
}
void CMhuk::Read(BinaryBufferReader* pReader)
{
  temp = (char*)malloc(_nBodylen);
  pReader->ReadBuffer(temp,_nBodylen);

}
void CMhuk::Write(BinaryBufferWriter* pWrite)
{
	if(temp)
		pWrite->WriteBuffer(temp,_nBodylen);
	else
		pWrite->WriteRepeatChar(0,_nBodylen);
}

CMhNodyUnknow::CMhNodyUnknow(int type,int nLen)
:CbaseMhod(type)
{
	BodyLen = nLen;
	pBody=NULL;
}
CMhNodyUnknow::~CMhNodyUnknow()
{
	if(pBody)
	{
		free(pBody);
		pBody = NULL;
	}
}

void CMhNodyUnknow::Read(BinaryBufferReader* pReader)
{
	pBody = (char*)malloc(BodyLen);
	pReader->ReadBuffer(pBody,BodyLen);

}
void CMhNodyUnknow::Write(BinaryBufferWriter* pWrite)
{
	if(pBody)
		pWrite->WriteBuffer(pBody,BodyLen);
	else
		pWrite->WriteRepeatChar(0,BodyLen);
}

void CMhit::FillNewStruct()
{
	memcpy(_head._name,"mhit",4);
	_head._headSize = 584;
	_nIsHide = 0;	
	_type1 = '0';
	_type2 = '0';
	_compilation = '0';
	_rating = '0';	
	_playcount=0;		
	_playcount2=0;		
	_lastplaytime=0;
	_isSelect='0';
	_playerSpeed='0';	
	ZeroMemory(_unknow,sizeof(_unknow));	
	ZeroMemory(_unknow2,sizeof(_unknow2));	
	ZeroMemory(_unknow3,sizeof(_unknow3));
	ZeroMemory(_unknow4,sizeof(_unknow4));
	_skipcount=0;				
	_lastskipped=0;	
	_skipwhenshuffing='0';
	_rememberplaybackposition='0';	
	_cannotplay = '0';			
	_unknow5 = '0';				
	ZeroMemory(_unknow6,sizeof(_unknow6));
	ZeroMemory(_unknow7,sizeof(_unknow7));		
	ZeroMemory(_unknow8,sizeof(_unknow8));	
	for(int i=0 ;i<7;i++)
	{	
		_unknowArray[i]=0;
	}	
	_unknow9=0;
	ZeroMemory(_keep,sizeof(_keep));
	_unknow10=0;		
	__int64 UNKONW11;
	UNKONW11 = 0x808080808080LL;
	memcpy(_unknow11,&UNKONW11,8);		//0x808080808080LL 
	ZeroMemory(_unknow12,sizeof(_unknow12));
	ZeroMemory(_unknow13,sizeof(_unknow13));
	ZeroMemory(_keep1,sizeof(_keep1));
	_unknow15=0;
	_unknow16=1;
	_unknow17=0;	
	ZeroMemory(_keep2,sizeof(_keep2));	
	ZeroMemory(_unknow14,sizeof(_unknow14));		//mediatypeΪbook��Ϊ0x00010001������Ϊ0 331	
	_begintime=0;			//��ʼʱ��72
	_endtime=0;			//����ʱ��76
	_teststring=0;		//����80
	_isMoving=0;			//��Ӱ��־
	_pregap=0;			
	_usecount=0;		//���ø���195
	_postgap=0;	
	_seasonnr=0;
	_episodenr=0;
	gaplessdata=0;
	_samplerate=0;		//������//139
	_releasetime=0;		//�ͷ�ʱ��143
	_ID=0;
	_nDRMID=0;										//DRM�û�ID104
	_nAddTime=0;										//����ʱ��108
	_nBoolmarkTime=0;
	_nIsHide = 0;
	_mhiiLink=0;	
}

void CMhbd::release(){return;}
void CMhuk::release()
{
	if(temp)
	{
		free(temp);
		temp = NULL;
	}
}
void CMhsd::release(){return;}

void CMhia::release(){return;}
void CMhla::release(){return;}
void CMhli::release(){return;}
void CMhii::release(){return;}
void CMhlt::release(){return;}
void CMhit::release(){return;}
void CMhlp::release(){return;}
void CMhyp::release(){return;}
void CMhip::release(){return;}

void CMhod::release(){
	_pbody->release();
	SAFE_DELETE(_pbody);
}
cdbBase::~cdbBase()
{
	//release();
}

void CMhNodyUnknow::release()
{
	if(pBody)
	{
		free(pBody);
		pBody = NULL;
	}
}
void CMhodBody102::release(){return;}
void CMhodBodyPLAYLIST::release(){return;}
void CMhodBody::release(){return;}


#endif