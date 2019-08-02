#include "pch_module.h"

#ifdef MODULE_CALENDAR

#include "Module/Calendar/CalendarEventActionCommand.h"
#include "Module/Calendar/CalendarStreamHelper.h"
#include "Module/Calendar/CalendarEventData.h"
#include "Module/AndroidDefine.h"

CalendarEventActionCommand::CalendarEventActionCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = CALENDAR;
	_pEvent = NULL;
}

CalendarEventActionCommand::~CalendarEventActionCommand(void)
{
}

void ND91Assistant::CalendarEventActionCommand::Execute()
{
	_success = false;
	_retValue = 0;
	DefaultExecute();
}

std::string ND91Assistant::CalendarEventActionCommand::GetAndroidBodyData()
{
	BinaryBufferWriter bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteChar(_type);
	switch(_type)
	{
		case EAddEvent:
			CalendarStreamHelper::CalendarEventToStream(bufwriter, _pEvent);
			break;
		case EEditEvent:
			{
				CalendarStreamHelper::CalendarEventToStream(bufwriter, _pEvent);
				bufwriter.WriteInt(_modifyType);
			}
			break;
		case EDeleteEvent:
			{
				bufwriter.WriteInt64(_pEvent->_start*1000);
				bufwriter.WriteInt64(_pEvent->_end*1000);
				bufwriter.WriteInt(_pEvent->_rowid);
				bufwriter.WriteInt(_modifyType);
				bufwriter.WriteBool(_modifyType==0?false:true);
				bufwriter.WriteStringAsUTF8(_pEvent->_phoneCategory);
				__int64 start=_pEvent->_start-8*3600;
				CDealTime time(start);
				string exdate = CStrOperation::Replace(time.TimeFormat()," ","T");
				exdate = CStrOperation::Replace(CStrOperation::Replace(exdate,"-",""),":","")+"Z";
				bufwriter.WriteString(exdate);
			}
			break;
		case EGetEventCount:
			break;
		default:
			break;
	}
	return bufwriter.GetBuffer();
}

void ND91Assistant::CalendarEventActionCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if(!parseAndroidPackageHead_Top16(&reader))
		return;
	if(reader.IsEnd())
		return;
	reader.ReadInt32(_retValue);
	if (_retValue != -1 && _retValue != 0)
		_success = true;
}

bool ND91Assistant::CalendarEventActionCommand::AddEvent( CalendarEventData* pEvent )
{
    _type = EAddEvent;
    _pEvent = pEvent;
    Execute();
    pEvent->_rowid = _retValue;
    return _success;
}

bool ND91Assistant::CalendarEventActionCommand::DeleteEvent( CalendarEventData* pEvent, int deleteType)
{
	_type = EDeleteEvent;
	_modifyType = deleteType;
	_pEvent = pEvent;
	Execute();
	return _success;
}

bool ND91Assistant::CalendarEventActionCommand::EditEvent( CalendarEventData* pEvent, int modifyType )
{
	_type = EEditEvent;
	_pEvent = pEvent;
	_modifyType = modifyType;
	Execute();
	return _success;
}

int ND91Assistant::CalendarEventActionCommand::GetEventCount()
{
	_type = EGetEventCount;
	Execute();
	return _retValue;
}


#endif