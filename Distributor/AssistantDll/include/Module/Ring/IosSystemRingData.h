#pragma once

#include "RingData.h"

namespace ND91Assistant
{
    class CLASSINDLL_CLASS_DECL IosSystemRingData : public RingData
    {
    public:
		virtual ~IosSystemRingData() {}

		virtual NodeData* Clone(DirData* pParent) const;

		// ios…Ë±∏ÃÊªª¡Â…˘
		bool Replace(wstring filePath);
	};
}
