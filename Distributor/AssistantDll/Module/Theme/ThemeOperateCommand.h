#pragma once
#include "Module/BaseCommand.h"
namespace ND91Assistant
{
// 主题操作命令
class ThemeOperateCommand : public BaseCommand
{
public:
	ThemeOperateCommand(const DeviceData* pDevice);
	virtual ~ThemeOperateCommand(void);

	// Summary  : 设为主题
	// Returns	: bool
	// Parameter: string themeID
	bool SetTheme(wstring themeID,bool IsSmart=false);

	// Summary  : 删除主题
	// Returns	: bool
	// Parameter: string themeID
	bool DelTheme(wstring themeID,bool IsSmart=false);

	// Summary  : 添加主题
	// Returns	: bool
	// Parameter: wstring aptRemotePath
	bool AddTheme(wstring aptRemotePath,bool IsSmart=false);

	// Summary  : 初始化主题信息
	// Returns	: bool
	bool InitUpdateThemes();

	// Summary  : 守护生成主题简要信息的xml文件
	// Returns	: bool
	bool GetThemeBriefInfo();

	bool GetSmartThemeXml();

	// Summary  : 守护生成单个主题详细信息的xml文件
	// Returns	: bool
	// Parameter: string themeID
	bool GetThemeDetailInfo(wstring themeID);

	// Summary  : 添加并设为当前主题
	// Returns	: bool
	// Parameter: wstring aptRemotePath
	bool AddThemeAndSetCurrentTheme(wstring aptRemotePath);

	// Summary  : 守护生成包含全部主题信息的xml文件
	// Returns	: bool
	// Parameter: wstring path
	bool GetThemeInfos(wstring path);

	// Summary  : 守护生成要导出的主题包文件
	// Returns	: bool
	// Parameter: string themeID
	bool ExportThemePackage(wstring themeID);

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

	enum OperationType
	{
		ESetTheme = 1,
		EDeleteTheme = 2,
		EAddTheme = 3,
		EAddThemeAndSetAsCurTheme = 4,
		EGetThemeInfos = 5,
		EInitUpdateTheme = 6,
		EGetThemeBriefInfo = 7,
		EGetThemeDetailInfo = 8,
		EExportThemePkg = 9
	};

	OperationType   _type;

	wstring    _themeId;

	wstring   _path;

	bool      _success;
};

}
