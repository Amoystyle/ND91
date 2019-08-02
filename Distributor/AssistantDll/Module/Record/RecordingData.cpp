#include "stdafx.h"

#ifdef MODULE_RECORD

#include "Module/Record/RecordingData.h"

RecordingData::RecordingData(void)
{
	_pk				= -1;			
	_labelPreset	= -1;   
	_duration		= -1;
	_size			= -1;
	_createDate		= 0;
}

RecordingData::~RecordingData(void)
{
}



#endif