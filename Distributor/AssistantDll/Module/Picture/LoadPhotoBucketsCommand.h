#pragma once
#include "Module/TableQueryCommand.h"
namespace ND91Assistant
{
	//加载图库分类
class AndroidPhotoBucket;
class LoadPhotoBucketsCommand :public TableQueryCommand
{
public:
	LoadPhotoBucketsCommand(const DeviceData* pDevice);
	virtual ~LoadPhotoBucketsCommand(void);

	// 执行命令
	virtual void Execute();

	// Summary  : 获得图库分类列表
	// Returns	: const deque<AndroidPhotoBucket*>
	const deque<AndroidPhotoBucket*> GetPhotoBuckets(){return _PhotoBuckets;}

protected:
	// 解析返回数据包
	virtual void SetAndroidReturnData(std::string data);

private:
	deque<AndroidPhotoBucket*>   _PhotoBuckets; //图库分类列表 
};

}
