#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Extention/WebPage.h"
#include "Module/Contact/vCard/Implementation/vItem.h"
#include "Module/Contact/vCard/vCard.h"

wstring WebPage::WebPage_WORK   = L"WORK";
wstring WebPage::WebPage_HOME   = L"HOME";
wstring WebPage::WebPage_OTHER  = L"OTHER";

WebPage::WebPage()
{
}

WebPage::~WebPage()
{
}

void WebPage::Init(Item* pItem)
{
    CustomItem::Init(pItem);
}


//#endif