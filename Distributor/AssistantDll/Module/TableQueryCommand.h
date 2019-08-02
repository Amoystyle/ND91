#pragma once
#include "Module/BaseCommand.h"
#include <deque>

using namespace std;

namespace ND91Assistant
{
// 数据库表格式数据获取命令基类
class TableQueryCommand :public BaseCommand
{
public:
	TableQueryCommand(const DeviceData* pDevice);
	virtual ~TableQueryCommand(void);

    // Summary  : 设置命令参数
    // Returns	: void
    // Parameter: string tablename 表名
    // Parameter: const char * * columns各列名
    // Parameter: string condition 查询条件
    // Parameter: string sort 返回结果按什么排序
    void SetParam(string tablename, const string columns, string condition, string sort);

	// 执行命令
	virtual void Execute();

protected:

	// 解析Android返回的数据包头 -- 20个字节
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

	// 命令传送到IOS设备上的文件名（全路径, unicode）
	virtual std::wstring GetIosCommandFilePath() {return L""; }

	// 从IOS上获取的命令结果文件名（全路径, unicode）
	virtual std::wstring GetIosResultFilePath(){return L"";}

	// 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename){}

private:
	string			_tableName;		// 表名
	string			_condition;		// 查询条件
	string			_sort;			// 结果按什么排序
	vector<string>	_tablecolumns;
};

}
