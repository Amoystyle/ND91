#pragma once

#include "SevenZipWrapper.h"
#include "SevenZipExtractor.h"
#include "SevenZipCompressor.h"

bool SevenZipWrapper::Compress( std::wstring strSrcDir, std::wstring strDesFile )
{
    try
    {
	    SevenZip::SevenZipLibrary lib;
	    lib.Load();
	
	    SevenZip::SevenZipCompressor compressor(lib, strDesFile.c_str());
	    compressor.CompressDirectory(strSrcDir.c_str());

        return true;
    }
//     catch( SevenZip::SevenZipException& e)
//     {
//         return false;
//     }
    catch (...)
    {
    	return false;
    }
}

bool SevenZipWrapper::Extract( std::wstring strSrcFile, std::wstring strDesDir )
{
    try
    {
	    SevenZip::SevenZipLibrary lib;
	    lib.Load();
	
	    SevenZip::SevenZipExtractor extractor(lib, strSrcFile.c_str());
	    extractor.ExtractArchive(strDesDir.c_str());

        return true;
    }
//     catch( SevenZip::SevenZipException& e)
//     {
//         return false;
//     }
    catch (...)
    {
        return false;
    }
}
