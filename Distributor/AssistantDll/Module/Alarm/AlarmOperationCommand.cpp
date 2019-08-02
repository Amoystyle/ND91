#include "pch_module.h"

#ifdef MODULE_ALARM

#include "AlarmOperationCommand.h"
#include "Module/AndroidDefine.h"

AlarmOperationCommand::AlarmOperationCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidChannelID = ALARM;
}

AlarmOperationCommand::~AlarmOperationCommand(void)
{
}

void ND91Assistant::AlarmOperationCommand::Execute()
{
	if(_pDevice->GetDevInfo()->_deviceType == Ios)
		return;
	_success = false;
	DefaultExecute();
}

std::string ND91Assistant::AlarmOperationCommand::GetAndroidBodyData()
{
	BinaryBufferWriter bufwriter;
	bufwriter.WriteInt(_nAndroidChannelID);
	bufwriter.WriteInt(_type);
	switch(_type)
	{
		case Load_Alarms :
			break;
		case Add_Alarm	 :
			_srcAlarm->AndroidDataToStream(bufwriter);
			break;
		case Delete_Alarm:
			bufwriter.WriteInt(_srcAlarm->Id());
			break;
		case Update_Alarm:
			_srcAlarm->AndroidDataToStream(bufwriter);
			break;
		default:
			break;
	}
	return bufwriter.GetBuffer();
}

void ND91Assistant::AlarmOperationCommand::SetAndroidReturnData( std::string data )
{
	BinaryBufferReader reader(data.c_str(), data.length());
	if (!parseAndroidPackageHead_Top16(&reader))
    {
        _result = COMMAND_EXECUTE_FAIL;
        return;
    }
	reader.ReadInt32(_nAndroidCommandID);
	switch(_type)
	{
		case Load_Alarms :
			{
				int count = 0;
				reader.ReadInt32(count);
				for (int i = 0; i < count; i++)
				{
					AlarmData* alarm = new AlarmData();
					alarm->ParseAndroidData(reader);
					_pAlarms->push_back(alarm);
				}
			}
			break;
		case Add_Alarm	 :
			{
				reader.ReadBool(_success);
				if(_success)
				{
					reader.ReadInt32(_srcAlarm->_id);
				}
			}
			break;
		case Delete_Alarm:
		case Update_Alarm:
			{
				reader.ReadBool(_success);
			}
			break;
		default:
			break;
	}
}

void ND91Assistant::AlarmOperationCommand::LoadAlarms( deque<AlarmData*>* pAlarms )
{
    _result = COMMAND_EXECUTE_SUCCESS;
	_type = Load_Alarms;
	_pAlarms = pAlarms;
	_pAlarms->clear();
	Execute();
}

bool ND91Assistant::AlarmOperationCommand::AddAlarm( AlarmData* alarm )
{
	RETURN_FALSE_IF(!alarm);
	_type = Add_Alarm;
	_srcAlarm = alarm;
	Execute();
	return _success;
}

bool ND91Assistant::AlarmOperationCommand::DeleteAlarm( AlarmData* alarm )
{
	RETURN_FALSE_IF(!alarm);
	_type = Delete_Alarm;
	_srcAlarm = alarm;
	Execute();
	return _success;
}

bool ND91Assistant::AlarmOperationCommand::UpdateAlarm( AlarmData* alarm )
{
	RETURN_FALSE_IF(!alarm);
	_type = Update_Alarm;
	_srcAlarm = alarm;
	Execute();
	return _success;
}

#endif