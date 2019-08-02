#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/CalendarManager.h"
#include "Module/Calendar/CalendarLibrary.h"
#include "Module/Calendar/LoadCalendarCommand.h"
#include "Module/Calendar/CalendarEventActionCommand.h"
#include "Module/Calendar/CalendarUtility.h"

CalendarManager::CalendarManager(const DeviceData* pDevice)
{
	_pDevice  = pDevice;
	_pLibrary = new CalendarLibrary();
}

CalendarManager::~CalendarManager(void)
{
	SAFE_DELETE(_pLibrary);
}

void ND91Assistant::CalendarManager::Load()
{
	SAFE_DELETE(_pLibrary);
	_pLibrary = new CalendarLibrary();
	LoadCalendarCommand cmd(_pDevice, _pLibrary);
	cmd.Execute();
	_pLibrary->InitAllEvents();
}

const deque<CalendarData*> ND91Assistant::CalendarManager::GetCalendars()
{
	return _pLibrary->GetCalendars();
}

deque<CalendarEventData*> ND91Assistant::CalendarManager::GetEvents( ViewType type, int calendarId, deque<__int64> timelist )
{
	deque<CalendarEventData*> eventlist;
	_pLibrary->GetEventList(type, calendarId, timelist, eventlist);
	return eventlist;
}


bool ND91Assistant::CalendarManager::AddEvent(CalendarEventData* pEvent)
{
	CalendarEventActionCommand cmd(_pDevice);
	if(cmd.AddEvent(pEvent))
	{
		_pLibrary->InsertCalendarEvent(pEvent->Clone());
		return true;
	}
	return false;
}


bool ND91Assistant::CalendarManager::DeleteEvent( CalendarEventData* pEvent, AffectState state )
{
	bool result = false;
	RETURN_FALSE_IF(!pEvent || !_pLibrary->ContainsEvent(pEvent->_rowid));
	if(pEvent->_repeatType == None)//默认无重复事件，删除所有
	{
		return DeleteAll(pEvent);
	}
	switch(state)
	{
		case ENone:
			break;
		case EOnlyInstance:
			{
				// TODO:
				return DeleteOnlyInstance(pEvent);
			}
			break;
		case EAllEvents:
			{
				return DeleteAll(pEvent);
			}
			break;
		case EFollowingInstance:
			{
				return DeleteFollowInstance(pEvent);
			}
			break;
		default:
			break;
	}
	return result;
}

bool ND91Assistant::CalendarManager::EditEvent( CalendarEventData* pEvent,CalendarEventData* pOldEvent, AffectState state, bool IsLenove)
{
	RETURN_FALSE_IF(!pEvent);
	bool result = false;
	CalendarEventActionCommand cmd(_pDevice);
	if(pEvent->_repeatType == None) // 默认无重复事件，修改所有
	{
		result = cmd.EditEvent(pEvent, 2);
		RETURN_FALSE_IF(!result);
		_pLibrary->RemoveEvent(cmd.GetRetId());
		pEvent->_rowid = cmd.GetRetId();
		_pLibrary->InsertCalendarEvent(pEvent->Clone());
		return result;
	}

	switch(state)
	{
		case EOnlyInstance:
			{
				return EditOnlyInstance(pEvent, pOldEvent, IsLenove);
			}
			break;
		case EAllEvents:
			{
				return EditAllInstances(pEvent, pOldEvent, IsLenove);
			}
			break;
		case EFollowingInstance:
			{
				return EditFollowInstances(pEvent, pOldEvent, IsLenove);
			}
			break;
		default:
			break;
	}
	return result;
}

bool ND91Assistant::CalendarManager::DeleteAll( CalendarEventData* pEvent )
{
	CalendarEventActionCommand cmd(_pDevice);
	bool result = cmd.DeleteEvent(pEvent, 2);
	if(result)
	{
		_pLibrary->RemoveEvent(pEvent->_rowid);
	}
	return result;
}

bool ND91Assistant::CalendarManager::DeleteOnlyInstance( CalendarEventData* pEvent )
{
	CalendarEventActionCommand cmd(_pDevice);
	bool result = cmd.DeleteEvent(pEvent, 0);
	RETURN_FALSE_IF(!result);

	CalendarEventData* oldEvent = _pLibrary->ContainsEvent(pEvent->_rowid);
	if (!oldEvent)
		return result;

	if(pEvent->_phoneCategory != L"lenovo")//非联想手机加入cancel队列
	{
		CalendarEventData* delEvent = pEvent->Clone();
		delEvent->_rowid = cmd.GetRetId();
		delEvent->_eventStatus = 2;
		delEvent->_originalEvent = CStrOperation::IntToWString(pEvent->_rowid);
		oldEvent->_cancelEventList[delEvent->_rowid] = delEvent;
	}
	else
	{
		CDealTime stTime(oldEvent->_start-8*3600);
		wstring addexdate = CStrOperation::ReplaceW(CCodeOperation::UTF_8ToUnicode(stTime.TimeFormat()), L" ", L"T");
		addexdate = CStrOperation::ReplaceW(CStrOperation::ReplaceW(addexdate, L"-", L""),L":", L"")+L"Z";
		if(!oldEvent->_exdate.empty())
		{
			oldEvent->_exdate = CStrOperation::trimRightW(oldEvent->_exdate,L",") + L","+addexdate;
		}
		else
			oldEvent->_exdate = addexdate;
	}
	return result;
}

bool ND91Assistant::CalendarManager::DeleteFollowInstance( CalendarEventData* pEvent )
{
	CalendarEventData* startEvent = _pLibrary->ContainsEvent(pEvent->_rowid);
	if(startEvent && startEvent->_start == pEvent->_start)
	{
		return DeleteAll(pEvent);
	}

	CalendarEventActionCommand cmd(_pDevice);
	bool result = cmd.DeleteEvent(pEvent, 1);
	RETURN_FALSE_IF(!result);

	if (!startEvent)
		return result;
	string repeatruleStr = CalendarUtility::ModifyRepeatRule(pEvent->_start, pEvent->_repeatRlue);
	CalendarUtility::SplitRepeatrule(startEvent->_repeatRlue, repeatruleStr);

	wstring s = pEvent->_repeatRlue._until;
	startEvent->_endRepeatDate = CDealTime::StringToTime(s);
	if (pEvent->_start != startEvent->_start)
	{
		startEvent->_repeatForever = false;
	}
	else
	{
		_pLibrary->RemoveEvent(startEvent->_rowid);
	}
	return result;
}

bool ND91Assistant::CalendarManager::EditAllInstances( CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove )
{
	bool result = false;
	CalendarEventActionCommand cmd(_pDevice);
	CalendarEventData* orginEvent = _pLibrary->ContainsEvent(pOldEvent->_rowid);
	
	if (orginEvent->_repeatType != None && orginEvent->_allDay 
		&& pEvent->_repeatType != None && pEvent->_allDay && !IsLenove)
	{
		// 非联想手机全天事件
		CalendarEventData* eventallday = pEvent->Clone();
		eventallday->_start -= 8*3600;
		eventallday->_end -= 8*3600;
		result = cmd.EditEvent(eventallday, 2);
		SAFE_DELETE(eventallday);
	}
	else
	{
		result = cmd.EditEvent(pEvent, 2);
	}
	int alldays = 0;
	if (orginEvent)
		alldays = CDealTime(orginEvent->_start).DaysToTime(CDealTime(pEvent->_start));
	pEvent->_start -= alldays*24*3600;
	pEvent->_end -= alldays*24*3600;

	if(result)
	{
		_pLibrary->RemoveEvent(cmd.GetRetId());
		pEvent->_rowid = cmd.GetRetId();
		_pLibrary->InsertCalendarEvent(pEvent->Clone());
	}
	return result;
}

bool ND91Assistant::CalendarManager::EditOnlyInstance( CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove )
{
	// 仅编辑当前事件
	CalendarEventActionCommand cmd(_pDevice);
	bool result = cmd.EditEvent(pEvent, 1);
	RETURN_FALSE_IF(!result);

	// 创建编辑后事件
	int old = pOldEvent->_rowid;
	CalendarEventData* editonly = pEvent->Clone();
	editonly->_rowid = cmd.GetRetId();
	editonly->_origEventId = old;
	editonly->_repeatType = None;
	editonly->_originalInstanceTime = CDealTime(pOldEvent->_start).GetTime();
	editonly->_lastDate = CDealTime(pOldEvent->_end).GetTime();
	CalendarEventData* orignEvent = _pLibrary->ContainsEvent(old);
	if(orignEvent)
	{
		if(!IsLenove)//非联想手机加入原始事件的更新队列
			orignEvent->_updateEventList[editonly->_rowid] = editonly;
		else
		{
			// 联想手机修改exdate值
			wstring addexdate = CCodeOperation::UTF_8ToUnicode(CalendarUtility::ConvertTimeRule(pEvent->_start-8*3600));
			if(!pOldEvent->_exdate.empty())
				orignEvent->_exdate = CStrOperation::trimRightW(pOldEvent->_exdate,L",")+L","+addexdate;
			else
				orignEvent->_exdate = addexdate;
		}
	}
	//加入事件列表
	if(!_pLibrary->ContainsEvent(cmd.GetRetId()))
	{
		_pLibrary->InsertCalendarEvent(editonly);
	}
	else
		SAFE_DELETE(editonly);

	return result;
}

bool ND91Assistant::CalendarManager::EditFollowInstances( CalendarEventData* pEvent, CalendarEventData* pOldEvent, bool IsLenove )
{
	CalendarEventActionCommand cmd(_pDevice);
	bool result = cmd.EditEvent(pEvent, 3);
	RETURN_FALSE_IF(!result);

	CalendarEventData* orginEvent =_pLibrary->ContainsEvent(pOldEvent->_rowid);
	if(orginEvent)
	{
		if(IsLenove)// 联想手机设置重复规则
		{
			__int64 lenovo_instanceTime =  CalendarUtility::FollowingInstanceEditUntilDate(orginEvent);
			string repeatruleStr = CalendarUtility::ModifyRepeatRule(lenovo_instanceTime, pEvent->_repeatRlue);
			CalendarUtility::SplitRepeatrule(pEvent->_repeatRlue, repeatruleStr);
			pEvent->_repeatRlue._strByDay = pEvent->_repeatRlue._byDays.ByDays();
		}

		if(cmd.GetRetId() == pOldEvent->_rowid)
		{
			_pLibrary->RemoveEvent(pOldEvent->_rowid);// id等于原始id则移除该事件
			pOldEvent = NULL;
		}
		else
		{
			// 修改原始事件重复规则
			orginEvent->_repeatForever = false;
			string repeatruleStr = CalendarUtility::ModifyRepeatRule(pEvent->_start, pEvent->_repeatRlue);
			CalendarUtility::SplitRepeatrule(orginEvent->_repeatRlue, repeatruleStr);
		}
	}
	/*if(pOldEvent)
		DeleteFollowInstance(pOldEvent);*/
	if(!_pLibrary->ContainsEvent(cmd.GetRetId()))
	{
		pEvent->_rowid = cmd.GetRetId();
		_pLibrary->InsertCalendarEvent(pEvent->Clone());
	}
	return result;
}

#endif