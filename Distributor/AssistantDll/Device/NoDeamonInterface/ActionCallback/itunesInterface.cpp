/*
 *  itunesInterface.cpp
 *
 *  Created by lindaojiang on 2012-6-8
 *  Copyright 2012 NetDragon Websoft Inc. All rights reserved.
 *
 */

#include "stdafx.h"
#include "itunesInterface.h"
#include <windows.h>
#include <stdlib.h>
#include "InfoLog.h"

//##################################################################################################################
static const CHAR * __gpf_iTunesMobileDevice_dll_text[ENUM_ITUNESMOBILEDEVICE_DLL_NUM] = 
{
	("AMDeviceNotificationSubscribe"),
	("AMDeviceGetConnectionID"),
	("AMDeviceCopyDeviceIdentifier"),
	("AMDeviceCopyValue"),
	("AMDeviceConnect"),
	("AMDeviceDisconnect"),
	("AMDeviceIsPaired"),
	("AMDeviceValidatePairing"),
	("AMDeviceStartSession"),
	("AMDeviceStopSession"),
	("AMDeviceRelease"),
	("AMDeviceRetain"),
	("AMDeviceStartService"),
	("AFCConnectionOpen"),
	("AFCConnectionClose"),
	("AFCFileRefOpen"),
	("AFCFileRefClose"),
	("AFCFileRefRead"),
	("AFCFileRefWrite"),
	("AFCFileRefSeek"),
	("AFCFileInfoOpen"),
	("AFCDirectoryOpen"),
	("AFCDirectoryRead"),
	("AFCDirectoryClose"),
	("AFCDirectoryCreate"),
	("AFCRemovePath"),
	("AFCRenamePath"),
	("AFCKeyValueRead"),
	("AFCKeyValueClose"),
	("AMDeviceGetInterfaceType"),
	("AMDevicePair"),
	("AMDPostNotification"),
	("AMDShutdownNotificationProxy"),
	("AMDeviceSetWirelessBuddyFlags"),
	("AMDeviceSecureStartService"),
	("AMDServiceConnectionGetSocket"),
	("AMDServiceConnectionGetSecureIOContext"),
	("AFCConnectionSetSecureContext"),
	("AMDServiceConnectionReceive"),
	("AMDServiceConnectionSend"),
	("AMDeviceCreateHouseArrestService"),
	("AMDeviceLookupApplications"),
	("AMDeviceSecureInstallApplication"),
	("USBMuxConnectByPort"),
};

FARPROC __gpf_iTunesMobileDevice_dll_ri[ENUM_ITUNESMOBILEDEVICE_DLL_NUM];

static HINSTANCE __g_iTunesMobileDevice_hDLL;
//##################################################################################################################
//##################################################################################################################
static const CHAR * __gpf_AirTrafficHost_dll_text[ENUM_AIRTRAFFICHOST_DLL_NUM] = 
{
	//("ATAddAppleSearchPathsToEnvironmentFromReg"),
	//("ATCFMessageCreate"),
	//("ATCFMessageGetName"),
	//("ATCFMessageGetParam"),
	//("ATCFMessageGetSessionNumber"),
	//("ATCFMessageVerify"),
	("ATHostAdvertiserStartWakeupService"),
	("ATHostAdvertiserStopWakeupService"),
	("ATHostConnectionCreate"),
	("ATHostConnectionCreateWithLibrary"),
	("ATHostConnectionDestroy"),
	("ATHostConnectionGetCurrentSessionNumber"),
	("ATHostConnectionGetGrappaSessionId"),
	("ATHostConnectionInvalidate"),
	("ATHostConnectionReadMessage"),
	("ATHostConnectionRelease"),
	("ATHostConnectionRetain"),
	("ATHostConnectionSendAssetCompleted"),
	("ATHostConnectionSendAssetMetricsRequest"),
	("ATHostConnectionSendFileBegin"),
	("ATHostConnectionSendFileError"),
	("ATHostConnectionSendFileProgress"),
	("ATHostConnectionSendHostInfo"),
	("ATHostConnectionSendMessage"),
	("ATHostConnectionSendMetadataSyncFinished"),
	("ATHostConnectionSendPing"),
	("ATHostConnectionSendPowerAssertion"),
	("ATHostConnectionSendSyncFailed"),
	("ATHostConnectionSendSyncRequest"),
	//("ATProcessLinkCopyMessageFromChild"),
	//("ATProcessLinkExitChild"),
	//("ATProcessLinkInvalidate"),
	//("ATProcessLinkRelease"),
	//("ATProcessLinkRetain"),
	//("ATProcessLinkRunChild"),
	//("ATProcessLinkSendMessage"),
	//("ATProcessLinkSendMessageToParent"),
	//("ATProcessLinkSetupParent"),
};

FARPROC __gpf_AirTrafficHost_dll_ri[ENUM_AIRTRAFFICHOST_DLL_NUM];

static HINSTANCE __g_AirTrafficHost_hDLL;
//##################################################################################################################

//##################################################################################################################
static const CHAR * __gpf_CoreFoundation_dll_text[ENUM_COREFOUNDATION_DLL_NUM] = 
{
	//("CFAbsoluteTimeAddGregorianUnits"),
	//("CFAbsoluteTimeGetCurrent"),
	//("CFAbsoluteTimeGetDayOfWeek"),
	//("CFAbsoluteTimeGetDayOfYear"),
	//("CFAbsoluteTimeGetDifferenceAsGregorianUnits"),
	//("CFAbsoluteTimeGetGregorianDate"),
	//("CFAbsoluteTimeGetWeekOfYear"),
	//("CFAllocatorAllocate"),
	//("CFAllocatorCreate"),
	//("CFAllocatorDeallocate"),
	//("CFAllocatorGetContext"),
	//("CFAllocatorGetDefault"),
	//("CFAllocatorGetPreferredSizeForSize"),
	//("CFAllocatorGetTypeID"),
	//("CFAllocatorReallocate"),
	//("CFAllocatorSetDefault"),
	//("CFArrayAppendArray"),
	("CFArrayAppendValue"),
	//("CFArrayApplyFunction"),
	//("CFArrayBSearchValues"),
	//("CFArrayContainsValue"),
	//("CFArrayCreate"),
	("CFArrayCreateCopy"),
	("CFArrayCreateMutable"),
	//("CFArrayCreateMutableCopy"),
	//("CFArrayExchangeValuesAtIndices"),
	("CFArrayGetCount"),
	//("CFArrayGetCountOfValue"),
	//("CFArrayGetFirstIndexOfValue"),
	//("CFArrayGetLastIndexOfValue"),
	//("CFArrayGetTypeID"),
	("CFArrayGetValueAtIndex"),
	//("CFArrayGetValues"),
	//("CFArrayInsertValueAtIndex"),
	//("CFArrayRemoveAllValues"),
	//("CFArrayRemoveValueAtIndex"),
	//("CFArrayReplaceValues"),
	//("CFArraySetValueAtIndex"),
	//("CFArraySortValues"),
	//("CFAttributedStringBeginEditing"),
	//("CFAttributedStringCreate"),
	//("CFAttributedStringCreateCopy"),
	//("CFAttributedStringCreateMutable"),
	//("CFAttributedStringCreateMutableCopy"),
	//("CFAttributedStringCreateWithSubstring"),
	//("CFAttributedStringEndEditing"),
	//("CFAttributedStringGetAttribute"),
	//("CFAttributedStringGetAttributeAndLongestEffectiveRange"),
	//("CFAttributedStringGetAttributes"),
	//("CFAttributedStringGetAttributesAndLongestEffectiveRange"),
	//("CFAttributedStringGetLength"),
	//("CFAttributedStringGetMutableString"),
	//("CFAttributedStringGetString"),
	//("CFAttributedStringGetTypeID"),
	//("CFAttributedStringOpenUText"),
	//("CFAttributedStringRemoveAttribute"),
	//("CFAttributedStringReplaceAttributedString"),
	//("CFAttributedStringReplaceString"),
	//("CFAttributedStringSetAttribute"),
	//("CFAttributedStringSetAttributes"),
	//("CFBSearch"),
	//("CFBagAddValue"),
	//("CFBagApplyFunction"),
	//("CFBagContainsValue"),
	//("CFBagCreate"),
	//("CFBagCreateCopy"),
	//("CFBagCreateMutable"),
	//("CFBagCreateMutableCopy"),
	//("CFBagGetCount"),
	//("CFBagGetCountOfValue"),
	//("CFBagGetTypeID"),
	//("CFBagGetValue"),
	//("CFBagGetValueIfPresent"),
	//("CFBagGetValues"),
	//("CFBagRemoveAllValues"),
	//("CFBagRemoveValue"),
	//("CFBagReplaceValue"),
	//("CFBagSetValue"),
	//("CFBinaryHeapAddValue"),
	//("CFBinaryHeapApplyFunction"),
	//("CFBinaryHeapContainsValue"),
	//("CFBinaryHeapCreate"),
	//("CFBinaryHeapCreateCopy"),
	//("CFBinaryHeapGetCount"),
	//("CFBinaryHeapGetCountOfValue"),
	//("CFBinaryHeapGetMinimum"),
	//("CFBinaryHeapGetMinimumIfPresent"),
	//("CFBinaryHeapGetTypeID"),
	//("CFBinaryHeapGetValues"),
	//("CFBinaryHeapRemoveAllValues"),
	//("CFBinaryHeapRemoveMinimumValue"),
	//("CFBitVectorContainsBit"),
	//("CFBitVectorCreate"),
	//("CFBitVectorCreateCopy"),
	//("CFBitVectorCreateMutable"),
	//("CFBitVectorCreateMutableCopy"),
	//("CFBitVectorFlipBitAtIndex"),
	//("CFBitVectorFlipBits"),
	//("CFBitVectorGetBitAtIndex"),
	//("CFBitVectorGetBits"),
	//("CFBitVectorGetCount"),
	//("CFBitVectorGetCountOfBit"),
	//("CFBitVectorGetFirstIndexOfBit"),
	//("CFBitVectorGetLastIndexOfBit"),
	//("CFBitVectorGetTypeID"),
	//("CFBitVectorSetAllBits"),
	//("CFBitVectorSetBitAtIndex"),
	//("CFBitVectorSetBits"),
	//("CFBitVectorSetCount"),
	//("CFBooleanGetTypeID"),
	("CFBooleanGetValue"),
	//("CFBundleCopyAuxiliaryExecutableURL"),
	//("CFBundleCopyBuiltInPlugInsURL"),
	//("CFBundleCopyBundleLocalizations"),
	//("CFBundleCopyBundleURL"),
	//("CFBundleCopyExecutableArchitectures"),
	//("CFBundleCopyExecutableArchitecturesForURL"),
	//("CFBundleCopyExecutableURL"),
	//("CFBundleCopyInfoDictionaryForURL"),
	//("CFBundleCopyInfoDictionaryInDirectory"),
	//("CFBundleCopyLocalizationForLocalizationInfo"),
	//("CFBundleCopyLocalizationsForPreferences"),
	//("CFBundleCopyLocalizationsForURL"),
	//("CFBundleCopyLocalizedString"),
	//("CFBundleCopyPreferredLocalizationsFromArray"),
	//("CFBundleCopyPrivateFrameworksURL"),
	//("CFBundleCopyResourceURL"),
	//("CFBundleCopyResourceURLForLocalization"),
	//("CFBundleCopyResourceURLInDirectory"),
	//("CFBundleCopyResourceURLsOfType"),
	//("CFBundleCopyResourceURLsOfTypeForLocalization"),
	//("CFBundleCopyResourceURLsOfTypeInDirectory"),
	//("CFBundleCopyResourcesDirectoryURL"),
	//("CFBundleCopySharedFrameworksURL"),
	//("CFBundleCopySharedSupportURL"),
	//("CFBundleCopySupportFilesDirectoryURL"),
	//("CFBundleCreate"),
	//("CFBundleCreateBundlesFromDirectory"),
	//("CFBundleGetAllBundles"),
	//("CFBundleGetBundleWithIdentifier"),
	//("CFBundleGetDataPointerForName"),
	//("CFBundleGetDataPointersForNames"),
	//("CFBundleGetDevelopmentRegion"),
	//("CFBundleGetFunctionPointerForName"),
	//("CFBundleGetFunctionPointersForNames"),
	//("CFBundleGetIdentifier"),
	//("CFBundleGetInfoDictionary"),
	//("CFBundleGetLocalInfoDictionary"),
	//("CFBundleGetLocalizationInfoForLocalization"),
	//("CFBundleGetMainBundle"),
	//("CFBundleGetPackageInfo"),
	//("CFBundleGetPackageInfoInDirectory"),
	//("CFBundleGetPlugIn"),
	//("CFBundleGetTypeID"),
	//("CFBundleGetValueForInfoDictionaryKey"),
	//("CFBundleGetVersionNumber"),
	//("CFBundleIsExecutableLoaded"),
	//("CFBundleLoadExecutable"),
	//("CFBundleLoadExecutableAndReturnError"),
	//("CFBundlePreflightExecutable"),
	//("CFBundleUnloadExecutable"),
	//("CFBurstTrieAdd"),
	//("CFBurstTrieAddCharacters"),
	//("CFBurstTrieAddCharactersWithWeight"),
	//("CFBurstTrieAddUTF8String"),
	//("CFBurstTrieAddUTF8StringWithWeight"),
	//("CFBurstTrieAddWithWeight"),
	//("CFBurstTrieContains"),
	//("CFBurstTrieContainsCharacters"),
	//("CFBurstTrieContainsUTF8String"),
	//("CFBurstTrieCreate"),
	//("CFBurstTrieCreateFromFile"),
	//("CFBurstTrieCreateFromMapBytes"),
	//("CFBurstTrieFind"),
	//("CFBurstTrieFindCharacters"),
	//("CFBurstTrieFindUTF8String"),
	//("CFBurstTrieGetCount"),
	//("CFBurstTrieInsert"),
	//("CFBurstTrieInsertCharacters"),
	//("CFBurstTrieInsertCharactersWithWeight"),
	//("CFBurstTrieInsertUTF8String"),
	//("CFBurstTrieInsertUTF8StringWithWeight"),
	//("CFBurstTrieInsertWithWeight"),
	//("CFBurstTrieRelease"),
	//("CFBurstTrieRetain"),
	//("CFBurstTrieSerialize"),
	//("CFBurstTrieSerializeWithFileDescriptor"),
	//("CFBurstTrieTraverse"),
	//("CFCalendarAddComponents"),
	//("CFCalendarComposeAbsoluteTime"),
	//("CFCalendarCopyCurrent"),
	//("CFCalendarCopyGregorianStartDate"),
	//("CFCalendarCopyLocale"),
	//("CFCalendarCopyTimeZone"),
	//("CFCalendarCreateWithIdentifier"),
	//("CFCalendarDecomposeAbsoluteTime"),
	//("CFCalendarGetComponentDifference"),
	//("CFCalendarGetFirstWeekday"),
	//("CFCalendarGetIdentifier"),
	//("CFCalendarGetMaximumRangeOfUnit"),
	//("CFCalendarGetMinimumDaysInFirstWeek"),
	//("CFCalendarGetMinimumRangeOfUnit"),
	//("CFCalendarGetOrdinalityOfUnit"),
	//("CFCalendarGetRangeOfUnit"),
	//("CFCalendarGetTimeRangeOfUnit"),
	//("CFCalendarGetTypeID"),
	//("CFCalendarSetFirstWeekday"),
	//("CFCalendarSetGregorianStartDate"),
	//("CFCalendarSetLocale"),
	//("CFCalendarSetMinimumDaysInFirstWeek"),
	//("CFCalendarSetTimeZone"),
	//("CFCharacterSetAddCharactersInRange"),
	//("CFCharacterSetAddCharactersInString"),
	//("CFCharacterSetCompact"),
	//("CFCharacterSetCreateBitmapRepresentation"),
	//("CFCharacterSetCreateCopy"),
	//("CFCharacterSetCreateInvertedSet"),
	//("CFCharacterSetCreateMutable"),
	//("CFCharacterSetCreateMutableCopy"),
	//("CFCharacterSetCreateWithBitmapRepresentation"),
	//("CFCharacterSetCreateWithCharactersInRange"),
	//("CFCharacterSetCreateWithCharactersInString"),
	//("CFCharacterSetFast"),
	//("CFCharacterSetGetPredefined"),
	//("CFCharacterSetGetTypeID"),
	//("CFCharacterSetHasMemberInPlane"),
	//("CFCharacterSetInitInlineBuffer"),
	//("CFCharacterSetIntersect"),
	//("CFCharacterSetInvert"),
	//("CFCharacterSetIsCharacterMember"),
	//("CFCharacterSetIsLongCharacterMember"),
	//("CFCharacterSetIsSupersetOfSet"),
	//("CFCharacterSetIsSurrogatePairMember"),
	//("CFCharacterSetRemoveCharactersInRange"),
	//("CFCharacterSetRemoveCharactersInString"),
	//("CFCharacterSetUnion"),
	//("CFCopyDescription"),
	//("CFCopyHomeDirectoryURLForUser"),
	//("CFCopySearchPathForDirectoriesInDomains"),
	//("CFCopySystemVersionString"),
	//("CFCopyTypeIDDescription"),
	//("CFCopyUserName"),
	//("CFDataAppendBytes"),
	("CFDataCreate"),
	//("CFDataCreateCopy"),
	//("CFDataCreateMutable"),
	//("CFDataCreateMutableCopy"),
	//("CFDataCreateWithBytesNoCopy"),
	//("CFDataDeleteBytes"),
	//("CFDataFind"),
	("CFDataGetBytePtr"),
	//("CFDataGetBytes"),
	("CFDataGetLength"),
	//("CFDataGetMutableBytePtr"),
	//("CFDataGetTypeID"),
	//("CFDataIncreaseLength"),
	//("CFDataReplaceBytes"),
	//("CFDataSetLength"),
	//("CFDateCompare"),
	//("CFDateCreate"),
	//("CFDateFormatterCopyProperty"),
	//("CFDateFormatterCreate"),
	//("CFDateFormatterCreateDateFormatFromTemplate"),
	//("CFDateFormatterCreateDateFormatsFromTemplates"),
	//("CFDateFormatterCreateDateFromString"),
	//("CFDateFormatterCreateStringWithAbsoluteTime"),
	//("CFDateFormatterCreateStringWithDate"),
	//("CFDateFormatterGetAbsoluteTimeFromString"),
	//("CFDateFormatterGetDateStyle"),
	//("CFDateFormatterGetFormat"),
	//("CFDateFormatterGetLocale"),
	//("CFDateFormatterGetTimeStyle"),
	//("CFDateFormatterGetTypeID"),
	//("CFDateFormatterSetFormat"),
	//("CFDateFormatterSetProperty"),
	//("CFDateGetAbsoluteTime"),
	//("CFDateGetTimeIntervalSinceDate"),
	//("CFDateGetTypeID"),
	("CFDictionaryAddValue"),
	//("CFDictionaryApplyFunction"),
	//("CFDictionaryContainsKey"),
	//("CFDictionaryContainsValue"),
	//("CFDictionaryCreate"),
	("CFDictionaryCreateCopy"),
	("CFDictionaryCreateMutable"),
	("CFDictionaryCreateMutableCopy"),
	("CFDictionaryGetCount"),
	//("CFDictionaryGetCountOfKey"),
	//("CFDictionaryGetCountOfValue"),
	//("CFDictionaryGetKeyIfPresent"),
	("CFDictionaryGetKeysAndValues"),
	("CFDictionaryGetTypeID"),
	("CFDictionaryGetValue"),
	//("CFDictionaryGetValueIfPresent"),
	//("CFDictionaryRemoveAllValues"),
	//("CFDictionaryRemoveValue"),
	//("CFDictionaryReplaceValue"),
	("CFDictionarySetValue"),
	//("CFEqual"),
	//("CFErrorCopyDescription"),
	//("CFErrorCopyFailureReason"),
	//("CFErrorCopyRecoverySuggestion"),
	//("CFErrorCopyUserInfo"),
	//("CFErrorCreate"),
	//("CFErrorCreateWithUserInfoKeysAndValues"),
	//("CFErrorGetCallBackForDomain"),
	//("CFErrorGetCode"),
	//("CFErrorGetDomain"),
	//("CFErrorGetTypeID"),
	//("CFErrorSetCallBackForDomain"),
	//("CFGetAllocator"),
	//("CFGetRetainCount"),
	//("CFGetSystemUptime"),
	("CFGetTypeID"),
	//("CFGetUserName"),
	//("CFGregorianDateGetAbsoluteTime"),
	//("CFGregorianDateIsValid"),
	//("CFHash"),
	//("CFHashBytes"),
	//("CFLocaleCopyAvailableLocaleIdentifiers"),
	//("CFLocaleCopyCommonISOCurrencyCodes"),
	//("CFLocaleCopyCurrent"),
	//("CFLocaleCopyDisplayNameForPropertyValue"),
	//("CFLocaleCopyISOCountryCodes"),
	//("CFLocaleCopyISOCurrencyCodes"),
	//("CFLocaleCopyISOLanguageCodes"),
	//("CFLocaleCopyPreferredLanguages"),
	//("CFLocaleCreate"),
	//("CFLocaleCreateCanonicalLanguageIdentifierFromString"),
	//("CFLocaleCreateCanonicalLocaleIdentifierFromScriptManagerCodes"),
	//("CFLocaleCreateCanonicalLocaleIdentifierFromString"),
	//("CFLocaleCreateComponentsFromLocaleIdentifier"),
	//("CFLocaleCreateCopy"),
	//("CFLocaleCreateLocaleIdentifierFromComponents"),
	//("CFLocaleCreateLocaleIdentifierFromWindowsLocaleCode"),
	//("CFLocaleGetIdentifier"),
	//("CFLocaleGetLanguageCharacterDirection"),
	//("CFLocaleGetLanguageLineDirection"),
	//("CFLocaleGetLanguageRegionEncodingForLocaleIdentifier"),
	//("CFLocaleGetSystem"),
	//("CFLocaleGetTypeID"),
	//("CFLocaleGetValue"),
	//("CFLocaleGetWindowsLocaleCodeFromLocaleIdentifier"),
	//("CFLog"),
	//("CFMakeCollectable"),
	//("CFMergeSortArray"),
	//("CFMessagePortCreateLocal"),
	//("CFMessagePortCreateRemote"),
	//("CFMessagePortCreateRunLoopSource"),
	//("CFMessagePortCreateUber"),
	//("CFMessagePortGetContext"),
	//("CFMessagePortGetInvalidationCallBack"),
	//("CFMessagePortGetName"),
	//("CFMessagePortGetTypeID"),
	//("CFMessagePortInvalidate"),
	//("CFMessagePortIsRemote"),
	//("CFMessagePortIsValid"),
	//("CFMessagePortSendRequest"),
	//("CFMessagePortSetCloneCallout"),
	//("CFMessagePortSetInvalidationCallBack"),
	//("CFMessagePortSetName"),
	//("CFMutableAttributedStringOpenUText"),
	//("CFMutableStringOpenUText"),
	//("CFNotificationCenterAddObserver"),
	//("CFNotificationCenterGetDarwinNotifyCenter"),
	//("CFNotificationCenterGetDistributedCenter"),
	//("CFNotificationCenterGetLocalCenter"),
	//("CFNotificationCenterGetTypeID"),
	//("CFNotificationCenterPostNotification"),
	//("CFNotificationCenterPostNotificationWithOptions"),
	//("CFNotificationCenterRemoveEveryObserver"),
	//("CFNotificationCenterRemoveObserver"),
	//("CFNullGetTypeID"),
	//("CFNumberCompare"),
	("CFNumberCreate"),
	//("CFNumberFormatterCopyProperty"),
	//("CFNumberFormatterCreate"),
	//("CFNumberFormatterCreateNumberFromString"),
	//("CFNumberFormatterCreateStringWithNumber"),
	//("CFNumberFormatterCreateStringWithValue"),
	//("CFNumberFormatterGetDecimalInfoForCurrencyCode"),
	//("CFNumberFormatterGetFormat"),
	//("CFNumberFormatterGetLocale"),
	//("CFNumberFormatterGetStyle"),
	//("CFNumberFormatterGetTypeID"),
	//("CFNumberFormatterGetValueFromString"),
	//("CFNumberFormatterSetFormat"),
	//("CFNumberFormatterSetProperty"),
	//("CFNumberGetByteSize"),
	//("CFNumberGetType"),
	//("CFNumberGetTypeID"),
	("CFNumberGetValue"),
	//("CFNumberIsFloatType"),
	//("CFPlugInAddInstanceForFactory"),
	//("CFPlugInCreate"),
	//("CFPlugInFindFactoriesForPlugInType"),
	//("CFPlugInFindFactoriesForPlugInTypeInPlugIn"),
	//("CFPlugInGetBundle"),
	//("CFPlugInGetTypeID"),
	//("CFPlugInInstanceCreate"),
	//("CFPlugInInstanceCreateWithInstanceDataSize"),
	//("CFPlugInInstanceGetFactoryName"),
	//("CFPlugInInstanceGetInstanceData"),
	//("CFPlugInInstanceGetInterfaceFunctionTable"),
	//("CFPlugInInstanceGetTypeID"),
	//("CFPlugInIsLoadOnDemand"),
	//("CFPlugInRegisterFactoryFunction"),
	//("CFPlugInRegisterFactoryFunctionByName"),
	//("CFPlugInRegisterPlugInType"),
	//("CFPlugInRemoveInstanceForFactory"),
	//("CFPlugInSetLoadOnDemand"),
	//("CFPlugInUnregisterFactory"),
	//("CFPlugInUnregisterPlugInType"),
	//("CFPreferencesAddSuitePreferencesToApp"),
	//("CFPreferencesAppSynchronize"),
	//("CFPreferencesAppValueIsForced"),
	//("CFPreferencesCopyAppValue"),
	//("CFPreferencesCopyApplicationList"),
	//("CFPreferencesCopyKeyList"),
	//("CFPreferencesCopyMultiple"),
	//("CFPreferencesCopyValue"),
	//("CFPreferencesFlushCaches"),
	//("CFPreferencesGetAppBooleanValue"),
	//("CFPreferencesGetAppIntegerValue"),
	//("CFPreferencesRemoveSuitePreferencesFromApp"),
	//("CFPreferencesSetAppValue"),
	//("CFPreferencesSetMultiple"),
	//("CFPreferencesSetValue"),
	//("CFPreferencesSynchronize"),
	("CFPropertyListCreateData"),
	//("CFPropertyListCreateDeepCopy"),
	//("CFPropertyListCreateFromStream"),
	("CFPropertyListCreateFromXMLData"),
	//("CFPropertyListCreateWithData"),
	//("CFPropertyListCreateWithStream"),
	("CFPropertyListCreateXMLData"),
	//("CFPropertyListIsValid"),
	//("CFPropertyListWrite"),
	("CFPropertyListWriteToStream"),
	//("CFQSortArray"),
	//("CFReadStreamClose"),
	//("CFReadStreamCopyError"),
	//("CFReadStreamCopyProperty"),
	//("CFReadStreamCreate"),
	//("CFReadStreamCreateWithBytesNoCopy"),
	//("CFReadStreamCreateWithData"),
	//("CFReadStreamCreateWithFile"),
	//("CFReadStreamGetBuffer"),
	//("CFReadStreamGetError"),
	//("CFReadStreamGetInfoPointer"),
	//("CFReadStreamGetStatus"),
	//("CFReadStreamGetTypeID"),
	//("CFReadStreamHasBytesAvailable"),
	//("CFReadStreamOpen"),
	//("CFReadStreamRead"),
	//("CFReadStreamScheduleWithRunLoop"),
	//("CFReadStreamSetClient"),
	//("CFReadStreamSetProperty"),
	//("CFReadStreamSignalEvent"),
	//("CFReadStreamUnscheduleFromRunLoop"),
	("CFRelease"),
	//("CFRetain"),
	//("CFRunArrayCreate"),
	//("CFRunArrayDelete"),
	//("CFRunArrayGetCount"),
	//("CFRunArrayGetTypeID"),
	//("CFRunArrayGetValueAtIndex"),
	//("CFRunArrayGetValueAtRunArrayIndex"),
	//("CFRunArrayInsert"),
	//("CFRunArrayReplace"),
	//("CFRunLoopAddCommonMode"),
	//("CFRunLoopAddObserver"),
	//("CFRunLoopAddSource"),
	//("CFRunLoopAddTimer"),
	//("CFRunLoopContainsObserver"),
	//("CFRunLoopContainsSource"),
	//("CFRunLoopContainsTimer"),
	//("CFRunLoopCopyAllModes"),
	//("CFRunLoopCopyCurrentMode"),
	//("CFRunLoopGetCurrent"),
	//("CFRunLoopGetMain"),
	//("CFRunLoopGetNextTimerFireDate"),
	//("CFRunLoopGetTypeID"),
	//("CFRunLoopIsWaiting"),
	//("CFRunLoopObserverCreate"),
	//("CFRunLoopObserverCreateWithHandler"),
	//("CFRunLoopObserverDoesRepeat"),
	//("CFRunLoopObserverGetActivities"),
	//("CFRunLoopObserverGetContext"),
	//("CFRunLoopObserverGetOrder"),
	//("CFRunLoopObserverGetTypeID"),
	//("CFRunLoopObserverInvalidate"),
	//("CFRunLoopObserverIsValid"),
	//("CFRunLoopPerformBlock"),
	//("CFRunLoopRemoveObserver"),
	//("CFRunLoopRemoveSource"),
	//("CFRunLoopRemoveTimer"),
	//("CFRunLoopRun"),
	//("CFRunLoopRunInMode"),
	//("CFRunLoopSourceCreate"),
	//("CFRunLoopSourceGetContext"),
	//("CFRunLoopSourceGetOrder"),
	//("CFRunLoopSourceGetTypeID"),
	//("CFRunLoopSourceInvalidate"),
	//("CFRunLoopSourceIsValid"),
	//("CFRunLoopSourceSignal"),
	//("CFRunLoopStop"),
	//("CFRunLoopTimerCreate"),
	//("CFRunLoopTimerCreateWithHandler"),
	//("CFRunLoopTimerDoesRepeat"),
	//("CFRunLoopTimerGetContext"),
	//("CFRunLoopTimerGetInterval"),
	//("CFRunLoopTimerGetNextFireDate"),
	//("CFRunLoopTimerGetOrder"),
	//("CFRunLoopTimerGetTypeID"),
	//("CFRunLoopTimerInvalidate"),
	//("CFRunLoopTimerIsValid"),
	//("CFRunLoopTimerSetNextFireDate"),
	//("CFRunLoopWakeUp"),
	//("CFSetAddValue"),
	//("CFSetApplyFunction"),
	//("CFSetContainsValue"),
	//("CFSetCreate"),
	//("CFSetCreateCopy"),
	//("CFSetCreateMutable"),
	//("CFSetCreateMutableCopy"),
	//("CFSetGetCount"),
	//("CFSetGetCountOfValue"),
	//("CFSetGetTypeID"),
	//("CFSetGetValue"),
	//("CFSetGetValueIfPresent"),
	//("CFSetGetValues"),
	//("CFSetRemoveAllValues"),
	//("CFSetRemoveValue"),
	//("CFSetReplaceValue"),
	//("CFSetSetValue"),
	//("CFShow"),
	//("CFShowStr"),
	//("CFSocketConnectToAddress"),
	//("CFSocketCopyAddress"),
	//("CFSocketCopyPeerAddress"),
	//("CFSocketCopyRegisteredSocketSignature"),
	//("CFSocketCopyRegisteredValue"),
	//("CFSocketCreate"),
	//("CFSocketCreateConnectedToSocketSignature"),
	//("CFSocketCreateRunLoopSource"),
	//("CFSocketCreateWithNative"),
	//("CFSocketCreateWithSocketSignature"),
	//("CFSocketDisableCallBacks"),
	//("CFSocketEnableCallBacks"),
	//("CFSocketGetContext"),
	//("CFSocketGetDefaultNameRegistryPortNumber"),
	//("CFSocketGetNative"),
	//("CFSocketGetSocketFlags"),
	//("CFSocketGetTypeID"),
	//("CFSocketInvalidate"),
	//("CFSocketIsValid"),
	//("CFSocketRegisterSocketSignature"),
	//("CFSocketRegisterValue"),
	//("CFSocketSendData"),
	//("CFSocketSetAddress"),
	//("CFSocketSetDefaultNameRegistryPortNumber"),
	//("CFSocketSetSocketFlags"),
	//("CFSocketUnregister"),
	//("CFStorageApplyBlock"),
	//("CFStorageApplyFunction"),
	//("CFStorageCreate"),
	//("CFStorageCreateWithSubrange"),
	//("CFStorageDeleteValues"),
	//("CFStorageGetConstValueAtIndex"),
	//("CFStorageGetCount"),
	//("CFStorageGetTypeID"),
	//("CFStorageGetValueAtIndex"),
	//("CFStorageGetValues"),
	//("CFStorageInsertValues"),
	//("CFStorageReplaceValues"),
	//("CFStreamCreateBoundPair"),
	//("CFStreamCreatePairWithPeerSocketSignature"),
	//("CFStreamCreatePairWithSocket"),
	//("CFStreamCreatePairWithSocketToHost"),
	//("CFStringAppend"),
	//("CFStringAppendCString"),
	//("CFStringAppendCharacters"),
	//("CFStringAppendFormat"),
	//("CFStringAppendFormatAndArguments"),
	//("CFStringAppendPascalString"),
	//("CFStringCapitalize"),
	("CFStringCompare"),
	("CFStringCompareWithOptions"),
	//("CFStringCompareWithOptionsAndLocale"),
	//("CFStringConvertEncodingToIANACharSetName"),
	//("CFStringConvertEncodingToNSStringEncoding"),
	//("CFStringConvertEncodingToWindowsCodepage"),
	//("CFStringConvertIANACharSetNameToEncoding"),
	//("CFStringConvertNSStringEncodingToEncoding"),
	//("CFStringConvertWindowsCodepageToEncoding"),
	//("CFStringCreateArrayBySeparatingStrings"),
	//("CFStringCreateArrayWithFindResults"),
	//("CFStringCreateByCombiningStrings"),
	("CFStringCreateCopy"),
	//("CFStringCreateExternalRepresentation"),
	//("CFStringCreateFromExternalRepresentation"),
	//("CFStringCreateMutable"),
	//("CFStringCreateMutableCopy"),
	//("CFStringCreateMutableWithExternalCharactersNoCopy"),
	//("CFStringCreateWithBytes"),
	//("CFStringCreateWithBytesNoCopy"),
	("CFStringCreateWithCString"),
	//("CFStringCreateWithCStringNoCopy"),
	("CFStringCreateWithCharacters"),
	//("CFStringCreateWithCharactersNoCopy"),
	//("CFStringCreateWithFileSystemRepresentation"),
	("CFStringCreateWithFormat"),
	//("CFStringCreateWithFormatAndArguments"),
	//("CFStringCreateWithPascalString"),
	//("CFStringCreateWithPascalStringNoCopy"),
	//("CFStringCreateWithSubstring"),
	//("CFStringDelete"),
	//("CFStringEncodingByteLengthForCharacters"),
	//("CFStringEncodingBytesToUnicode"),
	//("CFStringEncodingCharLengthForBytes"),
	//("CFStringEncodingIsValidEncoding"),
	//("CFStringEncodingListOfAvailableEncodings"),
	//("CFStringEncodingRegisterFallbackProcedures"),
	//("CFStringEncodingUnicodeToBytes"),
	//("CFStringFileSystemEncoding"),
	//("CFStringFind"),
	//("CFStringFindAndReplace"),
	//("CFStringFindCharacterFromSet"),
	//("CFStringFindWithOptions"),
	//("CFStringFindWithOptionsAndLocale"),
	//("CFStringFold"),
	//("CFStringGetBytes"),
	("CFStringGetCString"),
	("CFStringGetCStringPtr"),
	//("CFStringGetCharacterAtIndex"),
	//("CFStringGetCharacters"),
	//("CFStringGetCharactersPtr"),
	//("CFStringGetDoubleValue"),
	//("CFStringGetFastestEncoding"),
	//("CFStringGetFileSystemRepresentation"),
	//("CFStringGetHyphenationLocationBeforeIndex"),
	("CFStringGetIntValue"),
	("CFStringGetLength"),
	//("CFStringGetLineBounds"),
	//("CFStringGetListOfAvailableEncodings"),
	("CFStringGetMaximumSizeForEncoding"),
	//("CFStringGetMaximumSizeOfFileSystemRepresentation"),
	//("CFStringGetMostCompatibleMacStringEncoding"),
	//("CFStringGetNameOfEncoding"),
	//("CFStringGetParagraphBounds"),
	//("CFStringGetPascalString"),
	//("CFStringGetPascalStringPtr"),
	//("CFStringGetRangeOfCharacterClusterAtIndex"),
	//("CFStringGetRangeOfComposedCharactersAtIndex"),
	//("CFStringGetSmallestEncoding"),
	//("CFStringGetSystemEncoding"),
	("CFStringGetTypeID"),
	//("CFStringHasPrefix"),
	//("CFStringHasSuffix"),
	//("CFStringHashCString"),
	//("CFStringHashCharacters"),
	//("CFStringHashISOLatin1CString"),
	//("CFStringHashNSString"),
	//("CFStringInsert"),
	//("CFStringIsEncodingAvailable"),
	//("CFStringIsHyphenationAvailableForLocale"),
	//("CFStringLowercase"),
	//("CFStringNormalize"),
	//("CFStringOpenUText"),
	//("CFStringPad"),
	//("CFStringReplace"),
	//("CFStringReplaceAll"),
	//("CFStringSetExternalCharactersNoCopy"),
	//("CFStringTransform"),
	//("CFStringTrim"),
	//("CFStringTrimWhitespace"),
	//("CFStringUppercase"),
	//("CFTimeZoneCopyAbbreviation"),
	//("CFTimeZoneCopyAbbreviationDictionary"),
	//("CFTimeZoneCopyDefault"),
	//("CFTimeZoneCopyKnownNames"),
	//("CFTimeZoneCopyLocalizedName"),
	//("CFTimeZoneCopySystem"),
	//("CFTimeZoneCreate"),
	//("CFTimeZoneCreateWithName"),
	//("CFTimeZoneCreateWithTimeIntervalFromGMT"),
	//("CFTimeZoneGetData"),
	//("CFTimeZoneGetDaylightSavingTimeOffset"),
	//("CFTimeZoneGetName"),
	//("CFTimeZoneGetNextDaylightSavingTimeTransition"),
	//("CFTimeZoneGetSecondsFromGMT"),
	//("CFTimeZoneGetTypeID"),
	//("CFTimeZoneIsDaylightSavingTime"),
	//("CFTimeZoneResetSystem"),
	//("CFTimeZoneSetAbbreviationDictionary"),
	//("CFTimeZoneSetDefault"),
	//("CFTreeAppendChild"),
	//("CFTreeApplyFunctionToChildren"),
	//("CFTreeCreate"),
	//("CFTreeFindRoot"),
	//("CFTreeGetChildAtIndex"),
	//("CFTreeGetChildCount"),
	//("CFTreeGetChildren"),
	//("CFTreeGetContext"),
	//("CFTreeGetFirstChild"),
	//("CFTreeGetNextSibling"),
	//("CFTreeGetParent"),
	//("CFTreeGetTypeID"),
	//("CFTreeInsertSibling"),
	//("CFTreePrependChild"),
	//("CFTreeRemove"),
	//("CFTreeRemoveAllChildren"),
	//("CFTreeSetContext"),
	//("CFTreeSortChildren"),
	//("CFTypeGetTypeID"),
	//("CFURLCanBeDecomposed"),
	//("CFURLCopyAbsoluteURL"),
	//("CFURLCopyFileSystemPath"),
	//("CFURLCopyFragment"),
	//("CFURLCopyHostName"),
	//("CFURLCopyLastPathComponent"),
	//("CFURLCopyNetLocation"),
	//("CFURLCopyParameterString"),
	//("CFURLCopyPassword"),
	//("CFURLCopyPath"),
	//("CFURLCopyPathExtension"),
	//("CFURLCopyQueryString"),
	//("CFURLCopyResourceSpecifier"),
	//("CFURLCopyScheme"),
	//("CFURLCopyStrictPath"),
	//("CFURLCopyUserName"),
	//("CFURLCreateAbsoluteURLWithBytes"),
	//("CFURLCreateCopyAppendingPathComponent"),
	//("CFURLCreateCopyAppendingPathExtension"),
	//("CFURLCreateCopyDeletingLastPathComponent"),
	//("CFURLCreateCopyDeletingPathExtension"),
	//("CFURLCreateData"),
	//("CFURLCreateDataAndPropertiesFromResource"),
	//("CFURLCreateFromFileSystemRepresentation"),
	//("CFURLCreateFromFileSystemRepresentationRelativeToBase"),
	//("CFURLCreatePropertyFromResource"),
	//("CFURLCreateStringByAddingPercentEscapes"),
	//("CFURLCreateStringByReplacingPercentEscapes"),
	//("CFURLCreateStringByReplacingPercentEscapesUsingEncoding"),
	//("CFURLCreateWithBytes"),
	//("CFURLCreateWithFileSystemPath"),
	//("CFURLCreateWithFileSystemPathRelativeToBase"),
	//("CFURLCreateWithString"),
	//("CFURLDestroyResource"),
	//("CFURLEnumeratorCreateForDirectoryURL"),
	//("CFURLEnumeratorCreateForMountedVolumes"),
	//("CFURLEnumeratorGetDescendentLevel"),
	//("CFURLEnumeratorGetNextURL"),
	//("CFURLEnumeratorGetSourceDidChange"),
	//("CFURLEnumeratorGetTypeID"),
	//("CFURLEnumeratorSkipDescendents"),
	//("CFURLGetBaseURL"),
	//("CFURLGetByteRangeForComponent"),
	//("CFURLGetBytes"),
	//("CFURLGetFileSystemRepresentation"),
	//("CFURLGetPortNumber"),
	//("CFURLGetString"),
	//("CFURLGetTypeID"),
	//("CFURLHasDirectoryPath"),
	//("CFURLWriteDataAndPropertiesToResource"),
	//("CFUUIDCreate"),
	//("CFUUIDCreateFromString"),
	//("CFUUIDCreateFromUUIDBytes"),
	//("CFUUIDCreateString"),
	//("CFUUIDCreateWithBytes"),
	//("CFUUIDGetConstantUUIDWithBytes"),
	//("CFUUIDGetTypeID"),
	//("CFUUIDGetUUIDBytes"),
	//("CFUniCharCompatibilityDecompose"),
	//("CFUniCharDecompose"),
	//("CFUniCharDecomposeCharacter"),
	//("CFUniCharFillDestinationBuffer"),
	//("CFUniCharGetBitmapForPlane"),
	//("CFUniCharGetBitmapPtrForPlane"),
	//("CFUniCharGetConditionalCaseMappingFlags"),
	//("CFUniCharGetMappingData"),
	//("CFUniCharGetNumberOfPlanes"),
	//("CFUniCharGetNumberOfPlanesForUnicodePropertyData"),
	//("CFUniCharGetUnicodeProperty"),
	//("CFUniCharGetUnicodePropertyDataForPlane"),
	//("CFUniCharIsMemberOf"),
	//("CFUniCharMapCaseTo"),
	//("CFUniCharPrecompose"),
	//("CFUniCharPrecomposeCharacter"),
	//("CFUniCharPrioritySort"),
	//("CFWindowsNamedPipeCreateConnectToName"),
	//("CFWindowsNamedPipeCreateRunLoopSource"),
	//("CFWindowsNamedPipeCreateWithName"),
	//("CFWindowsNamedPipeGetContext"),
	//("CFWindowsNamedPipeGetHandle"),
	//("CFWindowsNamedPipeGetName"),
	//("CFWindowsNamedPipeGetTypeID"),
	//("CFWindowsNamedPipeInvalidate"),
	//("CFWindowsNamedPipeIsValid"),
	//("CFWindowsNamedPipeSetInvalidationCallBack"),
	//("CFWriteStreamCanAcceptBytes"),
	("CFWriteStreamClose"),
	//("CFWriteStreamCopyError"),
	("CFWriteStreamCopyProperty"),
	//("CFWriteStreamCreate"),
	("CFWriteStreamCreateWithAllocatedBuffers"),
	//("CFWriteStreamCreateWithBuffer"),
	//("CFWriteStreamCreateWithFile"),
	//("CFWriteStreamGetError"),
	//("CFWriteStreamGetInfoPointer"),
	//("CFWriteStreamGetStatus"),
	//("CFWriteStreamGetTypeID"),
	("CFWriteStreamOpen"),
	//("CFWriteStreamScheduleWithRunLoop"),
	//("CFWriteStreamSetClient"),
	//("CFWriteStreamSetProperty"),
	//("CFWriteStreamSignalEvent"),
	//("CFWriteStreamUnscheduleFromRunLoop"),
	//("CFWriteStreamWrite"),
	//("CFXMLCreateStringByEscapingEntities"),
	//("CFXMLCreateStringByUnescapingEntities"),
	//("CFXMLNodeCreate"),
	//("CFXMLNodeCreateCopy"),
	//("CFXMLNodeGetInfoPtr"),
	//("CFXMLNodeGetString"),
	//("CFXMLNodeGetTypeCode"),
	//("CFXMLNodeGetTypeID"),
	//("CFXMLNodeGetVersion"),
	//("CFXMLParserAbort"),
	//("CFXMLParserCopyErrorDescription"),
	//("CFXMLParserCreate"),
	//("CFXMLParserCreateWithDataFromURL"),
	//("CFXMLParserGetCallBacks"),
	//("CFXMLParserGetContext"),
	//("CFXMLParserGetDocument"),
	//("CFXMLParserGetLineNumber"),
	//("CFXMLParserGetLocation"),
	//("CFXMLParserGetSourceURL"),
	//("CFXMLParserGetStatusCode"),
	//("CFXMLParserGetTypeID"),
	//("CFXMLParserParse"),
	//("CFXMLTreeCreateFromData"),
	//("CFXMLTreeCreateFromDataWithError"),
	//("CFXMLTreeCreateWithDataFromURL"),
	//("CFXMLTreeCreateWithNode"),
	//("CFXMLTreeCreateXMLData"),
	//("CFXMLTreeGetNode"),
	//("NSBuddhistCalendar"),
	//("NSCalendarIdentifierBuddhist"),
	//("NSCalendarIdentifierChinese"),
	//("NSCalendarIdentifierCoptic"),
	//("NSCalendarIdentifierEthiopicAmeteAlem"),
	//("NSCalendarIdentifierEthiopicAmeteMihret"),
	//("NSCalendarIdentifierGregorian"),
	//("NSCalendarIdentifierHebrew"),
	//("NSCalendarIdentifierISO8601"),
	//("NSCalendarIdentifierIndian"),
	//("NSCalendarIdentifierIslamic"),
	//("NSCalendarIdentifierIslamicCivil"),
	//("NSCalendarIdentifierJapanese"),
	//("NSCalendarIdentifierPersian"),
	//("NSCalendarIdentifierRepublicOfChina"),
	//("NSChineseCalendar"),
	//("NSCurrentLocaleDidChangeNotification"),
	//("NSDefaultRunLoopMode"),
	//("NSGenericException"),
	//("NSGetSizeAndAlignment"),
	//("NSGregorianCalendar"),
	//("NSHebrewCalendar"),
	//("NSISO8601Calendar"),
	//("NSIndianCalendar"),
	//("NSInternalInconsistencyException"),
	//("NSInvalidArgumentException"),
	//("NSIslamicCalendar"),
	//("NSIslamicCivilCalendar"),
	//("NSJapaneseCalendar"),
	//("NSLocaleAlternateQuotationBeginDelimiterKey"),
	//("NSLocaleAlternateQuotationEndDelimiterKey"),
	//("NSLocaleCalendar"),
	//("NSLocaleCalendarIdentifier"),
	//("NSLocaleCalendarIdentifierKey"),
	//("NSLocaleCalendarKey"),
	//("NSLocaleCollationIdentifier"),
	//("NSLocaleCollationIdentifierKey"),
	//("NSLocaleCollatorIdentifier"),
	//("NSLocaleCollatorIdentifierKey"),
	//("NSLocaleCountryCode"),
	//("NSLocaleCountryCodeKey"),
	//("NSLocaleCurrencyCode"),
	//("NSLocaleCurrencyCodeKey"),
	//("NSLocaleCurrencySymbol"),
	//("NSLocaleCurrencySymbolKey"),
	//("NSLocaleDecimalSeparator"),
	//("NSLocaleDecimalSeparatorKey"),
	//("NSLocaleExemplarCharacterSet"),
	//("NSLocaleExemplarCharacterSetKey"),
	//("NSLocaleGroupingSeparator"),
	//("NSLocaleGroupingSeparatorKey"),
	//("NSLocaleIdentifier"),
	//("NSLocaleIdentifierKey"),
	//("NSLocaleLanguageCode"),
	//("NSLocaleLanguageCodeKey"),
	//("NSLocaleMeasurementSystem"),
	//("NSLocaleMeasurementSystemKey"),
	//("NSLocaleQuotationBeginDelimiterKey"),
	//("NSLocaleQuotationEndDelimiterKey"),
	//("NSLocaleScriptCode"),
	//("NSLocaleScriptCodeKey"),
	//("NSLocaleUsesMetricSystem"),
	//("NSLocaleUsesMetricSystemKey"),
	//("NSLocaleVariantCode"),
	//("NSLocaleVariantCodeKey"),
	//("NSMallocException"),
	//("NSPersianCalendar"),
	//("NSRangeException"),
	//("NSRepublicOfChinaCalendar"),
	//("NSRunLoopCommonModes"),
	//("NSStreamDataWrittenToMemoryStreamKey"),
	//("NSStreamFileCurrentOffsetKey"),
	//("NSSystemClockDidChangeNotification"),
	//("NSSystemTimeZoneDidChangeNotification"),
	//("OSAtomicAdd32"),
	//("OSAtomicAdd32Barrier"),
	//("OSAtomicCompareAndSwap32Barrier"),
	//("OSAtomicCompareAndSwapLong"),
	//("OSAtomicCompareAndSwapPtr"),
	//("OSAtomicCompareAndSwapPtrBarrier"),
	//("OSAtomicDecrement32"),
	//("OSAtomicDecrement32Barrier"),
	//("OSAtomicIncrement32"),
	//("OSAtomicIncrement32Barrier"),
	//("_Block_copy"),
	//("_Block_object_assign"),
	//("_Block_object_dispose"),
	//("_Block_release"),
	//("_CFAllocatorAllocateGC"),
	//("_CFAllocatorDeallocateGC"),
	//("_CFAllocatorReallocateGC"),
	//("_CFAppVersionCheck"),
	//("_CFAppVersionCheckLessThan"),
	//("_CFAppWindowsVersionCheck"),
	//("_CFAppWindowsVersionCheckLessThan"),
	//("_CFAppendPathComponent"),
	//("_CFAppendPathExtension"),
	//("_CFArrayCheckAndGetValueAtIndex"),
	//("_CFArrayFastEnumeration"),
	//("_CFArrayIsMutable"),
	//("_CFArrayReplaceValues"),
	//("_CFArraySetCapacity"),
	//("_CFAttributedStringCheckAndReplace"),
	//("_CFAttributedStringCheckAndReplaceAttributed"),
	//("_CFAttributedStringCheckAndSetAttribute"),
	//("_CFAttributedStringCheckAndSetAttributes"),
	//("_CFAttributedStringGetLength"),
	//("_CFAutoreleasePoolAddObject"),
	//("_CFAutoreleasePoolPop"),
	//("_CFAutoreleasePoolPrintPools"),
	//("_CFAutoreleasePoolPush"),
	//("_CFBagFastEnumeration"),
	//("_CFBagGetKVOBit"),
	//("_CFBagIsMutable"),
	//("_CFBagSetCapacity"),
	//("_CFBagSetKVOBit"),
	//("_CFBundleCopyAllBundles"),
	//("_CFBundleCopyBuiltInPlugInsURL"),
	//("_CFBundleCopyBundleRegionsArray"),
	//("_CFBundleCopyBundleURLForExecutableURL"),
	//("_CFBundleCopyExecutableURLInDirectory"),
	//("_CFBundleCopyFileTypeForFileData"),
	//("_CFBundleCopyFileTypeForFileURL"),
	//("_CFBundleCopyFrameworkURLForExecutablePath"),
	//("_CFBundleCopyInfoPlistURL"),
	//("_CFBundleCopyLanguageSearchListInDirectory"),
	//("_CFBundleCopyMainBundleExecutableURL"),
	//("_CFBundleCopyOtherExecutableURLInDirectory"),
	//("_CFBundleCopyPrivateFrameworksURL"),
	//("_CFBundleCopyResourceForkURL"),
	//("_CFBundleCopyResourceURLForLanguage"),
	//("_CFBundleCopyResourceURLsOfTypeForLanguage"),
	//("_CFBundleCopySharedFrameworksURL"),
	//("_CFBundleCopySharedSupportURL"),
	//("_CFBundleCreateError"),
	//("_CFBundleCreateIfLooksLikeBundle"),
	//("_CFBundleCreateIfMightBeBundle"),
	//("_CFBundleCreateWithExecutableURLIfLooksLikeBundle"),
	//("_CFBundleCreateWithExecutableURLIfMightBeBundle"),
	//("_CFBundleFlushBundleCaches"),
	//("_CFBundleFlushCaches"),
	//("_CFBundleFlushCachesForURL"),
	//("_CFBundleGetCFMFunctionPointerForName"),
	//("_CFBundleGetCFMFunctionPointersForNames"),
	//("_CFBundleGetCurrentPlatform"),
	//("_CFBundleGetExistingBundleWithBundleURL"),
	//("_CFBundleGetHasChanged"),
	//("_CFBundleGetLanguageAndRegionCodes"),
	//("_CFBundleGetLanguageSearchList"),
	//("_CFBundleGetLocalInfoDictionary"),
	//("_CFBundleGetMainBundleIfLooksLikeBundle"),
	//("_CFBundleGetPackageInfoInDirectory"),
	//("_CFBundleGetStringsFilesShared"),
	//("_CFBundleGetSupportedPlatforms"),
	//("_CFBundleGetValueForInfoKey"),
	//("_CFBundleLayoutVersion"),
	//("_CFBundleLoadExecutableAndReturnError"),
	//("_CFBundleMainBundleInfoDictionaryComesFromResourceFork"),
	//("_CFBundleSetCFMConnectionID"),
	//("_CFBundleSetDefaultLocalization"),
	//("_CFBundleSetStringsFilesShared"),
	//("_CFBundleURLLooksLikeBundle"),
	//("_CFCalendarAddComponentsV"),
	//("_CFCalendarComposeAbsoluteTimeV"),
	//("_CFCalendarDecomposeAbsoluteTimeV"),
	//("_CFCalendarGetComponentDifferenceV"),
	//("_CFCharacterSetCreateKeyedCodingString"),
	//("_CFCharacterSetGetKeyedCodingBuiltinType"),
	//("_CFCharacterSetGetKeyedCodingRange"),
	//("_CFCharacterSetGetKeyedCodingType"),
	//("_CFCharacterSetIsInverted"),
	//("_CFCharacterSetIsMutable"),
	//("_CFCharacterSetSetIsInverted"),
	//("_CFContentsOfDirectory"),
	//("_CFCopyExtensionForAbstractType"),
	//("_CFCopyServerVersionDictionary"),
	//("_CFCopySystemVersionDictionary"),
	//("_CFCreateApplicationRepositoryPath"),
	//("_CFCreateDirectory"),
	//("_CFCreateDirectoryWide"),
	//("_CFDNStart"),
	//("_CFDataFindBytes"),
	//("_CFDeleteFile"),
	//("_CFDictionaryFastEnumeration"),
	//("_CFDictionaryGetKVOBit"),
	//("_CFDictionaryIsMutable"),
	//("_CFDictionarySetCapacity"),
	//("_CFDictionarySetKVOBit"),
	//("_CFDoExceptionOperation"),
	//("_CFErrorCreateDebugDescription"),
	//("_CFErrorCreateLocalizedDescription"),
	//("_CFErrorCreateLocalizedFailureReason"),
	//("_CFErrorCreateLocalizedRecoverySuggestion"),
	//("_CFExecutableLinkedOnOrAfter"),
	//("_CFFindBundleResources"),
	//("_CFGetCurrentDirectory"),
	//("_CFGetFileProperties"),
	//("_CFGetOutOfMemoryErrorCallBack"),
	//("_CFGetPathProperties"),
	//("_CFGetProcessPath"),
	//("_CFGetProgname"),
	//("_CFGetTSD"),
	//("_CFGetWindowsAppleAppDataDirectory"),
	//("_CFGetWindowsAppleSystemLibraryDirectory"),
	//("_CFGetWindowsBinaryDirectories"),
	//("_CFHyphenationGetAllPossibleHyphenationLocations"),
	//("_CFHyphenationGetLinguisticDataPath"),
	//("_CFHyphenationSetLinguisticDataPath"),
	//("_CFIsAbsolutePath"),
	//("_CFIsDeallocating"),
	//("_CFKeyedArchiverUIDCreate"),
	//("_CFKeyedArchiverUIDGetTypeID"),
	//("_CFKeyedArchiverUIDGetValue"),
	//("_CFLengthAfterDeletingLastPathComponent"),
	//("_CFLengthAfterDeletingPathExtension"),
	//("_CFLocaleGetNoteCount"),
	//("_CFLogvEx"),
	//("_CFMainPThread"),
	//("_CFNotificationCenterSetRunLoop"),
	//("_CFNumberGetType2"),
	//("_CFPreferencesAlwaysUseVolatileUserDomains"),
	//("_CFPreferencesCreatePathToUsersManagedPrefsDirectory"),
	//("_CFPreferencesIsManaged"),
	//("_CFPreferencesManagementStatusChangedForDomains"),
	//("_CFPreferencesPostValuesChangedInDomains"),
	//("_CFPreferencesWriteDomainDictionaryToPath"),
	//("_CFPreferencesWriteManagedDomainForUser"),
	//("_CFProcessNameString"),
	//("_CFProcessPath"),
	//("_CFPropertyListCreateFromXMLData"),
	//("_CFPropertyListCreateFromXMLString"),
	//("_CFPropertyListCreateSingleValue"),
	//("_CFPropertyListCreateXMLDataWithExtras"),
	//("_CFReadBytesFromFile"),
	//("_CFReadStreamClearEvent"),
	//("_CFReadStreamCopyRunLoopsAndModes"),
	//("_CFReadStreamCreateFromFileDescriptor"),
	//("_CFReadStreamGetClient"),
	//("_CFReadStreamGetRunLoopsAndModes"),
	//("_CFReadStreamSignalEventDelayed"),
	//("_CFRemoveDirectory"),
	//("_CFRunLoop01"),
	//("_CFRunLoopFinished"),
	//("_CFRunLoopGet0"),
	//("_CFRunLoopGet0b"),
	//("_CFRunLoopGet2"),
	//("_CFRunLoopGet2b"),
	//("_CFRunLoopGetWindowsMessageQueueHandler"),
	//("_CFRunLoopGetWindowsMessageQueueMask"),
	//("_CFRunLoopGetWindowsThreadID"),
	//("_CFRunLoopModeContainsMode"),
	//("_CFRunLoopSetWindowsMessageQueueHandler"),
	//("_CFRunLoopSetWindowsMessageQueueMask"),
	//("_CFRunLoopStopMode"),
	//("_CFRuntimeCreateInstance"),
	//("_CFRuntimeGetClassWithTypeID"),
	//("_CFRuntimeInitStaticInstance"),
	//("_CFRuntimeRegisterClass"),
	//("_CFRuntimeSetCFMPresent"),
	//("_CFRuntimeSetInstanceTypeID"),
	//("_CFRuntimeUnregisterClassWithTypeID"),
	//("_CFSetFastEnumeration"),
	//("_CFSetGetKVOBit"),
	//("_CFSetIsMutable"),
	//("_CFSetOutOfMemoryErrorCallBack"),
	//("_CFSetSetCapacity"),
	//("_CFSetSetKVOBit"),
	//("_CFSetTSD"),
	//("_CFSocketStreamSetAuthenticatesServerCertificateDefault"),
	//("_CFStartOfLastPathComponent"),
	//("_CFStartOfPathExtension"),
	//("_CFStreamCreateWithConstantCallbacks"),
	//("_CFStreamInstanceSize"),
	//("_CFStreamSetInfoPointer"),
	//("_CFStreamSourceScheduleWithAllRunLoops"),
	//("_CFStreamSourceScheduleWithRunLoop"),
	//("_CFStreamSourceUncheduleFromAllRunLoops"),
	//("_CFStreamSourceUnscheduleFromRunLoop"),
	//("_CFStringAppendFormatAndArgumentsAux"),
	//("_CFStringCheckAndGetCharacterAtIndex"),
	//("_CFStringCheckAndGetCharacters"),
	//("_CFStringCreateHostName"),
	//("_CFStringCreateWithBytesNoCopy"),
	//("_CFStringCreateWithFormatAndArgumentsAux"),
	//("_CFStringEncodingSetForceASCIICompatibility"),
	//("_CFStringGetFileSystemRepresentation"),
	//("_CFStringGetLength2"),
	//("_CFStringHyphenationCompileHyphenatorData"),
	//("_CFStringIsLegalURLString"),
	//("_CFStripTrailingPathSlashes"),
	//("_CFTimeZoneGetNoteCount"),
	//("_CFTransmutePathSlashes"),
	//("_CFTryRetain"),
	//("_CFURLAlloc"),
	//("_CFURLCopyComponents"),
	//("_CFURLCopyPropertyListRepresentation"),
	//("_CFURLCreateCurrentDirectoryURL"),
	//("_CFURLCreateFromComponents"),
	//("_CFURLCreateFromPropertyListRepresentation"),
	//("_CFURLCreateOnlyUTF8CompatibleURLs"),
	//("_CFURLGetEncoding"),
	//("_CFURLGetWideFileSystemRepresentation"),
	//("_CFURLInitFSPath"),
	//("_CFURLInitWithString"),
	//("_CFWriteBytesToFile"),
	//("_CFWriteStreamCopyRunLoopsAndModes"),
	//("_CFWriteStreamCreateFromFileDescriptor"),
	//("_CFWriteStreamGetClient"),
	//("_CFWriteStreamGetRunLoopsAndModes"),
	//("_CFWriteStreamSignalEventDelayed"),
	//("_CFXNotificationCenterCreate"),
	//("_CFXNotificationCenterIsEmpty"),
	//("_CFXNotificationGetHostCenter"),
	//("_CFXNotificationGetSuspended"),
	//("_CFXNotificationGetTaskCenter"),
	//("_CFXNotificationPost"),
	//("_CFXNotificationRegisterObserver"),
	//("_CFXNotificationRemoveObservers"),
	//("_CFXNotificationSetSuspended"),
	//("_CFXPreferencesCopyDictionaryForApplicationPreferences"),
	//("_CFXPreferencesCopyDictionaryForNamedVolatileSource"),
	//("_CFXPreferencesCopyDictionaryForSourceWithBundleID"),
	//("_CFXPreferencesCopyLanguageList"),
	//("_CFXPreferencesCopyVolatileSourceNames"),
	//("_CFXPreferencesGetByHostIdentifierString"),
	//("_CFXPreferencesRegisterDefaultValues"),
	//("_CFXPreferencesRemoveNamedVolatileSource"),
	//("_CFXPreferencesReplaceValuesInNamedVolatileSource"),
	//("_CFXPreferencesReplaceValuesInSourceWithBundleID"),
	//("_CFXPreferencesSetUserDefaultsPointer"),
	//("_CF_forwarding_prep_0"),
	//("_NSConcreteGlobalBlock"),
	//("_NSConcreteStackBlock"),
	//("_NSMessageBuilder"),
	//("_NS_access"),
	//("_NS_chdir"),
	//("_NS_chmod"),
	//("_NS_getcwd"),
	//("_NS_getenv"),
	//("_NS_mkdir"),
	//("_NS_mkstemp"),
	//("_NS_open"),
	//("_NS_pthread_main_np"),
	//("_NS_pthread_setname_np"),
	//("_NS_rename"),
	//("_NS_rmdir"),
	//("_NS_stat"),
	//("_NS_unlink"),
	//("__CFArgStuff"),
	//("__CFAttributedStringMtbl"),
	//("__CFBinaryPlistCreateObject"),
	//("__CFBinaryPlistCreateObject2"),
	//("__CFBinaryPlistGetOffsetForValueFromArray2"),
	//("__CFBinaryPlistGetOffsetForValueFromDictionary2"),
	//("__CFBinaryPlistGetOffsetForValueFromDictionary3"),
	//("__CFBinaryPlistGetTopLevelInfo"),
	//("__CFBinaryPlistWrite"),
	//("__CFBinaryPlistWriteToStream"),
	//("__CFBinaryPlistWriteToStreamWithEstimate"),
	//("__CFBinaryPlistWriteToStreamWithOptions"),
	//("__CFCharToUniCharFunc"),
	//("__CFConstantStringClassReference"),
	//("__CFConstantStringClassReferencePtr"),
	//("__CFDataMtbl"),
	//("__CFDefaultEightBitStringEncoding"),
	//("__CFDoExternRefOperation"),
	//("__CFGetNextSearchPathEnumeration"),
	//("__CFInitialize"),
	//("__CFOASafe"),
	//("__CFPreferencesSetPathHackEnabled"),
	("__CFRangeMake"),
	//("__CFSetLastAllocationEventName"),
	//("__CFSocketInitializeWinSock"),
	//("__CFStartSearchPathEnumeration"),
	//("__CFStorageGetCapacity"),
	//("__CFStorageGetValueSize"),
	//("__CFStorageSetAlwaysFrozen"),
	//("__CFStringAppendBytes"),
	//("__CFStringCheckAndReplace"),
	//("__CFStringComputeEightBitStringEncoding"),
	//("__CFStringCreateImmutableFunnel2"),
	//("__CFStringDecodeByteStream2"),
	//("__CFStringDecodeByteStream3"),
	//("__CFStringEncodeByteStream"),
	//("__CFStringHash"),
	//("__CFStringInitializeSystemEncoding"),
	//("__CFStringIsEightBit"),
	("__CFStringMakeConstantString"),
	//("__CFStringMtbl"),
	//("__CFStringNoteErrors"),
	//("__CFURLReservedPtr"),
	//("__CFURLResourceInfoPtr"),
	//("__CFURLSetReservedPtr"),
	//("__CFURLSetResourceInfoPtr"),
	//("_kCFBundleAllowMixedLocalizationsKey"),
	//("_kCFBundleDisplayNameKey"),
	//("_kCFBundleDocumentTypesKey"),
	//("_kCFBundleExecutablePathKey"),
	//("_kCFBundleGetInfoHTMLKey"),
	//("_kCFBundleGetInfoStringKey"),
	//("_kCFBundleIconFileKey"),
	//("_kCFBundleInfoPlistURLKey"),
	//("_kCFBundleInitialPathKey"),
	//("_kCFBundleNumericVersionKey"),
	//("_kCFBundleOldDocumentTypesKey"),
	//("_kCFBundleOldExecutableKey"),
	//("_kCFBundleOldIconFileKey"),
	//("_kCFBundleOldInfoDictionaryVersionKey"),
	//("_kCFBundleOldNameKey"),
	//("_kCFBundleOldShortVersionStringKey"),
	//("_kCFBundleOldTypeExtensions1Key"),
	//("_kCFBundleOldTypeExtensions2Key"),
	//("_kCFBundleOldTypeIconFileKey"),
	//("_kCFBundleOldTypeNameKey"),
	//("_kCFBundleOldTypeOSTypesKey"),
	//("_kCFBundleOldTypeRoleKey"),
	//("_kCFBundlePackageTypeKey"),
	//("_kCFBundlePrincipalClassKey"),
	//("_kCFBundleRawInfoPlistURLKey"),
	//("_kCFBundleResolvedPathKey"),
	//("_kCFBundleResourceSpecificationKey"),
	//("_kCFBundleResourcesFileMappedKey"),
	//("_kCFBundleShortVersionStringKey"),
	//("_kCFBundleSignatureKey"),
	//("_kCFBundleSupportedPlatformsKey"),
	//("_kCFBundleTypeExtensionsKey"),
	//("_kCFBundleTypeIconFileKey"),
	//("_kCFBundleTypeMIMETypesKey"),
	//("_kCFBundleTypeNameKey"),
	//("_kCFBundleTypeOSTypesKey"),
	//("_kCFBundleTypeRoleKey"),
	//("_kCFBundleURLIconFileKey"),
	//("_kCFBundleURLNameKey"),
	//("_kCFBundleURLSchemesKey"),
	//("_kCFBundleURLTypesKey"),
	//("_kCFSystemVersionBuildStringKey"),
	//("_kCFSystemVersionBuildVersionKey"),
	//("_kCFSystemVersionProductCopyrightKey"),
	//("_kCFSystemVersionProductNameKey"),
	//("_kCFSystemVersionProductUserVisibleVersionKey"),
	//("_kCFSystemVersionProductVersionExtraKey"),
	//("_kCFSystemVersionProductVersionKey"),
	//("_kCFSystemVersionProductVersionStringKey"),
	//("kCFAbsoluteTimeIntervalSince1904"),
	//("kCFAbsoluteTimeIntervalSince1970"),
	//("kCFAllocatorDefault"),
	//("kCFAllocatorMalloc"),
	//("kCFAllocatorMallocZone"),
	//("kCFAllocatorNull"),
	//("kCFAllocatorSystemDefault"),
	//("kCFAllocatorUseContext"),
	("kCFBooleanFalse"),
	("kCFBooleanTrue"),
	//("kCFBuddhistCalendar"),
	//("kCFBundleDevelopmentRegionKey"),
	//("kCFBundleExecutableKey"),
	//("kCFBundleIdentifierKey"),
	//("kCFBundleInfoDictionaryVersionKey"),
	//("kCFBundleLocalizationsKey"),
	//("kCFBundleNameKey"),
	//("kCFBundleVersionKey"),
	//("kCFCalendarIdentifierBuddhist"),
	//("kCFCalendarIdentifierChinese"),
	//("kCFCalendarIdentifierCoptic"),
	//("kCFCalendarIdentifierEthiopicAmeteAlem"),
	//("kCFCalendarIdentifierEthiopicAmeteMihret"),
	//("kCFCalendarIdentifierGregorian"),
	//("kCFCalendarIdentifierHebrew"),
	//("kCFCalendarIdentifierISO8601"),
	//("kCFCalendarIdentifierIndian"),
	//("kCFCalendarIdentifierIslamic"),
	//("kCFCalendarIdentifierIslamicCivil"),
	//("kCFCalendarIdentifierJapanese"),
	//("kCFCalendarIdentifierPersian"),
	//("kCFCalendarIdentifierRepublicOfChina"),
	//("kCFChineseCalendar"),
	//("kCFCopyStringBagCallBacks"),
	//("kCFCopyStringDictionaryKeyCallBacks"),
	//("kCFCopyStringSetCallBacks"),
	//("kCFDateFormatterAMSymbol"),
	//("kCFDateFormatterAMSymbolKey"),
	//("kCFDateFormatterCalendar"),
	//("kCFDateFormatterCalendarIdentifier"),
	//("kCFDateFormatterCalendarIdentifierKey"),
	//("kCFDateFormatterCalendarKey"),
	//("kCFDateFormatterCalendarName"),
	//("kCFDateFormatterDefaultDate"),
	//("kCFDateFormatterDefaultDateKey"),
	//("kCFDateFormatterDefaultFormat"),
	//("kCFDateFormatterDefaultFormatKey"),
	//("kCFDateFormatterDoesRelativeDateFormattingKey"),
	//("kCFDateFormatterEraSymbols"),
	//("kCFDateFormatterEraSymbolsKey"),
	//("kCFDateFormatterGregorianStartDate"),
	//("kCFDateFormatterGregorianStartDateKey"),
	//("kCFDateFormatterIsLenient"),
	//("kCFDateFormatterIsLenientKey"),
	//("kCFDateFormatterLongEraSymbols"),
	//("kCFDateFormatterLongEraSymbolsKey"),
	//("kCFDateFormatterMonthSymbols"),
	//("kCFDateFormatterMonthSymbolsKey"),
	//("kCFDateFormatterPMSymbol"),
	//("kCFDateFormatterPMSymbolKey"),
	//("kCFDateFormatterQuarterSymbols"),
	//("kCFDateFormatterQuarterSymbolsKey"),
	//("kCFDateFormatterShortMonthSymbols"),
	//("kCFDateFormatterShortMonthSymbolsKey"),
	//("kCFDateFormatterShortQuarterSymbols"),
	//("kCFDateFormatterShortQuarterSymbolsKey"),
	//("kCFDateFormatterShortStandaloneMonthSymbols"),
	//("kCFDateFormatterShortStandaloneMonthSymbolsKey"),
	//("kCFDateFormatterShortStandaloneQuarterSymbols"),
	//("kCFDateFormatterShortStandaloneQuarterSymbolsKey"),
	//("kCFDateFormatterShortStandaloneWeekdaySymbols"),
	//("kCFDateFormatterShortStandaloneWeekdaySymbolsKey"),
	//("kCFDateFormatterShortWeekdaySymbols"),
	//("kCFDateFormatterShortWeekdaySymbolsKey"),
	//("kCFDateFormatterStandaloneMonthSymbols"),
	//("kCFDateFormatterStandaloneMonthSymbolsKey"),
	//("kCFDateFormatterStandaloneQuarterSymbols"),
	//("kCFDateFormatterStandaloneQuarterSymbolsKey"),
	//("kCFDateFormatterStandaloneWeekdaySymbols"),
	//("kCFDateFormatterStandaloneWeekdaySymbolsKey"),
	//("kCFDateFormatterTimeZone"),
	//("kCFDateFormatterTimeZoneKey"),
	//("kCFDateFormatterTwoDigitStartDate"),
	//("kCFDateFormatterTwoDigitStartDateKey"),
	//("kCFDateFormatterVeryShortMonthSymbols"),
	//("kCFDateFormatterVeryShortMonthSymbolsKey"),
	//("kCFDateFormatterVeryShortStandaloneMonthSymbols"),
	//("kCFDateFormatterVeryShortStandaloneMonthSymbolsKey"),
	//("kCFDateFormatterVeryShortStandaloneWeekdaySymbols"),
	//("kCFDateFormatterVeryShortStandaloneWeekdaySymbolsKey"),
	//("kCFDateFormatterVeryShortWeekdaySymbols"),
	//("kCFDateFormatterVeryShortWeekdaySymbolsKey"),
	//("kCFDateFormatterWeekdaySymbols"),
	//("kCFDateFormatterWeekdaySymbolsKey"),
	//("kCFErrorDebugDescriptionKey"),
	//("kCFErrorDescriptionKey"),
	//("kCFErrorDomainCocoa"),
	//("kCFErrorDomainCoreFoundation"),
	//("kCFErrorDomainMach"),
	//("kCFErrorDomainOSStatus"),
	//("kCFErrorDomainPOSIX"),
	//("kCFErrorFilePathKey"),
	//("kCFErrorLocalizedDescriptionKey"),
	//("kCFErrorLocalizedFailureReasonKey"),
	//("kCFErrorLocalizedRecoverySuggestionKey"),
	//("kCFErrorURLKey"),
	//("kCFErrorUnderlyingErrorKey"),
	//("kCFFileURLDirectoryContents"),
	//("kCFFileURLExists"),
	//("kCFFileURLLastModificationTime"),
	//("kCFFileURLPOSIXMode"),
	//("kCFFileURLSize"),
	//("kCFGregorianCalendar"),
	//("kCFHTTPURLStatusCode"),
	//("kCFHTTPURLStatusLine"),
	//("kCFHebrewCalendar"),
	//("kCFISO8601Calendar"),
	//("kCFIndianCalendar"),
	//("kCFIslamicCalendar"),
	//("kCFIslamicCivilCalendar"),
	//("kCFJapaneseCalendar"),
	//("kCFLocaleAlternateQuotationBeginDelimiterKey"),
	//("kCFLocaleAlternateQuotationEndDelimiterKey"),
	//("kCFLocaleCalendar"),
	//("kCFLocaleCalendarIdentifier"),
	//("kCFLocaleCalendarIdentifierKey"),
	//("kCFLocaleCalendarKey"),
	//("kCFLocaleCollationIdentifier"),
	//("kCFLocaleCollationIdentifierKey"),
	//("kCFLocaleCollatorIdentifier"),
	//("kCFLocaleCollatorIdentifierKey"),
	//("kCFLocaleCountryCode"),
	//("kCFLocaleCountryCodeKey"),
	//("kCFLocaleCurrencyCode"),
	//("kCFLocaleCurrencyCodeKey"),
	//("kCFLocaleCurrencySymbol"),
	//("kCFLocaleCurrencySymbolKey"),
	//("kCFLocaleCurrentLocaleDidChangeNotification"),
	//("kCFLocaleDecimalSeparator"),
	//("kCFLocaleDecimalSeparatorKey"),
	//("kCFLocaleExemplarCharacterSet"),
	//("kCFLocaleExemplarCharacterSetKey"),
	//("kCFLocaleGroupingSeparator"),
	//("kCFLocaleGroupingSeparatorKey"),
	//("kCFLocaleIdentifier"),
	//("kCFLocaleIdentifierKey"),
	//("kCFLocaleLanguageCode"),
	//("kCFLocaleLanguageCodeKey"),
	//("kCFLocaleMeasurementSystem"),
	//("kCFLocaleMeasurementSystemKey"),
	//("kCFLocaleQuotationBeginDelimiterKey"),
	//("kCFLocaleQuotationEndDelimiterKey"),
	//("kCFLocaleScriptCode"),
	//("kCFLocaleScriptCodeKey"),
	//("kCFLocaleUsesMetricSystem"),
	//("kCFLocaleUsesMetricSystemKey"),
	//("kCFLocaleVariantCode"),
	//("kCFLocaleVariantCodeKey"),
	//("kCFManagedPreferencesMCXNotificationName"),
	//("kCFManagedPreferencesMCXObjectName"),
	//("kCFNotificationAnyName"),
	//("kCFNotificationAnyObject"),
	//("kCFNotificationAnyObserver"),
	//("kCFNull"),
	//("kCFNumberFormatterAlwaysShowDecimalSeparator"),
	//("kCFNumberFormatterAlwaysShowDecimalSeparatorKey"),
	//("kCFNumberFormatterCurrencyCode"),
	//("kCFNumberFormatterCurrencyCodeKey"),
	//("kCFNumberFormatterCurrencyDecimalSeparator"),
	//("kCFNumberFormatterCurrencyDecimalSeparatorKey"),
	//("kCFNumberFormatterCurrencyGroupingSeparator"),
	//("kCFNumberFormatterCurrencyGroupingSeparatorKey"),
	//("kCFNumberFormatterCurrencySymbol"),
	//("kCFNumberFormatterCurrencySymbolKey"),
	//("kCFNumberFormatterDecimalSeparator"),
	//("kCFNumberFormatterDecimalSeparatorKey"),
	//("kCFNumberFormatterDefaultFormat"),
	//("kCFNumberFormatterDefaultFormatKey"),
	//("kCFNumberFormatterExponentSymbol"),
	//("kCFNumberFormatterExponentSymbolKey"),
	//("kCFNumberFormatterFormatWidth"),
	//("kCFNumberFormatterFormatWidthKey"),
	//("kCFNumberFormatterGroupingSeparator"),
	//("kCFNumberFormatterGroupingSeparatorKey"),
	//("kCFNumberFormatterGroupingSize"),
	//("kCFNumberFormatterGroupingSizeKey"),
	//("kCFNumberFormatterInfinitySymbol"),
	//("kCFNumberFormatterInfinitySymbolKey"),
	//("kCFNumberFormatterInternationalCurrencySymbol"),
	//("kCFNumberFormatterInternationalCurrencySymbolKey"),
	//("kCFNumberFormatterIsLenient"),
	//("kCFNumberFormatterIsLenientKey"),
	//("kCFNumberFormatterMaxFractionDigits"),
	//("kCFNumberFormatterMaxFractionDigitsKey"),
	//("kCFNumberFormatterMaxIntegerDigits"),
	//("kCFNumberFormatterMaxIntegerDigitsKey"),
	//("kCFNumberFormatterMaxSignificantDigits"),
	//("kCFNumberFormatterMaxSignificantDigitsKey"),
	//("kCFNumberFormatterMinFractionDigits"),
	//("kCFNumberFormatterMinFractionDigitsKey"),
	//("kCFNumberFormatterMinIntegerDigits"),
	//("kCFNumberFormatterMinIntegerDigitsKey"),
	//("kCFNumberFormatterMinSignificantDigits"),
	//("kCFNumberFormatterMinSignificantDigitsKey"),
	//("kCFNumberFormatterMinusSign"),
	//("kCFNumberFormatterMinusSignKey"),
	//("kCFNumberFormatterMultiplier"),
	//("kCFNumberFormatterMultiplierKey"),
	//("kCFNumberFormatterNaNSymbol"),
	//("kCFNumberFormatterNaNSymbolKey"),
	//("kCFNumberFormatterNegativePrefix"),
	//("kCFNumberFormatterNegativePrefixKey"),
	//("kCFNumberFormatterNegativeSuffix"),
	//("kCFNumberFormatterNegativeSuffixKey"),
	//("kCFNumberFormatterPaddingCharacter"),
	//("kCFNumberFormatterPaddingCharacterKey"),
	//("kCFNumberFormatterPaddingPosition"),
	//("kCFNumberFormatterPaddingPositionKey"),
	//("kCFNumberFormatterPerMillSymbol"),
	//("kCFNumberFormatterPerMillSymbolKey"),
	//("kCFNumberFormatterPercentSymbol"),
	//("kCFNumberFormatterPercentSymbolKey"),
	//("kCFNumberFormatterPlusSign"),
	//("kCFNumberFormatterPlusSignKey"),
	//("kCFNumberFormatterPositivePrefix"),
	//("kCFNumberFormatterPositivePrefixKey"),
	//("kCFNumberFormatterPositiveSuffix"),
	//("kCFNumberFormatterPositiveSuffixKey"),
	//("kCFNumberFormatterRoundingIncrement"),
	//("kCFNumberFormatterRoundingIncrementKey"),
	//("kCFNumberFormatterRoundingMode"),
	//("kCFNumberFormatterRoundingModeKey"),
	//("kCFNumberFormatterSecondaryGroupingSize"),
	//("kCFNumberFormatterSecondaryGroupingSizeKey"),
	//("kCFNumberFormatterUseGroupingSeparator"),
	//("kCFNumberFormatterUseGroupingSeparatorKey"),
	//("kCFNumberFormatterUseSignificantDigits"),
	//("kCFNumberFormatterUseSignificantDigitsKey"),
	//("kCFNumberFormatterZeroSymbol"),
	//("kCFNumberFormatterZeroSymbolKey"),
	//("kCFNumberNaN"),
	//("kCFNumberNegativeInfinity"),
	//("kCFNumberPositiveInfinity"),
	//("kCFPersianCalendar"),
	//("kCFPlugInDynamicRegisterFunctionKey"),
	//("kCFPlugInDynamicRegistrationKey"),
	//("kCFPlugInFactoriesKey"),
	//("kCFPlugInTypesKey"),
	//("kCFPlugInUnloadFunctionKey"),
	//("kCFPreferencesAnyApplication"),
	//("kCFPreferencesAnyHost"),
	//("kCFPreferencesAnyUser"),
	//("kCFPreferencesCurrentApplication"),
	//("kCFPreferencesCurrentHost"),
	//("kCFPreferencesCurrentUser"),
	//("kCFPreferencesDidChangeNotificationName"),
	//("kCFPreferencesDidChangeObject"),
	//("kCFRepublicOfChinaCalendar"),
	//("kCFRunLoopCommonModes"),
	//("kCFRunLoopDefaultMode"),
	//("kCFSocketCommandKey"),
	//("kCFSocketErrorKey"),
	//("kCFSocketNameKey"),
	//("kCFSocketRegisterCommand"),
	//("kCFSocketResultKey"),
	//("kCFSocketRetrieveCommand"),
	//("kCFSocketValueKey"),
	//("kCFStreamPropertyAppendToFile"),
	("kCFStreamPropertyDataWritten"),
	//("kCFStreamPropertyFileCurrentOffset"),
	//("kCFStreamPropertySocketNativeHandle"),
	//("kCFStreamPropertySocketRemoteHostName"),
	//("kCFStreamPropertySocketRemotePortNumber"),
	//("kCFStringBinaryHeapCallBacks"),
	//("kCFStringFormatLocaleKey"),
	//("kCFStringFormatSpecTypeKey"),
	//("kCFStringFormatValueTypeKey"),
	//("kCFStringGenderRuleType"),
	//("kCFStringLocalizedFormatKey"),
	//("kCFStringPluralRuleType"),
	//("kCFStringTransformFullwidthHalfwidth"),
	//("kCFStringTransformHiraganaKatakana"),
	//("kCFStringTransformLatinArabic"),
	//("kCFStringTransformLatinCyrillic"),
	//("kCFStringTransformLatinGreek"),
	//("kCFStringTransformLatinHangul"),
	//("kCFStringTransformLatinHebrew"),
	//("kCFStringTransformLatinHiragana"),
	//("kCFStringTransformLatinKatakana"),
	//("kCFStringTransformLatinThai"),
	//("kCFStringTransformMandarinLatin"),
	//("kCFStringTransformStripCombiningMarks"),
	//("kCFStringTransformStripDiacritics"),
	//("kCFStringTransformToLatin"),
	//("kCFStringTransformToUnicodeName"),
	//("kCFStringTransformToXMLHex"),
	//("kCFTimeZoneSystemTimeZoneDidChangeNotification"),
	("kCFTypeArrayCallBacks"),
	//("kCFTypeBagCallBacks"),
	("kCFTypeDictionaryKeyCallBacks"),
	("kCFTypeDictionaryValueCallBacks"),
	//("kCFTypeSetCallBacks"),
	//("kCFURLFileDirectoryContents"),
	//("kCFURLFileExists"),
	//("kCFURLFileLastModificationTime"),
	//("kCFURLFileLength"),
	//("kCFURLFileOwnerID"),
	//("kCFURLFilePOSIXMode"),
	//("kCFURLHTTPStatusCode"),
	//("kCFURLHTTPStatusLine"),
	//("kCFUseCollectableAllocator"),
	//("kCFXMLTreeErrorDescription"),
	//("kCFXMLTreeErrorLineNumber"),
	//("kCFXMLTreeErrorLocation"),
	//("kCFXMLTreeErrorStatusCode	"),
};

FARPROC __gpf_CoreFoundation_dll_ri[ENUM_COREFOUNDATION_DLL_NUM];

static HINSTANCE __g_CoreFoundation_hDLL;
//##################################################################################################################

//##################################################################################################################
//############################################ 内部函数开始 ########################################################
//##################################################################################################################

//##################################################################################################################
//############################################ 内部函数结束 ########################################################
//##################################################################################################################

//##################################################################################################################
//############################################ 外部接口开始 ########################################################
//##################################################################################################################
#include <stdio.h>
extern void DebugOut( LPTSTR lpFormat, ... )
{

#if 0
	TCHAR   wszOut[512];
	va_list args;
	FILE * fp;

	va_start( args, lpFormat );

	//_vstprintf( wszOut, lpFormat, args );
// 	fp = fopen( "D:\\Temp\\log.txt", "a+" );
// 	if( fp )
// 	{
// 		fwprintf( fp, L"[Time=%d], ", GetTickCount() );
// 		vfwprintf( fp, lpFormat, args  );
// 		fclose( fp );
// 	}
	_vftprintf_s(stdout, lpFormat, args);

	va_end( args );
#endif
}

//初始化ATH_INTERFACE"),
int ATH_Interface_Initialize( void )
{
	int i;

	__g_AirTrafficHost_hDLL = LoadLibrary( TEXT("AirTrafficHost.dll") );
	if( !__g_AirTrafficHost_hDLL )
	{
		DebugOut( TEXT("LoadLibrary error, errno=%d\n"), GetLastError() );
		return -1;
	}

	for( i = 0; i < ENUM_AIRTRAFFICHOST_DLL_NUM; i++ )
	{
		__gpf_AirTrafficHost_dll_ri[i] = GetProcAddress( __g_AirTrafficHost_hDLL, __gpf_AirTrafficHost_dll_text[i] );
		if( !__gpf_AirTrafficHost_dll_ri[i] )
		{
			DebugOut( TEXT("No found %s function\n"), __gpf_AirTrafficHost_dll_text[i] );
		}
	}
	return 0;
}

//释放ATH_INTERFACE"),
int ATH_Interface_Uninitialize( void )
{
	if( __g_AirTrafficHost_hDLL )
	{
		//FreeLibrary( __g_AirTrafficHost_hDLL );
	}
	return 0;
}

//初始化CoreF_INTERFACE"),
int CoreF_Interface_Initialize(void)
{
	int i;

	__g_CoreFoundation_hDLL = LoadLibrary(TEXT("CoreFoundation.dll"));
	if( !__g_CoreFoundation_hDLL )
		return -1;

	for( i = 0; i < ENUM_COREFOUNDATION_DLL_NUM; i++ )
	{
		__gpf_CoreFoundation_dll_ri[i] = GetProcAddress( __g_CoreFoundation_hDLL, __gpf_CoreFoundation_dll_text[i] );
		if( !__gpf_CoreFoundation_dll_ri[i] )
		{
			DebugOut( TEXT("No found %s function\n"), __gpf_CoreFoundation_dll_text[i] );
		}
	}
	return 0;
}

//释放CoreF_INTERFACE"),
int CoreF_Interface_Uninitialize( void )
{
	if( __g_CoreFoundation_hDLL )
	{
		//FreeLibrary( __g_CoreFoundation_hDLL );
	}
	return 0;
}


//初始化iTunesMobileDevice"),
int ITMD_Interface_Initialize(void)
{
	int i;

	__g_iTunesMobileDevice_hDLL = LoadLibrary( TEXT("iTunesMobileDevice.dll") );
	if( !__g_iTunesMobileDevice_hDLL )
	{
		DebugOut( TEXT("LoadLibrary error, errno=%d\n"), GetLastError() );
		return -1;
	}

	for( i = 0; i < ENUM_ITUNESMOBILEDEVICE_DLL_NUM; i++ )
	{
		//INFOLOG(_T("client load iTunesMobileDevice function %d"), i);
		__gpf_iTunesMobileDevice_dll_ri[i] = GetProcAddress( __g_iTunesMobileDevice_hDLL, __gpf_iTunesMobileDevice_dll_text[i] );
		if( !__gpf_iTunesMobileDevice_dll_ri[i] )
		{
			DebugOut( TEXT("No found %s function\n"), __gpf_iTunesMobileDevice_dll_text[i] );
		}
	}
	return 0;
}

//释放iTunesMobileDevice"),
int ITMD_Interface_Uninitialize( void )
{
	if( __g_iTunesMobileDevice_hDLL )
	{
		//FreeLibrary( __g_iTunesMobileDevice_hDLL );
	}
	return 0;
}


int __cdecl getItunesErrorString(int errcode)
{
	int result; 

	if ( errcode > 0 )
	{
		result = 0;
	}
	else
	{
		if ( errcode )
		{
			switch ( errcode )
			{
			case -402653183:
				result = (int)"kAMDUndefinedError";
				break;
			case -402653182:
				result = (int)"kAMDBadHeaderError";
				break;
			case -402653181:
				result = (int)"kAMDNoResourcesError";
				break;
			case -402653180:
				result = (int)"kAMDReadError";
				break;
			case -402653179:
				result = (int)"kAMDWriteError";
				break;
			case -402653178:
				result = (int)"kAMDUnknownPacketError";
				break;
			case -402653177:
				result = (int)"kAMDInvalidArgumentError";
				break;
			case -402653176:
				result = (int)"kAMDNotFoundError";
				break;
			case -402653175:
				result = (int)"kAMDIsDirectoryError";
				break;
			case -402653174:
				result = (int)"kAMDPermissionError";
				break;
			case -402653173:
				result = (int)"kAMDNotConnectedError";
				break;
			case -402653172:
				result = (int)"kAMDTimeOutError";
				break;
			case -402653171:
				result = (int)"kAMDOverrunError";
				break;
			case -402653170:
				result = (int)"kAMDEOFError";
				break;
			case -402653169:
				result = (int)"kAMDUnsupportedError";
				break;
			case -402653168:
				result = (int)"kAMDFileExistsError";
				break;
			case -402653167:
				result = (int)"kAMDBusyError";
				break;
			case -402653166:
				result = (int)"kAMDCryptoError";
				break;
			case -402653051:
				result = (int)"kAMDTooBigError";
				break;
			case -402653165:
				result = (int)"kAMDInvalidResponseError";
				break;
			case -402653164:
				result = (int)"kAMDMissingKeyError";
				break;
			case -402653163:
				result = (int)"kAMDMissingValueError";
				break;
			case -402653162:
				result = (int)"kAMDGetProhibitedError";
				break;
			case -402653161:
				result = (int)"kAMDSetProhibitedError";
				break;
			case -402653160:
				result = (int)"kAMDRemoveProhibitedError";
				break;
			case -402653159:
				result = (int)"kAMDImmutableValueError";
				break;
			case -402653158:
				result = (int)"kAMDPasswordProtectedError";
				break;
			case -402653157:
				result = (int)"kAMDMissingHostIDError";
				break;
			case -402653156:
				result = (int)"kAMDInvalidHostIDError";
				break;
			case -402653155:
				result = (int)"kAMDSessionActiveError";
				break;
			case -402653154:
				result = (int)"kAMDSessionInactiveError";
				break;
			case -402653153:
				result = (int)"kAMDMissingSessionIDError";
				break;
			case -402653152:
				result = (int)"kAMDInvalidSessionIDError";
				break;
			case -402653151:
				result = (int)"kAMDMissingServiceError";
				break;
			case -402653150:
				result = (int)"kAMDInvalidServiceError";
				break;
			case -402653093:
				result = (int)"kAMDServiceLimitError";
				break;
			case -402653090:
				result = (int)"kAMDCheckinSetupFailedError";
				break;
			case -402653149:
				result = (int)"kAMDInvalidCheckinError";
				break;
			case -402653148:
				result = (int)"kAMDCheckinTimeoutError";
				break;
			case -402653089:
				result = (int)"kAMDCheckinConnectionFailedError";
				break;
			case -402653088:
				result = (int)"kAMDCheckinReceiveFailedError";
				break;
			case -402653087:
				result = (int)"kAMDCheckinResponseFailedError";
				break;
			case -402653079:
				result = (int)"kAMDCheckinOutOfMemoryError";
				break;
			case -402653086:
				result = (int)"kAMDCheckinSendFailedError";
				break;
			case -402653147:
				result = (int)"kAMDMissingPairRecordError";
				break;
			case -402653092:
				result = (int)"kAMDInvalidPairRecordError";
				break;
			case -402653080:
				result = (int)"kAMDSavePairRecordFailedError";
				break;
			case -402653146:
				result = (int)"kAMDInvalidActivationRecordError";
				break;
			case -402653145:
				result = (int)"kAMDMissingActivationRecordError";
				break;
			case -402653091:
				result = (int)"kAMDServiceProhibitedError";
				break;
			case -402653055:
				result = (int)"kAMDEscrowLockedError";
				break;
			case -402653054:
				result = (int)"kAMDPairingProhibitedError";
				break;
			case -402653053:
				result = (int)"kAMDProhibitedBySupervision";
				break;
			case -402653144:
				result = (int)"kAMDWrongDroidError";
				break;
			case -402653143:
				result = (int)"kAMDSUVerificationError";
				break;
			case -402653142:
				result = (int)"kAMDSUPatchError";
				break;
			case -402653141:
				result = (int)"kAMDSUFirmwareError";
				break;
			case -402653140:
				result = (int)"kAMDProvisioningProfileNotValid";
				break;
			case -402653139:
				result = (int)"kAMDSendMessageError";
				break;
			case -402653138:
				result = (int)"kAMDReceiveMessageError";
				break;
			case -402653137:
				result = (int)"kAMDMissingOptionsError";
				break;
			case -402653136:
				result = (int)"kAMDMissingImageTypeError";
				break;
			case -402653135:
				result = (int)"kAMDDigestFailedError";
				break;
			case -402653134:
				result = (int)"kAMDStartServiceError";
				break;
			case -402653133:
				result = (int)"kAMDInvalidDiskImageError";
				break;
			case -402653132:
				result = (int)"kAMDMissingDigestError";
				break;
			case -402653131:
				result = (int)"kAMDMuxError";
				break;
			case -402653130:
				result = (int)"kAMDApplicationAlreadyInstalledError";
				break;
			case -402653129:
				result = (int)"kAMDApplicationMoveFailedError";
				break;
			case -402653128:
				result = (int)"kAMDApplicationSINFCaptureFailedError";
				break;
			case -402653127:
				result = (int)"kAMDApplicationSandboxFailedError";
				break;
			case -402653126:
				result = (int)"kAMDApplicationVerificationFailedError";
				break;
			case -402653125:
				result = (int)"kAMDArchiveDestructionFailedError";
				break;
			case -402653124:
				result = (int)"kAMDBundleVerificationFailedError";
				break;
			case -402653123:
				result = (int)"kAMDCarrierBundleCopyFailedError";
				break;
			case -402653122:
				result = (int)"kAMDCarrierBundleDirectoryCreationFailedError";
				break;
			case -402653121:
				result = (int)"kAMDCarrierBundleMissingSupportedSIMsError";
				break;
			case -402653120:
				result = (int)"kAMDCommCenterNotificationFailedError";
				break;
			case -402653119:
				result = (int)"kAMDContainerCreationFailedError";
				break;
			case -402653118:
				result = (int)"kAMDContainerP0wnFailedError";
				break;
			case -402653117:
				result = (int)"kAMDContainerRemovalFailedError";
				break;
			case -402653116:
				result = (int)"kAMDEmbeddedProfileInstallFailedError";
				break;
			case -402653115:
				result = (int)"kAMDErrorError";
				break;
			case -402653114:
				result = (int)"kAMDExecutableTwiddleFailedError";
				break;
			case -402653113:
				result = (int)"kAMDExistenceCheckFailedError";
				break;
			case -402653112:
				result = (int)"kAMDInstallMapUpdateFailedError";
				break;
			case -402653111:
				result = (int)"kAMDManifestCaptureFailedError";
				break;
			case -402653110:
				result = (int)"kAMDMapGenerationFailedError";
				break;
			case -402653109:
				result = (int)"kAMDMissingBundleExecutableError";
				break;
			case -402653108:
				result = (int)"kAMDMissingBundleIdentifierError";
				break;
			case -402653107:
				result = (int)"kAMDMissingBundlePathError";
				break;
			case -402653106:
				result = (int)"kAMDMissingContainerError";
				break;
			case -402653105:
				result = (int)"kAMDNotificationFailedError";
				break;
			case -402653104:
				result = (int)"kAMDPackageExtractionFailedError";
				break;
			case -402653103:
				result = (int)"kAMDPackageInspectionFailedError";
				break;
			case -402653102:
				result = (int)"kAMDPackageMoveFailedError";
				break;
			case -402653101:
				result = (int)"kAMDPathConversionFailedError";
				break;
			case -402653100:
				result = (int)"kAMDRestoreContainerFailedError";
				break;
			case -402653099:
				result = (int)"kAMDSeatbeltProfileRemovalFailedError";
				break;
			case -402653098:
				result = (int)"kAMDStageCreationFailedError";
				break;
			case -402653097:
				result = (int)"kAMDSymlinkFailedError";
				break;
			case -402653096:
				result = (int)"kAMDiTunesArtworkCaptureFailedError";
				break;
			case -402653095:
				result = (int)"kAMDiTunesMetadataCaptureFailedError";
				break;
			case -402653094:
				result = (int)"kAMDAlreadyArchivedError";
				break;
			case -402653082:
				result = (int)"kAMDUnknownCommandError";
				break;
			case -402653081:
				result = (int)"kAMDAPIInternalError";
				break;
			case -402653085:
				result = (int)"kAMDMuxCreateListenerError";
				break;
			case -402653084:
				result = (int)"kAMDMuxGetListenerError";
				break;
			case -402653083:
				result = (int)"kAMDMuxConnectError";
				break;
			case -402653078:
				result = (int)"kAMDDeviceTooNewError";
				break;
			case -402653077:
				result = (int)"kAMDDeviceRefNoGood";
				break;
			case -402653052:
				result = (int)"kAMDDeviceDisconnectedError";
				break;
			case -402653076:
				result = (int)"kAMDCannotTranslateError";
				break;
			case -402653075:
				result = (int)"kAMDMobileImageMounterMissingImageSignature";
				break;
			case -402653074:
				result = (int)"kAMDMobileImageMounterResponseCreationFailed";
				break;
			case -402653073:
				result = (int)"kAMDMobileImageMounterMissingImageType";
				break;
			case -402653072:
				result = (int)"kAMDMobileImageMounterMissingImagePath";
				break;
			case -402653071:
				result = (int)"kAMDMobileImageMounterImageMapLoadFailed";
				break;
			case -402653070:
				result = (int)"kAMDMobileImageMounterAlreadyMounted";
				break;
			case -402653069:
				result = (int)"kAMDMobileImageMounterImageMoveFailed";
				break;
			case -402653068:
				result = (int)"kAMDMobileImageMounterMountPathMissing";
				break;
			case -402653067:
				result = (int)"kAMDMobileImageMounterMountPathNotEmpty";
				break;
			case -402653066:
				result = (int)"kAMDMobileImageMounterImageMountFailed";
				break;
			case -402653065:
				result = (int)"kAMDMobileImageMounterTrustCacheLoadFailed";
				break;
			case -402653064:
				result = (int)"kAMDMobileImageMounterDigestFailed";
				break;
			case -402653063:
				result = (int)"kAMDMobileImageMounterDigestCreationFailed";
				break;
			case -402653062:
				result = (int)"kAMDMobileImageMounterImageVerificationFailed";
				break;
			case -402653061:
				result = (int)"kAMDMobileImageMounterImageInfoCreationFailed";
				break;
			case -402653060:
				result = (int)"kAMDMobileImageMounterImageMapStoreFailed";
				break;
			case -402653059:
				result = (int)"kAMDBonjourSetupError";
				break;
			case -402653057:
				result = (int)"kAMDNoWifiSyncSupportError";
				break;
			case -402653058:
				result = (int)"kAMDDeviceOSVersionTooLow";
				break;
			case -402653056:
				result = (int)"kAMDDeviceFamilyNotSupported";
				break;
			case -402653050:
				result = (int)"kAMDPackagePatchFailedError";
				break;
			case -402653049:
				result = (int)"kAMDIncorrectArchitectureError";
				break;
			case -402653048:
				result = (int)"kAMDPluginCopyFailedError";
				break;
			case -402653047:
				result = (int)"kAMDBreadcrumbFailedError";
				break;
			case -402653046:
				result = (int)"kAMDBreadcrumbUnlockError";
				break;
			case -402653045:
				result = (int)"kAMDGeoJSONCaptureFailedError";
				break;
			case -402653044:
				result = (int)"kAMDNewsstandArtworkCaptureFailedError";
				break;
			case -402653043:
				result = (int)"kAMDMissingCommandError";
				break;
			case -402653042:
				result = (int)"kAMDNotEntitledError";
				break;
			case -402653041:
				result = (int)"kAMDMissingPackagePathError";
				break;
			case -402653040:
				result = (int)"kAMDMissingContainerPathError";
				break;
			case -402653039:
				result = (int)"kAMDMissingApplicationIdentifierError";
				break;
			case -402653038:
				result = (int)"kAMDMissingAttributeValueError";
				break;
			case -402653037:
				result = (int)"kAMDLookupFailedError";
				break;
			case -402653036:
				result = (int)"kAMDDictCreationFailedError";
				break;
			default:
				{
					char *buf = (char *)malloc(21);
					_itoa_s(errcode, buf, 21, 10);
					result = (int)buf;
				}
				break;
			}
		}
		else
		{
			result = (int)"kAMDSuccess";
		}
	}
	return result;
}



//##################################################################################################################
//############################################ 外部接口结束 ########################################################
//##################################################################################################################
