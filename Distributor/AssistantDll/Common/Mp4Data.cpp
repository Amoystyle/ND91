#include "stdafx.h"
#include "Mp4Data.h"

Mp4Data::Mp4Data()
{	
	ntype			= 0;
	channelCount	= 0;
	sampleSize		= 0;
	sampleRate		= 0;
	audioType		= 0;

	stsd_entry_count= 0;

	stsz_sample_size= 0;
	stsz_sample_count=0;

	stts_entry_count =0;
	stts_sample_count=NULL;
	stts_sample_delta=NULL;

	stsc_entry_count =0;

	stco_entry_count =0;

	ctts_entry_count =0;

	decoderConfigLen =0;
	decoderConfig	 =0;

	maxBitrate		=0;
	avgBitrate		=0;

	timeScale		=0;
	duration		=0;
	
}

Mp4Data::~Mp4Data(void)
{
}


