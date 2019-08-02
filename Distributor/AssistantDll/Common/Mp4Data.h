#pragma once

#include <string>
#include <vector>

using namespace std;

namespace ND91Assistant
{
	// MP4≤•∑≈πÏº£¿‡
	class Mp4Data
	{ 
	public:
		Mp4Data();
		~Mp4Data(void);

	public:

		int ntype;
		int channelCount;
		int sampleSize;
		int sampleRate;
		int audioType;

		/* stsd */
		int stsd_entry_count;

		/* stsz */
		int stsz_sample_size;
		int stsz_sample_count;

		/* stts */
		int stts_entry_count;
		int *stts_sample_count;
		int *stts_sample_delta;

		/* stsc */
		int stsc_entry_count;

		/* stsc */
		int stco_entry_count;

		/* ctts */
		int ctts_entry_count;

		int decoderConfigLen;
		unsigned char* decoderConfig;

		int maxBitrate;
		int avgBitrate;

		int timeScale;
		long duration;

	};

}
