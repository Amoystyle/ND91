#pragma once

#include <string>

class SevenZipWrapper
{
public:
	// Summary  : ��ѹ
	// Returns  : bool �ɹ�����TURE
	// Parameter: std::wstring strSrcFile ��Ҫ��ѹ��7z�ļ�
	// Parameter: std::wstring strDesDir  ����ѹ���ŵ�·��
	static bool Extract(std::wstring strSrcFile, std::wstring strDesDir);

    // Summary  : ѹ��
    // Returns  : bool �ɹ�����TURE
    // Parameter: std::wstring strSrcDir  ��Ҫ��ѹ��Ŀ¼
    // Parameter: std::wstring strDesFile ��ѹ���ļ�·��
    static bool Compress(std::wstring strSrcDir, std::wstring strDesFile);
};

