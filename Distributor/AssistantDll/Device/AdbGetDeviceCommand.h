#pragma once

#include "AdbRemoteCommand.h"

//#define ADB_CMD_LIST_DEVICE		"host:track-devices"	//ö�ٵ�ǰ�����ӵ�Android�豸
#define ADB_CMD_LIST_DEVICE		"host:devices"
namespace ND91Assistant
{
    // ͨ��ADB ��ȡ����PC���ֻ��豸�б�(SerialNumber�б�)
    class AdbGetDeviceCommand : public AdbRemoteCommand
    {
    public:
        AdbGetDeviceCommand(string cmd = ADB_CMD_LIST_DEVICE);

        

        const vector<wstring>* GetSnList();
        int GetAllDeviceSize() {return _snMap.size();}; //ȡ�����豸����
        const map<wstring, wstring>* GetSnMap() {return &_snMap; };
		string GetReturnData();
    protected:
		virtual void InternalExecute();
        // ������������
        // ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
        virtual bool ParseReturnData();

    private:
        vector<wstring> _snList; //�������豸�б�
        map<wstring, wstring> _snMap; //�����豸�б���������״̬ <�豸���к�, �豸״̬>
		string _strCommand;
    };

}