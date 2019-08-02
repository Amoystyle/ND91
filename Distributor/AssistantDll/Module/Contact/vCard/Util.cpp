#include "pch_module.h"

//#ifdef MODULE_CONTACT

#include "Module/Contact/vCard/Util.h"
#include "Module/Contact/vCard/vCard.h"
#include "Module/Contact/vCard/TypeDefine/DeliveryAddressingTypes.h"
#include "Module/Contact/vCard/TypeDefine/ExplanatoryTypes.h"
#include "Module/Contact/vCard/TypeDefine/GeographicalTypes.h"
#include "Module/Contact/vCard/TypeDefine/IdentificationTypes.h"
#include "Module/Contact/vCard/TypeDefine/OrganizationalTypes.h"
#include "Module/Contact/vCard/TypeDefine/SecurityTypes.h"
#include "Module/Contact/vCard/TypeDefine/TelecommunicationsAddressingTypes.h"

wstring Util::FoldLine(wstring line)
{
    /* Folding rules:
    *) Line may not be bigger than 76 chars.
    *) If possible fold between TAB or SP
    *) If no fold point, just fold from char 76
    */

    // Data line too big, we need to fold data.
    if (!line.empty() && line.length() > 76)
    {
        size_t startPosition = 0;
        size_t lastPossibleFoldPos = -1;
        //StringBuilder retVal = new StringBuilder();
        wstring retVal;
        for (size_t i = 0; i < line.length(); i++)
        {
            wchar_t c = line.at(i);
            // We have possible fold point
            if (c == L' ' || c == L'\t')
                lastPossibleFoldPos = i;

            // End of data reached
            if (i == (line.length() - 1))
                retVal += line.substr(startPosition);
            else if ((i - startPosition) >= 76)// We need to fold
            {
                // There wasn't any good fold point(word is bigger than line), just fold from current position.
                if (lastPossibleFoldPos == -1)
                    lastPossibleFoldPos = i;

                retVal += line.substr(startPosition, lastPossibleFoldPos-startPosition)+L"\r\n\t";

                i = lastPossibleFoldPos;
                lastPossibleFoldPos = -1;
                startPosition = i;
            }
        }

        return retVal;
    }
    else
    {
        return line;
    }
}

time_t Util::PareDateTime(wstring dateTimeStr)
{//19841026 to 1984-10-26 00:00:00
    wstring temp = dateTimeStr;
    wstring year = temp.substr(0, 4);
    wstring month = temp.substr(4, 2);
    wstring date = temp.substr(6, 2);
    wstring timeStr = year+L"-"+month+L"-"+date+L" 00:00:00";
    return CDealTime::StringToTime(timeStr);
}

bool Util::StartLine(wstring line)
{
    RETURN_FALSE_IF(line.empty());

    line = CStrOperation::toUpperW(line);
    RETURN_TRUE_IF(line == vCard::BEGIN);
    RETURN_TRUE_IF(line == vCard::END);

    if (CStrOperation::startsWith(line, vCard::CUSTOM_TAG))
    {
        wstring tmp = line;
        int pIndex = line.find(vCard::Name_Param_Splitor);
        while (pIndex > 0)
        {
            if (tmp[pIndex-1] != L'\\')
                return true;
            else
            {
                if (pIndex+1 > (int)tmp.length())
                    break;

                tmp = tmp.substr(pIndex+1);
                pIndex = tmp.find(vCard::Name_Param_Splitor);
            }
        }

        tmp = line;
        int vIndex = line.find(vCard::Name_Value_Splitor);
        while (vIndex > 0)
        {
            if (tmp[vIndex-1] != L'\\')
                return true;
            else
            {
                if (vIndex+1 > (int)tmp.length())
                    break;

                tmp = tmp.substr(vIndex+1);
                vIndex = tmp.find(vCard::Name_Param_Splitor);
            }
        }
    }

    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::BDAY));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::FN));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::N));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::NICKNAME));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::PHOTO));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::ASSISTANTNAME));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::ASSISTANTTEL));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::CHILDREN));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::SPOUSE));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::SUFFIX));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::MANAGER));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::STATUSID));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::ACCOUNT));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, IdentificationTypes::CUSTOMERID));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, TelecommunicationsAddressingTypes::TEL));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, TelecommunicationsAddressingTypes::EMAIL));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, TelecommunicationsAddressingTypes::MAILER));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, TelecommunicationsAddressingTypes::DATE));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, OrganizationalTypes::AGENT));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, OrganizationalTypes::LOGO));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, OrganizationalTypes::ORG));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, OrganizationalTypes::ROLE));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, OrganizationalTypes::TITLE));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, DeliveryAddressingTypes::ADR));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, DeliveryAddressingTypes::LABEL));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::CATEGORIES));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::NOTE));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::PRODID));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::REV));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::SORT_STRING));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::SOUND));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::UID));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::URL));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, ExplanatoryTypes::VERSION));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, GeographicalTypes::GEO));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, GeographicalTypes::TZ));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, SecurityTypes::CLASS));
    RETURN_TRUE_IF(CStrOperation::startsWith(line, SecurityTypes::KEY));

    return false;
}


//#endif