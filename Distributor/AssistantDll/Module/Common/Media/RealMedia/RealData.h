#pragma once

#include <string>
#include <vector>

using namespace std;

namespace ND91Assistant
{
	// real�ļ����б�Ǽ����ݽṹ��
	struct FIELDDEF
	{
		std::string name;
		std::string type;
		int			size;
		std::string szieField;	
		std::string endian;		
	};

	// real�ļ������
	class RealData
	{ 
	public:
		RealData();
		~RealData(void);

	public:

		std::string			_name;			// �����
		vector<FIELDDEF*>	_fieldDef;		// ���ݼ�
	};

}
