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

		// ����������
		bool RenameRing(wstring title);

		// ����ΪiPhone����������
		bool SetIosRing();

	};
}
