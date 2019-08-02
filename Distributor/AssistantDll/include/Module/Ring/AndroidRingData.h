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

		// ����Android����
		bool SettingAndroidRing(MediaFileKind kind);

		// ��������
		void PlayRing();

		// ����������
		bool RenameRing(wstring title);

		// �޸���������
		bool ChangeRingType(MediaFileKind kind);

	private:
		bool ParseAndroidData(BinaryBufferReader& reader);
    };
}
