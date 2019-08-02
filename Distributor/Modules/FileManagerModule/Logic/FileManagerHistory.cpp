#include "StdAfx.h"
#include "FileManagerHistory.h"


FileManagerHistory::FileManagerHistory( unsigned int maxCount ) : _maxCount(maxCount)
{
    _current = _history.end();
    _enableGoto  = true;
}

void FileManagerHistory::Goto( wstring path )
{
    RETURN_IF( ! _enableGoto );
    RETURN_IF( path.empty() );

	list<wstring>::const_iterator     current_temp = _current;

    if ( ! _history.empty() && _current != --_history.end() )
    {
		current_temp++;
        _history.erase(current_temp, _history.end());
    }

    if ( _history.empty() || *_current != path )
    {
        _history.push_back(path);
        _current = --_history.end();
    }
}

std::wstring FileManagerHistory::Prev()
{
    if ( _current == _history.begin() )
        return L"";

    _current--;
    return *_current;
}

std::wstring FileManagerHistory::Next()
{
	if(_current==_history.end())
		return L"";

    _current++;

    if ( _current == _history.end() )
        return L"";

    return *_current;
}

std::wstring FileManagerHistory::GetCurrent()
{
    if ( _current == _history.end() )
        return L"";
    return *_current;
}

bool FileManagerHistory::IsHead()
{
    return _current == _history.begin();
}

bool FileManagerHistory::IsTail()
{
	if (_history.empty())
	{
		return false;
	}

	list<wstring>::const_iterator buf = (--_history.end());
    return _current == buf;
}

void FileManagerHistory::DeleteCurrent()
{
    RETURN_IF(_history.empty());

    _current = _history.erase(_current);

}

void FileManagerHistory::EnableGoto( bool val )
{
	_enableGoto = val;
}
