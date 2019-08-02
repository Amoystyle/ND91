#pragma once

#include "AssistantDll.h"
#include <iostream>
#include <stdio.h>

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef WIN32
typedef unsigned __int32 uint32;
#else
typedef unsigned int uint32;
#endif

typedef unsigned char uint8;

namespace ND91Assistant
{

	typedef struct st_PngChunk{
		uint32 length;
		uint32 name;
		uint8 *data;
		uint32 crc;
		struct st_PngChunk *next;
	} t_PngChunk;

	typedef struct {
		uint32 width;
		uint32 height;
		uint8 bit_depth;
		uint8 color_type;
		uint8 compression;
		uint8 filter;
		uint8 interlace;
	} t_PngIHDRChunk;

	class CLASSINDLL_CLASS_DECL IosPngConver
	{
	public:
		//strSrcFullName : 欲转换的PNG文件名（含路径）
		//strDesFullName : 转换后存放的PNG文件名（含路径）
		static bool ConverApplePngToNormal(
            const wchar_t * strSrcFullName,
            const wchar_t * strDesFullName);

	private:
		IosPngConver(void);
		~IosPngConver(void);
 
		int ConverToNomal(const wchar_t *srcFilename, const wchar_t *dstFilename);

	private:
		void Init();
		void ReadChunks(const wchar_t*);
		void WritePNG(const wchar_t *);
		void CleanUp();

        uint32 CRC(uint32 name, const uint8 *data, size_t size);
        void swapRB(const wchar_t*);

	private:
		bool m_containsCgBI;
		t_PngIHDRChunk m_pngIHDR;

		unsigned int m_maxInflatedBufSize;
		unsigned int m_maxDeflatedBufSize;
		t_PngChunk *m_chunks;		
	};
}
