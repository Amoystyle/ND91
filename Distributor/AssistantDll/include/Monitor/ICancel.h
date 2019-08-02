#pragma  once

#define IF_CANCEL(c)               if (c && c->IsCancel())
#define IF_CANCEL_RETURN(c)        if (c && c->IsCancel()) return;
#define IF_CANCEL_RETURN_FALSE(c)  if (c && c->IsCancel()) return false;
#define IF_CANCEL_RETURN_VALUE(c, value)  if (c && c->IsCancel()) return value;
#define IF_PASS_RETURN_FALSE(c)    if (c && c->IsSkip())   return false;
#define IF_PASS_RETURN_VALUE(c, value)    if (c && c->IsSkip())   return value;
#define IF_CANCEL_BREAK(c)         if (c && c->IsCancel()) break;
namespace ND91Assistant
{
    // ȡ���ӿ���
    class CLASSINDLL_CLASS_DECL ICancel
    {
    public:
        // ���캯��, ȡ����׼Ϊfalse
        ICancel() { _cancel = false; _skip = false;}

        // ����ȡ����־Ϊtrue(�ɽ������)
        void Cancel(bool cancel=true) { _cancel = cancel; }

        // ����������־��������ǰ������
        void Skip(bool bSkip=true) { _skip = bSkip; };

        // ���ȡ����׼(��DLL�ڲ�����)
        virtual bool IsCancel() { return _cancel; }

        // ���������־
        virtual bool IsSkip() { return _skip; };
    private:
        bool _cancel;
        bool _skip; // ������־���Դ��Ͳ�����Ƕ�׼��������ã��籸�ݻ�ԭ
    };
}