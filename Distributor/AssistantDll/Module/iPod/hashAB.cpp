#include "stdafx.h"
#ifdef MODULE_IPOD

#include "hashAB.h"
#include "Module/Common/Media/Utils/checksum.h"

#include <atlconv.h>
#include <winsock.h>
#include<sys/stat.h>
#include<sys/types.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning (disable:4244)
#pragma warning (disable:4018)
char g_uuid[24] = {0x14, 0x00, 0x00, 0x00};
char g_szrand[]="abcdefghigklmnopqrstuvwxyz123456789";
int g_nIsHashAB = 0;//判断是否用hashab, true表示是用hashab算法，false用hash72

DWORD g_dwsandSign = 0;
DWORD g_dwOther = 0;
DWORD g_dwunk = 0;

DWORD dword_100A9AE8 = 0xD01BB091;//0x6EAF393B;//0xD01BB091
DWORD dword_100A9AF0 = 0xE508E6E1;//0xA2E91CEA;//0xE508E6E1
DWORD dword_100A9AF8 = 0xFFFF15C7;
DWORD dword_100A9B00 = 0x000A0019;
BYTE byte_100A8DC4 = 0;	

typedef void (*calcHashAB_t)(unsigned char* sha1, const unsigned char* uuid, unsigned char* target, unsigned char* rnd_bytes/*ABCDEFGHIJKLMNOPQRSTUVW*/ );

calcHashAB_t g_calc_hashAB;

string HashAB::Char2Hex(string &strSrc)
{
    string strout = "";
	const char* a = strSrc.c_str();
	size_t i = 0;
	for(i=0; i<strSrc.size(); i++)
	{ 
		
		string str = "";
		int n = a[i];
		for (int i=2*sizeof(char) - 1; i>=0; i--)
		{
			str += "0123456789ABCDEF"[((n >> i*4) & 0xF)];
		}

		strout = strout + str;
	}
    return strout;
}

string HashAB::CharStr2HexStr(const char* pszSrc)
{
	string str;
	str = pszSrc;
    return Char2Hex(str);
}

//sub_1003CB60
int  HashAB::MyWriteFile(const void *Str, size_t Count, int a3, FILE *File)
{
  *(DWORD *)a3 = fwrite(Str, 1u, Count, File);
  return 0;
}


//sub_10045430
int HashAB::Caclbuffone(DWORD a1)
{
  int result; // eax@1

  result = a1;
  *(DWORD *)a1 = 0x67452301;
  *(DWORD *)(a1 + 4) = 0xEFCDAB89;
  *(DWORD *)(a1 + 8) = 0x98BADCFE;
  *(DWORD *)(a1 + 12) = 0x10325476;
  *(DWORD *)(a1 + 16) = 0xC3D2E1F0;
  *(DWORD *)(a1 + 24) = 0;
  *(DWORD *)(a1 + 20) = 0;
  return result;
}

//sub_100437D0
int  HashAB::Caclbufftwo(DWORD a1, DWORD a2)
{
   long result = 0; // eax@1
  long v3 = 0; // ecx@1
  long v4 = 0; // ebx@1
  long v5 = 0; // eax@1
  long v6 = 0; // ST4C_4@1
  unsigned long v7 = 0; // ST10_4@1
  long v8 = 0; // edi@1
  long v9 = 0; // ST50_4@1
  unsigned long v10 = 0; // edi@1
  long v11 = 0; // ST34_4@1
  unsigned long v12 = 0; // ST14_4@1
  long v13 = 0; // ST10_4@1
  long v14 = 0; // ST38_4@1
  unsigned long v15 = 0; // esi@1
  long v16 = 0; // ST18_4@1
  long v17 = 0; // ST3C_4@1
  long v18 = 0; // ST14_4@1
  long v19 = 0; // ST40_4@1
  unsigned long v20 = 0; // edi@1
  long v21 = 0; // ST44_4@1
  unsigned long v22 = 0; // ST18_4@1
  long v23 = 0; // ST48_4@1
  unsigned long v24 = 0; // esi@1
  long v25 = 0; // ST10_4@1
  long v26 = 0; // ST20_4@1
  long v27 = 0; // ST18_4@1
  long v28 = 0; // ST1C_4@1
  unsigned long v29 = 0; // edi@1
  long v30 = 0; // ST14_4@1
  long v31 = 0; // ST28_4@1
  unsigned long v32 = 0; // ST10_4@1
  long v33 = 0; // ST2C_4@1
  unsigned long v34 = 0; // esi@1
  long v35 = 0; // ST30_4@1
  unsigned long v36 = 0; // ST14_4@1
  long v37 = 0; // ST10_4@1
  long v38 = 0; // ST24_4@1
  unsigned long v39 = 0; // edi@1
  long v40 = 0; // ebx@1
  long v41 = 0; // edx@1
  long v42 = 0; // ST14_4@1
  long v43 = 0; // esi@1
  unsigned long v44 = 0; // ST10_4@1
  unsigned long v45 = 0; // ST4C_4@1
  unsigned long v46 = 0; // ST18_4@1
  unsigned long v47 = 0; // ST50_4@1
  unsigned long v48 = 0; // ST14_4@1
  long v49 = 0; // ST10_4@1
  unsigned long v50 = 0; // ST50_4@1
  long v51 = 0; // ST18_4@1
  unsigned long v52 = 0; // ST50_4@1
  long v53 = 0; // ST14_4@1
  unsigned long v54 = 0; // ST50_4@1
  unsigned long v55 = 0; // ST10_4@1
  unsigned long v56 = 0; // edi@1
  unsigned long v57 = 0; // ST50_4@1
  unsigned long v58 = 0; // ST18_4@1
  unsigned long v59 = 0; // edi@1
  unsigned long v60 = 0; // ST50_4@1
  unsigned long v61 = 0; // ST14_4@1
  long v62 = 0; // ST10_4@1
  unsigned long v63 = 0; // ST50_4@1
  long v64 = 0; // ST18_4@1
  unsigned long v65 = 0; // ST50_4@1
  long v66 = 0; // ST14_4@1
  long v67 = 0; // edi@1
  long v68 = 0; // ebp@1
  unsigned long v69 = 0; // ST10_4@1
  long v70 = 0; // edi@1
  long v71 = 0; // ebp@1
  unsigned long v72 = 0; // ebp@1
  long v73 = 0; // edi@1
  long v74 = 0; // ebx@1
  unsigned long v75 = 0; // ST14_4@1
  long v76 = 0; // ST10_4@1
  unsigned long v77 = 0; // ST50_4@1
  long v78 = 0; // ST18_4@1
  unsigned long v79 = 0; // ST50_4@1
  unsigned long v80 = 0; // edi@1
  long v81 = 0; // edx@1
  long v82 = 0; // ST14_4@1
  unsigned long v83 = 0; // edx@1
  long v84 = 0; // esi@1
  long v85 = 0; // esi@1
  unsigned long v86 = 0; // esi@1
  long v87 = 0; // edi@1
  long v88 = 0; // ST20_4@1
  unsigned long v89 = 0; // edi@1
  long v90 = 0; // ebx@1
  unsigned long v91 = 0; // edx@1
  unsigned long v92 = 0; // ST1C_4@1
  long v93 = 0; // edx@1
  long v94 = 0; // ST10_4@1
  long v95 = 0; // ST20_4@1
  unsigned long v96 = 0; // ebx@1
  unsigned long v97 = 0; // edx@1
  unsigned long v98 = 0; // ST20_4@1
  long v99 = 0; // ebp@1
  long v100 = 0; // edx@1
  long v101 = 0; // ST24_4@1
  long v102 = 0; // ST18_4@1
  unsigned long v103 = 0; // ebp@1
  unsigned long v104 = 0; // ST24_4@1
  long v105 = 0; // ST14_4@1
  unsigned long v106 = 0; // edi@1
  long v107 = 0; // edx@1
  long v108 = 0; // ST24_4@1
  unsigned long v109 = 0; // ebx@1
  long v110 = 0; // edx@1
  long v111 = 0; // ST24_4@1
  unsigned long v112 = 0; // ebp@1
  unsigned long v113 = 0; // ST24_4@1
  long v114 = 0; // ST10_4@1
  unsigned long v115 = 0; // edx@1
  unsigned long v116 = 0; // ST24_4@1
  long v117 = 0; // ST18_4@1
  unsigned long v118 = 0; // esi@1
  long v119 = 0; // edi@1
  long v120 = 0; // ST14_4@1
  long v121 = 0; // ST24_4@1
  unsigned long v122 = 0; // edi@1
  long v123 = 0; // ebx@1
  long v124 = 0; // edx@1
  unsigned long v125 = 0; // ST24_4@1
  unsigned long v126 = 0; // edx@1
  long v127 = 0; // ebx@1
  long v128 = 0; // esi@1
  unsigned long v129 = 0; // ST24_4@1
  unsigned long v130 = 0; // esi@1
  long v131 = 0; // ebx@1
  long v132 = 0; // edi@1
  unsigned long v133 = 0; // ST24_4@1
  unsigned long v134 = 0; // edi@1
  long v135 = 0; // ST18_4@1
  unsigned long v136 = 0; // ST24_4@1
  unsigned long v137 = 0; // edx@1
  long v138 = 0; // ST14_4@1
  unsigned long v139 = 0; // ST24_4@1
  unsigned long v140 = 0; // esi@1
  long v141 = 0; // ebx@1
  long v142 = 0; // ebp@1
  long v143 = 0; // edi@1
  unsigned long v144 = 0; // edi@1
  long v145 = 0; // ebx@1
  long v146 = 0; // ebp@1
  long v147 = 0; // edx@1
  unsigned long v148 = 0; // edx@1
  long v149 = 0; // ST10_4@1
  unsigned long v150 = 0; // ST20_4@1
  unsigned long v151 = 0; // esi@1
  long v152 = 0; // ebx@1
  unsigned long v153 = 0; // edi@1
  unsigned long v154 = 0; // ST1C_4@1
  long v155 = 0; // edi@1
  long v156 = 0; // ST20_4@1
  long v157 = 0; // ST18_4@1
  unsigned long v158 = 0; // edi@1
  long v159 = 0; // ebx@1
  unsigned long v160 = 0; // edx@1
  unsigned long v161 = 0; // ST20_4@1
  long v162 = 0; // edx@1
  long v163 = 0; // ST24_4@1
  long v164 = 0; // ST14_4@1
  unsigned long v165 = 0; // edx@1
  long v166 = 0; // ebx@1
  long v167 = 0; // esi@1
  unsigned long v168 = 0; // ST24_4@1
  unsigned long v169 = 0; // esi@1
  long v170 = 0; // ebx@1
  long v171 = 0; // edi@1
  unsigned long v172 = 0; // ST24_4@1
  unsigned long v173 = 0; // edi@1
  long v174 = 0; // ebx@1
  long v175 = 0; // edx@1
  unsigned long v176 = 0; // ST24_4@1
  unsigned long v177 = 0; // edx@1
  long v178 = 0; // ST18_4@1
  unsigned long v179 = 0; // ST24_4@1
  unsigned long v180 = 0; // esi@1
  long v181 = 0; // ST14_4@1
  unsigned long v182 = 0; // ST24_4@1
  unsigned long v183 = 0; // edi@1
  long v184 = 0; // ebx@1
  long v185 = 0; // edx@1
  unsigned long v186 = 0; // ST24_4@1
  unsigned long v187 = 0; // edx@1
  long v188 = 0; // ebx@1
  long v189 = 0; // esi@1
  unsigned long v190 = 0; // ST24_4@1
  unsigned long v191 = 0; // esi@1
  long v192 = 0; // ebx@1
  long v193 = 0; // edi@1
  long v194 = 0; // ST10_4@1
  unsigned long v195 = 0; // ST24_4@1
  unsigned long v196 = 0; // edi@1
  long v197 = 0; // ebx@1
  long v198 = 0; // edx@1
  long v199 = 0; // ST18_4@1
  unsigned long v200 = 0; // ST24_4@1
  unsigned long v201 = 0; // edx@1
  long v202 = 0; // ST14_4@1
  unsigned long v203 = 0; // ST24_4@1
  unsigned long v204 = 0; // esi@1
  unsigned long v205 = 0; // ST24_4@1
  long v206 = 0; // ebp@1
  long v207 = 0; // ebx@1
  long v208 = 0; // edx@1
  unsigned long v209 = 0; // edi@1
  unsigned long v210 = 0; // edx@1
  long v211 = 0; // ebx@1
  long v212 = 0; // esi@1
  unsigned long v213 = 0; // ST30_4@1
  unsigned long v214 = 0; // esi@1
  unsigned long v215 = 0; // ST20_4@1
  long v216 = 0; // ST18_4@1
  unsigned long v217 = 0; // edi@1
  unsigned long v218 = 0; // ST20_4@1
  long v219 = 0; // ST14_4@1
  unsigned long v220 = 0; // edx@1
  unsigned long v221 = 0; // ST24_4@1
  unsigned long v222 = 0; // esi@1
  long v223 = 0; // edi@1
  long v224 = 0; // ST24_4@1
  unsigned long v225 = 0; // edi@1
  long v226 = 0; // ebx@1
  long v227 = 0; // edx@1
  long v228 = 0; // ST10_4@1
  unsigned long v229 = 0; // ST24_4@1
  unsigned long v230 = 0; // edx@1
  long v231 = 0; // ebx@1
  long v232 = 0; // esi@1
  long v233 = 0; // ST18_4@1
  unsigned long v234 = 0; // ST24_4@1
  unsigned long v235 = 0; // esi@1
  unsigned long v236 = 0; // ST24_4@1
  long v237 = 0; // ST14_4@1
  long v238 = 0; // ebp@1
  long v239 = 0; // ebx@1
  long v240 = 0; // edx@1
  unsigned long v241 = 0; // edi@1
  unsigned long v242 = 0; // ST24_4@1
  unsigned long v243 = 0; // edx@1
  long v244 = 0; // esi@1
  long v245 = 0; // ST24_4@1
  unsigned long v246 = 0; // esi@1
  long v247 = 0; // ebx@1
  long v248 = 0; // edi@1
  long v249 = 0; // ST10_4@1
  unsigned long v250 = 0; // ST24_4@1
  unsigned long v251 = 0; // edi@1
  long v252 = 0; // ebx@1
  long v253 = 0; // edx@1
  unsigned long v254 = 0; // ST24_4@1
  unsigned long v255 = 0; // edx@1
  unsigned long v256 = 0; // ST24_4@1
  long v257 = 0; // ST14_4@1
  unsigned long v258 = 0; // esi@1
  unsigned long v259 = 0; // ST24_4@1
  unsigned long v260 = 0; // edi@1
  long v261 = 0; // edx@1
  long v262 = 0; // ST24_4@1
  unsigned long v263 = 0; // edx@1
  unsigned long v264 = 0; // ST24_4@1
  long v265 = 0; // ST10_4@1
  unsigned long v266 = 0; // ebp@1
  long v267 = 0; // ebx@1
  long v268 = 0; // esi@1
  long v269 = 0; // edi@1
  long v270 = 0; // eax@1
  long v271 = 0; // edi@1
  long v272 = 0; // esi@1
  long v273 = 0; // eax@1
  long v274 = 0; // edx@1
  long v275 = 0; // esi@1
  long v276 = 0; // eax@1
  unsigned long v277 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v278 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v279 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  unsigned long v280 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v281 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v282 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  unsigned long v283 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v284 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  unsigned long v285 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v286 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  unsigned long v287 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v288 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v289 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v290 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v291 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v292 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v293 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v294 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v295 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v296 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v297 = 0; // [sp+58h] [bp-6ED9EB49h]@1
  long v298 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  unsigned long v299 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v300 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v301 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  unsigned long v302 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v303 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  unsigned long v304 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v305 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  unsigned long v306 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v307 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v308 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v309 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v310 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v311 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v312 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v313 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v314 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v315 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v316 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v317 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v318 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1
  long v319 = 0; // [sp+5Ch] [bp-6ED9EB45h]@1

  v3 = a1;
  v4 = *(DWORD *)(a1 + 8);
  v277 = *(DWORD *)(a1 + 4);
  v5 = a2;
  v6 = (*(DWORD *)a2 >> 24) | ((*(DWORD *)a2 & 0xFF00) << 8) | ((*(DWORD *)a2 >> 8) | (*(DWORD *)a2 << 24)) & 0xFF00FF00;

  v7 = ((*(DWORD *)a2 >> 24) | ((*(DWORD *)a2 & 0xFF00) << 8) | ((*(DWORD *)a2 >> 8) | (*(DWORD *)a2 << 24)) & 0xFF00FF00)
     + ( *(DWORD *)v3 << 5 | (*(DWORD *)v3 >> 27))
     + (*(DWORD *)(v3 + 12) ^ v277 & (v4 ^ *(DWORD *)(v3 + 12)))
     + 0x5A827999
     + *(DWORD *)(v3 + 16);

  v8 = *(DWORD *)v3 & (((v277 >> 2) | (v277 << 30)) ^ *(DWORD *)(v3 + 8));

  v278 = (v277 >> 2) | (v277 << 30);

  v9 = (*(DWORD *)(a2 + 4) >> 24) | ((*(DWORD *)(a2 + 4) & 0xFF00) << 8) | ((*(DWORD *)(a2 + 4) >> 8) | (*(DWORD *)(a2 + 4) << 24)) & 0xFF00FF00;

  v10 = ((*(DWORD *)(a2 + 4) >> 24) | ((*(DWORD *)(a2 + 4) & 0xFF00) << 8) | ((*(DWORD *)(a2 + 4) >> 8) | (*(DWORD *)(a2 + 4) << 24)) & 0xFF00FF00)
      + (v7 << 5 | (v7 >> 27))
      + (*(DWORD *)(v3 + 8) ^ v8)
      + 0x5A827999
      + *(DWORD *)(v3 + 12);

  v298 = (*(DWORD *)v3 >> 2) | (*(DWORD *)v3 << 30);

  v11 = (*(DWORD *)(v5 + 8) >> 24) | ((*(DWORD *)(v5 + 8) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 8) >> 8) | (*(DWORD *)(v5 + 8) << 24)) & 0xFF00FF00;

  v12 = v11 + (32 * v10 | (v10 >> 27)) + (v278 ^ v7 & (v298 ^ v278)) + 0x5A827999 + *(DWORD *)(v3 + 8);

  v13 = (v7 >> 2) | (v7 << 30);

  v14 = (*(DWORD *)(v5 + 12) >> 24) | ((*(DWORD *)(v5 + 12) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 12) >> 8) | (*(DWORD *)(v5 + 12) << 24)) & 0xFF00FF00;

  v15 = v278 + v14 + (v12 << 5 | (v12 >> 27)) + (v298 ^ v10 & (v298 ^ v13)) + 0x5A827999;

  v16 = (v10 >> 2) | (v10 << 30);

  v17 = (*(DWORD *)(v5 + 16) >> 24) | ((*(DWORD *)(v5 + 16) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 16) >> 8) | (*(DWORD *)(v5 + 16) << 24)) & 0xFF00FF00;

  v299 = v17 + (v15 << 5 | (v15 >> 27)) + (v13 ^ v12 & (v16 ^ v13)) + 0x5A827999 + v298;//0x2d6963d

  v18 = (v12 >> 2) | (v12 << 30);

  v19 = (*(DWORD *)(v5 + 20) >> 24) | ((*(DWORD *)(v5 + 20) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 20) >> 8) | (*(DWORD *)(v5 + 20) << 24)) & 0xFF00FF00;

  v20 = v13 + v19 + (v299 << 5 | (v299 >> 27)) + (v16 ^ v15 & (v18 ^ v16)) + 0x5A827999;

  v279 = (v15 >> 2) | (v15 << 30);//0x2d696b5

  v21 = (*(DWORD *)(v5 + 24) >> 24) | ((*(DWORD *)(v5 + 24) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 24) >> 8) | (*(DWORD *)(v5 + 24) << 24)) & 0xFF00FF00;

  v22 = v21 + (v20 << 5 | (v20 >> 27)) + (v18 ^ v299 & (v279 ^ v18)) + 0x5A827999 + v16;

  v300 = (v299 >> 2) | (v299 << 30);//0x2d69724

  v23 = (*(DWORD *)(v5 + 28) >> 24) | ((*(DWORD *)(v5 + 28) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 28) >> 8) | (*(DWORD *)(v5 + 28) << 24)) & 0xFF00FF00;

  v24 = v18 + v23 + (v22 << 5 | (v22 >> 27)) + (v279 ^ v20 & (v300 ^ v279)) + 0x5A827999;

  v25 = (v20 >> 2) | (v20 << 30);//0x2d69785

  v26 = (*(DWORD *)(v5 + 32) >> 24) | ((*(DWORD *)(v5 + 32) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 32) >> 8) | (*(DWORD *)(v5 + 32) << 24)) & 0xFF00FF00;

  v280 = v279 + v26 + (v24 << 5 | (v24 >> 27)) + (v300 ^ v22 & (v300 ^ v25)) + 0x5A827999;

  v27 = (v22 >> 2) | (v22 << 30);//0x2d697f2

  v28 = (*(DWORD *)(v5 + 36) >> 24) | ((*(DWORD *)(v5 + 36) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 36) >> 8) | (*(DWORD *)(v5 + 36) << 24)) & 0xFF00FF00;

  v29 = v300 + v28 + (32 * v280 | (v280 >> 27)) + (v25 ^ v24 & (v27 ^ v25)) + 0x5A827999;

  v30 = (v24 >> 2) | (v24 << 30);//0x2d69853

  v31 = (*(DWORD *)(v5 + 40) >> 24) | ((*(DWORD *)(v5 + 40) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 40) >> 8) | (*(DWORD *)(v5 + 40) << 24)) & 0xFF00FF00;

  v32 = v31 + (v29 << 5 | (v29 >> 27)) + (v27 ^ v280 & (v30 ^ v27)) + 0x5A827999 + v25;

  v281 = (v280 >> 2) | (v280 << 30);

  v33 = (*(DWORD *)(v5 + 44) >> 24) | ((*(DWORD *)(v5 + 44) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 44) >> 8) | (*(DWORD *)(v5 + 44) << 24)) & 0xFF00FF00;

  v34 = v27 + v33 + (v32 << 5 | (v32 >> 27)) + (v30 ^ v29 & (v281 ^ v30)) + 0x5A827999;

  v301 = (v29 >> 2) | (v29 << 30);//0x2d69921

  v35 = (*(DWORD *)(v5 + 48) >> 24) | ((*(DWORD *)(v5 + 48) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 48) >> 8) | (*(DWORD *)(v5 + 48) << 24)) & 0xFF00FF00;

  v36 = v35 + (v34 << 5 | (v34 >> 27)) + (v281 ^ v32 & (v301 ^ v281)) + 0x5A827999 + v30;

  v37 = (v32 >> 2) | (v32 << 30);

  v38 = (*(DWORD *)(v5 + 52) >> 24) | ((*(DWORD *)(v5 + 52) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 52) >> 8) | (*(DWORD *)(v5 + 52) << 24)) & 0xFF00FF00;

  v39 = v281
      + ((*(DWORD *)(v5 + 52) >> 24) | ((*(DWORD *)(v5 + 52) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 52) >> 8) | (*(DWORD *)(v5 + 52) << 24)) & 0xFF00FF00)
      + (32 * v36 | (v36 >> 27))
      + (v301 ^ v34 & (v301 ^ v37))
      + 0x5A827999;

  v40 = (v34 >> 2) | (v34 << 30);//0x2d699eb

  v41 = (*(DWORD *)(v5 + 56) >> 24) | ((*(DWORD *)(v5 + 56) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 56) >> 8) | (*(DWORD *)(v5 + 56) << 24)) & 0xFF00FF00;

  v302 = v301 + v41 + (v39 << 5 | (v39 >> 27)) + (v37 ^ v36 & (v40 ^ v37)) + 0x5A827999;//0x2e99a3f

  v42 = (v36 >> 2) | (v36 << 30); //0x2e99a51

  unsigned long tmpa0 = *(DWORD *)(v5 + 60);
  unsigned long tmpa1 = tmpa0 << 0x18;
  unsigned long tmpa2 = tmpa0 >> 8;
  unsigned long tmpa3 = tmpa1 | tmpa2;
  unsigned long tmpa4 = tmpa0 & 0x0FF00;
  unsigned long tmpa5 = tmpa4 << 8;
  unsigned long tmpa6 = tmpa0 >> 0x18;
  unsigned long tmpa7 = tmpa3 & 0xFF00FF00;

  v43 = (tmpa7 | tmpa5) | tmpa6;
 // v43 = (*(DWORD *)(v5 + 60) >> 24) | ((*(DWORD *)(v5 + 60) & 0xFF00) << 8) | ((*(DWORD *)(v5 + 60) >> 8) | (*(DWORD *)(v5 + 60) << 24)) & 0xFF00FF00;

  v44 = v37 + v43 + (v302 << 5 | (v302 >> 27)) + (v40 ^ v39 & (v42 ^ v40)) + 0x5A827999;

  v282 = (v39 >> 2) | (v39 << 30);////0x2d69ac1

  v45 = 2 * (v6 ^ v11 ^ v26 ^ v38) | ((v6 ^ v11 ^ v26 ^ (unsigned int)v38) >> 31);

  *(DWORD *)v5 = v45;

  v46 = v40 + v45 + (v44 << 5 | (v44 >> 27)) + (v42 ^ v302 & (((v39 >> 2) | (v39 << 30)) ^ v42)) + 0x5A827999;

  v47 = 2 * (v9 ^ v14 ^ v28 ^ v41) | ((v9 ^ v14 ^ v28 ^ (unsigned int)v41) >> 31);

  *(DWORD *)(v5 + 4) = v47;

  v303 = (v302 >> 2) | (v302 << 30);

  v48 = v42 + v47 + (32 * v46 | (v46 >> 27)) + (v282 ^ v44 & (v303 ^ v282)) + 0x5A827999;

  v49 = (v44 >> 2) | (v44 << 30);

  v50 = 2 * (v11 ^ v17 ^ v31 ^ v43) | ((v11 ^ v17 ^ v31 ^ (unsigned int)v43) >> 31);

  *(DWORD *)(v5 + 8) = v50;

  v283 = v282 + v50 + (32 * v48 | (v48 >> 27)) + (v303 ^ v46 & (v303 ^ v49)) + 0x5A827999;

  v51 = (v46 >> 2) | (v46 << 30);

  v52 = 2 * (*(DWORD *)v5 ^ v14 ^ v19 ^ v33) | ((*(DWORD *)v5 ^ v14 ^ v19 ^ (unsigned int)v33) >> 31);

  *(DWORD *)(v5 + 12) = v52;

  v304 = v303 + v52 + (32 * v283 | (v283 >> 27)) + (v49 ^ v48 & (v51 ^ v49)) + 0x5A827999;

  v53 = (v48 >> 2) | (v48 << 30);//0x2d69c4d//0x2e99c4d

  v54 = 2 * (*(DWORD *)(v5 + 4) ^ v17 ^ v21 ^ v35) | ((*(DWORD *)(v5 + 4) ^ v17 ^ v21 ^ (unsigned int)v35) >> 31);

  *(DWORD *)(v5 + 16) = v54;

  v55 = v49 + v54 + (v304 << 5 | (v304 >> 27)) + (v283 ^ v53 ^ v51) + 0x6ED9EBA1;

  v284 = (v283 >> 2) | (v283 << 30);//0x2d69cad//0x2e99cad

  v56 = v55;//0x2e99cc8

  v57 = 2 * (*(DWORD *)(v5 + 8) ^ v19 ^ v23 ^ v38) | ((*(DWORD *)(v5 + 8) ^ v19 ^ v23 ^ (unsigned int)v38) >> 31);// ebp = 0x2e99cad 到 0x2e99cc8

  *(DWORD *)(v5 + 20) = v57;//0x2e99cd0

  unsigned long temp1 = ((v56 << 5) | (v56 >> 27));//0x2e99cdd

  unsigned long temp2 = v53 ^  v284;

  temp2 = temp2 ^ v304;

  v58 = v57 + temp1;//ebp = ebp + [esp + 50];//0x2e99ceb

  unsigned long temp3 = temp2 + v58; 

  unsigned long temp4 = v51 + temp3 + 0x6ED9EBA1;

  v305 = (v304 >> 2) | (v304 << 30);//0x2e99d04

  unsigned long temp5 = (*(DWORD *)(v5 + 12) ^ v21 ^ v26 ^ v41);

  unsigned long temp6 = (*(DWORD *)(v5 + 12) ^ v21 ^ v26 ^ (unsigned int)v41);

  unsigned long temp7 = temp5 + temp5;

  unsigned long temp8 = temp6 >> 0x1F;

  v60 = temp7 | temp8;

  *(DWORD *)(v5 + 24) = v60;

  unsigned long temp9 = temp4 >> 0x1B;

  unsigned long temp10 = temp4 << 5;

  unsigned long temp11 = temp9 | temp10; // edi = edi | ebp //0x2e99d2e

  //v59 = (v58 << 5 ) | ((v57 + (v56 << 5 | (v56 >> 27))) >> 27);

  unsigned long temp12 = v305 ^ v284 ^ v55;

  unsigned long temp13 = 0;

  temp13 = v60 + temp11 + temp12;

  v61 = v53 + temp13 + 0x6ED9EBA1;

  v62 = (v55 >> 2) | (v55 << 30);//0x2d69d6b//0x2e99d6b

  //v63 = 2 * (*(DWORD *)(v5 + 16) ^ v23 ^ v28 ^ v43) | ((*(DWORD *)(v5 + 16) ^ v23 ^ v28 ^ (unsigned int)v43) >> 31);

  unsigned long temp14 = (*(DWORD *)(v5 + 16) ^ v23 ^ v28 ^ v43);

  unsigned long temp15 = temp14 >> 0x1F;

  unsigned long temp16 = temp14 * 2;

  v63 = temp15 | temp16; //ebp = ebp | edi //0x2e99d82

  *(DWORD *)(v5 + 28) = v63;

  unsigned long temp17 = (v61 << 5)  | (v61 >> 27);

  unsigned long temp18 = v62 ^ temp4 ^ v305;

  v285 = v284 + v63 + temp17 + temp18 + 0x6ED9EBA1;//0x2e99dab

  v64 = (temp4 >> 2) | (temp4 << 30);

  v65 = 2 * (*(DWORD *)v5 ^ *(DWORD *)(v5 + 20) ^ v26 ^ v31) | ((*(DWORD *)v5 ^ *(DWORD *)(v5 + 20) ^ v26 ^ (unsigned int)v31) >> 31);//0x2e99de0



  *(DWORD *)(v5 + 32) = v65;

  v306 = v305 + v65 + (v285 << 5 | (v285 >> 27)) + (v61 ^ v64 ^ v62) + 0x6ED9EBA1;//0x2e99e0c

  v66 = (v61 >> 2) | (v61 << 30);//0x2e99e84

  v67 = *(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 24) ^ v28 ^ v33;

  v68 = 2 * v67 | ((unsigned int)v67 >> 31);

  *(DWORD *)(v5 + 36) = v68;

  v69 = v62 + v68 + (v306 << 5 | (v306 >> 27)) + (v285 ^ v66 ^ v64) + 0x6ED9EBA1;

  v286 = (v285 >> 2) | (v285 << 30);

  v70 = *(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 28) ^ v31 ^ v35;

  v71 = 2 * v70 | ((unsigned int)v70 >> 31);

  *(DWORD *)(v5 + 40) = v71;
 
  unsigned long temp19 = (v69 << 5) | (v69 >> 0x1B);

  unsigned long temp20 = v306 ^ v286 ^ v66;

  v72 = v64 + v71 + temp19 + temp20 + 0x6ED9EBA1;

  v307 = (v306 >> 2) | (v306 << 30);

  v73 = *(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 32) ^ v33 ^ v38;

  v74 = 2 * v73 | ((unsigned int)v73 >> 31);

  *(DWORD *)(v5 + 44) = v74;

  v75 = v66 + v74 + (32 * v72 | (v72 >> 27)) + (v307 ^ v286 ^ v69) + 0x6ED9EBA1;

  v76 = (v69 >> 2) | (v69 << 30);

  v77 = 2 * (*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 36) ^ v35 ^ v41) | ((*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 36) ^ v35 ^ (unsigned int)v41) >> 31);

  *(DWORD *)(v5 + 48) = v77;

  v287 = v286 + v77 + (v75 << 5 | (v75 >> 27)) + (v307 ^ v72 ^ v76) + 0x6ED9EBA1;

  v78 = (v72 >> 2) | (v72 << 30);

  v79 = 2 * (*(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 40) ^ v38 ^ v43) | ((*(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 40) ^ v38 ^ (unsigned int)v43) >> 31);

  *(DWORD *)(v5 + 52) = v79;

  v80 = v307 + v79 + (32 * v287 | (v287 >> 27)) + (v75 ^ v78 ^ v76) + 0x6ED9EBA1;

  v81 = *(DWORD *)v5 ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 44) ^ v41;

  v308 = 2 * v81 | ((unsigned int)v81 >> 31);

  *(DWORD *)(v5 + 56) = v308;

  v82 = (v75 >> 2) | (v75 << 30);

  v83 = v308 + (v80 << 5 | (v80 >> 27)) + (v287 ^ v82 ^ v78) + 0x6ED9EBA1 + v76;

  v84 = *(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 48) ^ v43;

  v309 = 2 * v84 | ((unsigned int)v84 >> 31);

  *(DWORD *)(v5 + 60) = v309;

  v288 = (v287 >> 2) | (v287 << 30);

  v85 = v309 + (v83 << 5 | (v83 >> 27));

  v310 = (v80 >> 2) | (v80 << 30);

  v86 = (v80 ^ v288 ^ v82) + 0x6ED9EBA1 + v85 + v78;//0x2E9A09C

  v87 = *(DWORD *)v5 ^ *(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 52);

  v88 = 2 * v87 | ((unsigned int)v87 >> 31);

  *(DWORD *)v5 = v88;

  v89 = v82 + v88 + (v86 << 5 | (v86 >> 27)) + (v310 ^ v288 ^ v83) + 0x6ED9EBA1;

  v90 = (v83 >> 2) | (v83 << 30);

  v91 = *(DWORD *)(v5 + 56);

  v92 = v91;

  v93 = *(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 36) ^ v91;

  v94 = v90;

  v95 = 2 * v93 | ((unsigned int)v93 >> 31);

  *(DWORD *)(v5 + 4) = v95;

  v96 = v288 + v95 + (32 * v89 | (v89 >> 27)) + (v310 ^ v86 ^ v90) + 0x6ED9EBA1;

  v97 = *(DWORD *)(v5 + 60);

  v98 = v97;

  v99 = (v86 >> 2) | (v86 << 30);

  v100 = *(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 40) ^ v97;

  v101 = 2 * v100 | ((unsigned int)v100 >> 31);

  *(DWORD *)(v5 + 8) = v101;

  v102 = v99;

  v103 = v310 + v101 + (32 * v96 | (v96 >> 27)) + (v89 ^ v99 ^ v94) + 0x6ED9EBA1;

  v104 = 2 * (*(DWORD *)v5 ^ *(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 44)) | ((*(DWORD *)v5 ^ *(DWORD *)(v5 + 12) ^ (unsigned int)(*(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 44))) >> 31);
 
  *(DWORD *)(v5 + 12) = v104;

  v105 = (v89 >> 2) | (v89 << 30);
 
  v106 = v94 + v104 + (32 * v103 | (v103 >> 27)) + (v96 ^ v105 ^ v102) + 0x6ED9EBA1;
 
  v107 = *(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 48);
 
  v108 = 2 * v107 | ((unsigned int)v107 >> 31);
 
  v289 = (v96 >> 2) | (v96 << 30);
 
  *(DWORD *)(v5 + 16) = v108;
 
  v109 = v102 + v108 + (32 * v106 | (v106 >> 27)) + (v103 ^ v289 ^ v105) + 0x6ED9EBA1;
 
  v311 = (v103 >> 2) | (v103 << 30);
 
  v110 = *(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 52);
 
  v111 = 2 * v110 | ((unsigned int)v110 >> 31);
 
  *(DWORD *)(v5 + 20) = v111;
 
  v112 = v105 + v111 + (32 * v109 | (v109 >> 27)) + (v311 ^ v289 ^ v106) + 0x6ED9EBA1;
 
  v113 = 2 * (*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 32) ^ v92) | ((*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 32) ^ v92) >> 31);
 
  *(DWORD *)(v5 + 24) = v113;
 
  v114 = (v106 >> 2) | (v106 << 30);
 
  v115 = v289 + v113 + (32 * v112 | (v112 >> 27)) + (v311 ^ v109 ^ v114) + 0x6ED9EBA1;
 
  v116 = 2 * (*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 36) ^ v98) | ((*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 36) ^ v98) >> 31);
 
  *(DWORD *)(v5 + 28) = v116;
 
  v117 = (v109 >> 2) | (v109 << 30);

  v118 = v311 + v116 + (v115 << 5 | (v115 >> 27)) + (v112 ^ v117 ^ v114) + 0x6ED9EBA1;


  v119 = *(DWORD *)v5 ^ *(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 40);

  v120 = (v112 >> 2) | (v112 << 30);

  v121 = 2 * v119 | ((unsigned int)v119 >> 31);

  *(DWORD *)(v5 + 32) = v121;
  v122 = v114 + v121 + (v115 & v120 | v117 & (v115 | v120)) + (32 * v118 | (v118 >> 27)) + 0x8F1BBCDC;
  v123 = (v115 >> 2) | (v115 << 30);
  v124 = *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 44);
  v290 = v123;
  v125 = 2 * (*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 24) ^ v124) | ((*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 24) ^ (unsigned int)v124) >> 31);
  *(DWORD *)(v5 + 36) = v125;
  v126 = v117 + v125 + (v118 & v123 | v120 & (v118 | v123)) + (32 * v122 | (v122 >> 27)) + 0x8F1BBCDC;
  v127 = (v118 >> 2) | (v118 << 30);
  v128 = *(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 48);
  v312 = v127;
  v129 = 2 * (*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 28) ^ v128) | ((*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 28) ^ (unsigned int)v128) >> 31);
  *(DWORD *)(v5 + 40) = v129;
  v130 = v120 + v129 + (v127 & v122 | v290 & (v127 | v122)) + (32 * v126 | (v126 >> 27)) + 0x8F1BBCDC;
  v131 = (v122 >> 2) | (v122 << 30);
  v132 = *(DWORD *)(v5 + 44) ^ *(DWORD *)(v5 + 52);
  v133 = 2 * (*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 32) ^ v132) | ((*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 32) ^ (unsigned int)v132) >> 31);
  *(DWORD *)(v5 + 44) = v133;
  v134 = v290 + v133 + (v126 & v131 | v312 & (v126 | v131)) + (32 * v130 | (v130 >> 27)) + 0x8F1BBCDC;
  v135 = (v126 >> 2) | (v126 << 30);
  v136 = 2 * (*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 48) ^ v92) | ((*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 48) ^ v92) >> 31);
  *(DWORD *)(v5 + 48) = v136;
  v137 = v312 + v136 + (v130 & v135 | v131 & (v130 | v135)) + (32 * v134 | (v134 >> 27)) + 0x8F1BBCDC;
  v138 = (v130 >> 2) | (v130 << 30);
  v139 = 2 * (*(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 52) ^ v98) | ((*(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 52) ^ v98) >> 31);
  *(DWORD *)(v5 + 52) = v139;
  v140 = v131 + v139 + (v134 & v138 | v135 & (v134 | v138)) + (32 * v137 | (v137 >> 27)) + 0x8F1BBCDC;
  v141 = (v134 >> 2) | (v134 << 30);
  v291 = (v134 >> 2) | (v134 << 30);
  v142 = *(DWORD *)v5 ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 44) ^ v92;
  v143 = 2 * v142 | ((unsigned int)v142 >> 31);
  *(DWORD *)(v5 + 56) = v143;
  v144 = v135 + v143 + (v137 & v141 | v138 & (v137 | v141)) + (32 * v140 | (v140 >> 27)) + 0x8F1BBCDC;
  v145 = (v137 >> 2) | (v137 << 30);
  v146 = *(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 48) ^ v98;
  v147 = 2 * v146 | ((unsigned int)v146 >> 31);
  *(DWORD *)(v5 + 60) = v147;
  v313 = v145;
  v148 = v138 + v147 + (v145 & v140 | v291 & (v145 | v140)) + (32 * v144 | (v144 >> 27)) + 0x8F1BBCDC;
  v149 = (v140 >> 2) | (v140 << 30);
  v150 = 2 * (*(DWORD *)v5 ^ *(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 52)) | ((*(DWORD *)v5 ^ *(DWORD *)(v5 + 8) ^ (unsigned int)(*(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 52))) >> 31);
  *(DWORD *)v5 = v150;
  v151 = v291
       + v150
       + (v144 & ((v140 >> 2) | (v140 << 30)) | v145 & (v144 | (v140 >> 2) | (v140 << 30)))
       + (32 * v148 | (v148 >> 27))
       + 0x8F1BBCDC;
  v152 = (v144 >> 2) | (v144 << 30);
  v153 = *(DWORD *)(v5 + 56);
  v154 = v153;
  v155 = *(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 36) ^ v153;
  v156 = 2 * v155 | ((unsigned int)v155 >> 31);
  *(DWORD *)(v5 + 4) = v156;
  v157 = v152;
  v158 = v313 + v156 + (v148 & v152 | v149 & (v148 | v152)) + (32 * v151 | (v151 >> 27)) + 0x8F1BBCDC;
  v159 = (v148 >> 2) | (v148 << 30);
  v160 = *(DWORD *)(v5 + 60);
  v161 = v160;
  v162 = *(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 40) ^ v160;
  v163 = 2 * v162 | ((unsigned int)v162 >> 31);
  *(DWORD *)(v5 + 8) = v163;
  v164 = v159;
  v165 = v149 + v163 + (v151 & v159 | v157 & (v151 | v159)) + (32 * v158 | (v158 >> 27)) + 0x8F1BBCDC;
  v166 = (v151 >> 2) | (v151 << 30);
  v167 = *(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 44);
  v292 = v166;
  v168 = 2 * (*(DWORD *)v5 ^ *(DWORD *)(v5 + 12) ^ v167) | ((*(DWORD *)v5 ^ *(DWORD *)(v5 + 12) ^ (unsigned int)v167) >> 31);
  *(DWORD *)(v5 + 12) = v168;
  v169 = v157 + v168 + (v158 & v166 | v164 & (v158 | v166)) + (32 * v165 | (v165 >> 27)) + 0x8F1BBCDC;
  v170 = (v158 >> 2) | (v158 << 30);
  v171 = *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 48);
  v314 = v170;
  v172 = 2 * (*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 16) ^ v171) | ((*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 16) ^ (unsigned int)v171) >> 31);
  *(DWORD *)(v5 + 16) = v172;
  v173 = v164 + v172 + (v170 & v165 | v292 & (v170 | v165)) + (32 * v169 | (v169 >> 27)) + 0x8F1BBCDC;
  v174 = (v165 >> 2) | (v165 << 30);
  v175 = *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 52);
  v176 = 2 * (*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 20) ^ v175) | ((*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 20) ^ (unsigned int)v175) >> 31);
  *(DWORD *)(v5 + 20) = v176;
  v177 = v292 + v176 + (v169 & v174 | v314 & (v169 | v174)) + (32 * v173 | (v173 >> 27)) + 0x8F1BBCDC;
  v178 = (v169 >> 2) | (v169 << 30);
  v179 = 2 * (*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 32) ^ v154) | ((*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 32) ^ v154) >> 31);
  *(DWORD *)(v5 + 24) = v179;
  v180 = v314 + v179 + (v173 & v178 | v174 & (v173 | v178)) + (32 * v177 | (v177 >> 27)) + 0x8F1BBCDC;
  v181 = (v173 >> 2) | (v173 << 30);
  v182 = 2 * (*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 36) ^ v161) | ((*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 36) ^ v161) >> 31);
  *(DWORD *)(v5 + 28) = v182;
  v183 = v174 + v182 + (v177 & v181 | v178 & (v177 | v181)) + (32 * v180 | (v180 >> 27)) + 0x8F1BBCDC;
  v184 = (v177 >> 2) | (v177 << 30);
  v185 = *(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 40);
  v293 = v184;
  v186 = 2 * (*(DWORD *)v5 ^ *(DWORD *)(v5 + 20) ^ v185) | ((*(DWORD *)v5 ^ *(DWORD *)(v5 + 20) ^ (unsigned int)v185) >> 31);
  *(DWORD *)(v5 + 32) = v186;
  v187 = v178 + v186 + (v180 & v184 | v181 & (v180 | v184)) + (32 * v183 | (v183 >> 27)) + 0x8F1BBCDC;
  v188 = (v180 >> 2) | (v180 << 30);
  v189 = *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 44);
  v315 = v188;
  v190 = 2 * (*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 24) ^ v189) | ((*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 24) ^ (unsigned int)v189) >> 31);
  *(DWORD *)(v5 + 36) = v190;
  v191 = v181 + v190 + (v188 & v183 | v293 & (v188 | v183)) + (32 * v187 | (v187 >> 27)) + 0x8F1BBCDC;
  v192 = (v183 >> 2) | (v183 << 30);
  v193 = *(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 48);
  v194 = v192;
  v195 = 2 * (*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 28) ^ v193) | ((*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 28) ^ (unsigned int)v193) >> 31);
  *(DWORD *)(v5 + 40) = v195;
  v196 = v293 + v195 + (v187 & v192 | v315 & (v187 | v192)) + (32 * v191 | (v191 >> 27)) + 0x8F1BBCDC;
  v197 = (v187 >> 2) | (v187 << 30);
  v198 = *(DWORD *)(v5 + 44) ^ *(DWORD *)(v5 + 52);
  v199 = v197;
  v200 = 2 * (*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 32) ^ v198) | ((*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 32) ^ (unsigned int)v198) >> 31);
  *(DWORD *)(v5 + 44) = v200;
  v201 = v315 + v200 + (v191 & v197 | v194 & (v191 | v197)) + (32 * v196 | (v196 >> 27)) + 0x8F1BBCDC;
  v202 = (v191 >> 2) | (v191 << 30);//0x2e9a9fc

  v203 = 2 * (*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 48) ^ v154) | ((*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 48) ^ v154) >> 31);
  *(DWORD *)(v5 + 48) = v203;
  v204 = v203 + (32 * v201 | (v201 >> 27)) + (v196 ^ v202 ^ v197)+ 0xCA62C1D6 + v194;
  v294 = (v196 >> 2) | (v196 << 30);
  v205 = 2 * (*(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 52) ^ v161) | ((*(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 52) ^ v161) >> 31);
  *(DWORD *)(v5 + 52) = v205;
  v206 = v201 ^ v294 ^ v202;
  v316 = (v201 >> 2) | (v201 << 30);
  v207 = *(DWORD *)v5 ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 44) ^ v154;
  v208 = 2 * v207 | ((unsigned int)v207 >> 31);
  v209 = v206+ 0xCA62C1D6 + v205 + (32 * v204 | (v204 >> 27)) + v199;
  *(DWORD *)(v5 + 56) = v208;
  v210 = v202 + v208 + (32 * v209 | (v209 >> 27)) + (v316 ^ v294 ^ v204)+ 0xCA62C1D6;
  v211 = (v204 >> 2) | (v204 << 30);
  v212 = *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 48) ^ v161;
  v213 = 2 * (*(DWORD *)(v5 + 4) ^ v212) | ((*(DWORD *)(v5 + 4) ^ (unsigned int)v212) >> 31);
  *(DWORD *)(v5 + 60) = v213;
  v214 = v294 + v213 + (32 * v210 | (v210 >> 27)) + (v316 ^ v209 ^ v211)+ 0xCA62C1D6;
  v215 = 2 * (*(DWORD *)v5 ^ *(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 52)) | ((*(DWORD *)v5 ^ *(DWORD *)(v5 + 8) ^ (unsigned int)(*(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 52))) >> 31);
  *(DWORD *)v5 = v215;
  v216 = (v209 >> 2) | (v209 << 30);
  v217 = v215 + (32 * v214 | (v214 >> 27)) + (v210 ^ v216 ^ v211)+ 0xCA62C1D6 + v316;
  v218 = 2 * (*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 56)) | ((*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 12) ^ (unsigned int)(*(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 56))) >> 31);
  *(DWORD *)(v5 + 4) = v218;
  v219 = (v210 >> 2) | (v210 << 30);
  v220 = v218 + (32 * v217 | (v217 >> 27)) + (v214 ^ v219 ^ v216)+ 0xCA62C1D6 + v211;
  v221 = 2 * (*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 60)) | ((*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 16) ^ (unsigned int)(*(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 60))) >> 31);
  *(DWORD *)(v5 + 8) = v221;
  v295 = (v214 >> 2) | (v214 << 30);
  v222 = (v217 ^ v295 ^ v219)+ 0xCA62C1D6 + v221 + (32 * v220 | (v220 >> 27)) + v216;
  v317 = (v217 >> 2) | (v217 << 30);
  v223 = *(DWORD *)v5 ^ *(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 44);
  v224 = 2 * v223 | ((unsigned int)v223 >> 31);
  *(DWORD *)(v5 + 12) = v224;
  v225 = v219 + v224 + (32 * v222 | (v222 >> 27)) + (v317 ^ v295 ^ v220)+ 0xCA62C1D6;
  v226 = (v220 >> 2) | (v220 << 30);
  v227 = *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 48);
  v228 = v226;
  v229 = 2 * (*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 16) ^ v227) | ((*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 16) ^ (unsigned int)v227) >> 31);
  *(DWORD *)(v5 + 16) = v229;
  v230 = v295 + v229 + (32 * v225 | (v225 >> 27)) + (v317 ^ v222 ^ v226)+ 0xCA62C1D6;
  v231 = (v222 >> 2) | (v222 << 30);
  v232 = *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 52);
  v233 = v231;
  v234 = 2 * (*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 20) ^ v232) | ((*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 20) ^ (unsigned int)v232) >> 31);
  *(DWORD *)(v5 + 20) = v234;
  v235 = (v225 ^ v231 ^ v228)+ 0xCA62C1D6 + v234 + (32 * v230 | (v230 >> 27)) + v317;
  v236 = 2 * (*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 56)) | ((*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 24) ^ (unsigned int)(*(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 56))) >> 31);
  *(DWORD *)(v5 + 24) = v236;
  v237 = (v225 >> 2) | (v225 << 30);
  v238 = v230 ^ v237 ^ v231;
  v239 = (v230 >> 2) | (v230 << 30);
  v240 = *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 60);
  v241 = v238+ 0xCA62C1D6 + v236 + (32 * v235 | (v235 >> 27)) + v228;
  v296 = v239;
  v242 = 2 * (*(DWORD *)(v5 + 16) ^ v240) | ((*(DWORD *)(v5 + 16) ^ (unsigned int)v240) >> 31);
  *(DWORD *)(v5 + 28) = v242;
  v243 = v242 + (32 * v241 | (v241 >> 27)) + (v235 ^ v239 ^ v237)+ 0xCA62C1D6 + v233;
  v318 = (v235 >> 2) | (v235 << 30);
  v244 = *(DWORD *)v5 ^ *(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 32) ^ *(DWORD *)(v5 + 40);
  v245 = 2 * v244 | ((unsigned int)v244 >> 31);
  *(DWORD *)(v5 + 32) = v245;
  v246 = v237 + v245 + (32 * v243 | (v243 >> 27)) + (v318 ^ v239 ^ v241)+ 0xCA62C1D6;
  v247 = (v241 >> 2) | (v241 << 30);
  v248 = *(DWORD *)(v5 + 36) ^ *(DWORD *)(v5 + 44);
  v249 = v247;
  v250 = 2 * (*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 24) ^ v248) | ((*(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 24) ^ (unsigned int)v248) >> 31);
  *(DWORD *)(v5 + 36) = v250;
  v251 = v296 + v250 + (32 * v246 | (v246 >> 27)) + (v318 ^ v243 ^ v247)+ 0xCA62C1D6;
  v252 = (v243 >> 2) | (v243 << 30);
  v253 = *(DWORD *)(v5 + 40) ^ *(DWORD *)(v5 + 48);
  v254 = 2 * (*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 28) ^ v253) | ((*(DWORD *)(v5 + 8) ^ *(DWORD *)(v5 + 28) ^ (unsigned int)v253) >> 31);
  *(DWORD *)(v5 + 40) = v254;
  v255 = v254 + (32 * v251 | (v251 >> 27)) + (v246 ^ v252 ^ v249)+ 0xCA62C1D6 + v318;
  v256 = 2 * (*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 44) ^ *(DWORD *)(v5 + 52) ^ *(DWORD *)(v5 + 32)) | ((*(DWORD *)(v5 + 12) ^ *(DWORD *)(v5 + 44) ^ (unsigned int)(*(DWORD *)(v5 + 52) ^ *(DWORD *)(v5 + 32))) >> 31);
  *(DWORD *)(v5 + 44) = v256;
  v257 = (v246 >> 2) | (v246 << 30);
  v258 = v256 + (32 * v255 | (v255 >> 27)) + (v251 ^ v257 ^ v252)+ 0xCA62C1D6 + v249;
  v297 = (v251 >> 2) | (v251 << 30);
  v259 = 2 * (*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 48) ^ *(DWORD *)(v5 + 56) ^ *(DWORD *)(v5 + 36)) | ((*(DWORD *)(v5 + 16) ^ *(DWORD *)(v5 + 48) ^ (unsigned int)(*(DWORD *)(v5 + 56) ^ *(DWORD *)(v5 + 36))) >> 31);
  *(DWORD *)(v5 + 48) = v259;
  v260 = v259 + (32 * v258 | (v258 >> 27)) + (v255 ^ ((v251 >> 2) | (v251 << 30)) ^ v257)+ 0xCA62C1D6 + v252;
  v319 = (v255 >> 2) | (v255 << 30);
  v261 = *(DWORD *)(v5 + 20) ^ *(DWORD *)(v5 + 52) ^ *(DWORD *)(v5 + 60) ^ *(DWORD *)(v5 + 40);
  v262 = 2 * v261 | ((unsigned int)v261 >> 31);
  *(DWORD *)(v5 + 52) = v262;
  v263 = v257 + v262 + (32 * v260 | (v260 >> 27)) + (v319 ^ v297 ^ v258)+ 0xCA62C1D6;
  v264 = 2 * (*(DWORD *)v5 ^ *(DWORD *)(v5 + 24) ^ *(DWORD *)(v5 + 56) ^ *(DWORD *)(v5 + 44)) | ((*(DWORD *)v5 ^ *(DWORD *)(v5 + 24) ^ (unsigned int)(*(DWORD *)(v5 + 56) ^ *(DWORD *)(v5 + 44))) >> 31);

  *(DWORD *)(v5 + 56) = v264;

  v265 = (v258 >> 2) | (v258 << 30);//

  v266 = v297 + v264 + (32 * v263 | (v263 >> 27)) + (v319 ^ v260 ^ v265)+ 0xCA62C1D6;//0x2e9b014

  v267 = (v260 >> 2) | (v260 << 30);

  v268 = *(DWORD *)(v5 + 4) ^ *(DWORD *)(v5 + 28) ^ *(DWORD *)(v5 + 60) ^ *(DWORD *)(v5 + 48);

  v269 = 2 * v268 | ((unsigned int)v268 >> 31);

  *(DWORD *)(v5 + 60) = v269;


 // v270 = v269 + 

  unsigned long tempa = (v266 << 5) | (v266 >> 27);

  unsigned long tempb = v265 ^ v267 ^ v263;

  v270 = v269 + tempa;

  unsigned long tempd = tempb + v270;
  
  v271 = *(DWORD *)(v3 + 4);//

  unsigned long tempc = v319 +  tempd + 0xCA62C1D6;

  v272 = tempc + *(DWORD *)v3;

  v273 = (v263 >> 2) | (v263 << 30);

  v274 = *(DWORD *)(v3 + 12);

  *(DWORD *)v3 = v272;//??

  v275 = v273 + *(DWORD *)(v3 + 8);//

  v276 = *(DWORD *)(v3 + 16);

  *(DWORD *)(v3 + 4) = v266 + v271;//??

  *(DWORD *)(v3 + 8) = v275;//??

  *(DWORD *)(v3 + 12) = v267 + v274;//??

  result = v265 + v276;

  *(DWORD *)(v3 + 16) = result;
  return result;
}

//sub_10045460
int  HashAB::Caclbuffthree(DWORD a1, const char *a2, DWORD a3)
{
  int v3; // eax@1
  unsigned int v4; // ecx@1
  int v5; // edx@1
  int v6; // ebp@4
  size_t i; // esi@4
  int result; // eax@8
  void *v9; // edi@8

  char *temp = (char*)malloc(a3);
 

  int tempnum = a3;

  v4 = a3;
  v5 = *(DWORD *)(a1 + 20) + 8 * a3;
  v3 = (*(DWORD *)(a1 + 20) >> 3) & 0x3F;
  *(DWORD *)(a1 + 20) = v5;
  if ( v5 < 8 * a3 )
    ++*(DWORD *)(a1 + 24);
  *(DWORD *)(a1 + 24) += a3 >> 29;
  if ( v3 + a3 <= 0x3F )
  {
    v6 = 0;
  }
  else
  {
    v6 = 64 - v3;
    memcpy((void *)(v3 + a1 + 28), a2, 64 - v3);
    Caclbufftwo(a1, a1 + 28);


    for ( i = v6 + 63; i < a3; v6 += 64 )
    {

      Caclbufftwo(a1, (int)(a2 + i - 63));
      i += 64;
	 
    }
    v4 = a3;
    v3 = 0;
	
  }

  v9 = (void *)(v3 + a1 + 28);
  result = v4 - v6;
  unsigned long temp1 = result >> 2;
  unsigned long temp2 = result & 3;

  memcpy(v9, (char *)a2 + v6, v4 - v6);
 // a2 = temp;
 // memcpy((char*)a2, a, tempnum);
 // free(temp);
  return result;
}

//sub_574D10
int HashAB::Caclbuff(DWORD a1, DWORD a2)
{
  unsigned int v2 = 0; // eax@1
  char v3 = 0; // ch@1
  unsigned int v4 = 0; // eax@1
  int v5 = 0; // edx@1
  unsigned int v6 = 0; // eax@3
  int v7 = 0; // edx@4
  int result = 0; // eax@5
  //DWORD Src = 0; // [sp+8h] [bp-Ch]@1
  DWORD Src =0;
  DWORD v10 = 0; // [sp+9h] [bp-Bh]@1
  char v11 = 0; // [sp+Ah] [bp-Ah]@1
  DWORD v12 = 0; // [sp+Bh] [bp-9h]@1
  DWORD v13 = 0; // [sp+Ch] [bp-8h]@1
  DWORD v14 = 0; // [sp+Dh] [bp-7h]@1
  char v15 = 0; // [sp+Eh] [bp-6h]@1
  DWORD v16 = 0; // [sp+Fh] [bp-5h]@1
  unsigned int v17 = 0; // [sp+10h] [bp-4h]@1



  v17 = (unsigned int)&Src ^ g_dwsandSign;
  v2 = *(DWORD *)(a2 + 24);
  Src = *(DWORD *)(a2 + 24) >> 24;
  v10 = v2 >> 16;
  v3 = BYTE(v2);
  v12 = v2;
  v4 = *(DWORD *)(a2 + 20);
  v5 = *(DWORD *)(a2 + 20) >> 24;
  v11 = v3;
  v13 = v5;
  v14 = v4 >> 16;
  v15 = BYTE(v4);
  v16 = v4;
  
  char chtemp[1024] = {0};
  chtemp[6] = *(BYTE *)(a2 + 21);
  chtemp[7] = *(BYTE *)(a2 + 20);

  unsigned char szTemp[] ={0x80,0x00, 0x00, 0x00}; 
  Caclbuffthree(a2, (char*)szTemp, 1u);//"�"
  while ( (*(DWORD *)(a2 + 20) & 0x1F8) != 448 )
    Caclbuffthree(a2, (char*)&g_dwunk, 1u);


  Caclbuffthree(a2, (char*)&chtemp, 8u);
  v6 = 0;
  do
  {
    v7 = *(DWORD *)(a2 + 4 * (v6 >> 2)) >> 8 * (3 - (v6 & 3));
    ++v6;
    *(BYTE *)(a1 + v6 - 1) = v7;
  }
  while ( v6 < 0x14 );
  memset((void *)(a2 + 28), 0, 0x40u);
  result = 0;
  *(DWORD *)a2 = 0;
  *(DWORD *)(a2 + 4) = 0;
  *(DWORD *)(a2 + 8) = 0;
  *(DWORD *)(a2 + 12) = 0;
  *(DWORD *)(a2 + 16) = 0;
  *(DWORD *)(a2 + 20) = 0;
  *(DWORD *)(a2 + 24) = 0;
  return result;
}



//sub_100428E0
int  HashAB::SetSendParam(DWORD a1, DWORD a2)
{
  signed int v2; // esi@1
  unsigned int v3; // eax@1
  DWORD v4; // eax@30
  DWORD v5; // ecx@3
  BYTE v6; // dl@4
  
  v3 = time(NULL);
  srand(v3);
  v2 = 0;
  do
  {
    ++v2;
	int nrand =  rand() % 35;//off_100A9798[rand() % 35];
	*(BYTE *)(v2 + a2 - 1) = g_szrand[nrand];

  }
  while ( v2 < 1023 );
  v4 = 0;
  v5 = 0x0A;
  do
  {
    v6 = *(BYTE *)(v4 + a1);
    *(BYTE *)(v5 + a2) = v6;
	v4++;
    if ( v4 == 0x31 )
      break;
    v5 += 0x0A;
  }
  while ( v5 < 1023 );
  return a2;
}


//sub_10045C61

//sub_10042950

//sub_10038FC0 获得地址的值
FILE * HashAB::SetFileToAddr(const char *Filename)
{
  FILE *result; // eax@1


  result = fopen(Filename, "rb");

  if ( result )
  {
    fseek(result, 36, 0);	
    fread(&dword_100A9AE8, 1u, 4u, result);//0xf10577db	

    fseek(result, 40, 0);
    fread(&dword_100A9AF0, 1u, 4u, result);//0xf8685e21
	//84
    /*
	fseek(result, 112, 0);
    fread(&byte_100A8DC4, 1u, 1u, result);
    if ( !byte_100A8DC4 )
      byte_100A8DC4 = 2;
    fseek(result, 160, 0);
    fread(&dword_100A9AF8, 1u, 4u, result);//0xffff15c7
    fseek(result, 164, 0);
    fread(&dword_100A9B00, 1u, 4u, result);//0x000a0019
	*/
    fclose(result);
	USES_CONVERSION;
	DeleteFile(A2W(Filename));
    result = (FILE *)1;
  }
  return result;

}



//sub_10040FA0
int  HashAB::Caclsendbuff(const char *a1, unsigned int a2, int a3)
{
  char v4[1024] = {0};; // [sp+0h] [bp-60h]@1
  int v5; // [sp+5Ch] [bp-4h]@1

  v5 = g_dwsandSign;
  Caclbuffone((int)&v4);//sub_10057980
  Caclbuffthree((int)&v4, a1, a2);//sub_100579B0
  Caclbuff(a3, (int)&v4);//sub_10057A70
  return 1;
}

//sub_1003AD90
signed int  HashAB::CreateiTunesCDB(char* a1, const char *Filename)
{
  FILE *v2; // eax@1
  FILE *v3; // edi@1
  signed int result; // eax@2
  __int32 v5; // ebx@3
  void *v6; // ebp@6
  char *v7; // eax@6
  FILE *v8; // esi@8
  int v9; // [sp+8h] [bp-28h]@1
  void *Memory; // [sp+Ch] [bp-24h]@6
  unsigned int DstBuf; // [sp+10h] [bp-20h]@1
  int v12; // [sp+14h] [bp-1Ch]@1
  char v13[1024] = {0}; // [sp+18h] [bp-18h]@6
  int v14; // [sp+2Ch] [bp-4h]@1

  v14 = g_dwsandSign;
  DstBuf = 0;
  v9 = 0;
  v12 = 57;
  v2 = fopen(Filename, "rb");
  v3 = v2;
  if ( v2 )
  {
    fseek(v2, 0, 2);
    v5 = ftell(v3);
    rewind(v3);
    if ( (unsigned int)v5 >= 0xF4 && (fseek(v3, 16, 0), v9 = fread(&DstBuf, 1u, 4u, v3), DstBuf >= 0x2E) )
    {
      v6 = malloc(v5);
      fseek(v3, 0, 0);
      v9 = fread(v6, 1u, v5, v3);
      fclose(v3);
	   

	  *((DWORD *)v6 + 9) = dword_100A9AE8;////0x6EAF393B
      *((DWORD *)v6 + 10) = dword_100A9AF0;////0xA2E91CEA
	  *((BYTE *)v6 + 108) = 0;
	  *((BYTE *)v6 + 109) = 0;
      *((BYTE *)v6 + 112) = 4;
      *((BYTE *)v6 + 113) = 0;
      *((DWORD *)v6 + 40) = dword_100A9AF8;////0xFFFF15C7
      *((DWORD *)v6 + 41) = dword_100A9B00;////0x000A0019
      *((BYTE *)v6 + 80) = 1;
      *((BYTE *)v6 + 168) = 1;
      *((BYTE *)v6 + 169) = 0;
      *((BYTE *)v6 + 170) = 0;
	  *((DWORD *)v6 + 5) = 7;//8;//9;
      *((DWORD *)v6 + 6) = 0;
      *((DWORD *)v6 + 7) = 0;
	  *((DWORD *)v6 + 14) = 0;////2DD9AE8
	  *((DWORD *)v6 + 15) = 0;////2DD9AE8
	  *((DWORD *)v6 + 16) = 0;////2DD9AE8
	  *((DWORD *)v6 + 17) = 0;////2DD9AE8
      *((DWORD *)v6 + 18) = 0;////2DD9AF0
	  *((DWORD *)v6 + 19) = 0;////2DD9AE8
	  *((DWORD *)v6 + 21) = 0;//0x0000000F;////2DD9AE8
 

      *(DWORD *)((char *)v6 + 50) = 0;
      *(DWORD *)((char *)v6 + 54) = 0;
      *(DWORD *)((char *)v6 + 58) = 0;
      *(DWORD *)((char *)v6 + 62) = 0;
      *(DWORD *)((char *)v6 + 66) = 0;
      *((DWORD *)v6 + 22) = 0;
      *((DWORD *)v6 + 23) = 0;
      *((DWORD *)v6 + 24) = 0;
      *((DWORD *)v6 + 25) = 0;
      *((DWORD *)v6 + 26) = 0;
      memset((char *)v6 + 114, 0, 0x2Cu);
      *((WORD *)v6 + 79) = 0;
      memset((char *)v6 + 171, 0, 0x38u);
      *((BYTE *)v6 + 227) = 0;
      v7 = (char*)malloc(v5);
      memcpy(v7, v6, v5);
      Memory = v7;

	 unsigned char tempsha1[20] = {0};

	 USES_CONVERSION;
	  getSha1(tempsha1, (unsigned char *)v7, v5);	//取sha1
	  OutputDebugString(L"shal is: ");
	  OutputDebugString(A2W(CharStr2HexStr((char*)tempsha1).c_str()));

    //  Caclsendbuff(v7, v5, (int)&v13);
	  char sz[20] = {0};
	  memcpy(sz, a1+4, 20);
	
	  

	  unsigned char rnd[] = "ABCDEFGHIJKLMNOPQRSTUVW";
	  g_calc_hashAB((unsigned char *)tempsha1, (unsigned char *)sz, ( unsigned char *)((char *)v6 + 171), rnd);
	  //g_calc_hashAB((unsigned char *)shal, (unsigned char *)sz, ( unsigned char *)((char *)v6 + 171), rnd);

      //if ( CaclHashFromSever(3, 0, sz, 20, (char*)shal/*v13*/, 20, (int)((char *)v6 + 171), (int)&v12) )
      //{
      //  //free(Memory);
      //  free(v6);
      //  result = -1;
      //}
	 
	  char szCmp[2] = {0};
	  memcpy(szCmp, ((char*)v6 + 171), 1);
	  if ( szCmp[0] == '0')
	  {
        free(Memory);
        free(v6);
        result = -1;
	  }
      else
      {
        v8 = fopen(Filename, "wb");

		fwrite(v6, 1u, v5, v8);
     //   MyWriteFile(v6, v5, (int)&v9, v8);
        fclose(v8);
        free(Memory);
        free(v6);
        result = 0;
      }
    }
    else
    {
      fclose(v3);
      result = -1;
    }
  }
  else
  {
    result = -1;
  }
  return result;
}


void HashAB::GetSandSign()
{
	FILETIME FileTime;
	memset(&FileTime, 0, sizeof(FileTime));
	GetSystemTimeAsFileTime(&FileTime);
	DWORD dwHigh = FileTime.dwHighDateTime;
	DWORD dwLow = FileTime.dwLowDateTime;

	DWORD dwEsi = dwHigh ^ dwLow;

	DWORD dwProcId = GetCurrentProcessId();
	dwEsi = dwEsi ^ dwProcId;

	DWORD dwThreadId = GetCurrentThreadId();
	dwEsi = dwEsi ^ dwThreadId;

	DWORD dwTick = GetTickCount();
	dwEsi = dwEsi ^ dwTick;

	LARGE_INTEGER PerformanceCount;
	BOOL bQuery = QueryPerformanceCounter(&PerformanceCount);

	DWORD dwQueryHigh = PerformanceCount.HighPart;
	DWORD dwQueryLow = PerformanceCount.LowPart;

	DWORD dwEax = dwQueryHigh ^ dwQueryLow;

	dwEsi = dwEsi ^ dwEax;

	g_dwsandSign = dwEsi;

	g_dwOther = ~dwEsi;//取反
}

//a1 == uuid, a3 = location.itdb path, path = location.itdb.cbk path
BOOL  HashAB::CreateLocationCBK(char* a1,  char *a3, char* path)
{

	long filelen = 0;
	FILE *pfile = NULL;
	char *buff = NULL;
	pfile = fopen(path, "rb");
	if (pfile != NULL)
	{

		fseek(pfile, 0, 2);
		filelen = ftell(pfile);
		fseek(pfile, 0, 0);
		buff = (char*)malloc(filelen);
		long lRead = fread(buff, 1, filelen, pfile);
		fclose(pfile);
	}
	
	unsigned char hashc[20];
	memset(hashc,0,sizeof(hashc));
	memcpy(hashc,buff+57,20);
	//int v18 = 0x39; // [sp+1Ch] [bp-464h]@1
	//char v19[1024] = {0}; // [sp+20h] [bp-460h]@7
	unsigned char Str[57] = {0}; // [sp+34h] [bp-44Ch]@8
	//char DstBuf[1024] = {0}; // [sp+70h] [bp-410h]@1
	//char v26[1024] = {0}; // [sp+71h] [bp-40Fh]@1	
	unsigned char rnd[] = "ABCDEFGHIJKLMNOPQRSTUVW";
	//****************************************************************
	BOOL bres = FALSE;

	char sz[20] = {0};
	memcpy(sz, a1+4, 20);
	g_calc_hashAB((unsigned char *)hashc, ((const unsigned char *)sz), Str, rnd);
	//if ( !CaclHashFromSever(3, 0, a1+4, 20, a3, 20, (int)&Str, (int)&v18) )
	//{
    if (Str[0] != '0')
	{
		/*long filelen = 0;
		FILE *pfile = NULL;
		pfile = fopen(path, "rb");*/
	//	if (pfile != NULL)
	//	{
			
			//fseek(pfile, 0, 2);
   //         filelen = ftell(pfile);
			//fseek(pfile, 0, 0);
			//char *buff = (char*)malloc(filelen);
			//long lRead = fread(buff, 1, filelen, pfile);
			//fclose(pfile);
		//	char buffout = malloc(filelen+11);

		//	memcpy()

			pfile = fopen(path, "wb");
			
			if (pfile != NULL)
			{
				memcpy(buff, Str, 57);

               fwrite(buff, filelen, 1, pfile);

			   fclose(pfile);
			}

			free(buff);
		//}
		bres = TRUE;
		
	}

  return bres;

}



bool HashAB::SetUuidAndHash(char* puuid, int nIsHashAB)
{
	GetSandSign();	

    g_nIsHashAB = nIsHashAB;

	int i = 0;
    for (i=4; i<24; i++)
	{
		g_uuid[i] = *(puuid + (i-4));
	}
	//g_uuid[24]=0;
	return true;
}



bool HashAB::CaclLocationCDK(char* Locationpath, char* cbkpath)
{	
	if (g_nIsHashAB == 1)
	{
		CreateLocationCBK(g_uuid, Locationpath, cbkpath);
	}
	return true;
}

BOOL HashAB::ReSetiTunesCDB(char* shal,char* iTunesCDBPath)
{
	//memset(g_szCDBPath, 0, sizeof(g_szCDBPath));

	//memcpy(g_szCDBPath, iTunesCDBPath, strlen(iTunesCDBPath));

	if (g_nIsHashAB == 1)
	{
		CreateiTunesCDB(g_uuid,  iTunesCDBPath);
	}
	return TRUE;
}

void HashAB::GetiTunesCDBFilePath(char* iTunesCDBPath)
{
	SetFileToAddr(iTunesCDBPath);	//解析iTunesCDB文件	
}




BOOL HashAB::InitInstance()
{
	//CWinApp::InitInstance();

	HINSTANCE g_HashAB = ::LoadLibrary(L"iPhoneCalc.dll");//计算hashAB的DLL
	if (g_HashAB)
	{
		g_calc_hashAB = (calcHashAB_t)GetProcAddress(g_HashAB, "xgen_hash_ab");//计算hashAB函数
		if (g_calc_hashAB == NULL)
		{
			//AfxMessageBox("Get xgen_hash_ab fun failed");
			return false;
		}
	}
	else
	{
     //   AfxMessageBox("Load iPhoneCalc dll failed");
	}

	//char uuid[] = {0x62, 0xd0, 0xe2, 0x0e, 0xb2, 0xe1, 0x53, 0x30, 0xf3, 0x95, 0xe3, 0x68, 0xd2, 0xa4, 0x0d, 0x0f, 0xe7, 0x5c, 0x0a, 0x44};
	//char uuid[] = {0xd7, 0x69, 0x56, 0xcc, 0x0e, 0x60, 0xb6, 0x17, 0x4d, 0x46, 0x4f, 0xdb, 0x39, 0xba, 0xbf, 0x4d, 0x76, 0x67, 0x11, 0xba};
	//char iTunesCDBPath[] = "D:\\Epan\\task\\iPhone 项目\\iPod算法提取\\iPod管理_发布\\ipod Demo程序代码 与 演示程序发布版本\\发布版本\\tqpod 2011-08-24\\tqpod\\debug\\iTunesCDB";

	//GetiTunesCDBFilePath(iTunesCDBPath);
	//g_nIsHashAB = 1;

	//SetUuidAndHash(uuid, 1);
	//ReSetiTunesCDB(iTunesCDBPath);


	return TRUE;
}


#endif