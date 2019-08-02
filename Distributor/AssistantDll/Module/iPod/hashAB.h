
#pragma  once
#include "AssistantDll.h"
#include <string>
using namespace std;
namespace ND91Assistant
{
	class HashAB
	{
	public:

	//	HashAB();


		BOOL InitInstance();

		bool SetUuidAndHash(char* puuid, int nIsHashAB);

		bool CaclLocationCDK(char* Locationpath, char* cbkpath);

		BOOL ReSetiTunesCDB(char* shal,char* iTunesCDBPath);

		void GetiTunesCDBFilePath(char* iTunesCDBPath);
	private:

		int  MyWriteFile(const void *Str, size_t Count, int a3, FILE *File);

		int Caclbuffone(DWORD a1);
		int  Caclbufftwo(DWORD a1, DWORD a2);
		int  Caclbuffthree(DWORD a1, const char *a2, DWORD a3);
		int  Caclbuff(DWORD a1, DWORD a2);
		int  SetSendParam(DWORD a1, DWORD a2);
		signed int  CaclHashFromSever(DWORD a1, DWORD a2, char* a3, DWORD a4, char* a5, DWORD a6, DWORD a7, DWORD a8);

		FILE * SetFileToAddr(const char *Filename);
		int  Caclsendbuff(const char *a1, unsigned int a2, int a3);
		signed int  CreateiTunesCDB(char* a1, const char *Filename);

		void GetSandSign();

		BOOL  CreateLocationCBK(char* a1,  char *a3, char* path);

		string Char2Hex(string &strSrc);

		string CharStr2HexStr(const char* pszSrc);		
	};
}