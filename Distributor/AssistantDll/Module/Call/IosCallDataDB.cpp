#include "pch_module.h"

#ifdef MODULE_CALL

#include "IosCallDataDB.h"

IosCallDataDB::IosCallDataDB(IosDBPath iosDBPath)
: IosDBBase(iosDBPath)
{
}


IosCallDataDB::~IosCallDataDB(void)
{
}

bool IosCallDataDB::GetCalls(vector<CallData*> *callDataVec)
{
    CppSQLite3Buffer sql;
    sql.Format("select * from [call];");
    CppSQLite3Query q = ExecQuery((const char*)sql);

    while (!q.Eof())
    {
        const char* iosData[6] = {0};
        for (int i = 0; i < 6; i++)
            iosData[i] = q.GetStringField(i);

        CallData* pApp = new CallData(iosData);
        callDataVec->push_back(pApp);

        q.NextRow();
    }
    return true;
}

bool IosCallDataDB::GetCallsSize(int* size)
{
    CppSQLite3Buffer sql;
    sql.Format("SELECT count(*) as CallNums from call;");
    int count = ExecuteAndGetId((const char*)sql);
    if (count == -1)
        *size = 0;
    else
        *size = count;

    return true;
}

int IosCallDataDB::AddCallData(const vector<const CallData*> *callDataVec,  ProgressSubject* pProgressSubject)
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    size_t i = 0;
    for (i = 0; i < callDataVec->size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)
        const CallData* pCall = callDataVec->at(i);

        CppSQLite3Buffer sql;
        sql.Format("insert into call (address,date,duration,flags,id) values('%s','%d','%d','%d','%d');",
                                                pCall->_phoneNumber.c_str(),
                                                (int)(pCall->_date),
                                                (int)(pCall->_duration),
                                                GetFlags(pCall),//(int)(pCall->_flags),
                                                pCall->_contactPersonId);
        ExecDML((const char*)sql);

        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
        pProgressSubject->SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(pCall->_phoneNumber));

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    return (int)i;
}

int IosCallDataDB::GetFlags(const CallData* pData)
{
    if (pData->_flags == CallData::INCOMING || pData->_flags == CallData::MISSED || pData->_flags == CallData::UNKNOW)
        return 4;
    else
        return 5;
}

int IosCallDataDB::DeleteCall(vector<int> rowidVec,ProgressSubject * pProgressSubject)
{
    // 判断用户是否取消，若取消则立即返回
    ProgressObserver* pObserver = NULL;
    if (!pProgressSubject->_observers.empty())
        pObserver = dynamic_cast<ProgressObserver*>(pProgressSubject->_observers[0]);

    size_t i = 0;
    for (i = 0; i < rowidVec.size(); i++)
    {
        IF_CANCEL_BREAK(pObserver)

        string idstr = "";
        CppSQLite3Buffer id;
        id.Format("%d", rowidVec[i]);
        idstr = id;

        CppSQLite3Buffer sql;
        sql.Format("Delete From [call] where rowid = '%s';", idstr.c_str());
        ExecDML((const char*)sql);

        pProgressSubject->IncreaseTaskNumberComplete();
        pProgressSubject->SendProgress();

        pProgressSubject->SetCurrentTaskSum(1);
        pProgressSubject->SetCurrentTaskName(CCodeOperation::UTF_8ToUnicode(idstr));

        pProgressSubject->AddTaskProgress(1);
        pProgressSubject->SendProgress();
    }
    return (int)i;

/*
    string idstr = GetRowidStr(rowidVec);
    CppSQLite3Buffer sql;
    sql.Format("Delete From [call] where rowid in %s;", idstr.c_str());
    return ExecDML((const char*)sql) > 0 ? true : false;
*/
}



#endif