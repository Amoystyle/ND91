#pragma  once

#include "Module/IManager.h"

namespace ND91Assistant
{
    class NoteSynchronizer;
    class DeviceData;
    class NoteData;

    class CLASSINDLL_CLASS_DECL NoteManager : public IManager
    {
    public:
        NoteManager(const DeviceData* pDevice);
        virtual ~NoteManager();

        // �Ƿ�֧��
        virtual bool IsSupport();

        // ����
        virtual CommandExecuteResult Load();

        // �ͷ�
        virtual void Release();

        // ȡ����
        virtual const vector<NoteData*>* GetDatas();

        // ɾ��
        virtual CommandExecuteResult DelNote(NoteData* note);

        // ���
        virtual NoteData* AddNote(wstring subject, wstring content);

		// ����
		virtual NoteData* ImportNote(NoteData* importnote){return NULL;}

        // �޸�
        virtual NoteData* EditNote(NoteData* note, wstring subject, wstring content);

        // �ύ�޸�
        virtual CommandExecuteResult Update();

    protected:

#pragma warning (disable:4251)
        vector<NoteData*> _datas;
#pragma warning (default:4251)
	};
}