#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"

namespace ND91Assistant
{
	// ipod管理播放列表表 数据类(以D开头)
	class DAvformatInfo : public IDataRow
	{
	public:
		DAvformatInfo();
		virtual ~DAvformatInfo(){}
		__int64	item_pid;
		int		sub_id;
		int	audio_format;	
		int	bit_rate;	
		int	sample_rate;
		int	duration;	
		int	gapless_heuristic_info;	
		int	gapless_encoding_delay;	
		int	gapless_encoding_drain;	
		int	gapless_last_frame_resynch;	
		int	analysis_inhibit_flags;	
		int	audio_fingerprint;	
		int	volume_normalization_energy;
			
		virtual const vector<wstring>* const GetValues();
		virtual wstring PrimaryKeyCondition() const;
#pragma region 必须实现的静态方法, 模板类BaseTable中用到
		static wstring GetTableName();
		static DAvformatInfo* FromValue(const vector<const char*>* pValues);
		static void GetColumns(vector<wstring>& vector);
#pragma endregion

	};
}
