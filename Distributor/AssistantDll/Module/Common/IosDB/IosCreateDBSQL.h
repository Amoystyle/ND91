#pragma once

/*********************************************************************
根据IOS设备的版本不同获取创建数据库的SQL语句
*********************************************************************/
#include <string>
using namespace std;

class IosCreateDBSQL
{
public:
    IosCreateDBSQL(void);
    ~IosCreateDBSQL(void);

public:
    // 创建通话记录数据库语句
    static string CreateCallDataDBSQL();

	// 创建书签数据库sql
	static string CreateBookMarkDBSQL();

    // 创建短信数据库语句
    static string CreateMessageDBSQL(int iProductVersion);

    // 创建联络人数据库语句
    static string CreateContactDBSQL(int iProductVersion);

    // 创建联络人图片数据库语句
    static string CreateContactImageDBSQL(int iProductVersion);

    // 创建日历数据库语句
    static string CreateCalenderDBSQL(int iProductVersion);

    // 删除短信数据库触发器语句
    static string DropSmsTriggersSQL();    

    // 创建短信数据库触发器语句
    static string CreateSmsTriggersSQL();

    // 安装排序补丁语句(1X版本)
    static string InstallPatchSQL_1X();

    // 安装排序补丁语句(2X和3X版本)
    static string InstallPatchSQL_2X_3X();

    // 安装排序补丁语句(4X版本)
    static string InstallPatchSQL_4X();

    // 卸载排序补丁语句(1X版本)
    static string UnInstallPatchSQL_1X();

    // 卸载排序补丁语句(2X,3X,4X版本)
    static string UnInstallPatchSQL_2X_3X_4X();
    
private:
    // 创建联络人数据库1X版本语句
    static string CreateContact1XDBSQL();

    // 创建联络人数据库2X版本语句
    static string CreateContact2XDBSQL();

    // 创建联络人数据库3X版本语句
    static string CreateContact3XDBSQL();

    // 创建联络人数据库4X版本语句
    static string CreateContact4XDBSQL();

    // 创建联络人数据库4.3.X版本语句
    static string CreateContact43XDBSQL();

    // 创建联络人图片数据库1X版本语句
    static string CreateContactImage1XDBSQL();

    // 创建联络人图片数据库2X版本语句
    static string CreateContactImage2XDBSQL();

    // 创建联络人图片数据库3X版本语句
    static string CreateContactImage3XDBSQL();

    // 创建联络人图片数据库4X版本语句
    static string CreateContactImage4XDBSQL();

    // 创建日历数据库1X版本语句
    static string CreateCalender1XDBSQL();

    // 创建日历数据库2X版本语句
    static string CreateCalender2XDBSQL();

    // 创建日历数据库3X版本语句
    static string CreateCalender3XDBSQL();

};
