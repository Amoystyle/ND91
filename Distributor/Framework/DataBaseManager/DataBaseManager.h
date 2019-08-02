#pragma once

#ifdef DATABASEMANAGER_EXPORTS
#define DATABASEMANAGER_API __declspec(dllexport)
#else
#define DATABASEMANAGER_API __declspec(dllimport)
#endif

class DataBaseManager
{
private:
	DataBaseManager();
	~DataBaseManager();

	//Р§зг
	static bool ExecuteSQl(string sql)
	{
		//DaseBase::GetInstance()->ExecuteSQl(sql);
	}

};
