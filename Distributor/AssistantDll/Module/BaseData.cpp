#include "pch_module.h"

#include "Module/BaseData.h"

BaseData::BaseData()
{
    _status = NO_CHANGE;
}

BaseData::~BaseData()
{

}

void BaseData::SetStatus( DATA_STATUS s )
{
    _status = s;
}

BaseData::DATA_STATUS BaseData::GetStatus() const
{
    return _status;
}