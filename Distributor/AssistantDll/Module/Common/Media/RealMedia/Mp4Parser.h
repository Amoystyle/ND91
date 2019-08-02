#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "AssistantDll.h"
using namespace std;

namespace ND91Assistant
{
	// MP4播放轨迹结构体
	struct MP4FF_TRACK_T
	{
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
		int* stts_sample_count;
		int* stts_sample_delta;
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

		MP4FF_TRACK_T()
		{
			ntype				=0;
			channelCount		=0;
			sampleSize			=0;
			sampleRate			=0;
			audioType			=0;
			stsd_entry_count	=0;

			stsz_sample_size	=0;
			stsz_sample_count	=0;

			stts_entry_count	=0;
			stts_sample_count	=NULL;
			stts_sample_delta	=NULL;	

			stsc_entry_count	=0;

			stco_entry_count	=0;
			ctts_entry_count	=0;

			decoderConfigLen	=0;
			decoderConfig		=NULL;

			maxBitrate			=0;
			avgBitrate			=0;

			timeScale			=0;
			duration			=0;	
		}
	};
	// MP4音频文件的固件参数
	struct MP4AUDIOSPECIFICCONFIG
	{
		/* Audio Specific Info */
		int objectTypeIndex;
		int samplingFrequencyIndex;
		int samplingFrequency;
		int channelsConfiguration;

		/* GA Specific Info */
		int frameLengthFlag;
		int dependsOnCoreCoder;
		int coreCoderDelay;
		int extensionFlag;
		int aacSectionDataResilienceFlag;
		int aacScalefactorDataResilienceFlag;
		int aacSpectralDataResilienceFlag;
		int epConfig;

		int sbr_present_flag;
		int forceUpSampling;
		int downSampledSBR;
	};

	class Mp4Parser : public MediaBase
	{ 
	public:
		Mp4Parser(const wstring path);
		~Mp4Parser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();

	private:

		// 获取时长等
		void Set_mp4AudioSpecificConfig();

		// 通过MP4文件特殊标记判断类型
		int mp4ff_atom_name_to_type(unsigned char* buf);
		
		// 获取MP4文件的标记
		// Returns  : void
		// Parameter: long totalAtomSize
		void parse_sub_atoms(long totalAtomSize);

		// 读取缓存区头部数据
		void read_atom_header(int& atomType, long& atomSize);

		// 根据不同类型获取数据
		void mp4ff_atom_read(int atomType, long atomSize);
		
		// stts类型数据的读取
		void mp4ff_read_stts();

		// stsd类型数据的读取
		void mp4ff_read_stsd();

		// mp4a类型数据的读取
		void mp4ff_read_mp4a();

		// esds类型数据的读取
		void mp4ff_read_esds();

		// 读取固定标记后的长度
		int mp4ff_read_mp4_descr_length();

		// meta类型数据的读取
		void mp4ff_read_meta(long atomSize);

		// metadata类型数据的读取
		void mp4ff_parse_metadata(long atomSize);
		
		// 读取文件标记获得元素值
		void mp4ff_parse_tag(int parent_atom_type, long size);

		// 解析其它数据
		void mp4ff_tag_add_field(std::string name, unsigned char* data);

		// 根据不同元素int标记获得元素名
		std::string mp4ff_set_metadata_name(int parent_atom_type);

		unsigned char* mp4ff_read_string(long readSize);

        // 解析媒体文件信息（包括播放时长）
        bool ParserHeader();

        // 解析tag信息
        bool GenerateID31Tag();

        // 解析Track信息
        bool GenerateTrack();

        // 解析其他信息
        bool GenerateOther();

		// MP4文件的数据收集
		std::vector<MP4FF_TRACK_T*>		_ltTrack;

		// 全局文件操作流指针
		fstream*						_fs;						

		// MP4文件固件参数对象
		MP4AUDIOSPECIFICCONFIG			_mp4AudioSpecificConfig;	 

		// MP4标记数据收集总数
		int								_currentTrack;				 

        // MP4文件的tag信息
        map<string, unsigned char*>     _dicTag;
	};
}
