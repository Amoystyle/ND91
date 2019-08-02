#pragma once

namespace ND91Assistant
{
    // 屏幕演示观察者类
    class ScreenImageData;
    class CLASSINDLL_CLASS_DECL ScreenObserver :public IObserver
    {
    public:
	    virtual ~ScreenObserver(void){}

	    virtual void freshScreen(ScreenImageData* pScreenImage)=0;
    };

}