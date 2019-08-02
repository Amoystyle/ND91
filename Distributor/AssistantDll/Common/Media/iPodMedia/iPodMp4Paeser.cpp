#include "stdafx.h"
#include "iPodMp4Paeser.h"

ciPodMP4Paeser::ciPodMP4Paeser(wstring path)
{
	_Mp4Parser = new Mp4Parser(path);
    _media = _Mp4Parser->GetMediaData();
}
void ciPodMP4Paeser::OnDestroy()
{
	SAFE_DELETE(_Mp4Parser);
}