#pragma once
#include <string>
using namespace std;

namespace ND91Assistant
{  
    // ���������ӿ�
    class HardwareInfo;
	class HttpDriver
	{
	public:
		HttpDriver(const HardwareInfo* pHardware);
		~HttpDriver();

		//��ȡ�������ص�ַ
		wstring GetDriverUrl();

		//��ȡĬ���������ص�ַ����GetDriverUrl����""������»�ȡ
		wstring GetDefaultAndroidDriverUrl();

		// ��ȡiTunes���ص�ַ
		static wstring GetITunesUrl();

	private:
		string Post(string postData);

		const HardwareInfo* _pHardware;
		bool _isX86;
	};
}