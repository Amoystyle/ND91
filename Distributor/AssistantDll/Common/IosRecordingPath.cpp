#include "stdafx.h"

#include "IosRecordingPath.h"
#include "Common/Path.h"

IosRecordingPath::IosRecordingPath(const wstring workFolder, bool cracked)
: _workFolder(workFolder)
, _cracked(cracked)
{
}



wstring IosRecordingPath::GetLocalRecordingPath() const
{
	//return _workFolder + L"Recordings\\";
    return _workFolder + L"Recordings" + WSTRING_PATH_SEPARATER;
}

wstring IosRecordingPath::GetLocalVoiceMemosDBPath() const
{
	return GetLocalRecordingPath() + L"Recordings.db";
}

wstring IosRecordingPath::GetIosVoiceMemosDBPath() const
{
	return _cracked ?
		L"/var/mobile/Media/Recordings/Recordings.db":
		L"/Recordings/Recordings.db";
}
