#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    // 铃声管理的目录
    class IosRecordingPath
	{ 
	public:
        IosRecordingPath(const wstring workFolder, bool cracked);

		wstring GetLocalRecordingPath() const;			// 本地缓存所有录音文件总目录
		wstring GetLocalVoiceMemosDBPath() const;		// 本地缓存系统录音数据库路径


		wstring GetIosVoiceMemosDBPath() const;			// 手机录音数据库文件路径

    private:
        const wstring	_workFolder;					// 临时工作目录
		bool			_cracked;						// 是否越狱
    };
}
