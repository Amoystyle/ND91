#pragma once

namespace ND91Assistant
{
    // ��Ļ��ʾ�۲�����
    class ScreenImageData;
    class CLASSINDLL_CLASS_DECL ScreenObserver :public IObserver
    {
    public:
	    virtual ~ScreenObserver(void){}

	    virtual void freshScreen(ScreenImageData* pScreenImage)=0;
    };

}