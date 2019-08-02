#include "stdafx.h"

ciPodMP3Paeser::ciPodMP3Paeser(wstring path)
{
	_Mp3Data = new MP3Data(path);
}
void ciPodMP3Paeser::OnDestroy()
{
	SAFE_DELETE(_Mp3Data);
}