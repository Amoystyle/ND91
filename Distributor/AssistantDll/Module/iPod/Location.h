#pragma once

#include "Common/SqlAccess/IDataRow.h"
#include "Common/SqlAccess/BaseTable.h"
#include <stdint.h>
namespace ND91Assistant
{
    // Location表 数据类(以D开头)
    class DLocation : public IDataRow
    {
    public:
        /*__int64*/int64_t item_pid;
        /*__int64*/int64_t sub_id;
        /*__int32*/int32_t base_location_id;
        /*__int32*/int32_t location_type;
        wstring location;
        /*__int32*/int32_t extension;
        /*__int32*/int32_t kind_id;
        time_t date_created;
        /*__int32*/int32_t file_size;
        /*__int32*/int32_t file_creator;
        /*__int32*/int32_t file_type;
        /*__int32*/int32_t num_dir_levels_file;
        /*__int32*/int32_t num_dir_levels_lib;
 
        DLocation();
        virtual ~DLocation() {}

        virtual const vector<wstring>* const GetValues();

        virtual wstring PrimaryKeyCondition() const;

        wstring ToString();

#pragma region 必须实现的静态方法, 模板类BaseTable中用到
        static wstring GetTableName();
        static DLocation* FromValue(const vector<const char*>* pValues);
        static void GetColumns(vector<wstring>& vector);
#pragma endregion
    };
}
