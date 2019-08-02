#pragma  once

#include "Module/Common/Media/Utils/iTunesMP3Lib_Type.h"

namespace ND91Assistant
{

	typedef struct __tag_location_cbk_buffer
	{
		ND_SIZE m_iSize;					//����С
		ND_SIZE m_iITunesCBKHeadSize;		//iTunes CBK ͷ��С
		ND_SIZE m_iBufferSize;				//m_pCheckSumB�ռ��С
		ND_SIZE m_iCheckSumSize;			//m_pCheckSumBУ���ַ�����Ч��С
		ND_UCHAR * m_pCheckSumB;			//������
	}Location_CBK_Buffer;

	//��iTunes�����޸ĺ���Ҫ���еĹ�ϣ��У�飬��Ϊcdb�ļ�У������ݿ��ļ�У�顣
	class HashVerify
	{
	public:		
		
		//************************************
		// Method:    У��iTunesCBD�ļ�
		// Returns:   int У����
		// Parameter: wstring iTunesPath �ļ�·��
		//************************************
		static int CheckCBDFile( wstring iTunesPath );
		//************************************
		// Method:    У��localtion���ݿ�
		// Returns:   int
		// Parameter: wstring iTunesPath �ļ�·��
		// Parameter: int Hashtype У�鷽ʽ
		//************************************
		static int checkSqlDb(wstring iTunesPath,int Hashtype);

		static void BackCheckSumiTunesCDB(void * pData, ND_UINT32 iDataSize, ND_UCHAR szParam1[16], ND_UCHAR szParam2[12]);

		static void HashVerify::MakeCheckSumiTunesCDB(void * pData, ND_UINT32 iDataSize, ND_UCHAR szParam1[16], ND_UCHAR szParam2[12]);

	private:
		//����cbkУ���ļ�У����
		static int SetLocationCbkDbChecksum( wstring filename,Location_CBK_Buffer * pCBK );
		//cdb�ļ�У��У����
		static int MakeLocationCbkiTunesChecksum( wstring filename,Location_CBK_Buffer * pCBK );
		//����У���ļ�key�ļ����У��keyֵ
		static int GetChecksum( ND_UCHAR * pFrame, ND_SIZE iFrameSize, ND_UCHAR *pszSha1 );
		//������ݿ��ļ�У��֡
		static ND_INT GetLocationDbFrame( FILE * pFile, ND_UCHAR * pFrame );
		//���cbkУ���ļ���С
		static ND_SIZE GetiTunesDeviceiTunesCbkSize( int Hashtype);
		//����cdkУ���ļ�
		static int SetLocationCBK(wstring iTunesPath, ND_UCHAR ** ppCheckSum ,int Hashtype);	
		//ʹ��У��������cdb�ļ�У������
		static int CheckSumITunesCDBData( void * pData, ND_UINT32 iDataSize, ND_UCHAR szSHA1[20], ND_UCHAR szParam1[16], ND_UCHAR szParam2[12] );
		//У��cdk�ļ���key�ļ�
		static void MakeCbkKeyFile( wstring iTunesPath );

		

	private:
	//static const int	m_iHashType = 2;
	static const int m_iCBKAlg = 0;
	};
}