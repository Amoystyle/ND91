#pragma once

#include "RingData.h"

namespace ND91Assistant
{
	class BinaryBufferReader;
	class CLASSINDLL_CLASS_DECL IosUserRingData : public RingData
	{
	public:
		virtual ~IosUserRingData() {}

		virtual NodeData* Clone(DirData* pParent) const;

		// 重命名铃声
		bool RenameRing(wstring title);

		// 设置为iPhone的来电铃声
		bool SetIosRing();

	};
}
