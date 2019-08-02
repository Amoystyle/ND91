#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    // 铃声管理的目录
    class ThemePath
	{ 
	public:
        ThemePath(const wstring workFolder);

        wstring GetPath() const;          // 主题模块的工作目录
        wstring GetThemeTempPath() const;      // 主题模块存放临时文件
        wstring GetPreviewPath() const;   // 主题预览用
        wstring GetAdpPath() const;       // 添加apt主题时安装包备份到本地的路径
        wstring GetUploadPath() const;     // 

    private:
        const wstring _workFolder;        // 临时工作目录
    };
}
