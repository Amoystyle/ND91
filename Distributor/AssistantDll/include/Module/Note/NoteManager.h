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

        // 是否支持
        virtual bool IsSupport();

        // 加载
        virtual CommandExecuteResult Load();

        // 释放
        virtual void Release();

        // 取数据
        virtual const vector<NoteData*>* GetDatas();

        // 删除
        virtual CommandExecuteResult DelNote(NoteData* note);

        // 添加
        virtual NoteData* AddNote(wstring subject, wstring content);

		// 导入
		virtual NoteData* ImportNote(NoteData* importnote){return NULL;}

        // 修改
        virtual NoteData* EditNote(NoteData* note, wstring subject, wstring content);

        // 提交修改
        virtual CommandExecuteResult Update();

    protected:

#pragma warning (disable:4251)
        vector<NoteData*> _datas;
#pragma warning (default:4251)
	};
}