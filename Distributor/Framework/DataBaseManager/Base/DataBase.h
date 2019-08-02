#pragma once

class DataBase
{
public:
	DataBase();
	virtual ~DataBase();

	//����
	virtual bool ExecuteSQl(string sql) = 0;

	static DataBase* GetInstance();

private:
	static DataBase* _pInstance;
};
