#pragma once
#include <stdint.h>
#include <time.h>
#include <string>
#include <vector>
#include "Common/CodeOperation.h"
using namespace std;
#ifdef WIN32

#else
    #include <sys/time.h>
    #include <unistd.h>
	#define _atoi64 atol
#endif

namespace ND91Assistant
{
    // �������ͽӿ���: ÿ���������������Ҫ�̳д˻���
    class IDataRow
    {
    public:
        IDataRow();
        virtual ~IDataRow();

        // Summary  : ���麯��: ���������ֵת��Ϊ�ַ�������
        // Returns  : pVector�ĵ�ַ
        virtual const vector<wstring>* const GetValues() = 0;

        // Summary  : ���麯��: ����PK����where����
        // ʵ��ע��: �����ı�����, ��Ҫ��WstrToStr�����
        // Returns  : �ַ���
        virtual wstring PrimaryKeyCondition() const = 0;

    protected:
        // ���߷���: time_t ת ��ʾ���ַ���
        static wstring timeToStr(time_t t);

        // PCʱ���1970.1.1��ʼ, MACʱ���2001.1.1��ʼ, ���31��
        #define SECOND_OF_31_YEAR 978278400

        // ���߷���: mac ʱ�� ת pc ʱ��
        static time_t MacToPc(time_t t);

        // ���߷���: pc ʱ�� ת mac ʱ��
        static time_t PcToMac(time_t t);

        // ���߷���: int64 ת string
        static wstring Int64ToStr(/*__int64*/int64_t i);

        // ���߷���: int32 ת string
        static wstring Int32ToStr(/*__int32*/int32_t i);

        // ���߷���: ���� wstring
        static wstring ConvertStr(wstring ws);
		
		//���������ݶ�ȡ
		static wstring BinToStr( string s);

        vector<wstring>* GetVector();

    private:
        vector<wstring>* _pVector;    // GetValues��ʹ��
    };
}
