#include "stdafx.h"
#ifdef MODULE_IPOD

#include "HashVerify.h"
#include "Module/iPod/VCiPodFileSync.h"
#include "Module/Common/Media/Utils/Hash.h"
#include "Module/Common/Media/Utils/checksum.h"

#include <atlconv.h>

//校验iTuneCDB数据
int HashVerify::CheckSumITunesCDBData( void * pData, ND_UINT32 iDataSize, ND_UCHAR szSHA1[20], ND_UCHAR szParam1[16], ND_UCHAR szParam2[12] )
{
	ND_UCHAR szHash72[46];
	ND_UCHAR szBack[46];
	ND_UCHAR backup18[8];
	ND_UCHAR backup32[20];
	ND_UCHAR backup58[20];
	int hr;
	
	memcpy( szHash72, (ND_UCHAR*)pData+0x72, 46 );
	memcpy( backup18, (ND_UCHAR*)pData+0x18, 8 );
	memcpy( backup32, (ND_UCHAR*)pData+0x32, 20 );
	memcpy( backup58, (ND_UCHAR*)pData+0x58, 20 );

	memset( (ND_UCHAR*)pData+0x72, 0, 46 );
	memset( (ND_UCHAR*)pData+0x18, 0, 8 );
	memset( (ND_UCHAR*)pData+0x32, 0, 20 );
	memset( (ND_UCHAR*)pData+0x58, 0, 20 );

	//计算sha1
	memset( szSHA1, 0, 20 );
	hr = GetChecksum(  (ND_UCHAR*)pData, iDataSize, szSHA1 );

	//解密
	hr = ND_Hash72Extract( szHash72, szSHA1, szParam1, szParam2 );
	memset( szBack, 0, 46 );
	ND_Hash72Generate( szBack, szSHA1, szParam1, szParam2 );
	if( memcmp( szBack, szHash72, 46 ) != 0 )
		return -1;

	memcpy( (ND_UCHAR*)pData+0x72, szBack, 46 );
	memcpy( (ND_UCHAR*)pData+0x18, backup18, 8 );
	memcpy( (ND_UCHAR*)pData+0x32, backup32, 20 );
	memcpy( (ND_UCHAR*)pData+0x58, backup32, 20 );
	return 0;
}
 void HashVerify::BackCheckSumiTunesCDB(void * pData, ND_UINT32 iDataSize, ND_UCHAR szParam1[16], ND_UCHAR szParam2[12])
 {
	 ND_UCHAR szHash72[46];
	 ND_UCHAR backup18[8];
	 ND_UCHAR backup32[20];
	 ND_UCHAR backup58[20];
	 ND_UCHAR szSHA1[20];
	 int hr;

	 memcpy( szHash72, (ND_UCHAR*)pData+0x72, 46 );
	 memcpy( backup18, (ND_UCHAR*)pData+0x18, 8 );
	 memcpy( backup32, (ND_UCHAR*)pData+0x32, 20 );
	 memcpy( backup58, (ND_UCHAR*)pData+0x58, 20 );

	 memset( (ND_UCHAR*)pData+0x72, 0, 46 );
	 memset( (ND_UCHAR*)pData+0x18, 0, 8 );
	 memset( (ND_UCHAR*)pData+0x32, 0, 20 );
	 memset( (ND_UCHAR*)pData+0x58, 0, 20 );

	 memset( szSHA1, 0, 20 );
	 hr = GetChecksum(  (ND_UCHAR*)pData, iDataSize, szSHA1 );

	 //解密
	 hr = ND_Hash72Extract( szHash72, szSHA1, szParam1, szParam2 );
	
	 memcpy( (ND_UCHAR*)pData+0x72, szHash72, 46 );
	 memcpy( (ND_UCHAR*)pData+0x18, backup18, 8 );
	 memcpy( (ND_UCHAR*)pData+0x32, backup32, 20 );
	 memcpy( (ND_UCHAR*)pData+0x58, backup32, 20 );
 }

 void HashVerify::MakeCheckSumiTunesCDB(void * pData, ND_UINT32 iDataSize, ND_UCHAR szParam1[16], ND_UCHAR szParam2[12])
 {
	 ND_UCHAR szHash72[46];
	 ND_UCHAR szBack[46];
	 ND_UCHAR backup18[8];
	 ND_UCHAR backup32[20];
	 ND_UCHAR backup58[20];
	 ND_UCHAR szSHA1[20];
	 int hr;

	 memcpy( szHash72, (ND_UCHAR*)pData+0x72, 46 );
	 memcpy( backup18, (ND_UCHAR*)pData+0x18, 8 );
	 memcpy( backup32, (ND_UCHAR*)pData+0x32, 20 );
	 memcpy( backup58, (ND_UCHAR*)pData+0x58, 20 );

	 memset( (ND_UCHAR*)pData+0x72, 0, 46 );
	 memset( (ND_UCHAR*)pData+0x18, 0, 8 );
	 memset( (ND_UCHAR*)pData+0x32, 0, 20 );
	 memset( (ND_UCHAR*)pData+0x58, 0, 20 );

	 //计算sha1
	 memset( szSHA1, 0, 20 );
	 hr = GetChecksum(  (ND_UCHAR*)pData, iDataSize, szSHA1 );

	 ND_Hash72Generate( szBack, szSHA1, szParam1, szParam2 );

	 memcpy( (ND_UCHAR*)pData+0x72, szBack, 46 );
	 memcpy( (ND_UCHAR*)pData+0x18, backup18, 8 );
	 memcpy( (ND_UCHAR*)pData+0x32, backup32, 20 );
	 memcpy( (ND_UCHAR*)pData+0x58, backup32, 20 );
 }

//设置LOCATIONS校验
//返回：成功返回CheckSum字节大小，否则返回<0，错误号
int HashVerify::SetLocationCBK(wstring iTunesPath, ND_UCHAR ** ppCheckSum ,int Hashtype)
{
	int hr=0;
	Location_CBK_Buffer cBuffer;	

	memset( &cBuffer, 0, sizeof(cBuffer) );

	cBuffer.m_iITunesCBKHeadSize = GetiTunesDeviceiTunesCbkSize( Hashtype );
	if( cBuffer.m_iITunesCBKHeadSize == 0 )
	{
		hr = E_NOTIMPL;
		goto ret_here;
	}

	hr = SetLocationCbkDbChecksum(  iTunesPath + L"\\iTunes\\iTunes Library.itlp\\Locations.itdb", &cBuffer );
	if( FAILED(hr) )
		goto ret_here;

	hr = MakeLocationCbkiTunesChecksum( iTunesPath + L"\\param.bin",&cBuffer );
	if( FAILED(hr) )
		goto ret_here;

ret_here:
	if( FAILED(hr) )
	{
		if( cBuffer.m_pCheckSumB )
			free( cBuffer.m_pCheckSumB );
		*ppCheckSum = NULL;
		hr = 0;
	}
	else
	{
		*ppCheckSum = cBuffer.m_pCheckSumB;
		hr = cBuffer.m_iCheckSumSize;
	}
	return hr;
}

int HashVerify::checkSqlDb(wstring iTunesPath,int Hashtype)
{
	//if(iPodFileSync::GetDBVer()>=430)//430校验方式不一样
	//	return 0;
	ND_UCHAR * pChecksumStr = NULL;
	FILE * pFile = NULL;
	ND_UINT32 iSize;

	USES_CONVERSION;

	wstring KeyFile = iTunesPath + L"\\param.bin";
	if(!CFileOperation::IsFileExist(KeyFile.c_str()))
			MakeCbkKeyFile(iTunesPath);

	iSize  = SetLocationCBK( iTunesPath, &pChecksumStr, Hashtype);
	if(iSize<=0)
		return  E_FAIL; 

	wstring CBKPath = iTunesPath + L"\\iTunes\\iTunes Library.itlp\\Locations.itdb.cbk";
	 fopen_s( &pFile,W2A(CBKPath.c_str()), "wb" );
	if( !pFile )
	{
		free(pChecksumStr);
	   return E_FAIL; 
	}

	if( fwrite( pChecksumStr, iSize, 1, pFile ) != 1 )
	{
		free(pChecksumStr);
		return E_FAIL;
	}
	free(pChecksumStr);

	if( pFile )
		fclose( pFile );

	return S_OK;
}



//获取iTunes Device所需校验Buffer大小
ND_SIZE HashVerify::GetiTunesDeviceiTunesCbkSize( int m_iHashType )
{
	switch( m_iHashType )
	{
	
	case ENUM_HASH_TYPE_HASH58:
	case ENUM_HASH_TYPE_HASH72:
		return ND_HASH72_BUFFER_SIZE;	
	case 4:
	case ENUM_HASH_TYPE_HASHAB:
		return ND_HASHAB_BUFFER_SIZE;
	}
	return 0;
}

//从文件中获取一帧
ND_INT HashVerify::GetLocationDbFrame( FILE * pFile, ND_UCHAR * pFrame )
{
	//sqlitedb都是1024对齐
	ND_INT read_count = fread( pFrame, SHA_BLOCK_SIZE, 1, pFile );
	if( ( read_count != 1 ) )
	{
		if( !feof( pFile ) )
		{
			//文件异常
		}
		return 0;
	}
	return SHA_BLOCK_SIZE;
}

//CheckSum一帧
int HashVerify::GetChecksum( ND_UCHAR * pFrame, ND_SIZE iFrameSize, ND_UCHAR *pszSha1 )
{
	ND_SIZE iSha1Len;

	ND_SIZE iCheckNumLen = GetChecksumStringLen(ENUM_CHECKSUM_TYPE (m_iCBKAlg) );
	ND_CheckSum * pCS = CreateChecksumObject(ENUM_CHECKSUM_TYPE (m_iCBKAlg) );
	if( !pCS )
		return E_OUTOFMEMORY;

	//GetChecksumUpdate( pCS, pFrame, SHA_BLOCK_SIZE );
	GetChecksumUpdate( pCS, pFrame, iFrameSize );
	GetChecksumDigest( pCS, pszSha1, &iSha1Len );
	ReleaseChecksumObject( pCS );

	return S_OK;
}

//获取Locations.itdb.cbk的数据库校验值
int HashVerify::SetLocationCbkDbChecksum( wstring filename,Location_CBK_Buffer * pCBK )
{
	USES_CONVERSION;
	int hr;
	ND_UCHAR * pBuffer = NULL;
	
	ND_SIZE iSize = 0, iLen;
	ND_UCHAR sha1[SHA1_DIGEST_LEN];
	ND_UCHAR * p;
	ND_UCHAR * pFrame = NULL;

	if( pCBK->m_pCheckSumB )
		free( pCBK->m_pCheckSumB );

	pBuffer = (ND_UCHAR*)malloc( pCBK->m_iITunesCBKHeadSize + SHA1_DIGEST_LEN + SHA_BLOCK_SIZE );
	if( !pBuffer )
		return E_OUTOFMEMORY;
	memset( pBuffer, 0, pCBK->m_iITunesCBKHeadSize + SHA1_DIGEST_LEN + SHA_BLOCK_SIZE );
	pCBK->m_iBufferSize = pCBK->m_iITunesCBKHeadSize + SHA1_DIGEST_LEN + SHA_BLOCK_SIZE;
	pCBK->m_iCheckSumSize = pCBK->m_iITunesCBKHeadSize + SHA1_DIGEST_LEN;

	p = (ND_UCHAR*)( pBuffer + pCBK->m_iCheckSumSize );
	pFrame = (ND_UCHAR*)malloc( SHA_BLOCK_SIZE );
	if( !pFrame )
	{
		hr = E_OUTOFMEMORY;
		goto ret_here;
	}

	//打开Locations.itdb数据库文件
	FILE * fp = NULL;
	int error =  _wfopen_s( &fp,filename.c_str(), L"rb" );
    unsigned long res = GetLastError();
	 
	if( !fp )
	{
		hr = E_INVALIDARG;
		goto ret_here;
	}

	//计算Locations.itdb的sha1数组
	do
	{
		iSize = GetLocationDbFrame( fp, pFrame );
		if( iSize > 0 )
		{
			hr = GetChecksum(  pFrame, iSize, sha1 );
			if( FAILED(hr) )
				goto ret_here;

			if( pCBK->m_iBufferSize >= pCBK->m_iCheckSumSize + SHA1_DIGEST_LEN )
			{
				memcpy( p, sha1, SHA1_DIGEST_LEN );
				p += SHA1_DIGEST_LEN;
			}
			else
			{
				iLen = pBuffer - p;
				pBuffer = (ND_UCHAR*)realloc( pBuffer, pCBK->m_iBufferSize+SHA_BLOCK_SIZE );
				if( !pBuffer )
				{
					hr = E_OUTOFMEMORY;
					goto ret_here;
				}
				pCBK->m_iBufferSize += SHA_BLOCK_SIZE;
				p = pBuffer+iLen;
				memcpy( p, sha1, SHA1_DIGEST_LEN );
				p += SHA1_DIGEST_LEN;
			}
			pCBK->m_iCheckSumSize += SHA1_DIGEST_LEN;
		}
	}while( iSize == SHA_BLOCK_SIZE );

	//计算sha1数组的sha1校验
	p = pBuffer+pCBK->m_iITunesCBKHeadSize+SHA1_DIGEST_LEN;
	hr = GetChecksum(  p, pCBK->m_iCheckSumSize-pCBK->m_iITunesCBKHeadSize-SHA1_DIGEST_LEN, sha1 );
	if( FAILED(hr) )
		goto ret_here;
	memcpy( pBuffer+pCBK->m_iITunesCBKHeadSize, sha1, SHA1_DIGEST_LEN );

	hr = S_OK;

ret_here:
	if( pFrame )
		free( pFrame );
	if( fp )
		fclose( fp );
	if( hr == S_OK )
	{		
		pCBK->m_pCheckSumB = pBuffer;
	}
	else
	{
		pCBK->m_iBufferSize = 0;
		pCBK->m_iCheckSumSize = 0;
		if( pBuffer )
			free( pBuffer );
	}
	return hr;
}

//获取Locations.itdb.cbk的iTunes校验值
int HashVerify::MakeLocationCbkiTunesChecksum(wstring filename, Location_CBK_Buffer * pCBK )
{
	USES_CONVERSION;
	ND_UCHAR szParam1[16];
	ND_UCHAR szParam2[12];
	FILE * fp = NULL;
	 fopen_s(&fp, W2A(filename.c_str()), "rb" ); //"D:\\ITUNES\\Media\\iTunes_Control\\param.bin"
	fread( szParam1, 16, 1, fp );
	fread( szParam2, 12, 1, fp );
	ND_Hash72Generate( pCBK->m_pCheckSumB, pCBK->m_pCheckSumB+46, szParam1, szParam2 );
	return S_OK;
}

int HashVerify::CheckCBDFile( wstring iTunesPath )
{
	if(iPodFileSync::GetDBVer()>=430)
						return S_OK;
	USES_CONVERSION;
	FILE * fp = NULL;
	wstring CBDPath = iTunesPath + L"\\iTunes\\iTunesCDB";
	if(fopen_s(&fp, W2A(CBDPath.c_str()), "rb" ) !=0)
		return 0;
	unsigned char * pszBuffer = NULL;
	int iPos = 0;
	unsigned char szSHA1[20];
	unsigned char szParam1[16];
	unsigned char szParam2[12];	

	fseek( fp, 0, SEEK_END );
	iPos = ftell( fp );
	pszBuffer = new unsigned char[iPos];
	memset( pszBuffer, 0, iPos );

	fseek( fp, 0, SEEK_SET );
	if( fread( pszBuffer, iPos, 1, fp ) != 1 )
		return E_FAIL;
	fclose( fp );

	CheckSumITunesCDBData( pszBuffer, iPos, szSHA1, szParam1, szParam2 );

	//CBDPath += L"1"; 
	fopen_s( &fp,W2A(CBDPath.c_str()), "wb" );
	if( !fp )
		return E_FAIL; 

	if( (fwrite( pszBuffer, iPos, 1, fp ) ) != 1 )
	{
		return E_FAIL;
	}
	fclose( fp );

	SAFE_DELETE_ARRAY(pszBuffer);

	return S_OK;
	
}


void HashVerify::MakeCbkKeyFile( wstring iTunesPath )
{
	USES_CONVERSION;
	FILE * fp = NULL;
	wstring CBKPath = iTunesPath + L"\\iTunes\\iTunes Library.itlp\\Locations.itdb.cbk";
	fopen_s(&fp, W2A(CBKPath.c_str()), "rb" );
	unsigned char szBuf1[46];
	unsigned char szBuf2[20];
	unsigned char szBuf3[16];
	unsigned char szBuf4[12];

	unsigned char szBuf21[46];
	wstring KeyFilePath = iTunesPath + L"\\param.bin";
	FILE * fout = NULL;
	fopen_s(&fout, W2A(KeyFilePath.c_str()), "wb" );

	if( fp )
	{
		fread( szBuf1, 46, 1, fp );
		fread( szBuf2, 20, 1, fp );
		ND_Hash72Extract( szBuf1, szBuf2, szBuf3, szBuf4 );
		memset( szBuf21, 0, 46 );
		ND_Hash72Generate( szBuf21, szBuf2, szBuf3, szBuf4 );
		
		fwrite( szBuf3, 16, 1, fout );
		fwrite( szBuf4, 12, 1, fout );
		fclose( fp );
		fclose( fout );
	}
}


#endif