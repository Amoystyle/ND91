#pragma once

#include <map>
#include <string>
#include <stdint.h>
#include "AssistantDll.h"
#include "Common/SqlAccess/SqliteAccess.h"
#include "Common/SqlAccess/IDataRow.h"
using namespace std;
namespace ND91Assistant
{
    // �������, ʹ�ü̳�IDataRow����������Ϊģ��
    template<class T>
    class BaseTable : public ISelect
    {
    public:
        BaseTable(SqliteAccess* pDB): _pDB(pDB) , _tableName(T::GetTableName())
        {
            T::GetColumns(_columns);
        }
        virtual ~BaseTable(void) { Clear(); }

        virtual void AddRow(const vector<const char*>* pValues)
        {
            _pDataSet.push_back(T::FromValue(pValues));
        }

#pragma region select��ط���

        void SelectAll()
        {
            _pDataSet.clear();
            _pDB->Select(this, _tableName, L"", &_columns);
        }

        void Select(wstring Condition)
        {
            _pDataSet.clear();
			int i = _pDataSet.size();
            _pDB->Select(this, _tableName, Condition, &_columns);
        }
		//����64λ��id��select
		void Select(/*__int64*/int64_t pid,wstring keyname)
		{
			_pDataSet.clear();
			int i = _pDataSet.size();
			wostringstream Condition;
			Condition << keyname << "=" << pid;
			_pDB->Select(this, _tableName, Condition.str(), &_columns);
		}
		//����64λpid�����ͬpid�ļ�¼����
		int Count(/*__int64*/int64_t pid,wstring keyname)
		{
			wostringstream condition;
			condition << keyname << "=" << pid;
			return _pDB->getCount(_tableName,condition.str());
		}
		int64_t MaxPid(wstring keyname)
		{
			return _pDB->SelectMaxPid(_tableName,keyname);
		}
        // ȡ�洢������(ÿ��select���ı�)
        int GetRowCount()
        {
			int i = _pDataSet.size();

            return _pDataSet.size();
        }

        T* GetDataRow(int i)
        {
            return _pDataSet[i];
        }

        void Clear()
        {
            RELEASE_VECTOR(_pDataSet);
        }
#pragma endregion

#pragma region 3��delete����
		//����64λpidɾ��һ����¼��ֻ�ʺϲ����Ե�һ64λpid��ΪPrimaryKey�ı�
		int Delete(/*__int64*/int64_t pid,wstring keyname)
		{
			_pDB->Begin();

			wostringstream condition;
			condition << keyname << "=" << pid;
			int ret = _pDB->Delete(_tableName, condition.str());
			_pDB->Commit();

			return ret;
		}

        int Delete(wstring condition)
        {
            _pDB->Begin();
            int ret = _pDB->Delete(_tableName, condition);
            _pDB->Commit();

            return ret;
        }

        int Delete(const vector<T*>* pVector)
        {
            RETURN_VALUE_IF(!pVector, 0);
            RETURN_VALUE_IF(pVector->empty(), 0);

            int count = 0;
            _pDB->Begin();

            for(size_t i = 0; i < pVector->size(); i++)
            {
                IDataRow* pDataRow = pVector->at(i);
                if ( ! pDataRow )
                    continue;

                count += _pDB->Delete(_tableName, pDataRow->PrimaryKeyCondition());
            }
            _pDB->Commit();

            return count;
        }

        int Delete(const T* pDataRow)
        {
            RETURN_VALUE_IF(!pDataRow, 0);

            return BaseTable::Delete(pDataRow->PrimaryKeyCondition());
        }
#pragma endregion

#pragma region 2�� insert ����


        int Insert(/*const */T* pDataRow)
        {
            RETURN_VALUE_IF(!pDataRow, 0);
		
            
            _pDB->Begin();
            int ret = _pDB->Insert(_tableName, &_columns/*GetColumns()*/, pDataRow->GetValues());
            _pDB->Commit();
			return ret;
        }

        int Insert(const vector<T*>* pVector)
        {
            RETURN_VALUE_IF(!pVector, 0);
            RETURN_VALUE_IF(pVector->empty(), 0);

            int count = 0;
            _pDB->Begin();

            for(size_t i = 0; i < pVector->size(); i++)
            {
                IDataRow* pDataRow = pVector->at(i);
                if ( ! pDataRow )
                    continue;

                count += _pDB->Insert(_tableName, &_columns/*GetColumns()*/, pDataRow->GetValues());
            }
            _pDB->Commit();

            return count;
        }
#pragma endregion

#pragma region 2��update����

        int Update(T* pDataRow)
        {
            RETURN_VALUE_IF(!pDataRow, 0);

            _pDB->Begin();
            int ret = _pDB->Update(_tableName, pDataRow->PrimaryKeyCondition(), &_columns, pDataRow->GetValues());
            _pDB->Commit();
			return ret;
        }

        int Update(const vector<T*>* pVector)
        {
            RETURN_VALUE_IF(!pVector, 0);
            RETURN_VALUE_IF(pVector->empty(), 0);

            int count = 0;
            _pDB->Begin();

            for(size_t i = 0; i < pVector->size(); i++)
            {
                IDataRow* pDataRow = pVector->at(i);
                if ( ! pDataRow )
                    continue;

                count += _pDB->Update(_tableName, pDataRow->PrimaryKeyCondition(), &_columns, pDataRow->GetValues());
            }
            _pDB->Commit();

            return count;
        }
#pragma endregion

    protected:
        const wstring   _tableName;
        SqliteAccess*   _pDB;
        vector<T*>      _pDataSet;
        vector<wstring> _columns;
    };
}
