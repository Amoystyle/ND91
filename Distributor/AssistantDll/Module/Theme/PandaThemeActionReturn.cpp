#include "pch_module.h"

#ifdef MODULE_THEME

#include "PandaThemeActionReturn.h"

PandaThemeActionReturn::PandaThemeActionReturn()
{
}

ND91Assistant::PandaThemeActionReturn::PandaThemeActionReturn( const PandaThemeActionReturn* pData )
{
    _nAction    = pData->_nAction;
    _nResult     = pData->_nResult;
    _strValue   = pData->_strValue;
}

PandaThemeActionReturn::~PandaThemeActionReturn()
{
}

PandaThemeActionReturn* ND91Assistant::PandaThemeActionReturn::FromDeamonNotifyBuffer( BinaryBufferReader* pReader )
{
    PandaThemeActionReturn* pPandaThemeActionReturn = new PandaThemeActionReturn();
    pReader->ReadInt32(pPandaThemeActionReturn->_nAction);
    pReader->ReadInt32(pPandaThemeActionReturn->_nResult);
    pReader->ReadStringFromUTF8(pPandaThemeActionReturn->_strValue);
    return pPandaThemeActionReturn;
}


#endif