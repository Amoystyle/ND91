#pragma once

#ifdef COMMONUI_EXPORTS
#define COMMONUI_API __declspec(dllexport)
#else
#define COMMONUI_API __declspec(dllimport)
#endif

#include "UI/FolderBrowser.h"
#include "UI/BaseView.h"

#include "Common/MessageDefine.h"
#include "Common/CommonUIMsg.h"
#include "UI/CommonUIFunc.h"

#include "UI/AnimationRender.h"
#include "UI/InputDlg.h"
#include "UI/MessageBoxUI.h"
#include "UI/MessageWnd.h"
#include "UI/FileExistConfirmWnd.h"
#include "UI/InputDlg.h"
#include "UI/TabItem.h"
#include "UI/CCompositeTabUI.h"
#include "UI/AutoGridLayoutUI.h"
#include "UI/AutoShowTipLabelUI.h"
#include "UI/SimpleListItem.h"
#include "UI/ToggleLayoutUI.h"
#include "UI/WebIEUI.h"
#include "UI/BaseOperation.h"
#include "UI/BigListComboUI.h"
#include "UI/ProgressRecvMsg.h"
#include "UI/DragDropMessageReceiver.h"
