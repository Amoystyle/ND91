#ifndef _DIRECTUI_H
#define _DIRECTUI_H


#ifndef DIRECTUI_STATICLIB
#ifdef DIRECTUI_EXPORTS
#define DUI_API __declspec(dllexport)
#else
#define DUI_API __declspec(dllimport)
#endif
#else
#define DUI_API 
#endif

#define _GDIPLUS_SUPPORT // «∑Ò÷ß≥÷GDI+

#include <tchar.h>
#include <string>
#include <vector>
#include <map>
using namespace std;
#include <comdef.h>

class CWindowUI;
#include <windows.h>
#include "UIDef.h"
#include "Utility.h"
#include "ImageLib.h"
#include "DataObjectImpl.h"
#include "ImageObj.h"
#include "FontObj.h"
#include "RenderObj.h"
#include "StyleObj.h"
#include "Animation.h"
#include "UIControl.h"
#include "UIScrollBar.h"
#include "UIContainer.h"
#include "UILayout.h"
#include "WindowBase.h"
#include "UIWindow.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UIRichEdit.h"

#ifndef DIRECTUI_LAYOUT_RTL
#include "UIEdit.h"
#else
#include "UIEdit_AR.h"
#endif

#include "UIList.h"
#include "UISearchEdit.h"
#include "UIComboBox.h"
#include "UIMessageBox.h"
#include "UIObject.h"
#include "UIAnimation.h"
#include "UIRadio.h"
#include "UISlider.h"
#include "UISplitter.h"
#include "UITab.h"
#include "UITree.h"
#include "UIMenu.h"
#include "UIActiveX.h"
#include "UIIE.h"
#include "UIProgressBar.h"
#include "UIComposite.h"
#include "UIControlWnd.h"
#include "UIToolTip.h"
#include "UIToolbar.h"
#include "UIHtmlLabel.h"
#endif