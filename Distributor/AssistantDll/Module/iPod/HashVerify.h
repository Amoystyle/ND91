#pragma  once

#include "Module/Common/Media/Utils/iTunesMP3Lib_Type.h"

namespace ND91Assistant
{

	typedef struct __tag_location_cbk_buffer
	{
		ND_SIZE m_iSize;					//包大小
		ND_SIZE m_iITunesCBKHeadSize;		//iTunes CBK 头大小
		ND_SIZE m_iBufferSize;				//m_pCheckSumB空间大小
		ND_SIZE m_iCheckSumSize;			//m_pCheckSumB校验字符串有效大小
		ND_UCHAR * m_pCheckSumB;			//缓冲区
	}Location_CBK_Buffer;

	//对iTunes进行修改后需要进行的哈希码校验，分为cdb文件校验跟数据库文件校验。
	class HashVerify
	{
	public:		
		
		//************************************
		// Method:    校验iTunesCBD文件
		// Returns:   int 校验结果
		// Parameter: wstring iTunesPath 文件路径
		//************************************
		static int CheckCBDFile( wstring iTunesPath );
		//************************************
		// Method:    校验localtion数据库
		// Returns:   int
		// Parameter: wstring iTunesPath 文件路径
		// Parameter: int Hashtype 校验方式
		//************************************
		static int checkSqlDb(wstring iTunesPath,int Hashtype);

		static void BackCheckSumiTunesCDB(void * pData, ND_UINT32 iDataSize, ND_UCHAR szParam1[16], ND_UCHAR szParam2[12]);

		static void HashVerify::MakeCheckSumiTunesCDB(void * pData, ND_UINT32 iDataSize, ND_UCHAR szParam1[16], ND_UCHAR szParam2[12]);

	private:
		//生成cbk校验文件校验码
		static int SetLocationCbkDbChecksum( wstring filename,Location_CBK_Buffer * pCBK );
		//cdb文件校验校验码
		static int MakeLocationCbkiTunesChecksum( wstring filename,Location_CBK_Buffer * pCBK );
		//根据校验文件key文件获得校验key值
		static int GetChecksum( ND_UCHAR * pFrame, ND_SIZE iFrameSize, ND_UCHAR *pszSha1 );
		//获得数据库文件校验帧
		static ND_INT GetLocationDbFrame( FILE * pFile, ND_UCHAR * pFrame );
		//获得cbk校验文件大小
		static ND_SIZE GetiTunesDeviceiTunesCbkSize( int Hashtype);
		//生成cdk校验文件
		static int SetLocationCBK(wstring iTunesPath, ND_UCHAR ** ppCheckSum ,int Hashtype);	
		//使用校验码生成cdb文件校验数据
		static int CheckSumITunesCDBData( void * pData, ND_UINT32 iDataSize, ND_UCHAR szSHA1[20], ND_UCHAR szParam1[16], ND_UCHAR szParam2[12] );
		//校验cdk文件的key文件
		static void MakeCbkKeyFile( wstring iTunesPath );

		

	private:
	//static const int	m_iHashType = 2;
	static const int m_iCBKAlg = 0;
	};
}