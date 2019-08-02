#include "stdafx.h"
#include "DataBase.h"
#include "SQLLiteDataBase/SQLLiteDataBase.h"

DataBase* DataBase::_pInstance = NULL;

DataBase::DataBase()
{

}

DataBase::~DataBase()
{

}

DataBase* DataBase::GetInstance()
{
	if(_pInstance == NULL)
	{
		//Dabbler Check
		//Lock
		if(_pInstance == NULL)
		{
			_pInstance = new SQLLiteDataBase();
		}
	}
	return _pInstance;
}
