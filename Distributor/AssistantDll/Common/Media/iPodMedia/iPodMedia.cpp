#include "stdafx.h"
#include "iPodMp4Paeser.h"

ciPodMedia::ciPodMedia(wstring path)
{
	USES_CONVERSION;
	_MediaData = NULL;
	if(CFileOperation::IsFileExist(path.c_str()))
	{
// 		FILE * fp = NULL;
// 		if (fopen_s(&fp, W2A(path.c_str()), "rb" )!=0 || fp==NULL)
// 		{
// 
// 			fseek( fp, 0, SEEK_SET );
// 
// 			char szTAG[4];
// 			if(  fread( szTAG, 3, 1, fp ) != 1 )
// 				return;
// 
// 			if( strncmp( szTAG, "ID3", 3 ) == 0 )//ID3标志找到由此判断是MP3文件
// 			{
// 				_MediaData= new ciPodMP3Paeser(path);
// 			}
// 			//其他类型在此添加
// 			fclose(fp);
		wstring fileExt = CFileOperation::GetFileExt(path);
		fileExt = CStrOperation::toLowerW(fileExt);
		if ( L"mp3" == fileExt )
			_MediaData = new ciPodMP3Paeser(path);
        else if ( L"m4a" == fileExt || L"m4v" == fileExt || L"mp4" == fileExt)
            _MediaData = new ciPodMP4Paeser(path);
	}
}
ciPodMedia::~ciPodMedia()
{
	if(_MediaData)
		_MediaData->OnDestroy();
	SAFE_DELETE(_MediaData);
}