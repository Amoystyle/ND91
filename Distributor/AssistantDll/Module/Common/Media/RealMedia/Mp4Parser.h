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
	// MP4���Ź켣�ṹ��
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
	// MP4��Ƶ�ļ��Ĺ̼�����
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

		// ��ȡý���ļ�����ʱ��
		virtual	long GetMediaDuration();

		// �ж��Ƿ�����ĳ�ָ�ʽ�ļ�
		bool ParserFormat();

	private:

		// ��ȡʱ����
		void Set_mp4AudioSpecificConfig();

		// ͨ��MP4�ļ��������ж�����
		int mp4ff_atom_name_to_type(unsigned char* buf);
		
		// ��ȡMP4�ļ��ı��
		// Returns  : void
		// Parameter: long totalAtomSize
		void parse_sub_atoms(long totalAtomSize);

		// ��ȡ������ͷ������
		void read_atom_header(int& atomType, long& atomSize);

		// ���ݲ�ͬ���ͻ�ȡ����
		void mp4ff_atom_read(int atomType, long atomSize);
		
		// stts�������ݵĶ�ȡ
		void mp4ff_read_stts();

		// stsd�������ݵĶ�ȡ
		void mp4ff_read_stsd();

		// mp4a�������ݵĶ�ȡ
		void mp4ff_read_mp4a();

		// esds�������ݵĶ�ȡ
		void mp4ff_read_esds();

		// ��ȡ�̶���Ǻ�ĳ���
		int mp4ff_read_mp4_descr_length();

		// meta�������ݵĶ�ȡ
		void mp4ff_read_meta(long atomSize);

		// metadata�������ݵĶ�ȡ
		void mp4ff_parse_metadata(long atomSize);
		
		// ��ȡ�ļ���ǻ��Ԫ��ֵ
		void mp4ff_parse_tag(int parent_atom_type, long size);

		// ������������
		void mp4ff_tag_add_field(std::string name, unsigned char* data);

		// ���ݲ�ͬԪ��int��ǻ��Ԫ����
		std::string mp4ff_set_metadata_name(int parent_atom_type);

		unsigned char* mp4ff_read_string(long readSize);

        // ����ý���ļ���Ϣ����������ʱ����
        bool ParserHeader();

        // ����tag��Ϣ
        bool GenerateID31Tag();

        // ����Track��Ϣ
        bool GenerateTrack();

        // ����������Ϣ
        bool GenerateOther();

		// MP4�ļ��������ռ�
		std::vector<MP4FF_TRACK_T*>		_ltTrack;

		// ȫ���ļ�������ָ��
		fstream*						_fs;						

		// MP4�ļ��̼���������
		MP4AUDIOSPECIFICCONFIG			_mp4AudioSpecificConfig;	 

		// MP4��������ռ�����
		int								_currentTrack;				 

        // MP4�ļ���tag��Ϣ
        map<string, unsigned char*>     _dicTag;
	};
}
