#pragma once
#include "Monitor/ProgressObserver.h"

namespace ND91Assistant
{
	class DefaultProgressObserver : public ProgressObserver
	{
    public:
        DefaultProgressObserver(EXIST_ACTION nAction) {_nAction = nAction; };
		// ���ս�����ʾ��������ʾ������ݣ�����������֧�֣��ɽ���ʵ��
		virtual void GetPrompt(MESSAGE_CODE msgCode) {};

		// ���ս�������
		virtual void GetProgress(ProgressData pData) {};

		// ���ս��������Ϣ
		virtual void Complete() {};

		// Summary  : ѯ���û��ļ�/Ŀ¼�Ѵ���ʱ��δ���
		// Returns  : ND91Assistant::EXIST_ACTION �û���ѡ��
		// Parameter: std::wstring name �ļ�/Ŀ¼��		
		virtual EXIST_ACTION GetExistAction(std::wstring name) { return _nAction; };

        EXIST_ACTION _nAction;
	};

    class OverwriteObserver : public ProgressObserver
    {
        // ���ս�����ʾ��������ʾ������ݣ�����������֧�֣��ɽ���ʵ��
        virtual void GetPrompt(MESSAGE_CODE msgCode) {};

        // ���ս�������
        virtual void GetProgress(ProgressData pData) {};

        // ���ս��������Ϣ
        virtual void Complete() {};

        // Summary  : ѯ���û��ļ�/Ŀ¼�Ѵ���ʱ��δ���
        // Returns  : ND91Assistant::EXIST_ACTION �û���ѡ��
        // Parameter: std::wstring name �ļ�/Ŀ¼��
        virtual EXIST_ACTION GetExistAction(std::wstring name) { return OVERWRITE_ALL; };
    };
}
