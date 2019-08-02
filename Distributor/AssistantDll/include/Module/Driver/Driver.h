#pragma once

#include <string>
#include <vector>
using namespace std;

namespace ND91Assistant
{    
    //���������࣬����һ����������Ҫ�Ĳ���
	class HardwareInfo;
    class CLASSINDLL_CLASS_DECL Driver
    {
	friend class DriverManager;
    public:
        Driver(HardwareInfo* pHardware);
        Driver(Driver* pDriver);
        ~Driver();

        //��װ���� Inf ��ʽ
        bool InstallInf(wstring strInf);

		//��װ���� Exe ��ʽ
		bool InstallExe(wstring strExe, wstring strParam=L"");

		//��װ���� Msi ��ʽ
		bool InstallMsi(wstring strMsi, wstring strParam=L"");


		//��ȡ�������ص�ַ
		wstring GetDriverUrl();

		//��ȡĬ���������ص�ַ
		wstring GetDefaultAndroidDriverUrl();

		const HardwareInfo* getHardwareInfo(){ return _pHardware;};

		// Summary  : ����Inf�ļ���ʹ�������Ӧ�µ�ID
		// Returns  : bool �Ƿ��д����
		// Parameter: wstring strInfFile inf�ļ�·��
		bool CheckDefaultInfFile(wstring strInfFile);

		// ��Щ������װ�����������Ч���÷���������������
		bool WakeupDevice();
    private:
		wstring GetInfHardID( const HardwareInfo* pInfo);
		//��ָ����Driver�����ͷ�
        HardwareInfo *_pHardware;
		// �Ƿ���Ĭ������
		bool _bDefaultDriver;
		// ��������״̬
		void SetDriverState(unsigned long nState);
    };
}