///////////////////////////////////////////////////////////
//  SetOwnCommand.h
//  Implementation of the Class SetOwnCommand
//  Created on:      04-����-2011 10:36:08
//  Original author: lwj
///////////////////////////////////////////////////////////

#if !defined(EA_EEF5FD9B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_)
#define EA_EEF5FD9B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_

#include "Module/BaseCommand.h"
#include <string>

using namespace std;

namespace ND91Assistant
{
    // ����Ȩ������
    class SetOwnCommand : public BaseCommand
    {

    public:
        SetOwnCommand(const DeviceData* pDevice);
        virtual ~SetOwnCommand();

		void  SetParams(const wstring& pathOnPhone, const string& own);
        // ִ������
        virtual void Execute();

#pragma region ���ĸ�������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
        virtual std::string GetAndroidBodyData(){return "";};

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data) {}

		// ����ios�ļ��������ļ����������߸���ɾ���ļ�
		virtual std::wstring GetIosCommandFileContent();

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion

	private:
		wstring  _pathOnPhone;
		 string  _own;

    };
}

#endif // !defined(EA_EEF5FD9B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_)
