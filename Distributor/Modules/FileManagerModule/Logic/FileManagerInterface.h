#pragma once

class FileManagerInterface
{
public:
    // 初始化
    virtual void OnInit(DirData* pRootDir) = 0;

    // 显示指定目录/文件
    virtual bool Goto(wstring path) = 0;

    // 设置导航按钮是否可用
    virtual void EnableButton(bool enablePrevButton, bool enableNextButton, bool enableParentButton) = 0;

    // 刷新目录树
    virtual void Refresh() = 0;

    // 显示空间信息
    virtual void ShowSpaceInfo(wstring info) = 0;


    // 查找指定文件所在的列表行
    virtual void SelectListLine(vector<wstring> paths) = 0;

	virtual void MyTrim(wstring & str) = 0;		//去除字符串右空格

	// 相当于析构操作
	virtual void Release() = 0;

	virtual void ShowLoading(bool visible = true) = 0;

};
