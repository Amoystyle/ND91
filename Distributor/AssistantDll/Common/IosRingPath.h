#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    // 铃声管理的目录
    class IosRingPath
	{ 
	public:
        IosRingPath(const wstring workFolder, bool cracked);

#pragma region 铃声管理模块本地临时目录

		wstring GetLocalRingListPath() const;     // 本地缓存自定义铃声列表路径
		wstring GetLocalRingSetListPath() const;  // 本地缓存添加自定义铃声设置文件
		wstring GetLocalRingWorkPath() const;

#pragma endregion

#pragma region 铃声管理模块本手机目录

		wstring GetIosRingListPath() const;		// 手机自定义铃声列表文件路径
		wstring GetIosRingSetListPath() const;    // 手机添加自定义铃声设置文件路径
		wstring GetIosUserRingPath() const;		// 手机存放系统铃声文件目录
		wstring GetIosSyetemRingPath() const;     // 手机存放系统铃声文件目录
		wstring GetIosRingLibraryFolder() const;

#pragma endregion

    private:
        const wstring _workFolder;        // 临时工作目录
        bool    _cracked;         // 是否越狱
    };
}
