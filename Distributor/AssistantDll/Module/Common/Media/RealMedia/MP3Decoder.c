/**************************************************************************/
/*                                                                        */
/* Copyright (c) 1999-2011 NetDragon Websoft Inc.                         */
/*                    �����������޹�˾��Ȩ���� 1999-2011                  */
/*                                                                        */
/*  PROPRIETARY RIGHTS of NetDragon Websoft Inc. are involved in the      */
/*  subject matter of this material. All manufacturing, reproduction,     */
/*  use, and sales rights pertaining to this subject matter are governed  */
/*  by the license agreement.  The recipient of this software implicitly  */ 
/*  accepts the terms of the license.                                     */
/*  ������ĵ������������������޹�˾��Ȩ���еĺϷ��ʲ����κ���ʿ�Ķ���ʹ  */
/*  �ñ����ϱ�������Ӧ��������Ȩ,�е��������κͽ�����Ӧ�ķ���Լ����     */
/*                                                                        */
/**************************************************************************/

/*
	֧��ID3 2.3.0Э�飬ID3 2.4.0Э�飬�ο�
	http://www.id3.org/id3v2.4.0-structure
	http://www.id3.org/id3v2.3.0
 */

#include "Module/Common/Media/Utils/iTunesMP3Lib_Type.h"
#include "MP3Decoder.h"
//#include <string>
#include <stdio.h>
#include <assert.h>

typedef struct __mp3_info MP3Info;

//ID3Э��TAG��־����
#define ID3_TAG_SIZE				3

//ID3Э��TAG�ַ���
#define ID3_2_TAGSTR				((ND_UCHAR*)"ID3")
//ID3Э��TAGͷ����
#define ID3_2_HEAD_SIZE				10

//ID3��LABEL TAG��־����
#define ID3_2_LABEL_TAG_SIZE		4
//ID32��Frameͷ����
#define ID3_2_FRAME_HEAD_SIZE		10

//MP3֡ͷ����
#define MP3_HEAD_SIZE				4

#define MAKEVER( ver, major, revision )					((((ND_UINT32)(ver&0xff))<<24)|(((ND_UINT32)(major&0xfff))<<12)|((ND_UINT32)(revision&0xfff)))
#define GETSAFELEN( str )		((((ND_UINT32)((str)[0]&0x7f))<<21)|(((ND_UINT32)((str)[1]&0x7f))<<14)|(((ND_UINT32)((str)[2]&0x7f))<<7)|(((ND_UINT32)((str)[3]&0x7f))))
#define GETLEN( str )			((((ND_UINT32)((str)[0]))<<24)|(((ND_UINT32)((str)[1]))<<16)|(((ND_UINT32)((str)[2]))<<8)|(((ND_UINT32)((str)[3]))))

#define MAKEMP3FRAMEHEAD( str )			((((ND_UINT32)((str)[0]))<<24)|(((ND_UINT32)((str)[1]))<<16)|(((ND_UINT32)((str)[2]))<<8)|(((ND_UINT32)((str)[3]))))

#define SETSAFELEN( iV32 )		(((iV32)&0x3f)<<24)|((((iV32)>>7)&0x3f)<<16)|((((iV32)>>14)&0x3f)<<8)|((((iV32)>>21)&0x3f))
#define SETLEN( iV32 )			(((iV32)&0xff)<<24)|((((iV32)>>8)&0xff)<<16)|((((iV32)>>16)&0xff)<<8)|((((iV32)>>24)&0xff))

#define BLOCKSIZE				4096

static const struct
{
	const char * const m_pName;
	const char * const m_pNo;
}_g_tcon[]=
{
	{ "Classic Rock", "1" }, { "Country", "2" }, { "Dance", "3" }, { "Disco", "4" }, {"Funk","5"}, {"Grunge","6"}, {"Hip-Hop","7"}, {"Jazz","8"}, {"Metal","9"}, {"New Age","10"},
	{ "Oldies", "11" }, { "Other", "12" }, { "Pop", "13" }, { "R&B", "14" }, {"Rap","15"}, {"Reggae","16"}, {"Rock","17"}, {"Techno","18"}, {"Industrial","19"}, {"Alternative","20"},
	{ "Ska", "21" }, { "Death Metal", "22" }, { "Pranks", "23" }, { "Soundtrack", "24" }, {"Euro-Techno","25"}, {"Ambient","26"}, {"Trip-Hop","27"}, {"Vocal","28"}, {"Jazz+Funk","29"}, {"Fusion","30"},
	{ "Trance", "31" }, { "Classical", "32" }, { "Instrumental", "33" }, { "Acid", "34" }, {"House","35"}, {"Game","36"}, {"Sound Clip","37"}, {"Gospel","38"}, {"Noise","39"}, {"AlternRock","40"},
	{ "Electronic", "52" }, { "Folk", "80" }, { "Easy Listening", "98" }
};

//Frame��Ϣ
typedef struct
{
	ND_UINT32 m_iSize;
	ND_UCHAR m_szTag[4];	
	ND_SIZE m_iFrameSize;
	struct
	{
		ND_UINT32 m_iTagAlterPreservation : 1;
		ND_UINT32 m_iFileAlterPreservation : 1;
		ND_UINT32 m_iReadOnly : 1;
		ND_UINT32 m_iGroupingIdentity : 1;
		ND_UINT32 m_iCompression : 1;
		ND_UINT32 m_iEncryption : 1;
		ND_UINT32 m_iUnsynchronisation : 1;
		ND_UINT32 m_iDataLengthIndicator : 1;
		ND_UINT32 m_iIsBlock : 1;
	};
	ND_UCHAR m_szData[4];
}ID3Frame;
typedef ID3Frame *		ID3FramePtr;

//ID3����Ϣ
typedef struct
{	
	ND_UINT32 m_iVer;

	//Pack
	struct
	{
		ND_UINT32 m_iUnsynchronisation : 1;				//
		ND_UINT32 m_bExtendedHeader : 1;				//�Ƿ������չͷ
		ND_UINT32 m_bExperimentalIndicator : 1;			//�Ƿ�����������
		ND_UINT32 m_bFooterPresent : 1;					//ҳ������
	}m_Head;
	ND_UINT32 m_iPackSize;

	//ExtendHead
	struct
	{
		//��չͷ��־ֻ��һ���ֽ�
		ND_UINT32 m_bTagIsAnUpdate : 1;
		ND_UINT32 m_bCRCDataPresent : 1;				//CRC-32 [ISO-3309]
		ND_UINT32 m_bTagRestrictions : 1;

		ND_UINT32 m_iTagSizeRestrictions : 2;
		ND_UINT32 m_iTextEncodingRestrictions : 1;
		ND_UINT32 m_iTextFieldsSizeRestrictions : 2;
		ND_UINT32 m_iImageEncodingRestrictions : 1;
		ND_UINT32 m_iImageSizeRestrictions : 2;

	}m_ExtendHead;
	ND_UCHAR m_szCRC[35];
	ND_UINT32 m_iExtendHeadSize;
	
	//Frame
	ND_UINT32 m_iFrameNum;
	ID3FramePtr * m_pFrames;
	
}ID3Pack;
typedef ID3Pack *		ID3PackPtr;

//ID3������Ϣ
typedef struct
{	
	ND_UINT32 m_ID3Num;						//һ���ļ����ܰ������ID3����Ϣ
	ID3PackPtr * m_pPacks;
	MP3Info * m_pOwner;
}ID3Info;

#define AUDIO_STEREO  0
#define AUDIO_JSTEREO 1
#define AUDIO_DUAL    2
#define AUDIO_MONO    3

//MP3 Audio������Ϣ
typedef struct
{
	//ff_mpa_decode_header
	struct
	{
		ND_UINT32 m_iCannel : 5;				//��Ƶͨ��		
		ND_UINT32 m_iLayer : 3;					//MPEG Audio Layer
		ND_UINT32 m_iLSF : 2;					//����Ƶ��
		ND_UINT32 m_iMpeg25 : 2;				//�Ƿ�MPEG2.5
		ND_UINT32 m_iPadding : 1;				//�Ƿ�Pad
	};

	ND_UINT32 m_iPlayTime;				//ʱ��(ms)
	ND_UINT32 m_iSampleRate;			//��������
	ND_UINT32 m_iBitSpeed;				//λ��
	ND_UINT32 m_iFrameSize;				//֡��С

}MP3AudioInfo;

//MPEG AUDIO Ƶ�ʱ�
static const ND_UINT16 __audio_frequency_table[3] = { 44100, 48000, 32000 };

//MPEG AUDIO λ�ٱ�
static const ND_UINT16 __audio_bitrate_table[2][3][15] = 
{
    { 
		{ 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448 },
		{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384 },
		{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 } 
	},

    {
		{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256 },
		{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 },
		{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 }
    }
};

//MP3Info
typedef struct __mp3_info
{
	ND_CHAR m_szFilePath[ND_PATH_MAXSIZE];
	ND_CHAR m_szNewFilePath[ND_PATH_MAXSIZE];
	ID3Info * m_pID3Info;
	MP3AudioInfo m_AudioInfo;
}MP3Info;

//#################################################################################################
//##################################### �ڲ�������ʼ ##############################################
//#################################################################################################

//TCON����NO����ȡName
static ND_UCHAR * __tcon_get_name_by_no( const ND_UCHAR * pszNo )
{
	ND_UINT32 i;

	for( i = 0; i < sizeof(_g_tcon)/sizeof(_g_tcon[0]); i++ )
	{
		if( strcmp( pszNo, _g_tcon[i].m_pNo ) == 0 )
			return _g_tcon[i].m_pName;
	}
	return NULL;
}

//TCON����Name����ȡNO
static ND_UCHAR * __tcon_get_no_by_name( const ND_UCHAR * pszNo )
{
	ND_UINT32 i;

	for( i = 0; i < sizeof(_g_tcon)/sizeof(_g_tcon[0]); i++ )
	{
		if( strcmp( pszNo, _g_tcon[i].m_pName ) == 0 )
			return _g_tcon[i].m_pNo;
	}
	return NULL;
}

//����padding
static void __skip_padding( FILE * pMP3File, ID3Info * pID3Info )
{
	ND_UINT32 iOffset = 0;
	ND_UINT32 i;

	for( i = 0; i < pID3Info->m_ID3Num; i++ )
		iOffset += pID3Info->m_pPacks[i]->m_iPackSize + ID3_2_HEAD_SIZE;
	fseek( pMP3File, iOffset, SEEK_SET );
}

//����ID3ͷ������ID3�汾��
static ND_UINT32 __parse_id3_head( FILE * pFile )
{
	ND_UCHAR szTAG[ID3_TAG_SIZE+1];
	ND_INT	iSize;
	ND_UINT32 iVer;
	
	assert( pFile );

	//��ȡTAG�ַ���
	fseek( pFile, 0, SEEK_SET );
	if( ( iSize = fread( szTAG, ID3_TAG_SIZE, 1, pFile ) ) != 1 )
		return 0;
	szTAG[ID3_TAG_SIZE] = 0;
	
	//�ж�ͷ
	if( strncmp( szTAG, ID3_2_TAGSTR, ID3_TAG_SIZE ) == 0 )
	{
		//����ID3_2�汾
		ND_UCHAR szID32Head[ID3_2_HEAD_SIZE-ID3_TAG_SIZE];
		if( ( iSize = fread( szID32Head, ID3_2_HEAD_SIZE-ID3_TAG_SIZE, 1, pFile ) ) != 1 )
			return 0;

		iVer = MAKEVER( 2, szID32Head[0], szID32Head[1] );

		return iVer;
	}
	return 0;
}

//��ȡMP3�ļ�ID3Э��汾
static ND_UINT32 __get_ID3_ver( const ND_CHAR * pszMP3FilePath )
{
	FILE * fp = NULL;
	ENUM_ID3VER ver;
	if (fopen_s(&fp, pszMP3FilePath, "rb" )!=0 || fp==NULL)
		return 0;

	//�����ļ�ͷ
	ver = __parse_id3_head( fp );

	fclose( fp );

	return (ND_UINT32)ver;
}

//��ȡID3ͷ��Ϣ
static ND_UINT32 __parse_id3_head_info( FILE * pFile, ID3Info * pInfo )
{
	ND_UCHAR szTAG[ID3_TAG_SIZE+1];
	ND_INT	iSize;
		
	assert( pFile );

	//��ȡTAG�ַ���
	if( ( iSize = fread( szTAG, ID3_TAG_SIZE, 1, pFile ) ) != 1 )
		return 0;
	szTAG[ID3_TAG_SIZE] = 0;
	
	//�ж�ͷ
	if( strncmp( szTAG, ID3_2_TAGSTR, ID3_TAG_SIZE ) == 0 )
	{
		//����ID3_2�汾�����ȴ���һ��ID3Pack����
		ID3Pack * pPack = NULL;
		ND_UCHAR szID32Head[ID3_2_HEAD_SIZE-ID3_TAG_SIZE];

		//����ID3Info.m_pPacks
		if( pInfo->m_ID3Num == 0 )
		{
			pInfo->m_pPacks = (ID3PackPtr*)malloc(sizeof(ID3PackPtr));
		}
		else
		{
			pInfo->m_pPacks = (ID3PackPtr*)realloc( pInfo->m_pPacks, sizeof(ID3PackPtr)*(pInfo->m_ID3Num+1) );
		}
		if( !(pInfo->m_pPacks) )
			return 0;

		pPack = (ID3Pack*)malloc(sizeof(ID3Pack));
		if( !pPack )
		{			
			return 0;
		}
		memset( pPack, 0, sizeof(ID3Pack) );
		if( ( iSize = fread( szID32Head, ID3_2_HEAD_SIZE-ID3_TAG_SIZE, 1, pFile ) ) != 1 )
			return 0;

		pPack->m_iVer = MAKEVER( 2, szID32Head[0], szID32Head[1] );
		switch( pPack->m_iVer )
		{
		case ENUM_ID3VER_2_4_0:
			if( szID32Head[2] & 0x10 )
				pPack->m_Head.m_bFooterPresent = 1;
		case ENUM_ID3VER_2_3_0:
			if( szID32Head[2] & 0x80 )
				pPack->m_Head.m_iUnsynchronisation = 1;
			if( szID32Head[2] & 0x40 )
				pPack->m_Head.m_bExtendedHeader = 1;
			if( szID32Head[2] & 0x20 )
				pPack->m_Head.m_bExperimentalIndicator = 1;
			break;
		}
		pPack->m_iPackSize = GETSAFELEN(&(szID32Head[3]));

		if( pPack->m_Head.m_bExtendedHeader )
		{
			//����ExtentedHeader��Ϣ����ʱ������
		}

		pInfo->m_pPacks[pInfo->m_ID3Num] = pPack;
		pInfo->m_ID3Num++;
		
		return pPack->m_iVer;
	}
	return 0;
}

//��ȡ֡��Ϣ
static HRESULT __parse_id3_frame_info( FILE * pMP3File, ID3Info * pID3Info )
{
	HRESULT hr;
	ND_UCHAR szFH[ID3_2_FRAME_HEAD_SIZE];
	ID3PackPtr pCurPack;
	ND_UINT32 iSize;
	ND_UINT32 iFrameSize;
	ND_UINT64 iOffset=0;
	ID3Frame * pFrame;
	ND_BOOL isBlock = FALSE;

	//��ȡ�ĸ�TAG
	if( ( hr = fread( szFH, ID3_2_FRAME_HEAD_SIZE, 1, pMP3File ) ) != 1 )
		return -1;
	if( !( szFH[0] >= 'A' && szFH[0] <= 'Z' ) )
	{
		fseek( pMP3File, 0-ID3_2_FRAME_HEAD_SIZE, SEEK_CUR );
		return 1;
	}

	//����m_pFrames
	pCurPack = pID3Info->m_pPacks[pID3Info->m_ID3Num-1];
	assert( pCurPack );
	if( pCurPack->m_iFrameNum == 0 )
	{
		pCurPack->m_pFrames = (ID3FramePtr*)malloc( sizeof(ID3FramePtr) );
	}
	else
	{
		pCurPack->m_pFrames = (ID3FramePtr*)realloc( pCurPack->m_pFrames, sizeof(ID3FramePtr)*(pCurPack->m_iFrameNum+1) );
	}
	if( !pCurPack->m_pFrames )
		return -3;

	//��ȡ֡������Frame���ݴ��USLT��APIC�ֶΣ����ô洢�ļ�ƫ�����ķ�ʽ����
	switch( pCurPack->m_iVer )
	{
	case ENUM_ID3VER_2_4_0:		iFrameSize = GETSAFELEN(&(szFH[4]));	break;
	case ENUM_ID3VER_2_3_0:		iFrameSize = GETLEN(&(szFH[4]));	break;
	default: return -4;
	}
	iSize = iFrameSize;
	if( strncmp( szFH, "USLT", 4 ) == 0 ||
		strncmp( szFH, "APIC", 4 ) == 0 )
	{
		//��ʺ�Cover�����ļ����ܴ�Ϊ�˲������ڴ棬ֱ�Ӵ��ļ�ƫ����
		iOffset = _ftelli64( pMP3File );
		iSize = sizeof(iOffset);
		isBlock = TRUE;
	}
	pFrame = (ID3Frame*)malloc(sizeof(ID3Frame)+iSize);
	if( !pFrame )
		return -5;
	memset( pFrame, 0, sizeof(ID3Frame)+iSize );

	pFrame->m_iSize = sizeof(ID3Frame)+iSize;
	if( isBlock )
		pFrame->m_iIsBlock = 1;
	memcpy( pFrame->m_szTag, szFH, 4 );
	pFrame->m_iFrameSize = iFrameSize;
	switch( pCurPack->m_iVer )
	{
	case ENUM_ID3VER_2_4_0:
		if( szFH[8] & 0x40 )
			pFrame->m_iTagAlterPreservation = 1;
		if( szFH[8] & 0x20 )
			pFrame->m_iFileAlterPreservation = 1;
		if( szFH[8] & 0x10 )
			pFrame->m_iReadOnly = 1;
		if( szFH[9] & 0x40 )
			pFrame->m_iGroupingIdentity = 1;
		if( szFH[9] & 0x08 )
			pFrame->m_iCompression = 1;
		if( szFH[9] & 0x04 )
			pFrame->m_iEncryption = 1;
		if( szFH[9] & 0x02 )
			pFrame->m_iUnsynchronisation = 1;
		if( szFH[9] & 0x01 )
			pFrame->m_iDataLengthIndicator = 1;
		break;
	case ENUM_ID3VER_2_3_0:
		if( szFH[8] & 0x80 )
			pFrame->m_iTagAlterPreservation = 1;
		if( szFH[8] & 0x40 )
			pFrame->m_iFileAlterPreservation = 1;
		if( szFH[8] & 0x20 )
			pFrame->m_iReadOnly = 1;
		if( szFH[9] & 0x80 )
			pFrame->m_iCompression = 1;
		if( szFH[9] & 0x40 )
			pFrame->m_iEncryption = 1;
		if( szFH[9] & 0x20 )
			pFrame->m_iGroupingIdentity = 1;
		break;
	}
	if( iOffset )
	{
		memcpy( pFrame->m_szData, &iOffset, sizeof(iOffset) );
		fseek( pMP3File, pFrame->m_iFrameSize, SEEK_CUR );
	}
	else
	{
		if( ( hr = fread( pFrame->m_szData, pFrame->m_iFrameSize, 1, pMP3File ) ) != 1 )
		{
			free( pFrame );
			return -6;
		}
	}
	pCurPack->m_pFrames[pCurPack->m_iFrameNum] = pFrame;
	pCurPack->m_iFrameNum++;

	return S_OK;
}

//����һ��ID3Info����
static ID3Info * __create_ID3_info( void )
{
	ID3Info * p = (ID3Info*)malloc(sizeof(ID3Info));
	if( p )
	{
		memset( p, 0, sizeof(ID3Info) );
	}
	return p;
}

//�ͷ�һ��ID3Info����
static void __release_ID3_info( ID3Info * pID3Info )
{
	ND_UINT i,k;

	if( pID3Info )
	{
		for( i = 0; i < pID3Info->m_ID3Num; i++ )
		{
			for( k = 0; k < pID3Info->m_pPacks[i]->m_iFrameNum; k++ )
			{
				if( pID3Info->m_pPacks[i]->m_pFrames[k] )
					free( pID3Info->m_pPacks[i]->m_pFrames[k] );
			}
			if( pID3Info->m_pPacks[i]->m_pFrames )
				free( pID3Info->m_pPacks[i]->m_pFrames );
		}
		if( pID3Info->m_pPacks )
			free( pID3Info->m_pPacks );
	}
}

//���MP3֡ͷ�Ƿ���Ч
static ND_INT __mp3_check_header( ND_UINT32 iHead )
{
    if( ( iHead & 0xffe00000 ) != 0xffe00000 )
        return -1;
    /* layer check */
    if( ( iHead & (3<<17) ) == 0 )
        return -1;
    /* bit rate */
    if( ( iHead & (0xf<<12)) == 0xf<<12 )
        return -1;
    /* frequency */
    if( ( iHead & (3<<10) ) == 3<<10 )
        return -1;
    return 0;
}

//����MP3֡ͷ
static ND_INT __mpegaudio_decode_header( MP3AudioInfo * pMP3AudioInfo, ND_UINT32 iHeader )
{
	ND_UINT32 iSize;
	ND_UINT32 iIndex;

	//����Layer
	pMP3AudioInfo->m_iLayer = 4 - ( ( iHeader >> 17 ) & 3 );
	
	//����������
	if( iHeader & (1<<20) )
	{
		pMP3AudioInfo->m_iLSF = ( iHeader & (1<<19) ) ? 0 : 1;
		pMP3AudioInfo->m_iMpeg25 = 0;
    }
	else
	{
        pMP3AudioInfo->m_iLSF = 1;
        pMP3AudioInfo->m_iMpeg25 = 1;
    }
	pMP3AudioInfo->m_iSampleRate = __audio_frequency_table[((iHeader>>10)&3)] >> (pMP3AudioInfo->m_iLSF+pMP3AudioInfo->m_iMpeg25);

	//����λ��
	iIndex = ( iHeader >> 12) & 0xf;
	pMP3AudioInfo->m_iPadding = ( iHeader >> 9 ) & 1;
	if( iIndex != 0 )
	{
		iSize = __audio_bitrate_table[pMP3AudioInfo->m_iLSF][pMP3AudioInfo->m_iLayer-1][iIndex];		
		pMP3AudioInfo->m_iBitSpeed = iSize * 1000;
		switch( pMP3AudioInfo->m_iLayer )
		{
		case 1:
			pMP3AudioInfo->m_iFrameSize = ( iSize * 12000 ) / pMP3AudioInfo->m_iSampleRate;
            pMP3AudioInfo->m_iFrameSize = ( pMP3AudioInfo->m_iFrameSize + pMP3AudioInfo->m_iPadding ) * 4;
            break;
		case 2:
			pMP3AudioInfo->m_iFrameSize = ( iSize * 144000 ) / pMP3AudioInfo->m_iSampleRate;
            pMP3AudioInfo->m_iFrameSize += pMP3AudioInfo->m_iPadding;
			break;
		case 3:
		default:
			pMP3AudioInfo->m_iFrameSize = ( iSize * 144000 ) / ( pMP3AudioInfo->m_iSampleRate << pMP3AudioInfo->m_iLSF );
            pMP3AudioInfo->m_iFrameSize += pMP3AudioInfo->m_iPadding;
			break;
		}
	}

	//����
	iIndex = ( iHeader >> 6) & 3;
	pMP3AudioInfo->m_iCannel = iIndex;

    return 0;
}

//����MP3ʱ��
static void __mpegaudio_compute_playtime( FILE * pMP3File, ID3Info * pID3Info, MP3AudioInfo * pMP3AudioInfo )
{
	ND_UINT64 iSize;
	ND_UINT64 iPos;
	ND_UINT32 i;

	iPos = _ftelli64( pMP3File );
	fseek( pMP3File, 0, SEEK_END );
	iSize = _ftelli64( pMP3File );
	_fseeki64( pMP3File, iPos, SEEK_SET );
	for( i = 0; i < pID3Info->m_ID3Num; i++ )
	{
		iSize -= pID3Info->m_pPacks[i]->m_iPackSize + ID3_2_HEAD_SIZE;
	}
	pMP3AudioInfo->m_iPlayTime = (ND_UINT32)( iSize * 8000 / pMP3AudioInfo->m_iBitSpeed );
}

//�ж�MP3��Ϣ
static HRESULT __parse_mp3_info( FILE * pMP3File, ID3Info * pID3Info, MP3AudioInfo * pMP3AudioInfo )
{
	ND_UCHAR szHead[MP3_HEAD_SIZE];
	ND_UINT32 iFrameHead;
	HRESULT hr;

	//Ѱ��MP3��һ֡��ͷ����MP3����Ϣ
	if( ( hr = fread( szHead, MP3_HEAD_SIZE, 1, pMP3File ) ) == 1 )
	{
		iFrameHead = MAKEMP3FRAMEHEAD( szHead );
		if( __mp3_check_header( iFrameHead ) == 0 )
		{
			__mpegaudio_decode_header( pMP3AudioInfo, iFrameHead );
			__mpegaudio_compute_playtime( pMP3File, pID3Info, pMP3AudioInfo );
			return S_OK;
		}
	}
	return -1;
}

//����MP3�ļ���ID3��Ϣ
static HRESULT __parse_ID3_info( FILE * pMP3File, ID3Info * pID3Info, MP3AudioInfo * pMP3AudioInfo )
{
	ENUM_ID3VER ver;
	HRESULT hr;
	ND_UINT32 iPos;
	
	while( !feof(pMP3File) )
	{
		iPos = ftell( pMP3File );
		//��ȡͷ��Ϣ
		ver = __parse_id3_head_info( pMP3File, pID3Info );
		if( ver == 0 )
		{
			fseek( pMP3File, iPos, SEEK_SET );
			return __parse_mp3_info( pMP3File, pID3Info, pMP3AudioInfo );
		}
		while( 1 )
		{
			//��ȡ֡��Ϣ
			hr = __parse_id3_frame_info( pMP3File, pID3Info );
			if( FAILED(hr) )
				return hr;
			if( hr == 1 )	//��ʾID3����
				break;
		}
		__skip_padding( pMP3File, pID3Info );
	}
	return S_OK;
}

//����ID3�汾
static ND_BOOL __is_valid_ID3_ver( ND_UINT32 iVer )
{
	switch( iVer )
	{
	case ENUM_ID3VER_2_4_0:
	case ENUM_ID3VER_2_3_0:
		return TRUE;
	}
	return FALSE;
}

//��ȡFrame Data
static HRESULT __get_frame_data( ID3Info * pID3Info, const ND_UCHAR * pszTag )
{
	ND_UINT32 i,k;

	for( i = 0; i < pID3Info->m_ID3Num; i++ )
	{
		for( k = 0; k < pID3Info->m_pPacks[i]->m_iFrameNum; k++ )
		{
			if( memcmp( pszTag, pID3Info->m_pPacks[i]->m_pFrames[k]->m_szTag, ID3_2_LABEL_TAG_SIZE ) == 0 )
				return (i<<16)+k;
		}
	}
	return E_FAIL;
}

//��ȡ�����ַ����ṹ
static ND_UINT32 __get_frame_data_string( ID3Info * pID3Info, ND_UINT32 iIndex, ND_UCHAR ** ppBuffer )
{
	ND_UINT32 iID3PackIndex = (iIndex>>16)&0xffff;
	ND_UINT32 iFrameIndex = iIndex&0xffff;
	ND_UINT32 iSize,iSize2;
	ID3Frame * pFrame = pID3Info->m_pPacks[iID3PackIndex]->m_pFrames[iFrameIndex];
	ND_UCHAR * p = NULL;

	if( pFrame )
	{
		//ͳһת��Ϊascii
		switch( pFrame->m_szData[0] )
		{
		case 0:	//ASCII
			iSize = pFrame->m_iFrameSize - 1;
			p = malloc( iSize+1 );
			if( p )
			{
				memcpy( p, &(pFrame->m_szData[1]), iSize );
				p[iSize] = 0;
				*ppBuffer = p;
				return iSize+1;
			}
			break;
		case 1:	//utf16,ͷ����ΪFFFEͬ��ͷ
			iSize = (pFrame->m_iFrameSize - 3)/2;
			iSize2 = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)(&(pFrame->m_szData[3])), iSize, NULL, 0, NULL, NULL );
			if( iSize2 > 0 && ( p = malloc(iSize2+1) ) != NULL )
			{
				if( WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)(&(pFrame->m_szData[3])), iSize, p, iSize2, NULL, NULL ) != 0 )
				{
					p[iSize2] = 0;
					*ppBuffer = p;
					return iSize2+1;
				}
			}
			break;
		case 2: //utf16 big-endian
			break;
		case 3: //utf8
			break;
		}
	}
	*ppBuffer = NULL;
	return 0;
}

//��ȡBlock��С
static ND_UINT32 __get_frame_data_block_size( ID3Info * pID3Info, ND_UINT32 iIndex )
{
	ND_UINT32 iID3PackIndex = (iIndex>>16)&0xffff;
	ND_UINT32 iFrameIndex = iIndex&0xffff;
	ID3Frame * pFrame = pID3Info->m_pPacks[iID3PackIndex]->m_pFrames[iFrameIndex];
	
	return pFrame->m_iFrameSize;
}

//��ȡBlock
static ND_UCHAR * __get_frame_data_block( ID3Info * pID3Info, ND_UINT32 iIndex )
{
	ND_UINT32 iID3PackIndex = (iIndex>>16)&0xffff;
	ND_UINT32 iFrameIndex = iIndex&0xffff;
	ID3Frame * pFrame = pID3Info->m_pPacks[iID3PackIndex]->m_pFrames[iFrameIndex];
	ND_UINT32 iPos;
	ND_UCHAR * p = NULL;
	FILE * pFile = NULL;
	
	p = malloc( pFrame->m_iFrameSize );
	if( !p ) return NULL;

	if( pFrame->m_iIsBlock )
	{
		if( fopen_s(&pFile, pID3Info->m_pOwner->m_szFilePath, "rb" )==0 && pFile)
		{
			memcpy( &iPos, pFrame->m_szData, sizeof(iPos) );
			fseek( pFile, iPos, SEEK_SET );
			fread( p, pFrame->m_iFrameSize, 1, pFile );
			fclose( pFile );
		}
	}
	else
	{
		memcpy( p, pFrame->m_szData, pFrame->m_iFrameSize );
	}
	return p;
}

//��ȡ֡�ַ���
static HRESULT __get_frame_string( ID3Info * pID3Info, const ND_UCHAR * pszTag, void * pData, ND_UINT32 iDataSize )
{
	ND_UCHAR * p = NULL;
	ND_UINT32 iSize;
	HRESULT hr;

	hr = __get_frame_data( pID3Info, pszTag );
	if( SUCCEEDED(hr) )
	{
		hr = __get_frame_data_string( pID3Info, hr, &p );
		if( hr > 0 )
		{
			if( iDataSize != -1 )	
				iSize = iDataSize;
			else 
				iSize = strlen(p)+1;
			iSize = min((ND_UINT32)hr,iSize);
			if( pData )
				memcpy( pData, p, iSize );
			free( p );
			return iSize;
		}
	}
	return E_NOTIMPL;
}

//��ȡ֡Block��
static HRESULT __get_frame_block( ID3Info * pID3Info, const ND_UCHAR * pszTag, void * pData, ND_UINT32 iDataSize )
{
	ND_UCHAR * p;
	ND_UINT32 iV32;
	ND_UINT32 iSize;
	HRESULT hr;

	hr = __get_frame_data( pID3Info, pszTag );
	if( SUCCEEDED(hr) )
	{
		iV32 = __get_frame_data_block_size( pID3Info, hr );
		if( iDataSize != -1 )	iSize = iDataSize;
		else iSize = iV32;
		if( pData )
		{
			p = __get_frame_data_block( pID3Info, hr );
			memcpy( pData, p, iSize );
			free( p );
		}
		return iSize;
	}
	return E_NOTIMPL;
}

//delete frame
static HRESULT __delete_frame_data( ID3Info * pID3Info, ND_UINT32 iIndex )
{
	ND_UINT32 i;
	ND_UINT32 iID3PackIndex = (iIndex>>16)&0xffff;
	ND_UINT32 iFrameIndex = iIndex&0xffff;

	if( iFrameIndex >= 0 && iFrameIndex < pID3Info->m_pPacks[iID3PackIndex]->m_iFrameNum )
	{
		for( i = iFrameIndex; i < pID3Info->m_pPacks[iID3PackIndex]->m_iFrameNum-1; i++ )
		{
			pID3Info->m_pPacks[iID3PackIndex]->m_pFrames[i] = pID3Info->m_pPacks[iID3PackIndex]->m_pFrames[i+1];
		}
		pID3Info->m_pPacks[iID3PackIndex]->m_iFrameNum--;
		return pID3Info->m_pPacks[iID3PackIndex]->m_iFrameNum;
	}
	return E_FAIL;
}

//�����µ�Frame
static HRESULT __gen_frame_string( const ND_UCHAR * pszTag, ND_UINT32 iVer, ND_UINT32 iCodeType, void * pData, ND_UINT32 iDataSize, ID3Frame ** ppFrame )
{
	ID3Frame * p = NULL;
	HRESULT hr;
	ND_UINT32 iSize;
	ND_UINT32 iLen;
	ND_UCHAR szBuf[3];
	ND_UINT32 iV32;
	ND_UINT16 iV16;

	if( iDataSize == -1 )
	{
		switch( iCodeType )
		{
		case 0:
		case 3:
			iSize = strlen((ND_UCHAR*)pData) + 1;
			break;
		case 1:
		case 2:
			iSize = ( wcslen((ND_UINT16*)pData) + 1 ) * sizeof(ND_UINT16);
			break;
		default:
			return E_FAIL;
		}
	}
	else
		iSize = iDataSize;

	switch( iCodeType )
	{
	case 0:	iLen = 4+2+1; szBuf[0] = 0; break;
	case 1:	iLen = 4+2+3; szBuf[0] = 1; szBuf[1] = 0xFF; szBuf[2] = 0xFE; break;
	case 2:	iLen = 4+2+3; szBuf[0] = 2; szBuf[1] = 0xFF; szBuf[2] = 0xFE; break;
	case 3:	iLen = 4+2+1; szBuf[0] = 3; break;
	default: return E_FAIL;
	}

	p = malloc(sizeof(ID3Frame)+iSize+iLen);
	if( !p )
	{
		hr = E_OUTOFMEMORY;
		goto ret_here;
	}
	memset( p, 0, sizeof(ID3Frame)+iSize+iLen );
	p->m_iSize = sizeof(ID3Frame)+iSize+iLen;
	memcpy( p->m_szTag, pszTag, 4 );
	p->m_iFrameSize = iSize+iLen-6;

	switch( iVer )
	{
	case ENUM_ID3VER_2_4_0: iV32 = SETSAFELEN( p->m_iFrameSize ); break;
	case ENUM_ID3VER_2_3_0: iV32 = SETLEN( p->m_iFrameSize ); break;
		break;
	default:
		free( p );
		return E_FAIL;
	}
	memcpy( p->m_szData, &iV32, sizeof(iV32) );

	iV16 = 0;
	memcpy( p->m_szData+sizeof(iV32), &iV16, sizeof(iV16) );
	
	memcpy( p->m_szData+sizeof(iV32)+sizeof(iV16), szBuf, iLen-6 );
	memcpy( p->m_szData+iLen, pData, iSize );

	hr = S_OK;
	
ret_here:
	*ppFrame = p;
	return hr;
}

//׷�ӵ�ID3Info��
static HRESULT __add_frame( ID3PackPtr pPack, ID3Frame * pFrame )
{
	if( pPack->m_iFrameNum == 0 )
		pPack->m_pFrames = malloc(sizeof(ID3FramePtr));
	else
		pPack->m_pFrames = realloc( pPack->m_pFrames, (pPack->m_iFrameNum+1)*sizeof(ID3FramePtr) );
	if( !(pPack->m_pFrames) )
		return E_OUTOFMEMORY;

	pPack->m_pFrames[pPack->m_iFrameNum++] = pFrame;
	return pPack->m_iFrameNum;
}

//����Frame String
static ND_UINT32 __set_frame_string( ID3Info * pID3Info, const ND_UCHAR * pszTag, void * pData, ND_UINT32 iDataSize )
{
	HRESULT hr;
	ID3Frame * p = NULL;
	ND_UINT32 iID3PackIndex = 0;

	hr = __get_frame_data( pID3Info, pszTag );
	if( SUCCEEDED(hr) )
	{
		iID3PackIndex = (hr>>16)&0xffff;
		//ɾ��ԭ����frame
		hr = __delete_frame_data( pID3Info, hr );
		if( FAILED(hr) ) return hr;
	}
	//�����µ�Frame
	hr = __gen_frame_string( pszTag, pID3Info->m_pPacks[iID3PackIndex]->m_iVer, 0, pData, iDataSize, &p );
	if( FAILED(hr) ) return hr;

	//׷�ӵ�ID3Info��
	hr = __add_frame( pID3Info->m_pPacks[iID3PackIndex], p );
	if( FAILED(hr) )
	{
		free( p );
		return hr;
	}
	return S_OK;
}

//дͷ
static HRESULT __write_head( ID3Pack * pPack, FILE * pOut )
{
	HRESULT hr;
	ND_UINT16 iV16 = 0;
	ND_UINT8 iV8 = 0;
	ND_UINT32 iV32 = 0;

	//tag ID3
	hr = fwrite( "ID3", 3, 1, pOut );
	if( hr != 1 ) return E_FAIL;

	//Ver
	switch( pPack->m_iVer )
	{
	case ENUM_ID3VER_2_4_0: iV16 = 4; break;
	case ENUM_ID3VER_2_3_0: iV16 = 3; break;
	default: return E_FAIL;
	}
	hr = fwrite( &iV16, sizeof(iV16), 1, pOut );
	if( hr != 1 ) return E_FAIL;
	
	//attr
	hr = fwrite( &iV16, sizeof(iV8), 1, pOut );
	if( hr != 1 ) return E_FAIL;

	//size
	hr = fwrite( &iV32, sizeof(iV32), 1, pOut );
	if( hr != 1 ) return E_FAIL;

	return S_OK;
}

//дFrame
static HRESULT __write_frame( ID3Pack * pPack, ND_UINT32 iIndex, FILE * pOut, FILE * pIn )
{
	HRESULT hr;
	ND_UINT32 iV32;
	ND_UINT8 iV8[2];

	//tag
	hr = fwrite( pPack->m_pFrames[iIndex]->m_szTag, ID3_2_LABEL_TAG_SIZE, 1, pOut );
	if( hr != 1 ) return E_FAIL;

	//size
	switch( pPack->m_iVer )
	{
	case ENUM_ID3VER_2_4_0: iV32 = SETSAFELEN( pPack->m_pFrames[iIndex]->m_iFrameSize ); break;
	case ENUM_ID3VER_2_3_0: iV32 = SETLEN( pPack->m_pFrames[iIndex]->m_iFrameSize ); break;
	default: return -4;
	}
	hr = fwrite( &iV32, sizeof(iV32), 1, pOut );
	if( hr != 1 ) return E_FAIL;

	//attr
	iV8[0] = 0; iV8[1] = 0;
	switch( pPack->m_iVer )
	{
	case ENUM_ID3VER_2_4_0:
		if( pPack->m_pFrames[iIndex]->m_iTagAlterPreservation == 1 )
			iV8[0] |= 0x40;
		if( pPack->m_pFrames[iIndex]->m_iFileAlterPreservation == 1 )
			iV8[0] |= 0x20;
		if( pPack->m_pFrames[iIndex]->m_iReadOnly == 1 )
			iV8[0] |= 0x10;
		if( pPack->m_pFrames[iIndex]->m_iGroupingIdentity == 1 )
			iV8[1] |= 0x40;
		if( pPack->m_pFrames[iIndex]->m_iCompression == 1 )
			iV8[1] |= 0x08;
		if( pPack->m_pFrames[iIndex]->m_iEncryption == 1 )
			iV8[1] |= 0x04;
		if( pPack->m_pFrames[iIndex]->m_iUnsynchronisation == 1 )
			iV8[1] |= 0x02;
		if( pPack->m_pFrames[iIndex]->m_iDataLengthIndicator == 1 )
			iV8[1] |= 0x01;			
		break;
	case ENUM_ID3VER_2_3_0:
		if( pPack->m_pFrames[iIndex]->m_iTagAlterPreservation == 1 )
			iV8[0] |= 0x80;
		if( pPack->m_pFrames[iIndex]->m_iFileAlterPreservation == 1 )
			iV8[0] |= 0x40;
		if( pPack->m_pFrames[iIndex]->m_iReadOnly == 1 )
			iV8[0] |= 0x20;
		if( pPack->m_pFrames[iIndex]->m_iCompression == 1 )
			iV8[1] |= 0x80;
		if( pPack->m_pFrames[iIndex]->m_iEncryption == 1 )
			iV8[1] |= 0x40;
		if( pPack->m_pFrames[iIndex]->m_iGroupingIdentity == 1 )
			iV8[1] |= 0x20;	
		break;
	}
	hr = fwrite( iV8, 2, 1, pOut );
	if( hr != 1 ) return E_FAIL;

	//data
	if( pPack->m_pFrames[iIndex]->m_iIsBlock != 1 )
	{
		//data
		hr = fwrite( (pPack->m_pFrames[iIndex]->m_szData+ID3_2_HEAD_SIZE), pPack->m_pFrames[iIndex]->m_iFrameSize, 1, pOut );
		if( hr != 1 ) return E_FAIL;
	}
	else
	{	//�Ȼ�ȡ����
		ND_UINT8 * pData = NULL;

		pData = malloc( pPack->m_pFrames[iIndex]->m_iFrameSize );
		if( !pData ) return E_FAIL;

		memcpy( &iV32, pPack->m_pFrames[iIndex]->m_szData, sizeof(iV32) );
		fseek( pIn, iV32, SEEK_SET );
		hr = fread( pData, pPack->m_pFrames[iIndex]->m_iFrameSize, 1, pIn );
		if( hr != 1 ){ free( pData ); return E_FAIL; }

		hr = fwrite( pData, pPack->m_pFrames[iIndex]->m_iFrameSize, 1, pOut );
		if( hr != 1 ){ free(pData);  return E_FAIL; }

		free( pData );
	}
	return S_OK;
}

//����ͷ
static HRESULT __fill_in_head( ID3Pack * pPack, FILE * pOut )
{
	HRESULT hr;
	ND_UINT32 iSize = 0;
	ND_UINT32 iSize2 = 0;
	ND_UINT32 i;
	ND_UCHAR * p;

	//����padding
	for( i = 0; i < pPack->m_iFrameNum; i++ )
		iSize += ID3_2_FRAME_HEAD_SIZE+pPack->m_pFrames[i]->m_iFrameSize;
	
	srand( GetTickCount() );

	p = malloc( BLOCKSIZE );
	if( !p ) return E_FAIL;
	memset( p, 0, BLOCKSIZE );

	iSize2 = iSize+(rand()%BLOCKSIZE);
	iSize += iSize2;

	do
	{
		i = min( BLOCKSIZE, iSize2 );
		hr = fwrite( p, i, 1, pOut );
		if( hr != 1 ){ free( p ); return E_FAIL; };
		iSize2 -= BLOCKSIZE;
	}while( i == BLOCKSIZE );

	fseek( pOut, 0-iSize-sizeof(ND_UINT32), SEEK_CUR );
	iSize2 = SETSAFELEN( iSize );
	if( fwrite( &iSize2, sizeof(iSize2), 1, pOut ) != 1 )
	{ free( p ); return E_FAIL; };

	fseek( pOut, iSize, SEEK_CUR );

	free( p );
	return S_OK;
}

//дMP3����
static HRESULT __write_mp3_data( FILE * pIn, FILE * pOut )
{
	ND_UINT32 iV32;
	ND_UCHAR * pBuffer = NULL;
	HRESULT hr;
	ND_UCHAR szHead[ID3_2_HEAD_SIZE];

	//Ѱ��MP3 Data��ʼλ��
	do
	{
		hr = fread( szHead, ID3_2_HEAD_SIZE, 1, pIn );
		if( hr != 1 ) return E_FAIL;
		if( strncmp( szHead, "ID3", 3 ) != 0 )
		{
			fseek( pIn, 0-ID3_2_HEAD_SIZE, SEEK_CUR );
			break;
		}
		iV32 = GETSAFELEN( &(szHead[6]) );
		fseek( pIn, iV32, SEEK_CUR );
	}while(1);

	//����MP3 Data
	pBuffer = malloc(BLOCKSIZE);
	if( !pBuffer ) return E_FAIL;

	while( !feof(pIn) )
	{
		hr = fread( pBuffer, 1, BLOCKSIZE, pIn );		
		if( fwrite( pBuffer, hr, 1, pOut ) != 1 )
		{
			free( pBuffer );
			return E_FAIL;
		}		
	}

	free( pBuffer );
	return S_OK;
}

static FILE * __temp_file()
{
	return NULL;
}

//#################################################################################################
//##################################### �ڲ��������� ##############################################
//#################################################################################################

//#################################################################################################
//##################################### �ⲿ������ʼ ##############################################
//#################################################################################################

//�ж��Ƿ�֧��MP3�ļ�����
ND_BOOL ND_ValidMP3Cap( const ND_CHAR * pszMP3FilePath )
{
	ND_UINT32 iVer;

	iVer = __get_ID3_ver( pszMP3FilePath );

	return __is_valid_ID3_ver( iVer );	
}

//�����ļ�
HANDLE ND_ParseMP3Info( const ND_CHAR * pszMP3FilePath )
{
	HRESULT hr;
	FILE * pMP3File = NULL;
	MP3Info * pMP3Info = NULL;
		
	if( fopen_s(&pMP3File, pszMP3FilePath, "rb" )==0 && pMP3File)
	{
		pMP3Info = malloc(sizeof(MP3Info));
		if( pMP3Info )
		{
			memset( pMP3Info, 0, sizeof(MP3Info) );
			pMP3Info->m_pID3Info = __create_ID3_info();
			if( pMP3Info->m_pID3Info )
			{
				pMP3Info->m_pID3Info->m_pOwner = pMP3Info;
				strncpy_s( pMP3Info->m_szFilePath, ND_PATH_MAXSIZE, pszMP3FilePath, ND_PATH_MAXSIZE );
				pMP3Info->m_szFilePath[ND_PATH_MAXSIZE-1] = 0;
				hr = __parse_ID3_info( pMP3File, pMP3Info->m_pID3Info, &(pMP3Info->m_AudioInfo) );
				if( FAILED(hr) )
				{
					__release_ID3_info( pMP3Info->m_pID3Info );
					pMP3Info->m_pID3Info = NULL;
					free( pMP3Info );
					pMP3Info = NULL;
				}
			}
		}		
		fclose( pMP3File );
	}

	return pMP3Info;
}

size_t GB2312ToUnicode(wchar_t** pOut, const char *pIn, size_t nSize)
{
	int nLength = 0;
	int newSize = -1;
	if (*pOut)
	{
		free(*pOut);
		*pOut = NULL;
	}	

	if (pIn != NULL && nSize > 0)
	{
		nLength = MultiByteToWideChar(CP_ACP, 0, pIn, nSize, NULL, 0);
		if ( nLength <= 0 ) goto ret_here;

		*pOut = (wchar_t*)malloc((nLength + 1)*sizeof(wchar_t));
		if (! (*pOut)) goto ret_here;

		memset(*pOut, 0, (nLength + 1)*sizeof(wchar_t));
		newSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pIn, nSize, *pOut, nLength);
	}	

ret_here:
	return newSize;
}

size_t UnicodeToGB2312(char** pOut, const wchar_t *pIn, size_t nSize)
{
	int nLength = 0;
	int newSize = -1;
	if (*pOut)
	{
		free(*pOut);
		*pOut = NULL;
	}

	if (pIn != NULL && nSize > 0)
	{
		// �����ڴ�
		nLength = WideCharToMultiByte(CP_ACP, 0, pIn, nSize, NULL, 0, NULL, NULL);
		if (nLength <= 0) goto ret_here;

		*pOut = (char*)malloc((nLength+1)*sizeof(char));
		if (!(*pOut)) goto ret_here;

		memset(*pOut, 0, (nLength+1)*sizeof(char));
		newSize = WideCharToMultiByte(CP_ACP, 0, pIn, nSize, *pOut, nLength, NULL, NULL);
	}	

ret_here:
	return newSize;
}

//д�ļ�
HRESULT ND_WriteMP3Info( HANDLE hMP3 )
{
	MP3Info * pMP3Info;
	ND_BOOL bIsTempFile = FALSE;
	ND_UINT32 i,k;
	FILE *pIn = NULL;
	FILE *pOut = NULL;
	HRESULT hr;
	wchar_t *pNewFilePathUnicode = NULL;
	size_t nNewFilePathSize = 0;
	wchar_t *pFilePathUnicode = NULL;
	size_t nFilePathSize = 0;

	if( !hMP3 )
		return E_INVALIDARG;

	pMP3Info = (MP3Info*)hMP3;

	if(fopen_s(&pIn, pMP3Info->m_szFilePath, "rb" )!=0 || !pIn ) return E_FAIL;

	if (fopen_s(&pOut, pMP3Info->m_szNewFilePath, "wb" ) != 0 || !pOut)return E_FAIL;

	//дpack
	for( i = 0; i < pMP3Info->m_pID3Info->m_ID3Num; i++ )
	{
		//дͷ
		hr = __write_head( pMP3Info->m_pID3Info->m_pPacks[i], pOut );
		if( FAILED(hr) ) goto ret_here;

		//дFrame
		for( k = 0; k < pMP3Info->m_pID3Info->m_pPacks[i]->m_iFrameNum; k++ )
		{
			hr = __write_frame( pMP3Info->m_pID3Info->m_pPacks[i], k, pOut, pIn );
			if( FAILED(hr) ) goto ret_here;
		}

		//����ͷ
		hr = __fill_in_head( pMP3Info->m_pID3Info->m_pPacks[i], pOut );
		if( FAILED(hr) ) goto ret_here;
	}

	//дMP3����
	hr = __write_mp3_data( pIn, pOut );
	if( FAILED(hr) ) goto ret_here;

	hr = S_OK;

ret_here:
	if( pIn ) fclose(pIn) ;
	if( pOut ) fclose(pOut);

	nFilePathSize = GB2312ToUnicode(&pFilePathUnicode, pMP3Info->m_szFilePath, strlen(pMP3Info->m_szFilePath));
	nNewFilePathSize = GB2312ToUnicode(&pNewFilePathUnicode, pMP3Info->m_szNewFilePath, strlen(pMP3Info->m_szNewFilePath));
	if (nFilePathSize > 0 && pFilePathUnicode && nNewFilePathSize > 0 && pNewFilePathUnicode)
	{
		if (hr == S_OK)
			CopyFile(pNewFilePathUnicode, pFilePathUnicode, FALSE);

		DeleteFile(pNewFilePathUnicode);

		free(pFilePathUnicode);
		pFilePathUnicode = NULL;
		free(pNewFilePathUnicode);
		pNewFilePathUnicode = NULL;
	}	

	return hr;
}

//��ȡMP3��Ϣ
HRESULT ND_GetMP3Info( HANDLE hMP3, ND_UINT32 iType, void * pData, ND_INT32 iDataSize )
{
	MP3Info * pMP3Info;
	HRESULT hr;
	ND_UINT32 iSize;
	ND_UCHAR szBuf[256];

	if( !hMP3 )
		return E_INVALIDARG;

	pMP3Info = (MP3Info*)hMP3;
	
	switch( iType )
	{
	case ENUM_MP3_INFO_PARA_PATH:						//·��
		{
			if( iDataSize != -1 )	iSize = iDataSize;
			else iSize = strlen(pMP3Info->m_szFilePath)+1;
			if( pData )
				memcpy( pData, pMP3Info->m_szFilePath, iSize );
			return iSize;
		}
	case ENUM_MP3_INFO_PARA_BITRATE:					//λ��
		{
			if( iDataSize != -1 )	iSize = iDataSize;
			else iSize = sizeof(ND_UINT32);
			if( pData )
				memcpy( pData, &(pMP3Info->m_AudioInfo.m_iBitSpeed), iSize );
			return iSize;
		}
	case ENUM_MP3_INFO_PARA_SAMPLERATE:					//��������
		{
			if( iDataSize != -1 )	iSize = iDataSize;
			else iSize = sizeof(ND_UINT32);
			if( pData )
				memcpy( pData, &(pMP3Info->m_AudioInfo.m_iSampleRate), iSize );
			return iSize;
		}
	case ENUM_MP3_INFO_PARA_FORMAT:						//��Ƶ��ʽ
		{			
			if( pMP3Info->m_AudioInfo.m_iLSF == 0 )
				sprintf_s( szBuf, 256, "MPEG-1,Layer %d", pMP3Info->m_AudioInfo.m_iLayer );
			else if( pMP3Info->m_AudioInfo.m_iMpeg25 == 0 )
				sprintf_s( szBuf, 256, "MPEG-2,Layer %d", pMP3Info->m_AudioInfo.m_iLayer );
			else
				sprintf_s( szBuf, 256, "MPEG-2.5,Layer %d", pMP3Info->m_AudioInfo.m_iLayer );
			if( iDataSize != -1 )	iSize = iDataSize;
			else iSize = strlen(szBuf)+1;
			if( pData )
				memcpy( pData, szBuf, iSize );
			return iSize;
		}
	case ENUM_MP3_INFO_PARA_CHANNEL:					//ͨ��
		{
			switch( pMP3Info->m_AudioInfo.m_iCannel )
			{
			case AUDIO_STEREO:	sprintf_s( szBuf, 256, "������" ); break;
			case AUDIO_JSTEREO:	sprintf_s( szBuf, 256, "����������" ); break;
			case AUDIO_DUAL:	sprintf_s( szBuf, 256, "˫����" ); break;
			case AUDIO_MONO:	sprintf_s( szBuf, 256, "������" ); break;
			default:			sprintf_s( szBuf, 256, "δ֪" ); break;
			}
			if( iDataSize != -1 )	iSize = iDataSize;
			else iSize = strlen(szBuf)+1;
			if( pData )
				memcpy( pData, szBuf, iSize );
			return iSize;
		}
	case ENUM_MP3_INFO_PARA_ID3VER:						//ID3���
		{
			if( iDataSize != -1 )	iSize = iDataSize;
			else iSize = sizeof(ND_UINT32);
			if( pData )
				memcpy( pData, &(pMP3Info->m_pID3Info->m_pPacks[0]->m_iVer), iSize );
			return iSize;
		}
	//case ENUM_MP3_INFO_PARA_CODES:						//��Ƶ���뷽ʽ
	//	break;
	case ENUM_MP3_INFO_PARA_TIT2:						//����
		return __get_frame_string( pMP3Info->m_pID3Info, "TIT2", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TPE1:						//������
		return __get_frame_string( pMP3Info->m_pID3Info, "TPE1", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TPE2:						//ר��������
		return __get_frame_string( pMP3Info->m_pID3Info, "TPE2", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TCOM:						//������
		return __get_frame_string( pMP3Info->m_pID3Info, "TCOM", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TALB:						//ר��
		return __get_frame_string( pMP3Info->m_pID3Info, "TALB", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_USLT:						//���
		return __get_frame_block( pMP3Info->m_pID3Info, "USLT", pData, iDataSize );		
	case ENUM_MP3_INFO_PARA_APIC:						//����
		return __get_frame_block( pMP3Info->m_pID3Info, "APIC", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TRCK:						//�������
		return __get_frame_string( pMP3Info->m_pID3Info, "TRCK", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TPOS:						//���̺���
		return __get_frame_string( pMP3Info->m_pID3Info, "TPOS", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TYER:						//���
		{
			if (pMP3Info->m_pID3Info->m_pPacks[0]->m_iVer == ENUM_ID3VER_2_3_0)
				return __get_frame_string( pMP3Info->m_pID3Info, "TYER", pData, iDataSize );
			else if (pMP3Info->m_pID3Info->m_pPacks[0]->m_iVer == ENUM_ID3VER_2_4_0)
				return __get_frame_string( pMP3Info->m_pID3Info, "TDRC", pData, iDataSize );
			else
				return 0;
		}
	case ENUM_MP3_INFO_PARA_COMM:						//ע��
		//return __get_frame_string( pMP3Info->m_pID3Info, "COMM", pData, iDataSize );
		return __get_frame_block( pMP3Info->m_pID3Info, "COMM", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TIT1:						//����
		return __get_frame_string( pMP3Info->m_pID3Info, "TIT1", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TCON:						//���
		{
			//return __get_frame_string( pMP3Info->m_pID3Info, "TCON", pData, iDataSize );
			ND_UCHAR szBuf[128];
			ND_UCHAR * p;
			hr = __get_frame_string( pMP3Info->m_pID3Info, "TCON", szBuf, 128 );
			if( FAILED(hr) ) return hr;
			szBuf[127] = 0;
			p = __tcon_get_name_by_no( szBuf );
			if( p )
				strcpy( pData, p );
			else
				strcpy( pData, szBuf );
			return strlen(pData)+1;
		}
	case ENUM_MP3_INFO_PARA_TIT3:						//����
		return __get_frame_string( pMP3Info->m_pID3Info, "TIT3", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TBPM:						//BPM
		return __get_frame_string( pMP3Info->m_pID3Info, "TBPM", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TCMP:						//����ѡ��
		return __get_frame_string( pMP3Info->m_pID3Info, "TCMP", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSOT:						//��������
		return __get_frame_string( pMP3Info->m_pID3Info, "TSOT", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSOA:						//ר������
		return __get_frame_string( pMP3Info->m_pID3Info, "TSOA", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSOP:						//����������
		return __get_frame_string( pMP3Info->m_pID3Info, "TSOP", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSO2:						//ר������������
		return __get_frame_string( pMP3Info->m_pID3Info, "TSO2", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSOC:						//����������
		return __get_frame_string( pMP3Info->m_pID3Info, "TSOC", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_RVAD:						//��������
		{
			ND_UCHAR szBuf[32];
			ND_UINT16 iV16;

			if (pMP3Info->m_pID3Info->m_pPacks[0]->m_iVer == ENUM_ID3VER_2_3_0)
				hr = __get_frame_block( pMP3Info->m_pID3Info, "RVAD", szBuf, 32 );
			else if (pMP3Info->m_pID3Info->m_pPacks[0]->m_iVer == ENUM_ID3VER_2_4_0)
				hr = __get_frame_block( pMP3Info->m_pID3Info, "RVA2", szBuf, 32 );
			else
				return 0;
			if( FAILED(hr) ) return hr;
			iV16 = ((ND_UINT16)szBuf[0]<<8)|szBuf[1];
			if( iV16 == 1 )
			{
				iV16 = ((ND_UINT16)szBuf[2]<<8)|szBuf[3];
				*(ND_UINT16*)pData = iV16;
			}
			else
				*(ND_UINT16*)pData = 0;
			return sizeof(ND_UINT16);
		}		
	case ENUM_MP3_INFO_PARA_PCST:						//ý������podcast
		return __get_frame_string( pMP3Info->m_pID3Info, "PCST", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_ITNU:						//ý������iTunes U	
		return __get_frame_string( pMP3Info->m_pID3Info, "ITNU", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_PLAYTIME:
		{
			if( iDataSize != -1 )	iSize = iDataSize;
			else iSize = sizeof(ND_UINT32);
			if( iSize != sizeof(ND_UINT32) ) return E_INVALIDARG;
			if( pData )
				memcpy( pData, &(pMP3Info->m_AudioInfo.m_iPlayTime), iSize );
			return iSize;
		}
	default:
		hr = E_NOTIMPL;
	}
	return hr;
}

//����MP3��Ϣ
HRESULT ND_SetMP3Info( HANDLE hMP3, ND_UINT32 iType, void * pData, ND_INT32 iDataSize )
{
	MP3Info * pMP3Info;
	HRESULT hr;
	ND_UINT32 iSize;

	if( !hMP3 || !pData )
		return E_INVALIDARG;

	pMP3Info = (MP3Info*)hMP3;

	switch( iType )
	{
	case ENUM_MP3_INFO_PARA_PATH:						//·��
		{
			if( iDataSize != -1 )	iSize = iDataSize;
			else iSize = strlen((ND_UCHAR*)pData)+1;
			memcpy( pMP3Info->m_szNewFilePath, pData, iSize );
			return iSize;
		}
		break;
	//case ENUM_MP3_INFO_PARA_BITRATE:					//λ��
	//	break;
	//case ENUM_MP3_INFO_PARA_SAMPLERATE:					//��������
	//	break;
	//case ENUM_MP3_INFO_PARA_FORMAT:						//��Ƶ��ʽ		
	//	break;
	//case ENUM_MP3_INFO_PARA_CHANNEL:					//ͨ��
	//	break;
	//case ENUM_MP3_INFO_PARA_ID3VER:						//ID3���
	//	break;
	//case ENUM_MP3_INFO_PARA_CODES:						//��Ƶ���뷽ʽ
	//	break;
	case ENUM_MP3_INFO_PARA_TIT2:						//����
		return __set_frame_string( pMP3Info->m_pID3Info, "TIT2", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TPE1:						//������
		return __set_frame_string( pMP3Info->m_pID3Info, "TPE1", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TPE2:						//ר��������
		return __set_frame_string( pMP3Info->m_pID3Info, "TPE2", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TCOM:						//������
		return __set_frame_string( pMP3Info->m_pID3Info, "TCOM", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TALB:						//ר��
		return __set_frame_string( pMP3Info->m_pID3Info, "TALB", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_USLT:						//���
		return __set_frame_string( pMP3Info->m_pID3Info, "USLT", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_APIC:						//����
		return __set_frame_string( pMP3Info->m_pID3Info, "APIC", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TRCK:						//�������
		return __set_frame_string( pMP3Info->m_pID3Info, "TRCK", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TPOS:						//���̺���
		return __set_frame_string( pMP3Info->m_pID3Info, "TPOS", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TYER:						//���
		return __set_frame_string( pMP3Info->m_pID3Info, "TYER", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_COMM:						//ע��
		return __set_frame_string( pMP3Info->m_pID3Info, "COMM", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TIT1:						//����
		return __set_frame_string( pMP3Info->m_pID3Info, "TIT1", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TCON:						//���
		return __set_frame_string( pMP3Info->m_pID3Info, "TCON", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TIT3:						//����
		return __set_frame_string( pMP3Info->m_pID3Info, "TIT3", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TBPM:						//BPM
		return __set_frame_string( pMP3Info->m_pID3Info, "TBPM", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TCMP:						//����ѡ��
		return __set_frame_string( pMP3Info->m_pID3Info, "TCMP", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSOT:						//��������
		return __set_frame_string( pMP3Info->m_pID3Info, "TSOT", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSOA:						//ר������
		return __set_frame_string( pMP3Info->m_pID3Info, "TSOA", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSOP:						//����������
		return __set_frame_string( pMP3Info->m_pID3Info, "TSOP", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSO2:						//ר������������
		return __set_frame_string( pMP3Info->m_pID3Info, "TSO2", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_TSOC:						//����������
		return __set_frame_string( pMP3Info->m_pID3Info, "TSOC", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_RVAD:						//��������
		return __set_frame_string( pMP3Info->m_pID3Info, "RVAD", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_PCST:						//ý������podcast
		return __set_frame_string( pMP3Info->m_pID3Info, "PCST", pData, iDataSize );
	case ENUM_MP3_INFO_PARA_ITNU:						//ý������iTunes U	
		return __set_frame_string( pMP3Info->m_pID3Info, "ITNU", pData, iDataSize );
	//case ENUM_MP3_INFO_PARA_PLAYTIME:
	//	break;
	default:
		hr = E_NOTIMPL;
	}
	return hr;
}

//�ͷ�MP3��Ϣ
void ND_ReleaseMP3Info( HANDLE hMP3 )
{
	if( hMP3 )
	{
		MP3Info * pMP3Info = (MP3Info*)hMP3;
		if( pMP3Info->m_pID3Info )
			__release_ID3_info( pMP3Info->m_pID3Info );
		free( pMP3Info );
	}
}

//��ȡeng��Ϣ
HRESULT ND_GetEngString( void * pData, ND_UINT32 iDataSize, ND_UCHAR ** ppStr, ND_UINT32 * pStrLen )
{
	ND_UCHAR * p = (ND_UCHAR*)pData+10;
	*ppStr = NULL;
	*pStrLen = UnicodeToGB2312( ppStr, p, iDataSize-10 );
	if( *pStrLen < 0 )
		return E_FAIL;
	return S_OK;
}

//����eng��Ϣ
HRESULT ND_SetEngString( ND_UCHAR * pSourceStr, ND_UINT32 iSourceLen, void ** ppDest, ND_UINT32 * pDestLen )
{
	ND_UINT32 iSize;
	wchar_t * p = NULL;
	wchar_t * p2 = NULL;

	iSize = GB2312ToUnicode( &p, pSourceStr, iSourceLen );
	if( iSize > 0 )
	{
		p2 = malloc(iSize+10);
		if( p2 )
		{
			memcpy( p2, "\0x1eng\0xFF\0xFE\0x0\0x0\0xFF\0xFE", 10 );
			memcpy( p2+10, p, iSize );
			*ppDest = p2;
			*pDestLen = iSize+10;
			free( p );
			return S_OK;
		}
	}
	if( p )
		free( p );
	*ppDest = NULL;
	*pDestLen = 0;
	return E_FAIL;	
}
//#################################################################################################
//##################################### �ⲿ�������� ##############################################
//#################################################################################################