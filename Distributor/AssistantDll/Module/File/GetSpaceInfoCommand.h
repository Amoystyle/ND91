#pragma once

#include "Module/BaseCommand.h"
#include "Common/greta/RegexprHelper.h"

namespace ND91Assistant
{
class GetSpaceInfoCommand :public BaseCommand
{
public:
	GetSpaceInfoCommand(const DeviceData* pDevice);
	virtual ~GetSpaceInfoCommand();

    // 执行命令
    virtual void Execute();

	void  setPath(const wstring& apath){_Path = apath;}

	deque<SpaceInfo> GetDirInfos(){return _SpaceInfos;}

    // 生成Android命令数据包
    virtual std::string GetAndroidBodyData();

    // 解析Android返回结果数据包
    virtual void SetAndroidReturnData(std::string data);

    // 命令传送到IOS设备上的文件名（全路径）
    virtual std::wstring GetIosCommandFilePath() { return L""; }

    // 从IOS上获取的命令结果文件名（全路径）
    virtual std::wstring GetIosResultFilePath() { return L""; }

    // 生成ios文件。返回文件名，调用者负责删除文件
    virtual std::wstring GetIosCommandFileContent() { return L""; }

    // 解析返回文件，解析后删除文件
    virtual  void SetIosReturnFile(std::wstring filename) { }

private:

	//解析返回结果
	void Analyze(const string& result);

	//解析但条目录空间信息
	SpaceInfo AnalyzeSpaceInfo(const string& result);

	wstring  _Path; //传入的路径

	deque<SpaceInfo>  _SpaceInfos;//返回结果

    RegexprHelper _rh;

};

}
