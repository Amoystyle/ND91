#pragma once
#include "Base/DataBase.h"

class SQLLiteDataBase 
	: public DataBase
{
public:
	SQLLiteDataBase();
	~SQLLiteDataBase();

	bool ExecuteSQl(string sql);
};