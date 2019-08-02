#pragma once
#include <stdint.h>
#include <time.h>
#include <string>
#include <vector>
#include "Common/CodeOperation.h"
using namespace std;
#ifdef WIN32

#else
    #include <sys/time.h>
    #include <unistd.h>
	#define _atoi64 atol
#endif

namespace ND91Assistant
{
    // 数据类型接口类: 每个表的数据类型需要继承此基类
    class IDataRow
    {
    public:
        IDataRow();
        virtual ~IDataRow();

        // Summary  : 纯虚函数: 将数据类的值转换为字符串数组
        // Returns  : pVector的地址
        virtual const vector<wstring>* const GetValues() = 0;

        // Summary  : 纯虚函数: 按照PK生成where条件
        // 实现注意: 若有文本类型, 需要用WstrToStr处理过
        // Returns  : 字符串
        virtual wstring PrimaryKeyCondition() const = 0;

    protected:
        // 工具方法: time_t 转 显示用字符串
        static wstring timeToStr(time_t t);

        // PC时间从1970.1.1开始, MAC时间从2001.1.1开始, 相差31年
        #define SECOND_OF_31_YEAR 978278400

        // 工具方法: mac 时间 转 pc 时间
        static time_t MacToPc(time_t t);

        // 工具方法: pc 时间 转 mac 时间
        static time_t PcToMac(time_t t);

        // 工具方法: int64 转 string
        static wstring Int64ToStr(/*__int64*/int64_t i);

        // 工具方法: int32 转 string
        static wstring Int32ToStr(/*__int32*/int32_t i);

        // 工具方法: 处理 wstring
        static wstring ConvertStr(wstring ws);
		
		//二进制数据读取
		static wstring BinToStr( string s);

        vector<wstring>* GetVector();

    private:
        vector<wstring>* _pVector;    // GetValues中使用
    };
}
