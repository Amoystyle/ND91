#pragma once

#include "Module/BaseData.h"
#include <string>
using namespace std;
namespace ND91Assistant
{
    //���ڽ����ֻ����ص������࣬���ǵ��������Ҳ��Ҫ�õ������Լ̳�BaseData
    class ProcessData : public BaseData
    {
        friend class AdbPSCommand;
    public:
	    ProcessData();
	    virtual ~ProcessData();

        //���̵�״̬
	    enum AndroidProcessState 
	    {
		    Unknow, // δ֪��Ĭ�ϣ�
		    D,      // �޷��жϵ�����״̬��ͨ�� IO �Ľ��̣�
		    R,      // �������п����ڶ����пɹ��е�
		    S,      // ��������״̬
		    T,      // ֹͣ��׷��
		    W,      // �����ڴ潻�������ں�2.6��ʼ��Ч��
		    X,      // �����Ľ���
		    Z       // ��ʬ����
	    };

        virtual void Update();

	    string  _strUser;
	    int     _nPID;
	    int     _nPPID;
	    long    _nSize;
	    long    _nRss;
	    string  _strName;
	    AndroidProcessState _eState;



    private:
        bool ParseAndroidData(string strLine);
        
    };
}
