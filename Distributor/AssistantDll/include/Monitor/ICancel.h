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
    // 取消接口类
    class CLASSINDLL_CLASS_DECL ICancel
    {
    public:
        // 构造函数, 取消标准为false
        ICancel() { _cancel = false; _skip = false;}

        // 设置取消标志为true(由界面调用)
        void Cancel(bool cancel=true) { _cancel = cancel; }

        // 设置跳过标志，跳过当前处理项
        void Skip(bool bSkip=true) { _skip = bSkip; };

        // 检查取消标准(由DLL内部调用)
        virtual bool IsCancel() { return _cancel; }

        // 检查跳过标志
        virtual bool IsSkip() { return _skip; };
    private:
        bool _cancel;
        bool _skip; // 跳过标志，对大型操作即嵌套级操作有用，如备份还原
    };
}