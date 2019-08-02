#pragma once

#include "RingData.h"

namespace ND91Assistant
{
	class BinaryBufferReader;
    class CLASSINDLL_CLASS_DECL AndroidRingData : public RingData
    {
		friend class LoadRingsCommand;
		friend class AddRingCommand;
    public:
		virtual ~AndroidRingData() {}

		virtual NodeData* Clone(DirData* pParent) const;

		// 设置Android铃声
		bool SettingAndroidRing(MediaFileKind kind);

		// 播放铃声
		void PlayRing();

		// 重命名铃声
		bool RenameRing(wstring title);

		// 修改铃声类型
		bool ChangeRingType(MediaFileKind kind);

	private:
		bool ParseAndroidData(BinaryBufferReader& reader);
    };
}
