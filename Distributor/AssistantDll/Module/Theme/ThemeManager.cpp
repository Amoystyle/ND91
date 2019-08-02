#include "pch_module.h"

#ifdef MODULE_THEME

#include "Module/Theme/ThemeManager.h"

#include "Common/Path.h"

ThemeManager::ThemeManager(const DeviceData* pDevice)
{
	_pDevice = pDevice;

	_strPreviewImageFolder = _pDevice->GetThemePath()->GetPreviewPath()
        + CFileOperation::GetGuidW();

    CFileOperation::DeleteDirectory(_strPreviewImageFolder.c_str());
    CFileOperation::MakeLocalDir(_strPreviewImageFolder.c_str());
}

ThemeManager::~ThemeManager(void)
{
	ClearThemeList();
	CFileOperation::DeleteDirectory(_strPreviewImageFolder);
}

void ThemeManager::ClearThemeList()
{
	for(map<wstring, ThemeData*>::iterator it = _themeList.begin(); it != _themeList.end(); ++it )
	{
		SAFE_DELETE( it->second );
	}

	_themeList.clear();
}

void ThemeManager::AddTheme(wstring id, ThemeData* pThemeData)
{
	_themeList[id] = pThemeData;
}

ThemeData* ThemeManager::FindTheme(wstring id)
{
	map<wstring, ThemeData*>::const_iterator it = _themeList.find(id);

	if ( it != _themeList.end() )
		return it->second;
	else
		return NULL;
}

ThemeData* ThemeManager::FindTheme(ThemeType eType, wstring strThemeName)
{
	for(map<wstring, ThemeData*>::iterator it = _themeList.begin(); it != _themeList.end(); ++it )
	{
		ThemeData* pTemp = it->second;
		if (pTemp && pTemp->_themeType == eType && pTemp->_name == strThemeName)
			return pTemp;
	}
	
	return NULL;
}

deque<ThemeData*> ThemeManager::GetThemeList()
{
	deque<ThemeData*> vtReturn;

	for(map<wstring, ThemeData*>::iterator it = _themeList.begin(); it != _themeList.end(); ++it )
	{
		vtReturn.push_back(it->second);
	}

	return vtReturn;
}

void ND91Assistant::ThemeManager::RemoveTheme( ThemeData* pThemeData )
{
    map<wstring, ThemeData*>::iterator find = _themeList.find(pThemeData->_id);
    if (find != _themeList.end())
    {
        SAFE_DELETE(find->second);
        _themeList.erase(find);
    }
}

void ND91Assistant::ThemeManager::AddTheme( ThemeData* pThemeData )
{
    if (pThemeData && !pThemeData->_id.empty())
        AddTheme(pThemeData->_id, pThemeData);
}


#endif