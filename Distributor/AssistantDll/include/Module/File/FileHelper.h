#pragma once

#include "Monitor/ProgressSubject.h"
#include "NodeData.h"
#include "RootData.h"

namespace ND91Assistant
{
    // 文件辅助类：负责执行各种文件操作
    // 放在这里的都是静态方法，亦即不适合归到各个节点类的方法
	class CLASSINDLL_CLASS_DECL FileHelper
	{
	public:

        // Summary  : 得到指定路径对应的节点
        // Returns  : NodeData*         : 节点指针，返回NULL表示找不到
        // Parameter: DirData * pRoot   : 根目录
        // Parameter: wstring path      : 路径
        // Parameter: bool bExactMatch  : 是否完全匹配
        static NodeData* GetNode(DirData* pRoot, wstring path, bool bFindLinkTarget = true, bool bExactMatch = true);


		// Summary  : 得到指定路径对应的节点,中间有链接的话直接解析链接地址
		// Returns  : NodeData*         : 节点指针，返回NULL表示找不到
		// Parameter: DirData * pRoot   : 根目录
		// Parameter: wstring path      : 路径
		static NodeData* GetRealNode(DirData* pRoot, wstring path);

        // Summary:   下载到指定目录（要求所有节点都属于同一个设备）
        // Returns:   传输结果
        // Parameter: vector<NodeData*>* pNodes     : 要下载的所有节点
        // Parameter: wstring destDir               : 下载到此目录
        // Parameter: ProgressObserver* pObserver   : 下载进度接受者
        static TransferResult Download(vector<NodeData*>* pNodes, wstring destDir,ProgressObserver* pObserver);

        // Summary:   批量删除子节点
        // Returns:   NULL: 成功 非NULL: 删除失败的节点
        // Parameter: vector<NodeData * > * pNodes  : 要删除的所有节点
        // Parameter: ProgressObserver * pObserver  : 进度接收者
        static NodeData* BatchDelete(vector<NodeData*>* pNodes, ProgressObserver* pObserver);

        // Summary:   按路径删除一个文件或目录，无进度支持
        // Returns:   是否成功
        // Parameter: const DeviceData * pDevice    : 设备指针
        // Parameter: wstring path                  : 完整路径
        static bool Delete(const DeviceData* pDevice, wstring path);

        // Summary:   统计指定节点的文件目录数量和大小总和，并设置到pSubject中
        // Returns:   void
        // Parameter: vector<NodeData*>*  pNodes    : 节点数组
        // Parameter: DownloadFileCommand & cmd     : 存储进度数据用
        static void SetTaskSize(vector<NodeData*>* pNodes, ProgressSubject* pSubject,ProgressObserver* pObserver);

        // Summary  : pc文件或目录, 上传到指定设备路径
        // Returns  : 是否成功
        // Parameter: const DeviceData * pDevice    : 设备指针
        // Parameter: wstring sourcePath            : 本地pc路径, 文件或目录
        // Parameter: wstring destDir               : 设备上的目录
        // Parameter: ProgressObserver * pObserver  : 进度接收者
        static bool Upload(const DeviceData* pDevice, wstring sourcePath, wstring destDir, ProgressObserver* pObserver, bool bSetAttr = false);

        // 因为需要区分失败还是取消，增加一个方法，仅返回值不同
        static TransferResult Upload2(const DeviceData* pDevice, wstring sourcePath, wstring destDir, ProgressObserver* pObserver, bool bSetAttr = false);

        // Summary  : 在设备上创建指定路径
        // Returns  : 创建成功或已存在返回节点指针, 失败返回NULL
        // Parameter: const DeviceData * pDevice    : 设备指针
        // Parameter: wstring dirInDevice           : 设备上的目录
        static DirData* CreateDir(const DeviceData* pDevice, wstring dirInDevice);

        // Summary  : 获取应用目录节点在界面显示的名称
        // Returns  : void
        // Parameter: NodeData * pNode 应用目录节点
        // Parameter: const DeviceData * pDevice
        static void IosGetDisplayName(NodeData* pNode, const DeviceData* pDevice);
	};
}
