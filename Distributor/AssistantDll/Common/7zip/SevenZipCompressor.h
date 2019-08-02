#pragma once


#include <vector>
#include "SevenZipLibrary.h"
#include "FileInfo.h"
#include "CompressionLevel.h"


namespace SevenZip
{
	class SevenZipCompressor
	{
	private:

		const SevenZipLibrary& m_library;
		TString m_archivePath;
		CompressionLevelEnum m_compressionLevel;

	public:

		SevenZipCompressor( const SevenZipLibrary& library, const TString& archivePath );
		virtual ~SevenZipCompressor();

		void SetCompressionLevel( const CompressionLevelEnum& level );

		// Includes the last directory as the root in the archive, e.g. specifying "C:\Temp\MyFolder"
		// makes "MyFolder" the single root item in archive with the files within it included.
		virtual void CompressDirectory( const TString& directory, bool includeSubdirs = true );

		// Excludes the last directory as the root in the archive, its contents are at root instead. E.g.
		// specifying "C:\Temp\MyFolder" make the files in "MyFolder" the root items in the archive.
		virtual void CompressFiles( const TString& directory, const TString& searchFilter, bool includeSubdirs = true );
		virtual void CompressAllFiles( const TString& directory, bool includeSubdirs = true );

		// Compress just this single file as the root item in the archive.
		virtual void CompressFile( const TString& filePath );

	private:

		CComPtr< IStream > OpenArchiveStream();
		void FindAndCompressFiles( const TString& directory, const TString& searchPattern, const TString& pathPrefix, bool recursion );
		void CompressFilesToArchive( const TString& pathPrefix, const std::vector< intl::FilePathInfo >& filePaths );
		void SetCompressionProperties( IUnknown* outArchive );
	};
}
