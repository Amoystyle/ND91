#pragma once
#include "Module/BaseData.h"
#include <string>

using namespace std;

namespace ND91Assistant
{

	class CLASSINDLL_CLASS_DECL RecordingData : public BaseData
	{
	public:
		RecordingData(void);
		virtual ~RecordingData(void);
		
		int			_pk;					// DB����ļ�ֵ
		int			_labelPreset;           // ϵͳԤ�õı�ǩֵ
		double		_duration;				// ¼��ʱ��	
		wstring		_path;					// ¼����ŵ�·��
		wstring		_customLabel;			// �û��Զ����ǩ(��ҪϵͳԤ�ñ�ǩֵΪ7)
		time_t		_createDate;            // ¼��������ʱ��

		wstring		_labelPresetText;		// ϵͳԤ�ñ�ǩֵ��Ӧ���ı�
		wstring		_name;
		int			_size;

	};

}
