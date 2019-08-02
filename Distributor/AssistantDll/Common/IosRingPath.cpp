#include "stdafx.h"

#include "IosRingPath.h"
#include "Common/Path.h"

IosRingPath::IosRingPath(const wstring workFolder, bool cracked)
: _workFolder(workFolder)
, _cracked(cracked)
{
}

wstring IosRingPath::GetLocalRingListPath() const
{
	return _workFolder + L"Ringtones.plist";
}

wstring IosRingPath::GetLocalRingSetListPath() const
{
	return _workFolder + L"ringSetPlist.plist";
}

wstring IosRingPath::GetIosRingListPath() const
{
    return _cracked ?
        L"/private/var/mobile/Media/iTunes_Control/iTunes/Ringtones.plist" :
        L"/iTunes_Control/iTunes/Ringtones.plist";
}

wstring IosRingPath::GetIosRingSetListPath() const
{
	return L"/private/var/mobile/Library/Preferences/com.apple.springboard.plist";
}

wstring IosRingPath::GetIosUserRingPath() const
{
    return _cracked ?
        L"/private/var/mobile/Media/iTunes_Control/Ringtones/" :
        L"/iTunes_Control/Ringtones/";
}

wstring IosRingPath::GetIosSyetemRingPath() const
{
	return L"/System/Library/Audio/UISounds/";
}

std::wstring ND91Assistant::IosRingPath::GetLocalRingWorkPath() const
{
	//return _workFolder+L"\\Ringtones\\";
    return _workFolder + WSTRING_PATH_SEPARATER + L"Ringtones" + WSTRING_PATH_SEPARATER;
}

std::wstring ND91Assistant::IosRingPath::GetIosRingLibraryFolder() const
{
	return L"/Library/Ringtones/";
}